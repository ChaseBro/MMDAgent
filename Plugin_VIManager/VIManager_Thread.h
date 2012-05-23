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

/* VIManager_Event: input message buffer */
typedef struct _VIManager_Event {
   char *type;
   char *args;
   struct _VIManager_Event *next;
} VIManager_Event;

/* VIManager_EventQueue: queue of VIManager_Event */
typedef struct _VIManager_EventQueue {
   VIManager_Event *head;
   VIManager_Event *tail;
} VIManager_EventQueue;

typedef struct _VIManager_Link {
   VIManager vim;
   struct _VIManager_Link *next;
} VIManager_Link;

/* VIManager_Thread: thread of VIManager */
class VIManager_Thread
{
private:

   MMDAgent *m_mmdagent;

   GLFWmutex m_mutex;
   GLFWcond m_cond;
   GLFWthread m_thread;

   int m_count;

   bool m_kill;

   VIManager_EventQueue eventQueue; /* queue of input message */

   VIManager m_vim;           /* main FST */
   VIManager_Link *m_sub;     /* sub FST */
   VIManager_Logger m_logger; /* logger */

   /* initialize: initialize thread */
   void initialize();

   /* clear: free thread */
   void clear();

public:

   /* VIManager_Thraed: thread constructor */
   VIManager_Thread();

   /* ~VIManager_Thread: thread destructor */
   ~VIManager_Thread();

   /* loadAndStart: load FST and start thread */
   void loadAndStart(MMDAgent *mmdagent, const char *file);

   /* stopAndRelease: stop thread and release */
   void stopAndRelease();

   /* run: main loop */
   void run();

   /* isRunning: check running */
   bool isRunning();

   /* enqueueBuffer: enqueue buffer to check */
   void enqueueBuffer(const char *type, const char *args);

   /* renderLog: render log message */
   void renderLog();
};
