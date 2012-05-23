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

#define PLUGINOPENJTALK_NAME         "Open_JTalk"
#define PLUGINOPENJTALK_STARTCOMMAND "SYNTH_START"
#define PLUGINOPENJTALK_STOPCOMMAND  "SYNTH_STOP"

/* headers */

#include "MMDAgent.h"

#include "mecab.h"
#include "njd.h"
#include "jpcommon.h"
#include "HTS_engine.h"

#include "text2mecab.h"
#include "mecab2njd.h"
#include "njd2jpcommon.h"

#include "njd_set_pronunciation.h"
#include "njd_set_digit.h"
#include "njd_set_accent_phrase.h"
#include "njd_set_accent_type.h"
#include "njd_set_unvoiced_vowel.h"
#include "njd_set_long_vowel.h"

#include "Open_JTalk.h"
#include "Open_JTalk_Thread.h"
#include "Open_JTalk_Manager.h"

/* variables */

static Open_JTalk_Manager open_jtalk_manager;
static bool enable;

/* extAppStart: load amodels and start thread */
EXPORT void extAppStart(MMDAgent *mmdagent)
{
   int len;
   char dic[MMDAGENT_MAXBUFLEN];
   char *config;

   /* get dictionary directory name */
   sprintf(dic, "%s%c%s", mmdagent->getAppDirName(), MMDAGENT_DIRSEPARATOR, PLUGINOPENJTALK_NAME);

   /* get config file */
   config = MMDAgent_strdup(mmdagent->getConfigFileName());
   len = MMDAgent_strlen(config);

   /* load */
   if (len > 4) {
      config[len-4] = '.';
      config[len-3] = 'o';
      config[len-2] = 'j';
      config[len-1] = 't';
      open_jtalk_manager.loadAndStart(mmdagent, dic, config);
   }

   if(config)
      free(config);

   enable = true;
   mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINENABLE, PLUGINOPENJTALK_NAME);
}

/* extProcCommand: process command message */
EXPORT void extProcCommand(MMDAgent *mmdagent, const char *type, const char *args)
{
   if(enable == true) {
      if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINDISABLE)) {
         if(MMDAgent_strequal(args, PLUGINOPENJTALK_NAME)) {
            enable = false;
            mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINDISABLE, PLUGINOPENJTALK_NAME);
         }
      } else if (open_jtalk_manager.isRunning()) {
         if (MMDAgent_strequal(type, PLUGINOPENJTALK_STARTCOMMAND)) {
            open_jtalk_manager.synthesis(args);
         } else if (MMDAgent_strequal(type, PLUGINOPENJTALK_STOPCOMMAND)) {
            open_jtalk_manager.stop(args);
         }
      }
   } else {
      if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINENABLE)) {
         if(MMDAgent_strequal(args, PLUGINOPENJTALK_NAME)) {
            enable = true;
            mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINENABLE, PLUGINOPENJTALK_NAME);
         }
      }
   }
}

/* extAppEnd: stop and free thread */
EXPORT void extAppEnd(MMDAgent *mmdagent)
{
   open_jtalk_manager.stopAndRelease();
}
