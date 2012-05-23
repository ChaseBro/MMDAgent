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

/* DLLibrary: dynamic link library for MMDAgent */
typedef struct _DLLibrary {
   char *name;
   void *handle;

   void (*appStart)(MMDAgent *mmdagent);
   void (*appEnd)(MMDAgent *mmdagent);
   void (*procCommand)(MMDAgent *mmdagent, const char *type, const char *args);
   void (*procEvent)(MMDAgent *mmdagent, const char *type, const char *args);
   void (*update)(MMDAgent *mmdagent, double deltaFrame);
   void (*render)(MMDAgent *mmdagent);

   struct _DLLibrary *next;
} DLLibrary;

/* Plugin: plugin list */
class Plugin
{
private:

   DLLibrary *m_head;
   DLLibrary *m_tail;

   /* initialize: initialize plugin list */
   void initialize();

   /* clear: free plugin list */
   void clear();

public:

   /* Plugin: constructor */
   Plugin();

   /* ~Plugin: destructor */
   ~Plugin();

   /* load: load all DLLs in a directory */
   bool load(const char *dir);

   /* execAppStart: run when application is start */
   void execAppStart(MMDAgent *mmdagent);

   /* execAppEnd: run when application is end */
   void execAppEnd(MMDAgent *mmdagent);

   /* execProcCommand: process command message */
   void execProcCommand(MMDAgent *mmdagent, const char *type, const char *args);

   /* execProcEvent: process event message */
   void execProcEvent(MMDAgent *mmdagent, const char *type, const char *args);

   /* execUpdate: run when motion is updated */
   void execUpdate(MMDAgent *mmdagent, double deltaFrame);

   /* execRender: run when scene is rendered */
   void execRender(MMDAgent *mmdagent);
};
