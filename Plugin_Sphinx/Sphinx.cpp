#include "pocketsphinx.h"
#include <ad.h>
#include <cont_ad.h>
#include <MMDAgent.h>
#include "Sphinx.h"
#include "err.h"
#include <sys/stat.h>


void Sphinx::initialize()
{
   m_ps = NULL; /* Recognizer instance */
   m_config = NULL; /* Configuration instance */

   m_mmdagent = NULL;

   m_ad = NULL;
   m_cont = NULL;

   m_languageModel = NULL;
   m_dictionary = NULL;
   m_acousticModel = NULL;
   m_configFile = NULL;
   m_logFolder = NULL;
   m_uttLogFolder = NULL;

   m_callbackRecogBeginData = NULL; /* data for callback function for beginning of recognition */
   m_callbackRecogResultData = NULL; /* data for callback function for end of recognition */
   m_callbackRecogBegin = NULL;
   m_callbackRecogResult = NULL;
}

void Sphinx::clear()
{
   if(m_languageModel != NULL)
      free(m_languageModel);
   if(m_dictionary != NULL)
      free(m_dictionary);
   if(m_acousticModel != NULL)
      free(m_acousticModel);
   if(m_configFile != NULL)
      free(m_configFile);
   if(m_logFolder != NULL)
      free(m_logFolder);
   if(m_uttLogFolder != NULL)
      free(m_uttLogFolder);

   if(m_ps)
     ps_free(m_ps);

   initialize();
}

Sphinx::Sphinx()
{
   initialize();
}

Sphinx::~Sphinx()
{
   clear();
}

bool Sphinx::load(MMDAgent *mmdagent, const char *languageModel, const char *dictionary, const char *acousticModel, const char *configFile, const char *logFolder)
{
   m_mmdagent = mmdagent;

   m_languageModel = MMDAgent_strdup(languageModel);
   m_dictionary = MMDAgent_strdup(dictionary);
   m_acousticModel = MMDAgent_strdup(acousticModel);
   m_configFile = MMDAgent_strdup(configFile);
   m_logFolder = MMDAgent_strdup(logFolder);

   /* load models */
   m_config = cmd_ln_init(NULL, ps_args(), 1,
                          "-lm", MMDAgent_pathdup(m_languageModel),
                          "-dict", MMDAgent_pathdup(m_dictionary),
                          "-mdef", MMDAgent_pathdup(m_acousticModel),
                          NULL);
   if (m_config == NULL) {
      printf("Failed to load models\n");
      return false;
   }
   m_ps = ps_init(m_config);
   if (m_ps == NULL)
      return false;
   return true;
}

void Sphinx::start()
{
   if (m_ps == NULL)
      return;

   if ((m_ad = ad_open_dev(cmd_ln_str_r(m_config, "-adcdev"),
       (int)cmd_ln_float32_r(m_config, "-samprate"))) == NULL) {
      printf("Failed to open audio device\n");
      return;
   }

   /* Initialize continuous listening module */
   if ((m_cont = cont_ad_init(m_ad, ad_read)) == NULL) {
      printf("Failed to initialize voice activity detection\n");
      return;
   }

   char logFolderPath[MMDAGENT_MAXBUFLEN];
   sprintf(logFolderPath, "%s%ld%c", m_logFolder, time(NULL), MMDAGENT_DIRSEPARATOR);
   free(m_logFolder);
   m_logFolder = MMDAgent_strdup(logFolderPath);
   printf("Sphinx log path: %s\n", MMDAgent_pathdup(m_logFolder));
   if (mkdir(MMDAgent_pathdup(m_logFolder), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
      printf("mkdir failed.\n");
      return;
   }

   char uttLogPath[MMDAGENT_MAXBUFLEN];
   sprintf(uttLogPath, "%sutts%c", m_logFolder, MMDAGENT_DIRSEPARATOR);
   m_uttLogFolder = MMDAgent_strdup(uttLogPath);
   printf("Sphinx utterance log path: %s\n", uttLogPath);
   if (mkdir(MMDAgent_pathdup(uttLogPath), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0) {
      printf("mkdir failed.\n");
      return;
   }

   FILE *logFile;
   char logFilePath[MMDAGENT_MAXBUFLEN];
   sprintf(logFilePath, "%s%s", m_logFolder, "sphinxlog.log");
   logFile = fopen(MMDAgent_pathdup(logFilePath), "w");
   if (logFile == NULL) {
      printf("Failed to open log file.\n");
      return;
   }
   if (cont_ad_set_logfp(m_cont, logFile) != 0) {
      printf("Failed to initialize Sphinx logging.\n");
      fclose(logFile);
      return;
   }
   err_set_logfp(logFile);

   FILE *rawLogFile;
   char audioLogFilePath[MMDAGENT_MAXBUFLEN];
   sprintf(audioLogFilePath, "%s%s", m_logFolder, "sphinxaudio.raw");
   rawLogFile = fopen(MMDAgent_pathdup(audioLogFilePath), "w");
   if (rawLogFile == NULL) {
      printf("Failed to open audio log file.\n");
      fclose(logFile);
      return;
   }
   if (cont_ad_set_rawfp(m_cont, rawLogFile) != 0) {
      printf("Failed to initalize Sphinx audio logging.\n");
      fclose(rawLogFile);
      fclose(logFile);
      return;
   }

   run();
}

/* Main recognition loop */
void Sphinx::run()
{
   int16 adbuf[4096];
   int32 k, ts, rem;
   char const *hyp;
   char const *uttid;

   /* Resume A/D recording for next utterance */
   if (ad_start_rec(m_ad) < 0) {
      printf("Failed to start recording\n");
      return;
   }
   if (cont_ad_calib(m_cont) < 0) {
      printf("Failed to calibrate voice activity detection\n");
      return;
   }

   while (true) {
      /* Indicate listening for next utterance */
      printf("READY....\n");

      /* Wait data for next utterance */
      while ((k = cont_ad_read(m_cont, adbuf, 4096)) == 0)
         glfwSleep(0.1);

      if (k < 0) {
         printf("Failed to read audio\n");
         return;
      }

      /*
      * Non-zero amount of data received; start recognition of new utterance.
      * NULL argument to uttproc_begin_utt => automatic generation of utterance-id.
      */
      if (ps_start_utt(m_ps, NULL) < 0) {
         printf("Failed to start utterance\n");
         return;
      }
      ps_process_raw(m_ps, adbuf, k, FALSE, FALSE);
      printf("Listening...\n");
      if (m_callbackRecogBegin != NULL)
         m_callbackRecogBegin(m_callbackRecogBeginData);

      /* Note timestamp for this first block of data */
      ts = m_cont->read_ts;

      FILE *uttFile;
      char uttFilePath[MMDAGENT_MAXBUFLEN];
      uttid = ps_get_uttid(m_ps);
      sprintf(uttFilePath, "%s%s.raw", m_uttLogFolder, uttid);
      uttFile = fopen(uttFilePath, "w");
      fwrite(adbuf, sizeof(int16), k, uttFile);
      fflush(uttFile);

      /* Decode utterance until end (marked by a "long" silence, >1sec) or told to pause */
      while (true) {
         /* Read non-silence audio data, if any, from continuous listening module */
         if ((k = cont_ad_read(m_cont, adbuf, 4096)) < 0) {
            printf("Failed to read audio\n");
            return;
         }
         if (k == 0) {
            /*
            * No speech data available; check current timestamp with most recent
            * speech to see if more than 1 sec elapsed.  If so, end of utterance.
            */
            if ((m_cont->read_ts - ts) > DEFAULT_SAMPLES_PER_SEC)
               break;
         }
         else {
               /* New speech data received; note current timestamp */
               ts = m_cont->read_ts;
         }

         fwrite(adbuf, sizeof(int16), k, uttFile);
         fflush(uttFile);

         /*
          * Decode whatever data was read above.
          */
         rem = ps_process_raw(m_ps, adbuf, k, FALSE, FALSE);

         /* If no work to be done, sleep a bit */
         if ((rem == 0) && (k == 0))
            glfwSleep(0.02);
      }

      fclose(uttFile);
      /*
      * Utterance ended; flush any accumulated, unprocessed A/D data and stop
      * listening until current utterance completely decoded
      */
      ad_stop_rec(m_ad);
      while (ad_read(m_ad, adbuf, 4096) >= 0);
      cont_ad_reset(m_cont);

      printf("Stopped listening, please wait...\n");
      /* Finish decoding, obtain and print result */
      ps_end_utt(m_ps);
      hyp = ps_get_hyp(m_ps, NULL, &uttid);
      printf("%s: %s\n", uttid, hyp);
      if (m_callbackRecogResult != NULL)
         m_callbackRecogResult(hyp, m_callbackRecogResultData);

      /* Resume A/D recording for next utterance */
      if (ad_start_rec(m_ad) < 0) {
         printf("Failed to start recording\n");
         return;
      }
   }

   /* Stop recording until un-paused */
   ad_stop_rec(m_ad);
   while (ad_read(m_ad, adbuf, 4096) >= 0);
   cont_ad_reset(m_cont);
}

/* Tell the recognizer to pause */
void Sphinx::pause()
{
}

/* Tell the recognizer to resume */
void Sphinx::resume()
{
   run();
}

/* Register callback function when recognition starts */
void Sphinx::set_callback_begin(void (*callbackRecogBegin)(void *data), void *data)
{
   m_callbackRecogBegin = callbackRecogBegin;
   m_callbackRecogBeginData = data;
}

/* Register callback function when recognition ends */
void Sphinx::set_callback_return(void (*callbackRecogResult)(const char *result, void *data), void *data)
{
   m_callbackRecogResult = callbackRecogResult;
   m_callbackRecogResultData = data;
}

