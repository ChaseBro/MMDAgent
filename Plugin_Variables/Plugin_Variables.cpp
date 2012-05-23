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

#define PLUGINVARIABLES_NAME              "Variables"
#define PLUGINVARIABLES_TIMERSTARTCOMMAND "TIMER_START"
#define PLUGINVARIABLES_TIMERSTOPCOMMAND  "TIMER_STOP"
#define PLUGINVARIABLES_VALUESETCOMMAND   "VALUE_SET"
#define PLUGINVARIABLES_VALUEUNSETCOMMAND "VALUE_UNSET"
#define PLUGINVARIABLES_VALUEEVALCOMMAND  "VALUE_EVAL"
#define PLUGINVARIABLES_VALUEGETCOMMAND   "VALUE_GET"

/* headers */

#include "MMDAgent.h"
#include "Variables.h"
#include "CountDown_Thread.h"

/* variables */

static Variables variables;
static CountDown_Thread countdown_thread;
static bool enable;

/* extAppStart: load models and start thread */
EXPORT void extAppStart(MMDAgent *mmdagent)
{
   variables.setup(mmdagent);
   countdown_thread.setupAndStart(mmdagent);

   enable = true;
   mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINENABLE, PLUGINVARIABLES_NAME);
}

/* extProcCommand: process command message */
EXPORT void extProcCommand(MMDAgent *mmdagent, const char *type, const char *args)
{
   char *buff, *p1, *p2, *p3, *save;

   if(enable == true) {
      if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINDISABLE)) {
         if(MMDAgent_strequal(args, PLUGINVARIABLES_NAME)) {
            enable = false;
            mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINDISABLE, PLUGINVARIABLES_NAME);
         }
      } else if (MMDAgent_strequal(type, PLUGINVARIABLES_VALUESETCOMMAND)) {
         /* VALUE_SET command */
         buff = MMDAgent_strdup(args);
         p1 = MMDAgent_strtok(buff, "|", &save);
         p2 = MMDAgent_strtok(NULL, "|", &save);
         p3 = MMDAgent_strtok(NULL, "|", &save);
         variables.set(p1, p2, p3);
         if(buff)
            free(buff);
      } else if (MMDAgent_strequal(type, PLUGINVARIABLES_VALUEUNSETCOMMAND)) {
         /* VALUE_UNSET command */
         variables.unset(args);
      } else if (MMDAgent_strequal(type, PLUGINVARIABLES_VALUEEVALCOMMAND)) {
         /* VALUE_EVAL command */
         buff = MMDAgent_strdup(args);
         p1 = MMDAgent_strtok(buff, "|", &save);
         p2 = MMDAgent_strtok(NULL, "|", &save);
         p3 = MMDAgent_strtok(NULL, "|", &save);
         variables.evaluate(p1, p2, p3);
         if(buff)
            free(buff);
      } else if (MMDAgent_strequal(type, PLUGINVARIABLES_VALUEGETCOMMAND)) {
         /* VALUE_GET command */
         variables.get(args);
      } else if (MMDAgent_strequal(type, PLUGINVARIABLES_TIMERSTARTCOMMAND)) {
         /* TIMER_START command */
         buff = MMDAgent_strdup(args);
         p1 = MMDAgent_strtok(buff, "|", &save);
         p2 = MMDAgent_strtok(NULL, "|", &save);
         countdown_thread.set(p1, p2);
         if(buff)
            free(buff);
      } else if (MMDAgent_strequal(type, PLUGINVARIABLES_TIMERSTOPCOMMAND)) {
         /* TIMER_STOP command */
         countdown_thread.unset(args);
      }
   } else {
      if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINENABLE)) {
         if(MMDAgent_strequal(args, PLUGINVARIABLES_NAME)) {
            enable = true;
            mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINENABLE, PLUGINVARIABLES_NAME);
         }
      }
   }
}

/* extAppEnd: stop and free thread */
EXPORT void extAppEnd(MMDAgent *mmdagent)
{
   countdown_thread.stopAndRelease();
}
