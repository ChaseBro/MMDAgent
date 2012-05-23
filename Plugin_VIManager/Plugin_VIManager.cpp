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

#define PLUGINVIMANAGER_NAME "VIManager"

/* headers */

#include <locale.h>
#include "MMDAgent.h"
#include "VIManager.h"
#include "VIManager_Logger.h"
#include "VIManager_Thread.h"

/* variables */

static VIManager_Thread vimanager_thread;
static bool enable;
static bool enable_log;

/* extAppStart: load FST and start thread */
EXPORT void extAppStart(MMDAgent *mmdagent)
{
   char *buf;
   int len;

   setlocale(LC_CTYPE, "japanese");

   buf = MMDAgent_strdup(mmdagent->getConfigFileName());
   len = MMDAgent_strlen(buf);
   if (len > 4) {
      buf[len-4] = '.';
      buf[len-3] = 'f';
      buf[len-2] = 's';
      buf[len-1] = 't';
      vimanager_thread.loadAndStart(mmdagent, buf);
   }
   if(buf)
      free(buf);

   enable = true;
   enable_log = false;
   mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINENABLE, PLUGINVIMANAGER_NAME);
}

/* extProcCommand: process command message */
EXPORT void extProcCommand(MMDAgent *mmdagent, const char *type, const char *args)
{
   if(enable == true) {
      if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINDISABLE)) {
         if(MMDAgent_strequal(args, PLUGINVIMANAGER_NAME)) {
            enable = false;
            mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINDISABLE, PLUGINVIMANAGER_NAME);
         }
      }
   } else {
      if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINENABLE)) {
         if(MMDAgent_strequal(args, PLUGINVIMANAGER_NAME)) {
            enable = true;
            mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINENABLE, PLUGINVIMANAGER_NAME);
         }
      }
   }
}

/* extProcEvent: process event message */
EXPORT void extProcEvent(MMDAgent *mmdagent, const char *type, const char *args)
{
   if(enable == true) {
      if (vimanager_thread.isRunning()) {
         if (type != NULL) {
            vimanager_thread.enqueueBuffer(type, args); /* enqueue */
         }
      }
      if(MMDAgent_strequal(type, MMDAGENT_EVENT_KEY) && MMDAgent_strequal(args, "F")) {
         if(enable_log == true)
            enable_log = false;
         else
            enable_log = true;
      }
   }
}

/* extRender: render log */
EXPORT void extRender(MMDAgent *mmdagent)
{
   if(enable == true && enable_log == true)
      vimanager_thread.renderLog();
}

/* extAppEnd: stop and free thread */
EXPORT void extAppEnd(MMDAgent *mmdagent)
{
   vimanager_thread.stopAndRelease();
}
