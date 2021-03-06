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

#define PLUGINKADE_NAME          "Kade"
#define PLUGINKADE_CONFIGFILE    MMDAGENT_DIRSEPARATOR, PLUGINKADE_NAME, MMDAGENT_DIRSEPARATOR, "jconf.txt"
#define SPHINXTHREAD_EVENTSTOP  "RECOG_EVENT_STOP"
#define PLUGINPHOENIX_STOPEVENT    "PARSE_RESULT"
#define CHARACTER "mei"
#define VOICE "cmu_us_awb"
#define SYNTH_START "SYNTH_START"

/* headers */

#include "MMDAgent.h"
#include "Kade_Thread.h"

/* variables */

static Kade_Thread kade_thread;
static bool enable;
char *plainText, *parseText;

/* extAppStart: load models and start thread */
EXPORT void extAppStart(MMDAgent *mmdagent)
{
   int len;
   char configFile[MMDAGENT_MAXBUFLEN];

   /* set model file names */
   sprintf(configFile, "%s%c%s%c%s", mmdagent->getAppDirName(), PLUGINKADE_CONFIGFILE);

   /* load models and start thread */
   kade_thread.load(mmdagent, configFile);

   enable = true;
   mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINENABLE, PLUGINKADE_NAME);
}

/* extProcCommand: process command message */
EXPORT void extProcCommand(MMDAgent *mmdagent, const char *type, const char *args)
{
   if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINDISABLE)) {
      if(MMDAgent_strequal(args, PLUGINKADE_NAME) && enable == true) {
         kade_thread.pause();
         enable = false;
         mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINDISABLE, PLUGINKADE_NAME);
      }
   } else if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINENABLE)) {
      if(MMDAgent_strequal(args, PLUGINKADE_NAME) && enable == false) {
         kade_thread.resume();
         enable = true;
         mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINENABLE, PLUGINKADE_NAME);
      }
   }
}

/* extProcEvent: process event message */
EXPORT void extProcEvent(MMDAgent *mmdagent, const char *type, const char *args)
{
   char *answer, newArgs[1000];

   if(enable) {
      if (MMDAgent_strequal(type, SPHINXTHREAD_EVENTSTOP)) {
         if (plainText)
            free(plainText);

         plainText = (char *)malloc(strlen(args) + 1);
         sprintf(plainText, "%s", args);
      } else if (MMDAgent_strequal(type, PLUGINPHOENIX_STOPEVENT)) {
         if (parseText)
            free(parseText);

         parseText = (char *)malloc(strlen(args) + 1);
         sprintf(parseText, "%s", args);
         answer = kade_thread.procParse(plainText, parseText);
         if (answer) {
            sprintf(newArgs, "%s|%s|%s", CHARACTER, VOICE, answer);
            mmdagent->sendCommandMessage(SYNTH_START, newArgs);
         }
      }
   }
}

/* extAppEnd: stop thread and free kade */
EXPORT void extAppEnd(MMDAgent *mmdagent)
{
   enable = false;
   if (plainText)
      free(plainText);
   plainText = NULL;
   if (parseText)
      free(parseText);
   parseText = NULL;
}

