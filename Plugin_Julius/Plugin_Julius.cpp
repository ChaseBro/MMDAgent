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

#define PLUGINJULIUS_NAME          "Julius"
#define PLUGINJULIUS_LANGUAGEMODEL MMDAGENT_DIRSEPARATOR, PLUGINJULIUS_NAME, MMDAGENT_DIRSEPARATOR, "lang_m", MMDAGENT_DIRSEPARATOR, "web.60k.8-8.bingramv5.gz"
#define PLUGINJULIUS_DICTIONARY    MMDAGENT_DIRSEPARATOR, PLUGINJULIUS_NAME, MMDAGENT_DIRSEPARATOR, "lang_m", MMDAGENT_DIRSEPARATOR, "web.60k.htkdic"
#define PLUGINJULIUS_ACOUSTICMODEL MMDAGENT_DIRSEPARATOR, PLUGINJULIUS_NAME, MMDAGENT_DIRSEPARATOR, "phone_m", MMDAGENT_DIRSEPARATOR, "clustered.mmf.16mix.all.julius.binhmm"
#define PLUGINJULIUS_TRIPHONELIST  MMDAGENT_DIRSEPARATOR, PLUGINJULIUS_NAME, MMDAGENT_DIRSEPARATOR, "phone_m", MMDAGENT_DIRSEPARATOR, "tri_tied.list.bin"
#define PLUGINJULIUS_CONFIGFILE    MMDAGENT_DIRSEPARATOR, PLUGINJULIUS_NAME, MMDAGENT_DIRSEPARATOR, "jconf.txt"

/* headers */

#include "MMDAgent.h"
#include "julius/juliuslib.h"
#include "Julius_Logger.h"
#include "Julius_Thread.h"

/* variables */

static Julius_Thread julius_thread;
static bool enable;

/* extAppStart: load models and start thread */
EXPORT void extAppStart(MMDAgent *mmdagent)
{
   int len;
   char languageModel[MMDAGENT_MAXBUFLEN];
   char dictionary[MMDAGENT_MAXBUFLEN];
   char acousticModel[MMDAGENT_MAXBUFLEN];
   char triphoneList[MMDAGENT_MAXBUFLEN];
   char configFile[MMDAGENT_MAXBUFLEN];
   char userDictionary[MMDAGENT_MAXBUFLEN];

   /* set model file names */
   sprintf(languageModel, "%s%c%s%c%s%c%s", mmdagent->getAppDirName(), PLUGINJULIUS_LANGUAGEMODEL);
   sprintf(dictionary, "%s%c%s%c%s%c%s", mmdagent->getAppDirName(), PLUGINJULIUS_DICTIONARY);
   sprintf(acousticModel, "%s%c%s%c%s%c%s", mmdagent->getAppDirName(), PLUGINJULIUS_ACOUSTICMODEL);
   sprintf(triphoneList, "%s%c%s%c%s%c%s", mmdagent->getAppDirName(), PLUGINJULIUS_TRIPHONELIST);
   sprintf(configFile, "%s%c%s%c%s", mmdagent->getAppDirName(), PLUGINJULIUS_CONFIGFILE);

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
   julius_thread.loadAndStart(mmdagent, languageModel, dictionary, acousticModel, triphoneList, configFile, userDictionary);

   enable = true;
   mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINENABLE, PLUGINJULIUS_NAME);
}

/* extProcCommand: process command message */
EXPORT void extProcCommand(MMDAgent *mmdagent, const char *type, const char *args)
{
   if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINDISABLE)) {
      if(MMDAgent_strequal(args, PLUGINJULIUS_NAME) && enable == true) {
         julius_thread.pause();
         enable = false;
         mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINDISABLE, PLUGINJULIUS_NAME);
      }
   } else if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINENABLE)) {
      if(MMDAgent_strequal(args, PLUGINJULIUS_NAME) && enable == false) {
         julius_thread.resume();
         enable = true;
         mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINENABLE, PLUGINJULIUS_NAME);
      }
   }
}

/* extProcEvent: process event message */
EXPORT void extProcEvent(MMDAgent *mmdagent, const char *type, const char *args)
{
   if(MMDAgent_strequal(type, MMDAGENT_EVENT_KEY)) {
      if(MMDAgent_strequal(args, "J")) {
         if(julius_thread.getLogActiveFlag() == true)
            julius_thread.setLogActiveFlag(false);
         else
            julius_thread.setLogActiveFlag(true);
      }
   }
}

/* extAppEnd: stop thread and free julius */
EXPORT void extAppEnd(MMDAgent *mmdagent)
{
   julius_thread.stopAndRelease();
   enable = false;
}

/* extUpdate: update log view */
EXPORT void extUpdate(MMDAgent *mmdagent, double frame)
{
   julius_thread.updateLog(frame);
}

/* extRender: render log view when debug display mode */
EXPORT void extRender(MMDAgent *mmdagent)
{
   julius_thread.renderLog();
}
