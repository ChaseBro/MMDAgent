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

/* definitions */

#ifdef _WIN32
#define EXPORT extern "C" __declspec(dllexport)
#else
#define EXPORT extern "C"
#endif /* _WIN32 */

#define PLUGINSPHINX_NAME          "Sphinx"
#define PLUGINSPHINX_LANGUAGEMODEL MMDAGENT_DIRSEPARATOR, PLUGINSPHINX_NAME, MMDAGENT_DIRSEPARATOR, "model", MMDAGENT_DIRSEPARATOR, "lm", MMDAGENT_DIRSEPARATOR, "en_US", MMDAGENT_DIRSEPARATOR, "hub4.5000.DMP"
#define PLUGINSPHINX_DICTIONARY    MMDAGENT_DIRSEPARATOR, PLUGINSPHINX_NAME, MMDAGENT_DIRSEPARATOR, "model", MMDAGENT_DIRSEPARATOR, "lm", MMDAGENT_DIRSEPARATOR, "en_US", MMDAGENT_DIRSEPARATOR, "hub4.5000.dic"
#define PLUGINSPHINX_ACOUSTICMODEL MMDAGENT_DIRSEPARATOR, PLUGINSPHINX_NAME, MMDAGENT_DIRSEPARATOR, "model", MMDAGENT_DIRSEPARATOR, "hmm", MMDAGENT_DIRSEPARATOR, "en_US", MMDAGENT_DIRSEPARATOR, "hub4wsj_sc_8k", MMDAGENT_DIRSEPARATOR, "mdef"
#define PLUGINSPHINX_CONFIGFILE    MMDAGENT_DIRSEPARATOR, PLUGINSPHINX_NAME, MMDAGENT_DIRSEPARATOR, "jconf.txt"

/* headers */

#include "MMDAgent.h"
#include <ad.h>
#include <cont_ad.h>
#include "pocketsphinx.h"
#include "Sphinx.h"
#include "Sphinx_Thread.h"
//#include "Sphinx_Logger.h"

/* variables */

static Sphinx_Thread sphinx_thread;
static bool enable;

/* extAppStart: load models and start thread */
EXPORT void extAppStart(MMDAgent *mmdagent)
{
   int len;
   char languageModel[MMDAGENT_MAXBUFLEN];
   char dictionary[MMDAGENT_MAXBUFLEN];
   char acousticModel[MMDAGENT_MAXBUFLEN];
   char configFile[MMDAGENT_MAXBUFLEN];
   char userDictionary[MMDAGENT_MAXBUFLEN];

   /* set model file names */
   sprintf(languageModel, "%s%c%s%c%s%c%s%c%s%c%s", mmdagent->getAppDirName(), PLUGINSPHINX_LANGUAGEMODEL);
   sprintf(dictionary, "%s%c%s%c%s%c%s%c%s%c%s", mmdagent->getAppDirName(), PLUGINSPHINX_DICTIONARY);
   sprintf(acousticModel, "%s%c%s%c%s%c%s%c%s%c%s%c%s", mmdagent->getAppDirName(), PLUGINSPHINX_ACOUSTICMODEL);
   sprintf(configFile, "%s%c%s%c%s", mmdagent->getAppDirName(), PLUGINSPHINX_CONFIGFILE);

   /* user dictionary */
   strcpy(userDictionary, mmdagent->getConfigFileName());
   len = MMDAgent_strlen(userDictionary);
   if(len > 4) {
      userDictionary[len-4] = '.';
      userDictionary[len-3] = 'd';
      userDictionary[len-2] = 'i';
      userDictionary[len-1] = 'c';
   } else {
      strcpy(userDictionary, "");
   }

   /* load models and start thread */
   sphinx_thread.loadAndStart(mmdagent, languageModel, dictionary, acousticModel, configFile, userDictionary);

   enable = true;
   mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINENABLE, PLUGINSPHINX_NAME);
}

/* extProcCommand: process command message */
EXPORT void extProcCommand(MMDAgent *mmdagent, const char *type, const char *args)
{
   if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINDISABLE)) {
      if(MMDAgent_strequal(args, PLUGINSPHINX_NAME) && enable == true) {
         sphinx_thread.pause();
         enable = false;
         mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINDISABLE, PLUGINSPHINX_NAME);
      }
   } else if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINENABLE)) {
      if(MMDAgent_strequal(args, PLUGINSPHINX_NAME) && enable == false) {
         sphinx_thread.resume();
         enable = true;
         mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINENABLE, PLUGINSPHINX_NAME);
      }
   }
}

/* extProcEvent: process event message */
EXPORT void extProcEvent(MMDAgent *mmdagent, const char *type, const char *args)
{
   if(MMDAgent_strequal(type, MMDAGENT_EVENT_KEY)) {
      if(MMDAgent_strequal(args, "J")) {
         if(sphinx_thread.getLogActiveFlag() == true)
            sphinx_thread.setLogActiveFlag(false);
         else
            sphinx_thread.setLogActiveFlag(true);
      }
   }
}

/* extAppEnd: stop thread and free sphinx */
EXPORT void extAppEnd(MMDAgent *mmdagent)
{
   sphinx_thread.stopAndRelease();
   enable = false;
}

/* extUpdate: update log view */
EXPORT void extUpdate(MMDAgent *mmdagent, double frame)
{
   sphinx_thread.updateLog(frame);
}

/* extRender: render log view when debug display mode */
EXPORT void extRender(MMDAgent *mmdagent)
{
   sphinx_thread.renderLog();
}
