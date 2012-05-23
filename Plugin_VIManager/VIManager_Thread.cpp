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

/* headers */

#include "MMDAgent.h"
#include "VIManager.h"
#include "VIManager_Logger.h"
#include "VIManager_Thread.h"

/* VIManager_Event_initialize: initialize input message buffer */
static void VIManager_Event_initialize(VIManager_Event *e, const char *type, const char *args)
{
   if (type != NULL)
      e->type = MMDAgent_strdup(type);
   else
      e->type = NULL;
   if (args != NULL)
      e->args = MMDAgent_strdup(args);
   else
      e->args = NULL;
   e->next = NULL;
}

/* VIManager_Event_clear: free input message buffer */
static void VIManager_Event_clear(VIManager_Event *e)
{
   if (e->type != NULL)
      free(e->type);
   if (e->args != NULL)
      free(e->args);
   VIManager_Event_initialize(e, NULL, NULL);
}

/* VIManager_EventQueue_initialize: initialize queue */
static void VIManager_EventQueue_initialize(VIManager_EventQueue *q)
{
   q->head = NULL;
   q->tail = NULL;
}

/* VIManager_EventQueue_clear: free queue */
static void VIManager_EventQueue_clear(VIManager_EventQueue *q)
{
   VIManager_Event *tmp1, *tmp2;

   for (tmp1 = q->head; tmp1 != NULL; tmp1 = tmp2) {
      tmp2 = tmp1->next;
      VIManager_Event_clear(tmp1);
      free(tmp1);
   }
   VIManager_EventQueue_initialize(q);
}

/* VIManager_EventQueue_enqueue: enqueue */
static void VIManager_EventQueue_enqueue(VIManager_EventQueue *q, const char *type, const char *args)
{
   if (q->tail == NULL) {
      q->tail = (VIManager_Event *) calloc(1, sizeof (VIManager_Event));
      VIManager_Event_initialize(q->tail, type, args);
      q->head = q->tail;
   } else {
      q->tail->next = (VIManager_Event *) calloc(1, sizeof (VIManager_Event));
      VIManager_Event_initialize(q->tail->next, type, args);
      q->tail = q->tail->next;
   }
}

/* VIManager_EventQueue_dequeue: dequeue */
static int VIManager_EventQueue_dequeue(VIManager_EventQueue *q, char *type, char *args)
{
   VIManager_Event *tmp;

   if (q->head == NULL) {
      if (type != NULL)
         strcpy(type, "");
      if (args != NULL)
         strcpy(type, "");
      return 0;
   }
   if (type != NULL)
      strcpy(type, q->head->type);
   if (args != NULL)
      strcpy(args, q->head->args);
   tmp = q->head->next;
   VIManager_Event_clear(q->head);
   free(q->head);
   q->head = tmp;
   if (tmp == NULL)
      q->tail = NULL;
   return 1;
}

/* mainThread: main thread */
static void mainThread(void *param)
{
   VIManager_Thread *vimanager_thread = (VIManager_Thread *) param;
   vimanager_thread->run();
}

/* VIManager_Thread::initialize: initialize thread */
void VIManager_Thread::initialize()
{
   m_sub = NULL;

   m_mmdagent = NULL;

   m_kill = false;

   m_mutex = NULL;
   m_cond = NULL;
   m_thread = -1;

   m_count = 0;

   VIManager_EventQueue_initialize(&eventQueue);
}

/* VIManager_Thread::clear: free thread */
void VIManager_Thread::clear()
{
   VIManager_Link *tmp1, *tmp2;

   m_kill = true;

   if(m_cond != NULL)
      glfwSignalCond(m_cond);

   /* stop thread & close mutex */
   if(m_mutex != NULL || m_cond != NULL || m_thread >= 0) {
      if(m_thread >= 0) {
         glfwWaitThread(m_thread, GLFW_WAIT);
         glfwDestroyThread(m_thread);
      }
      if(m_cond != NULL)
         glfwDestroyCond(m_cond);
      if(m_mutex != NULL)
         glfwDestroyMutex(m_mutex);
      glfwTerminate();
   }

   /* free */
   VIManager_EventQueue_clear(&eventQueue);

   for(tmp1 = m_sub; tmp1 != NULL; tmp1 = tmp2) {
      tmp2 = tmp1->next;
      delete tmp1;
   }

   initialize();
}

/* VIManager_Thread::VIManager_Thread: thread constructor */
VIManager_Thread::VIManager_Thread()
{
   initialize();
}

/* VIManager_Thread::~VIManager_Thread: thread destructor */
VIManager_Thread::~VIManager_Thread()
{
   clear();
}

/* VIManager_Thread::loadAndStart: load FST and start thread */
void VIManager_Thread::loadAndStart(MMDAgent *mmdagent, const char *file)
{
   DIRECTORY *dp;
   char buf[MMDAGENT_MAXBUFLEN];
   char *dir, *fst;
   VIManager_Link *l, *last = NULL;

   if(mmdagent == NULL)
      return;

   /* load FST for VIManager */
   if (m_vim.load(file) == 0)
      return;

   /* setup logger */
   m_logger.setup(mmdagent);

   m_mmdagent = mmdagent;

   /* get dir and fst */
   dir = MMDAgent_dirname(file);
   fst = MMDAgent_basename(file);

   /* load sub fst */
   dp = MMDAgent_opendir(dir);
   if(dp != NULL) {
      while(MMDAgent_readdir(dp, buf) == true) {
         if(MMDAgent_strequal(buf, fst) == false && MMDAgent_strheadmatch(buf, fst) == true && (MMDAgent_strtailmatch(buf, ".fst") == true || MMDAgent_strtailmatch(buf, ".FST") == true)) {
            l = new VIManager_Link;
            l->next = NULL;
            if(l->vim.load(buf) == 0) {
               delete l;
            } else {
               if(m_sub == NULL)
                  m_sub = l;
               else
                  last->next = l;
               last = l;
            }
         }
      }
      MMDAgent_closedir(dp);
   }

   free(dir);
   free(fst);

   /* start thread */
   glfwInit();
   m_mutex = glfwCreateMutex();
   m_cond = glfwCreateCond();
   m_thread = glfwCreateThread(mainThread, this);
   if(m_mutex == NULL || m_cond == NULL || m_thread < 0) {
      clear();
      return;
   }
}

/* VIManager_Thread::stopAndRelease: stop thread and release */
void VIManager_Thread::stopAndRelease()
{
   clear();
}

/* VIManager_Thread::run: main loop */
void VIManager_Thread::run()
{
   char itype[MMDAGENT_MAXBUFLEN];
   char iargs[MMDAGENT_MAXBUFLEN];
   char otype[MMDAGENT_MAXBUFLEN];
   char oargs[MMDAGENT_MAXBUFLEN];
   VIManager_Link *l;

   /* first epsilon step */
   while (m_logger.setTransition(m_vim.transition(VIMANAGER_EPSILON, NULL, otype, oargs)) == true) {
      if (MMDAgent_strequal(otype, VIMANAGER_EPSILON) == false)
         m_mmdagent->sendCommandMessage(otype, oargs);
   }

   for(l = m_sub; l != NULL; l = l->next) {
      while (l->vim.transition(VIMANAGER_EPSILON, NULL, otype, oargs) != NULL) {
         if (MMDAgent_strequal(otype, VIMANAGER_EPSILON) == false)
            m_mmdagent->sendCommandMessage(otype, oargs);
      }
   }

   while(m_kill == false) {
      /* wait transition event */
      glfwLockMutex(m_mutex);
      while(m_count <= 0) {
         glfwWaitCond(m_cond, m_mutex, GLFW_INFINITY);
         if(m_kill == true)
            return;
      }
      VIManager_EventQueue_dequeue(&eventQueue, itype, iargs);
      m_count--;
      glfwUnlockMutex(m_mutex);

      /* state transition with input symbol */
      m_logger.setTransition(m_vim.transition(itype, iargs, otype, oargs));
      if (MMDAgent_strequal(otype, VIMANAGER_EPSILON) == false)
         m_mmdagent->sendCommandMessage(otype, oargs);

      /* state transition with epsilon */
      while (m_logger.setTransition(m_vim.transition(VIMANAGER_EPSILON, NULL, otype, oargs)) == true) {
         if (MMDAgent_strequal(otype, VIMANAGER_EPSILON) == false)
            m_mmdagent->sendCommandMessage(otype, oargs);
      }

      for(l = m_sub; l != NULL; l = l->next) {
         l->vim.transition(itype, iargs, otype, oargs);
         if (MMDAgent_strequal(otype, VIMANAGER_EPSILON) == false)
            m_mmdagent->sendCommandMessage(otype, oargs);

         /* state transition with epsilon */
         while (l->vim.transition(VIMANAGER_EPSILON, NULL, otype, oargs) != NULL) {
            if (MMDAgent_strequal(otype, VIMANAGER_EPSILON) == false)
               m_mmdagent->sendCommandMessage(otype, oargs);
         }
      }
   }
}

/* VIManager_Thread::isRunning: check running */
bool VIManager_Thread::isRunning()
{
   if (m_kill == true || m_mutex == NULL || m_cond == NULL || m_thread < 0)
      return false;
   else
      return true;
}

/* VIManager_Thread::enqueueBuffer: enqueue buffer to check */
void VIManager_Thread::enqueueBuffer(const char *type, const char *args)
{
   /* wait buffer */
   glfwLockMutex(m_mutex);

   /* save event */
   VIManager_EventQueue_enqueue(&eventQueue, type, args);
   m_count++;

   /* start state transition thread */
   if(m_count <= 1)
      glfwSignalCond(m_cond);

   /* release buffer */
   glfwUnlockMutex(m_mutex);
}

/* VIManager_Thread::renderLog: render log message */
void VIManager_Thread::renderLog()
{
   m_logger.render(m_vim.getCurrentState());
}
