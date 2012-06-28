#include "pocketsphinx.h"
#include <ad.h>
#include <cont_ad.h>
#include <MMDAgent.h>
#include "Sphinx.h"


bool m_pause;

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
   m_userDictionary = NULL;

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
   if(m_userDictionary != NULL)
      free(m_userDictionary);

   if (m_cont)
      cont_ad_close(m_cont);
   if (m_ad)
      ad_close(m_ad);

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

bool Sphinx::load(MMDAgent *mmdagent, const char *languageModel, const char *dictionary, const char *acousticModel, const char *configFile, const char *userDictionary)
{
   m_mmdagent = mmdagent;

   m_languageModel = MMDAgent_strdup(languageModel);
   m_dictionary = MMDAgent_strdup(dictionary);
   m_acousticModel = MMDAgent_strdup(acousticModel);
   m_configFile = MMDAgent_strdup(configFile);
   m_userDictionary = MMDAgent_strdup(userDictionary);

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
   m_pause = false;

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

   while (!m_pause) {
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

      /* Decode utterance until end (marked by a "long" silence, >1sec) or told to pause */
      while (!m_pause) {
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

         /*
          * Decode whatever data was read above.
          */
         rem = ps_process_raw(m_ps, adbuf, k, FALSE, FALSE);

         /* If no work to be done, sleep a bit */
         if ((rem == 0) && (k == 0))
            glfwSleep(0.02);
      }

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
   m_pause = true;
}

/* Tell the recognizer to resume */
void Sphinx::resume()
{
   m_pause = false;
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

