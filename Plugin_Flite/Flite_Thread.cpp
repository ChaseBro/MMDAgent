/* headers */

#include "MMDAgent.h"
#include "flite.h"

#include "Flite.h"
#include "Flite_Thread.h"

/* mainThread: main thread */
static void mainThread(void *param)
{
   Flite_Thread *flite_thread = (Flite_Thread *) param;
   flite_thread->run();
}

/* Flite_Thread::initialize: initialize thread */
void Flite_Thread::initialize()
{
   m_mmdagent = NULL;

   m_mutex = NULL;
   m_cond = NULL;
   m_thread = -1;

   m_count = 0;

   m_speaking = false;
   m_kill = false;

   m_charaBuff = NULL;
   m_styleBuff = NULL;
   m_textBuff = NULL;

   m_numModels = 0;
   m_modelNames = NULL;

}

/* Flite_Thread::clear: free thread */
void Flite_Thread::clear()
{
   int i;

   stop();
   m_kill = true;

   /* wait */
   if (m_cond != NULL)
      glfwSignalCond(m_cond);

   if(m_mutex != NULL || m_cond != NULL || m_thread >= 0) {
      if(m_thread >= 0) {
         glfwWaitThread(m_thread, GLFW_WAIT);
         glfwDestroyThread(m_thread);
      }
      if(m_cond != NULL)
         glfwDestroyCond(m_cond);
      if(m_mutex != NULL)
         glfwDestroyMutex(m_mutex);
      glfwTerminate();
   }

   if(m_charaBuff) free(m_charaBuff);
   if(m_styleBuff) free(m_styleBuff);
   if(m_textBuff) free(m_textBuff);

   /* free model names */
   if (m_numModels > 0) {
      for (i = 0; i < m_numModels; i++)
         free(m_modelNames[i]);
      free(m_modelNames);
   }

   initialize();
}

/* Flite_Thread::Flite_Thread: thread constructor */
Flite_Thread::Flite_Thread()
{
   initialize();
}

/* Flite_Thread::~Flite_Thread: thread destructor */
Flite_Thread::~Flite_Thread()
{
   clear();
}

/* Flite_Thread::loadAndStart: load models and start thread */
bool Flite_Thread::loadAndStart(MMDAgent *mmdagent, const char *config)
{
   int i, j, k;
   char buff[MMDAGENT_MAXBUFLEN];
   FILE *fp;
   bool err = false;
   double *weights;

   /* load config file */
   fp = MMDAgent_fopen(config, "r");
   if (fp == NULL)
      return false;

   /* number of speakers */
   i = MMDAgent_fgettoken(fp, buff);
   if (i <= 0) {
      fclose(fp);
      clear();
      return false;
   }
   m_numModels = MMDAgent_str2int(buff);
   if (m_numModels <= 0) {
      fclose(fp);
      clear();
      return false;
   }

   /* model directory names */
   m_modelNames = (char **) malloc(sizeof(char *) * m_numModels);
   for (i = 0; i < m_numModels; i++) {
      j = MMDAgent_fgettoken(fp, buff);
      if (j <= 0)
         err = true;
      m_modelNames[i] = MMDAgent_strdup(buff);
   }
   if (err) {
      fclose(fp);
      clear();
      return false;
   }
   fclose(fp);

   /* load models for TTS */
   if (m_flite.load(m_modelNames, m_numModels) != true) {
      clear();
      return false;
   }

   m_mmdagent = mmdagent;

   /* start thread */
   glfwInit();
   m_mutex = glfwCreateMutex();
   m_cond = glfwCreateCond();
   m_thread = glfwCreateThread(mainThread, this);
   if(m_mutex == NULL || m_cond == NULL || m_thread < 0) {
      clear();
      return false;
   }

   return true;
}

/* Flite_Thread::stopAndRelease: stop thread and free Flite */
void Flite_Thread::stopAndRelease()
{
   clear();
}

/* Flite_Thread::run: main thread loop for TTS */
void Flite_Thread::run()
{
   char lip[MMDAGENT_MAXBUFLEN];
   char *chara, *style, *text;
   int index;

   while (m_kill == false) {
      /* wait text */
      glfwLockMutex(m_mutex);
      while(m_count <= 0) {
         glfwWaitCond(m_cond, m_mutex, GLFW_INFINITY);
         if(m_kill == true)
            return;
      }
      chara = MMDAgent_strdup(m_charaBuff);
      style = MMDAgent_strdup(m_styleBuff);
      text = MMDAgent_strdup(m_textBuff);
      m_count--;
      glfwUnlockMutex(m_mutex);

      m_speaking = true;

      /* search style index */
      for (index = 0; index < m_numModels; index++)
         if (MMDAgent_strequal(m_modelNames[index], style))
            break;
      if (index >= m_numModels) /* unknown style */
         index = 0;

      /* send SYNTH_EVENT_START */
      m_mmdagent->sendEventMessage(EVENTSTART, chara);

      /* synthesize */
      // prepare text (don't need?) m_openJTalk.prepare(text);
      // Get Phonemes to pass to lipsync m_openJTalk.getPhonemeSequence(lip);
      // m_mmdagent->sendCommandMessage(COMMANDSTARTLIP, "%s|%s", chara, lip);

      m_flite.synthesize(style, text);

      /* send SYNTH_EVENT_STOP */
      m_mmdagent->sendEventMessage(EVENTSTOP, chara);

      if(chara) free(chara);
      if(style) free(style);
      if(text) free(text);
      m_speaking = false;
   }
}

/* Flite_Thread::isRunning: check running */
bool Flite_Thread::isRunning()
{
   if(m_kill == true || m_mutex == NULL || m_cond == NULL || m_thread < 0)
      return false;
   else
      return true;
}

/* Flite_Thread::isSpeaking: check speaking */
bool Flite_Thread::isSpeaking()
{
   return m_speaking;
}

/* checkCharacter: check speaking character */
bool Flite_Thread::checkCharacter(const char *chara)
{
   bool ret;

   /* check */
   if(isRunning() == false)
      return false;

   /* wait buffer mutex */
   glfwLockMutex(m_mutex);

   /* save character name, speaking style, and text */
   ret = MMDAgent_strequal(m_charaBuff, chara);

   /* release buffer mutex */
   glfwUnlockMutex(m_mutex);

   return ret;
}

/* Flite_Thread::synthesis: start synthesis */
void Flite_Thread::synthesis(const char *chara, const char *style, const char *text)
{
   /* check */
   if(isRunning() == false)
      return;
   if(MMDAgent_strlen(chara) <= 0 || MMDAgent_strlen(style) <= 0 || MMDAgent_strlen(text) <= 0)
      return;

   /* wait buffer mutex */
   glfwLockMutex(m_mutex);

   /* save character name, speaking style, and text */
   if(m_charaBuff) free(m_charaBuff);
   if(m_styleBuff) free(m_styleBuff);
   if(m_textBuff) free(m_textBuff);
   m_charaBuff = MMDAgent_strdup(chara);
   m_styleBuff = MMDAgent_strdup(style);
   m_textBuff = MMDAgent_strdup(text);
   m_count++;

   /* start synthesis thread */
   if(m_count <= 1)
      glfwSignalCond(m_cond);

   /* release buffer mutex */
   glfwUnlockMutex(m_mutex);
}

/* Flite_Thread::stop: barge-in function */
void Flite_Thread::stop()
{
   if(isRunning() == true) {
      // m_openJTalk.stop();

      /* wait buffer mutex */
      glfwLockMutex(m_mutex);

      /* stop lip sync */
      m_mmdagent->sendCommandMessage(COMMANDSTOPLIP, m_charaBuff);

      /* release buffer mutex */
      glfwUnlockMutex(m_mutex);

   }
}
