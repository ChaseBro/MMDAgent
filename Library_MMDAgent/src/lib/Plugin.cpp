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

/* DLLibrary_initialize: initialize dynamic link library */
void DLLibrary_initialize(DLLibrary *d)
{
   d->name = NULL;
   d->handle = NULL;

   d->appStart = NULL;
   d->appEnd = NULL;
   d->procCommand = NULL;
   d->procEvent = NULL;
   d->update = NULL;
   d->render = NULL;

   d->next = NULL;
}

/* DLLibrary_clear: free dynamic link library */
void DLLibrary_clear(DLLibrary *d)
{
   if(d->handle != NULL)
      MMDAgent_dlclose(d->handle);
   if (d->name)
      free(d->name);

   DLLibrary_initialize(d);
}

/* DLLibrary_load: load dynamic link library */
bool DLLibrary_load(DLLibrary *d, const char *dir, const char *file)
{
   char *buf;

   if(d == NULL || dir == NULL || file == NULL) return false;
   DLLibrary_clear(d);

   /* open */
   buf = (char *) malloc(sizeof(char) * (MMDAgent_strlen(dir) + 1 + MMDAgent_strlen(file) + 1));
   sprintf(buf, "%s%c%s", dir, MMDAGENT_DIRSEPARATOR, file);
   d->handle = MMDAgent_dlopen(buf);
   free(buf);
   if (!d->handle)
      return false;

   /* set function pointers */
   d->appStart = (void (*)(MMDAgent *)) MMDAgent_dlsym(d->handle, "extAppStart");
   d->appEnd = (void (*)(MMDAgent *)) MMDAgent_dlsym(d->handle, "extAppEnd");
   d->procCommand = (void (*)(MMDAgent *, const char *, const char *)) MMDAgent_dlsym(d->handle, "extProcCommand");
   d->procEvent = (void (*)(MMDAgent *, const char *, const char *)) MMDAgent_dlsym(d->handle, "extProcEvent");
   d->update = (void (*)(MMDAgent *, double)) MMDAgent_dlsym(d->handle, "extUpdate");
   d->render = (void (*)(MMDAgent *)) MMDAgent_dlsym(d->handle, "extRender");

   if (d->appStart || d->appEnd || d->procCommand || d->procEvent || d->update || d->render) {
      /* save file name */
      d->name = MMDAgent_strdup(file);
      return true;
   } else {
      /* if none, exit */
      DLLibrary_clear(d);
      return false;
   }
}

/* Plugin::initialize: initialize plugin list */
void Plugin::initialize()
{
   m_head = NULL;
   m_tail = NULL;
}

/* Plugin::clear: free plugin list */
void Plugin::clear()
{
   DLLibrary *d1, *d2;

   for (d1 = m_head; d1; d1 = d2) {
      d2 = d1->next;
      DLLibrary_clear(d1);
      free(d1);
   }

   initialize();
}

/* Plugin::Plugin: constructor */
Plugin::Plugin()
{
   initialize();
}

/* Plugin::~Plugin: destructor */
Plugin::~Plugin()
{
   clear();
}

/* Plugin::load: load all DLLs in a directory */
bool Plugin::load(const char *dir)
{
   DIRECTORY *dp;
   char buf[MMDAGENT_MAXBUFLEN];
   bool ret = false;
   DLLibrary *d;

   if(dir == NULL)
      return false;

   /* search file */
   dp = MMDAgent_opendir(dir);
   if(dp == NULL)
      return false;

   /* add */
   while(MMDAgent_readdir(dp, buf) == true) {
      if(MMDAgent_strtailmatch(buf, ".dll") == true || MMDAgent_strtailmatch(buf, ".DLL") == true || MMDAgent_strtailmatch(buf, ".so") == true || MMDAgent_strtailmatch(buf, ".SO") == true) {
         d = (DLLibrary *) malloc(sizeof(DLLibrary));
         DLLibrary_initialize(d);
         if(DLLibrary_load(d, dir, buf) == false) {
            free(d);
         } else {
            if(m_tail == NULL)
               m_head = d;
            else
               m_tail->next = d;
            m_tail = d;
            ret = true;
         }
      }
   }

   /* end */
   MMDAgent_closedir(dp);

   return ret;
}

/* Plugin::execAppStart: run when application is start */
void Plugin::execAppStart(MMDAgent *mmdagent)
{
   DLLibrary *d;

   for (d = m_head; d; d = d->next)
      if (d->appStart != NULL)
         d->appStart(mmdagent);
}

/* Plugin::execAppEnd: run when application is end */
void Plugin::execAppEnd(MMDAgent *mmdagent)
{
   DLLibrary *d;

   for (d = m_head; d; d = d->next)
      if (d->appEnd != NULL)
         d->appEnd(mmdagent);
}

/* Plugin::execProcCommand: process command message */
void Plugin::execProcCommand(MMDAgent *mmdagent, const char *type, const char *args)
{
   DLLibrary *d;

   for (d = m_head; d; d = d->next)
      if (d->procCommand != NULL)
         d->procCommand(mmdagent, type, args);
}

/* Plugin::execProcEvent: process event message */
void Plugin::execProcEvent(MMDAgent *mmdagent, const char *type, const char *args)
{
   DLLibrary *d;

   for (d = m_head; d; d = d->next)
      if (d->procEvent != NULL)
         d->procEvent(mmdagent, type, args);
}

/* Plugin::execUpdate: run when motion is updated */
void Plugin::execUpdate(MMDAgent *mmdagent, double deltaFrame)
{
   DLLibrary *d;

   for (d = m_head; d; d = d->next)
      if (d->update != NULL)
         d->update(mmdagent, deltaFrame);
}

/* Plugin::execRender: run when scene is rendered */
void Plugin::execRender(MMDAgent *mmdagent)
{
   DLLibrary *d;

   for (d = m_head; d; d = d->next)
      if (d->render != NULL)
         d->render(mmdagent);
}
