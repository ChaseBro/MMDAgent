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

#define PLUGINAUDIO_NAME         "Audio"
#define PLUGINAUDIO_DEFAULTALIAS "audio"
#define PLUGINAUDIO_STARTCOMMAND "SOUND_START"
#define PLUGINAUDIO_STOPCOMMAND  "SOUND_STOP"

/* headers */

#include "MMDAgent.h"
#include "Audio_Thread.h"
#include "Audio_Manager.h"

/* variables */

static Audio_Manager audio_manager;
static bool enable;
static char *drop_motion;

/* extAppStart: setup and start thread */
EXPORT void extAppStart(MMDAgent *mmdagent)
{
   audio_manager.setupAndStart(mmdagent);

   enable = true;
   drop_motion = NULL;
   mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINENABLE, PLUGINAUDIO_NAME);
}

/* extProcCommand: process command message */
EXPORT void extProcCommand(MMDAgent *mmdagent, const char *type, const char *args)
{
   if(enable == true) {
      if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINDISABLE)) {
         if(MMDAgent_strequal(args, PLUGINAUDIO_NAME)) {
            enable = false;
            mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINDISABLE, PLUGINAUDIO_NAME);
         }
      } else if (audio_manager.isRunning()) {
         if (MMDAgent_strequal(type, PLUGINAUDIO_STARTCOMMAND)) {
            audio_manager.play(args);
         } else if (MMDAgent_strequal(type, PLUGINAUDIO_STOPCOMMAND)) {
            audio_manager.stop(args);
         }
      }
   } else {
      if(MMDAgent_strequal(type, MMDAGENT_COMMAND_PLUGINENABLE)) {
         if(MMDAgent_strequal(args, PLUGINAUDIO_NAME)) {
            enable = true;
            mmdagent->sendEventMessage(MMDAGENT_EVENT_PLUGINENABLE, PLUGINAUDIO_NAME);
         }
      }
   }
}

/* extProcEvent: process event message */
EXPORT void extProcEvent(MMDAgent *mmdagent, const char *type, const char *args)
{
   int i;
   FILE *fp;
   char *buf, *p, *q;
   PMDObject *objs;
   MotionPlayer *motionPlayer;

   if(enable == true) {
      if(MMDAgent_strequal(type, MMDAGENT_EVENT_DRAGANDDROP)) {
         buf = MMDAgent_strdup(args);
         p = MMDAgent_strtok(buf, "|", &q);
         if(MMDAgent_strtailmatch(p, ".mp3") || MMDAgent_strtailmatch(p, ".MP3")) {
            /* if there is a motion file which have the same name, store it */
            if(drop_motion != NULL)
               free(drop_motion);
            drop_motion = MMDAgent_strdup(p);
            i = MMDAgent_strlen(drop_motion);
            drop_motion[i-4] = '.';
            drop_motion[i-3] = 'v';
            drop_motion[i-2] = 'm';
            drop_motion[i-1] = 'd';
            fp = MMDAgent_fopen(drop_motion, "rb");
            if(fp != NULL) {
               fclose(fp);
            } else {
               free(drop_motion);
               drop_motion = NULL;
            }
            /* start mp3 */
            audio_manager.stop(PLUGINAUDIO_DEFAULTALIAS);
            q = (char *) malloc(sizeof(char) * (strlen(PLUGINAUDIO_DEFAULTALIAS) + 1 + strlen(p) + 1));
            sprintf(q, "%s|%s", PLUGINAUDIO_DEFAULTALIAS, p);
            audio_manager.play(q);
            free(q);
         }
         if(buf)
            free(buf);
      } else if(MMDAgent_strequal(type, AUDIOTHREAD_EVENTSTART)) {
         if(drop_motion != NULL) {
            if(MMDAgent_strequal(args, PLUGINAUDIO_DEFAULTALIAS) == true) {
               objs = mmdagent->getModelList();
               for (i = 0; i < mmdagent->getNumModel(); i++) {
                  if (objs[i].isEnable() == true && objs[i].allowMotionFileDrop() == true) {
                     for (motionPlayer = objs[i].getMotionManager()->getMotionPlayerList(); motionPlayer; motionPlayer = motionPlayer->next) {
                        if (motionPlayer->active == true && MMDAgent_strequal(motionPlayer->name, "base") == true) {
                           mmdagent->sendCommandMessage(MMDAGENT_COMMAND_MOTIONCHANGE, "%s|%s|%s", objs[i].getAlias(), "base", drop_motion);
                           break;
                        }
                     }
                     if (!motionPlayer)
                        mmdagent->sendCommandMessage(MMDAGENT_COMMAND_MOTIONADD, "%s|%s|%s|FULL|ONCE|ON|ON", objs[i].getAlias(), "base", drop_motion);
                  }
               }
               mmdagent->resetAdjustmentTimer();
            }
            free(drop_motion);
            drop_motion = NULL;
         }
      }
   }
}

/* extAppEnd: stop and free thread */
EXPORT void extAppEnd(MMDAgent * mmdagent)
{
   if(drop_motion != NULL)
      free(drop_motion);
   audio_manager.stopAndRelease();
}
