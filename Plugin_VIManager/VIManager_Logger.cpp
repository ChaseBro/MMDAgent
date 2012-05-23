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

/* VIManager_Logger::initialize: initialize logger */
void VIManager_Logger::initialize()
{
   int i;

   m_mmdagent = NULL;
   for(i = 0; i < VIMANAGERLOGGER_TEXTHEIGHT + 1; i++)
      m_history[i] = NULL;
}

/* VIManager_Logger::clear: free logger */
void VIManager_Logger::clear()
{
   initialize();
}

/* VIManager_Logger::VIManager_Logger: constructor */
VIManager_Logger::VIManager_Logger()
{
   initialize();
}

/* VIManager_Logger::~VIManager_Logger: destructor */
VIManager_Logger::~VIManager_Logger()
{
   clear();
}

/* VIManager_Logger::setup: setup logger */
void VIManager_Logger::setup(MMDAgent *mmdagent)
{
   m_mmdagent = mmdagent;
}

/* VIManager_Logger::setTransition: store state transition */
bool VIManager_Logger::setTransition(VIManager_Arc *arc)
{
   int i;
   VIManager_Arc *tmp1, *tmp2;

   if(arc == NULL)
      return false;

   tmp2 = arc;
   for(i = 0; i < VIMANAGERLOGGER_TEXTHEIGHT + 1; i++) {
      tmp1 = m_history[i];
      m_history[i] = tmp2;
      tmp2 = tmp1;
      if(tmp2 == NULL)
         break;
   }

   return true;
}

void VIManager_Logger::drawArc(unsigned int from, VIManager_Arc *arc)
{
   int i;
   static char buf1[MMDAGENT_MAXBUFLEN], buf2[MMDAGENT_MAXBUFLEN]; /* static buffer */

   strcpy(buf1, arc->input_event_type);
   for (i = 0; i < arc->input_event_argc; i++) {
      strcat(buf1, "|");
      strcat(buf1, arc->input_event_args[i]);
   }
   if (MMDAgent_strlen(arc->output_command_args) >= 0)
      sprintf(buf2, "%d %d %s %s|%s", from, arc->next_state->number, buf1, arc->output_command_type, arc->output_command_args);
   else
      sprintf(buf2, "%d %d %s %s", from, arc->next_state->number, buf1, arc->output_command_type);
   m_mmdagent->drawString(buf2);
}

/* VIManager_Logger::render: render log */
void VIManager_Logger::render(VIManager_State *currentState)
{
   int i;
   VIManager_Arc *arc;
   VIManager_Arc *prevArc;

   if(m_mmdagent == NULL || currentState == NULL)
      return;

   /* start of draw */
   glPushMatrix();
   glDisable(GL_CULL_FACE);
   glDisable(GL_LIGHTING);

   /* show the history */
   glPushMatrix();
   glTranslatef(VIMANAGERLOGGER_POSITION1);
   glRotatef(VIMANAGERLOGGER_ROTATE1);
   glScalef(VIMANAGERLOGGER_SCALE, VIMANAGERLOGGER_SCALE, VIMANAGERLOGGER_SCALE);
   glColor4f(VIMANAGERLOGGER_BGCOLOR1);
   glBegin(GL_QUADS);
   glVertex3f(0.0f, 0.0f, 0.0f);
   glVertex3f(VIMANAGERLOGGER_WIDTH1, 0.0f, 0.0f);
   glVertex3f(VIMANAGERLOGGER_WIDTH1, VIMANAGERLOGGER_HEIGHT1, 0.0f);
   glVertex3f(0.0f, VIMANAGERLOGGER_HEIGHT1, 0.0f);
   glEnd();
   glTranslatef(0.5f, VIMANAGERLOGGER_LINESTEP * 0.7f, 0.01f);
   glColor4f(VIMANAGERLOGGER_TEXTCOLOR1);
   for(i = 0; i < VIMANAGERLOGGER_TEXTHEIGHT; i++) {
      arc = m_history[i];
      if(arc == NULL) break;
      prevArc = m_history[i+1];
      glPushMatrix();
      drawArc(prevArc == NULL ? VIMANAGER_STARTSTATE : prevArc->next_state->number, arc);
      glPopMatrix();
      glTranslatef(0.0f, VIMANAGERLOGGER_LINESTEP, 0.0f);
   }
   glPopMatrix();

   /* show the future */
   glPushMatrix();
   glTranslatef(VIMANAGERLOGGER_POSITION2);
   glRotatef(VIMANAGERLOGGER_ROTATE2);
   glScalef(VIMANAGERLOGGER_SCALE, VIMANAGERLOGGER_SCALE, VIMANAGERLOGGER_SCALE);
   glColor4f(VIMANAGERLOGGER_BGCOLOR2);
   glBegin(GL_QUADS);
   glVertex3f(0.0f, 0.0f, 0.0f);
   glVertex3f(VIMANAGERLOGGER_WIDTH2, 0.0f, 0.0f);
   glVertex3f(VIMANAGERLOGGER_WIDTH2, VIMANAGERLOGGER_HEIGHT2, 0.0f);
   glVertex3f(0.0f, VIMANAGERLOGGER_HEIGHT2, 0.0f);
   glEnd();
   glTranslatef(0.5f, VIMANAGERLOGGER_HEIGHT2 - VIMANAGERLOGGER_LINESTEP * 1.2f, 0.01f);
   glColor4f(VIMANAGERLOGGER_TEXTCOLOR2);
   for(i = 0, arc = currentState->arc_list.head; i < VIMANAGERLOGGER_TEXTHEIGHT && arc != NULL; i++, arc = arc->next) {
      glPushMatrix();
      drawArc(currentState->number, arc);
      glPopMatrix();
      glTranslatef(0.0f, -VIMANAGERLOGGER_LINESTEP, 0.0f);
   }
   glPopMatrix();

   /* end of draw */
   glEnable(GL_LIGHTING);
   glEnable(GL_CULL_FACE);
   glPopMatrix();
}
