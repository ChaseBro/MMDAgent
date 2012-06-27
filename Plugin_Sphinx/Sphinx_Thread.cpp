/* ----------------------------------------------------------------- */
/*           The Toolkit for Building Voice Interaction Systems      */
/*           "MMDAgent" developed by MMDAgent Project Team           */
/*           http://www.mmdagent.jp/                                 */
/* ----------------------------------------------------------------- */
/*                                                                   */
/*  Copyright (c) 2009-2011  Nagoya Institute of Technology          */
/*                           Department of Computer Science          */
/*                                                                   */
/* All rights reserved.                                              */
/*                                                                   */
/* Redistribution and use in source and binary forms, with or        */
/* without modification, are permitted provided that the following   */
/* conditions are met:                                               */
/*                                                                   */
/* - Redistributions of source code must retain the above copyright  */
/*   notice, this list of conditions and the following disclaimer.   */
/* - Redistributions in binary form must reproduce the above         */
/*   copyright notice, this list of conditions and the following     */
/*   disclaimer in the documentation and/or other materials provided */
/*   with the distribution.                                          */
/* - Neither the name of the MMDAgent project team nor the names of  */
/*   its contributors may be used to endorse or promote products     */
/*   derived from this software without specific prior written       */
/*   permission.                                                     */
/*                                                                   */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND            */
/* CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,       */
/* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF          */
/* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE          */
/* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS */
/* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,          */
/* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,     */
/* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON */
/* ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,   */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY    */
/* OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE           */
/* POSSIBILITY OF SUCH DAMAGE.                                       */
/* ----------------------------------------------------------------- */

/* headers */

#include "MMDAgent.h"
#include <ad.h>
#include <cont_ad.h>
#include "pocketsphinx.h"
#include "Sphinx.h"
//#include "Sphinx_Logger.h"
#include "Sphinx_Thread.h"

/* callbackRecogBegin: callback for beginning of recognition */
static void callbackRecogBegin(void *data)
{
   Sphinx_Thread *ps = (Sphinx_Thread *) data;
   ps->sendMessage(SPHINXTHREAD_EVENTSTART, NULL);
}

/* callbackRecogResult: callback for recognitional result */
static void callbackRecogResult(const char *result, void *data)
{
   unsigned int i;
   char m_result[strlen(result)];
   strcpy(m_result, result);
   Sphinx_Thread *ps = (Sphinx_Thread *) data;

   if (strlen(m_result) > 0) {
      for (i = 0; i < strlen(m_result); i++)
      {
         if (m_result[i] == '_')
            m_result[i] = ' ';
      }

      ps->sendMessage(SPHINXTHREAD_EVENTSTOP, m_result);
   }
}

/* mainThread: main thread */
static void mainThread(void *param)
{
   Sphinx_Thread *sphinx_thread = (Sphinx_Thread *) param;
   sphinx_thread->run();
}

/* Sphinx_Thread::initialize: initialize thread */
void Sphinx_Thread::initialize()
{
   m_config = NULL;
   m_recog = NULL;

   m_mmdagent = NULL;

   m_thread = -1;

   m_languageModel = NULL;
   m_dictionary = NULL;
   m_acousticModel = NULL;
   m_configFile = NULL;
   m_userDictionary = NULL;
}

/* Sphinx_Thread::clear: free thread */
void Sphinx_Thread::clear()
{
   if(m_thread >= 0) {
      if(m_recog)
         //j_close_stream(m_recog);
      glfwWaitThread(m_thread, GLFW_WAIT);
      glfwDestroyThread(m_thread);
      glfwTerminate();
   }
   if (m_recog) {
      //ps_free(m_recog); /* m_config is also released in ps_free */
   }

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

   initialize();
}

/* Sphinx_Thread::Sphinx_Thread: thread constructor */
Sphinx_Thread::Sphinx_Thread()
{
   initialize();
}

/* Sphinx_Thread::~Sphinx_Thread: thread destructor */
Sphinx_Thread::~Sphinx_Thread()
{
   clear();
}

/* Sphinx_Thread::loadAndStart: load models and start thread */
void Sphinx_Thread::loadAndStart(MMDAgent *mmdagent, const char *languageModel, const char *dictionary, const char *acousticModel, const char *configFile, const char *userDictionary)
{
   /* reset */
   clear();

   m_mmdagent = mmdagent;

   m_languageModel = MMDAgent_strdup(languageModel);
   m_dictionary = MMDAgent_strdup(dictionary);
   m_acousticModel = MMDAgent_strdup(acousticModel);
   m_configFile = MMDAgent_strdup(configFile);
   m_userDictionary = MMDAgent_strdup(userDictionary);

   if(m_mmdagent == NULL || m_languageModel == NULL || m_dictionary == NULL || m_acousticModel == NULL || m_configFile == NULL) {
      clear();
      return;
   }

   /* load config file
   tmp = MMDAgent_pathdup(m_configFile);
   if(j_config_load_file(m_jconf, tmp) < 0) {
      free(tmp);
      return;
   }
   free(tmp);
   */

   /* load user dictionary
   fp = MMDAgent_fopen(m_userDictionary, "r");
   if(fp != NULL) {
      fclose(fp);
      tmp = MMDAgent_pathdup(m_userDictionary);
      j_add_dict(m_jconf->lm_root, tmp);
      free(tmp);
   }
   */

   /* create recognition thread */
   glfwInit();
   m_thread = glfwCreateThread(mainThread, this);
   if(m_thread < 0) {
      clear();
      return;
   }
}

/* Sphinx_Thread::stopAndRelease: stop thread and release sphinx */
void Sphinx_Thread::stopAndRelease()
{
   clear();
}

/* Sphinx_Thread::run: main loop */
void Sphinx_Thread::run()
{
   char buff[MMDAGENT_MAXBUFLEN];

   /* create instance */
   m_recog = new Sphinx();
   if (!m_recog->load(m_mmdagent, m_languageModel, m_dictionary, m_acousticModel, m_configFile, m_userDictionary)) {
      printf("Failed to load config\n");
      return;
   }
   printf("Config loaded\n");

   /* register callback functions */
   m_recog->set_callback_begin(callbackRecogBegin, this);
   m_recog->set_callback_return(callbackRecogResult, this);

   /* setup logger */
   //m_logger.setup(m_recog);

   /* start logger */
   //m_logger.setActiveFlag(true);

   /* start recognize */
   printf("Attempting to start\n");
   m_recog->start();
}

/* Sphinx_Thread::pause: pause recognition process */
void Sphinx_Thread::pause()
{
   if(m_recog != NULL)
      m_recog->pause();
}

/* Sphinx_Thread::resume: resume recognition process */
void Sphinx_Thread::resume()
{
   if(m_recog != NULL)
      m_recog->resume();
}

/* Sphinx_Thread::sendMessage: send message to MMDAgent */
void Sphinx_Thread::sendMessage(const char *str1, const char *str2)
{
   m_mmdagent->sendEventMessage(str1, str2);
}

/* Sphinx_Thread::getLogActiveFlag: get active flag of logger */
bool Sphinx_Thread::getLogActiveFlag()
{
   //return m_logger.getActiveFlag();
   return false;
}

/* Sphinx_Thread::setLogActiveFlag: set active flag of logger */
void Sphinx_Thread::setLogActiveFlag(bool b)
{
   // m_logger.setActiveFlag(b);
}

/* Sphinx_Thread::updateLog: update log view per step */
void Sphinx_Thread::updateLog(double frame)
{
   //m_logger.update(frame);
}

/* Sphinx_Thread::renderLog: render log view */
void Sphinx_Thread::renderLog()
{
   //m_logger.render();
}
