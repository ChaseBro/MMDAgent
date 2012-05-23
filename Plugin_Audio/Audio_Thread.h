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

#define AUDIOTHREAD_ENDSLEEPMS   200                 /* check per 0.2 sec at end */
#define AUDIOTHREAD_STARTSLEEPMS 20                  /* check per 0.02 sec at start*/
#define AUDIOTHREAD_EVENTSTART   "SOUND_EVENT_START"
#define AUDIOTHREAD_EVENTSTOP    "SOUND_EVENT_STOP"

/* Audio_Thread: thread for audio */
class Audio_Thread
{
private:

   MMDAgent *m_mmdagent;

   GLFWmutex m_mutex;
   GLFWcond m_cond;
   GLFWthread m_thread;

   int m_count;

   bool m_playing;
   bool m_kill;

   char *m_alias;
   char *m_file;

   /* initialize: initialize thread */
   void initialize();

   /* clear: free thread */
   void clear();

public:

   /* Audio_Thraed: thread constructor */
   Audio_Thread();

   /* ~Audio_Thread: thread destructor */
   ~Audio_Thread();

   /* setupAndStart: setup audio and start thread */
   void setupAndStart(MMDAgent *mmdagent);

   /* stopAndRelease: stop thread and free audio */
   void stopAndRelease();

   /* run: main thread loop for audio */
   void run();

   /* isRunning: check running */
   bool isRunning();

   /* isPlaying: check playing */
   bool isPlaying();

   /* checkAlias: check playing alias */
   bool checkAlias(const char *alias);

   /* play: start playing */
   void play(const char *alias, const char *file);

   /* stop: stop playing */
   void stop();
};
