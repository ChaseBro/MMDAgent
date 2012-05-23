/* ----------------------------------------------------------------- */
/*           Toolkit for Building Voice Interaction Systems          */
/*           MMDAgent developed by MMDAgent Project Team             */
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

#include "MMDAgent.h"

/* MessageLink_initialize: initialize message */
static void MessageLink_initialize(MessageLink *l, const char *type, const char *value)
{
   l->type = MMDAgent_strdup(type);
   l->value = MMDAgent_strdup(value);
   l->next = NULL;
}

/* MessageLink_clear: free message */
static void MessageLink_clear(MessageLink *l)
{
   if(l->type != NULL)
      free(l->type);
   if(l->value != NULL)
      free(l->value);
}

/* MessageQueue_initialize: initialize message list */
static void MessageQueue_initialize(MessageQueue *q)
{
   q->head = NULL;
   q->tail = NULL;
}

/* MessageQueue_clear: free message list */
static void MessageQueue_clear(MessageQueue *q)
{
   MessageLink *tmp1, *tmp2;

   for(tmp1 = q->head; tmp1 ; tmp1 = tmp2) {
      tmp2 = tmp1->next;
      MessageLink_clear(tmp1);
      free(tmp1);
   }
}

/* MessageQueue_enqueue: enqueue */
static void MessageQueue_enqueue(MessageQueue *q, const char *type, const char *value)
{
   if(MMDAgent_strlen(type) <= 0)
      return;

   if(q->tail == NULL) {
      q->tail = (MessageLink *) malloc(sizeof(MessageLink));
      MessageLink_initialize(q->tail, type, value);
      q->head = q->tail;
   } else {
      q->tail->next = (MessageLink *) malloc(sizeof(MessageLink));
      MessageLink_initialize(q->tail->next, type, value);
      q->tail = q->tail->next;
   }
}

/* MessageQueue_dequeue: dequeue */
static bool MessageQueue_dequeue(MessageQueue *q, char *type, char *value)
{
   MessageLink *tmp;

   if(q->head == NULL) {
      strcpy(type, "");
      if(value != NULL)
         strcpy(value, "");
      return false;
   }

   strcpy(type, q->head->type);
   if(q->head->value != NULL && value != NULL)
      strcpy(value, q->head->value);

   tmp = q->head->next;
   MessageLink_clear(q->head);
   free(q->head);
   q->head = tmp;
   if(tmp == NULL)
      q->tail = NULL;

   return true;
}

/* Message::initialize: initialize message queue */
void Message::initialize()
{
   m_mutexCommand = NULL;
   m_mutexEvent = NULL;
   m_mutexLog = NULL;

   MessageQueue_initialize(&m_queueCommand);
   MessageQueue_initialize(&m_queueEvent);
   MessageQueue_initialize(&m_queueLog);
}

/* Message::clear: free message queue */
void Message::clear()
{
   if(m_mutexCommand)
      glfwDestroyMutex(m_mutexCommand);
   if(m_mutexEvent)
      glfwDestroyMutex(m_mutexEvent);
   if(m_mutexLog)
      glfwDestroyMutex(m_mutexLog);

   MessageQueue_clear(&m_queueCommand);
   MessageQueue_clear(&m_queueEvent);
   MessageQueue_clear(&m_queueLog);

   initialize();
}

/* Message::Message: constructor */
Message::Message()
{
   initialize();
}

/* Message::~Message: destructor */
Message::~Message()
{
   clear();
}

/* Message::setup: setup message queue */
bool Message::setup()
{
   clear();

   m_mutexCommand = glfwCreateMutex();
   m_mutexEvent = glfwCreateMutex();
   m_mutexLog = glfwCreateMutex();

   if(m_mutexCommand == NULL || m_mutexEvent == NULL || m_mutexLog == NULL) {
      clear();
      return false;
   }

   return true;
}

/* Message::enqueueCommnd: enqueue command message */
void Message::enqueueCommand(const char *type, const char *value)
{
   glfwLockMutex(m_mutexCommand);
   MessageQueue_enqueue(&m_queueCommand, type, value);
   glfwUnlockMutex(m_mutexCommand);
}

/* Message::enqueueEvent: enqueue event message */
void Message::enqueueEvent(const char *type, const char *value)
{
   glfwLockMutex(m_mutexEvent);
   MessageQueue_enqueue(&m_queueEvent, type, value);
   glfwUnlockMutex(m_mutexEvent);
}

/* Message::enqueueLog: enqueue log message */
void Message::enqueueLog(const char *log)
{
   glfwLockMutex(m_mutexLog);
   MessageQueue_enqueue(&m_queueLog, log, NULL);
   glfwUnlockMutex(m_mutexLog);
}

/* Message::dequeueCommand: dequeue command message */
bool Message::dequeueCommand(char *type, char *value)
{
   bool result;
   glfwLockMutex(m_mutexCommand);
   result = MessageQueue_dequeue(&m_queueCommand, type, value);
   glfwUnlockMutex(m_mutexCommand);
   return result;
}

/* Message::dequeueEvent: dequeue event message */
bool Message::dequeueEvent(char *type, char *value)
{
   bool result;
   glfwLockMutex(m_mutexEvent);
   result = MessageQueue_dequeue(&m_queueEvent, type, value);
   glfwUnlockMutex(m_mutexEvent);
   return result;
}

/* Message::dequeueLog: dequeue log message */
bool Message::dequeueLog(char *log)
{
   bool result;
   glfwLockMutex(m_mutexLog);
   result = MessageQueue_dequeue(&m_queueLog, log, NULL);
   glfwUnlockMutex(m_mutexLog);
   return result;
}
