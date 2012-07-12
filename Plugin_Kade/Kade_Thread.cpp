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
#include "Kade_Thread.h"
#include <Python.h>

/* File Globals */
PyObject *pModule;
PyObject *pProcPlainText, *pProcParse;
bool m_pause;

/* mainThread: main thread */
static void mainThread(void *param)
{
   Kade_Thread *kade_thread = (Kade_Thread *) param;
   kade_thread->run();
}

/* Kade_Thread::initialize: initialize thread */
void Kade_Thread::initialize()
{
   m_mmdagent = NULL;

   m_thread = -1;

   m_configFile = NULL;

   pModule = NULL;
   pProcPlainText = NULL;
   pProcParse = NULL;
}

/* Kade_Thread::clear: free thread */
void Kade_Thread::clear()
{
   if(m_thread >= 0) {
      glfwWaitThread(m_thread, GLFW_WAIT);
      glfwDestroyThread(m_thread);
      glfwTerminate();
   }

   if(m_configFile != NULL)
      free(m_configFile);

   if (pProcParse)
      Py_DECREF(pProcParse);
   if (pModule)
      Py_DECREF(pModule);
   Py_Finalize();

   initialize();
}

/* Kade_Thread::Kade_Thread: thread constructor */
Kade_Thread::Kade_Thread()
{
   initialize();
}

/* Kade_Thread::~Kade_Thread: thread destructor */
Kade_Thread::~Kade_Thread()
{
   clear();
}

/* Kade_Thread::load: load models and start thread */
void Kade_Thread::load(MMDAgent *mmdagent, const char *configFile)
{
   PyObject *pName;
   char name[MMDAGENT_MAXBUFLEN];

   Py_Initialize();

   m_mmdagent = mmdagent;
   m_configFile = MMDAgent_strdup(configFile);

   PyRun_SimpleString("import sys"); 
   PyRun_SimpleString("sys.path.append(\"/home/robocep/MMDAgent/Release/AppData/Kade\")");

   sprintf(name, "%s%c%s%c%s", mmdagent->getAppDirName(), MMDAGENT_DIRSEPARATOR, "Kade",MMDAGENT_DIRSEPARATOR, "kade");
   pName = PyString_FromString("kade");

   pModule = PyImport_Import(pName);
   Py_DECREF(pName);
   if (pModule == NULL) {
      printf("Error Loading python module: %s\n", PyString_AsString(pName));
      return;
   }

   pProcParse = PyObject_GetAttrString(pModule, "procParse");
   if (pProcParse == NULL) {
      Py_DECREF(pModule);
      printf("Error Loading procParse Function.\n");
      return;
   }

   if(m_configFile == NULL) {
      Py_DECREF(pProcParse);
      Py_DECREF(pModule);
      clear();
      printf("Error Loading Config File.\n");
      return;
   }

   m_pause = false;

   /* create recognition thread
   glfwInit();
   m_thread = glfwCreateThread(mainThread, this);
   if(m_thread < 0) {
      clear();
      return;
   }
   */
}

/* Kade_Thread::run: main loop */
void Kade_Thread::run()
{
}

/* Kade_Thread::pause: pause recognition process */
void Kade_Thread::pause()
{
   m_pause = true;
}

/* Kade_Thread::resume: resume recognition process */
void Kade_Thread::resume()
{
   m_pause = false;
}

/* Kade_Thread::sendMessage: send message to MMDAgent */
void Kade_Thread::sendMessage(const char *str1, const char *str2)
{
   m_mmdagent->sendEventMessage(str1, str2);
}

char* Kade_Thread::procParse(const char *plainText, const char *parse)
{
   PyObject *pParse, *pAnswer, *pAnswerStr, *pArgs, *pPlain;

   if (!pProcParse || !PyCallable_Check(pProcParse)) {
      printf("procParse does not exists or is not callable.\n");
      if (PyErr_Occurred())
         PyErr_Print();
      return NULL;
   }

   pParse = PyString_FromString(parse);
   pPlain = PyString_FromString(plainText);
   if (pParse != NULL && pPlain != NULL) {

      pArgs = PyTuple_New(2);
      if (pArgs != NULL) {
         PyTuple_SetItem(pArgs, 0, pPlain);
         PyTuple_SetItem(pArgs, 1, pParse);
         pAnswer = PyObject_CallObject(pProcParse, pArgs);

         if (pAnswer != NULL) {
            pAnswerStr = PyObject_Str(pAnswer);
            Py_DECREF(pAnswer);
            Py_DECREF(pArgs);
            Py_DECREF(pParse);
            printf("Answer: %s\n", PyString_AsString(pAnswerStr));
            return PyString_AsString(pAnswerStr);
         }
         Py_DECREF(pArgs);
      } else
         Py_DECREF(pParse);
   }
   printf("Error\n");
   if (PyErr_Occurred())
      PyErr_Print();
   return NULL;
}

char* Kade_Thread::procPlainText(char *text)
{
   return NULL;
}

