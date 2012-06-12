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

#define FLITEMANAGER_INITIALNTHREAD 1 /* initial number of thread */

/* Flite_Link: thread list for Open JTalk */
typedef struct _Flite_Link {
   Flite_Thread flite_thread;
   struct _Flite_Link *next;
} Flite_Link;

/* Flite_Event: input message buffer */
typedef struct _Flite_Event {
   char *event;
   struct _Flite_Event *next;
} Flite_Event;

/* Flite_EventQueue: queue of Flite_Event */
typedef struct _Flite_EventQueue {
   Flite_Event *head;
   Flite_Event *tail;
} Flite_EventQueue;

/* Flite_Manager: multi thread manager for Open JTalk */
class Flite_Manager
{
private:

   MMDAgent *m_mmdagent;

   GLFWmutex m_mutex;
   GLFWcond m_cond;
   GLFWthread m_thread;

   int m_count;

   bool m_kill;

   Flite_EventQueue m_bufferQueue;
   Flite_Link *m_list;

   char *m_config;

   /* initialize: initialize */
   void initialize();

   /* clear: clear */
   void clear();

public:

   /* Flite_Manager: constructor */
   Flite_Manager();

   /* ~Flite_Manager: destructor */
   ~Flite_Manager();

   /* loadAndStart: load and start thread */
   void loadAndStart(MMDAgent *mmdagent, const char *config);

   /* stopAndRelease: stop and release thread */
   void stopAndRelease();

   /* run: main loop */
   void run();

   /* isRunning: check running */
   bool isRunning();

   /* synthesis: start synthesis */
   void synthesis(const char *str);

   /* stop: stop synthesis */
   void stop(const char *str);
};
