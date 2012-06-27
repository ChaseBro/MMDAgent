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

#define SPHINXTHREAD_LATENCY    50
#define SPHINXTHREAD_EVENTSTART "RECOG_EVENT_START"
#define SPHINXTHREAD_EVENTSTOP  "RECOG_EVENT_STOP"

/* Sphinx_Thead: thread for Sphinx */
class Sphinx_Thread
{
private:

   Sphinx *m_recog; /* Recognizer instance */
   cmd_ln_t *m_config; /* Configuration instance */

   MMDAgent *m_mmdagent;

   GLFWthread m_thread; /* thread */

   char *m_languageModel;
   char *m_dictionary;
   char *m_acousticModel;
   char *m_configFile;
   char *m_userDictionary;

   //Sphinx_Logger m_logger;

   /* initialize: initialize thread */
   void initialize();

   /* clear: free thread */
   void clear();

public:

   /* Sphinx_Thread: thread constructor */
   Sphinx_Thread();

   /* ~Sphinx_Thread: thread destructor  */
   ~Sphinx_Thread();

   /* loadAndStart: load models and start thread */
   void loadAndStart(MMDAgent *m_mmdagent, const char *languageModel, const char *dictionary, const char *acousticModel, const char *configFile, const char *userDictionary);

   /* stopAndRlease: stop thread and release sphinx */
   void stopAndRelease();

   /* run: main loop */
   void run();

   /* pause: pause recognition process */
   void pause();

   /* resume: resume recognition process */
   void resume();

   /* sendMessage: send message to MMDAgent */
   void sendMessage(const char *str1, const char *str2);

   /* getLogActiveFlag: get active flag of logger */
   bool getLogActiveFlag();

   /* setLogActiveFlag: set active flag of logger */
   void setLogActiveFlag(bool b);

   /* updateLog: update log view per step */
   void updateLog(double frame);

   /* renderLog: render log view */
   void renderLog();
};
