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

#include <stdarg.h>
#include "MMDAgent.h"

/* LogText::initialize: initialize logger */
void LogText::initialize()
{
   m_textRenderer = NULL;

   m_textWidth = 0;
   m_textHeight = 0;
   m_textX = 0.0;
   m_textY = 0.0;
   m_textZ = 0.0;
   m_textScale = 0.0;

   m_textList = NULL;
   m_displayList = NULL;
   m_length = NULL;
   m_updated = NULL;
}

/* LogText::clear: free logger */
void LogText::clear()
{
   int i;

   if (m_textList) {
      for (i = 0; i < m_textHeight; i++)
         free(m_textList[i]);
      free(m_textList);
   }
   if (m_displayList) {
      for (i = 0; i < m_textHeight; i++)
         free(m_displayList[i]);
      free(m_displayList);
   }
   if (m_length)
      free(m_length);
   if (m_updated)
      free(m_updated);
   initialize();
}

/* LogText::LogText: constructor */
LogText::LogText()
{
   initialize();
}

/* LogText::~LogText: destructor */
LogText::~LogText()
{
   clear();
}

/* LogText::setup: initialize and setup logger with args */
void LogText::setup(TextRenderer *text, const int *size, const float *position, float scale)
{
   int i;

   if (text == NULL || size[0] <= 0 || size[1] <= 0 || scale <= 0.0) return;

   clear();

   m_textRenderer = text;

   m_textWidth = size[0];
   m_textHeight = size[1];
   m_textX = position[0];
   m_textY = position[1];
   m_textZ = position[2];
   m_textScale = scale;

   m_textList = (char **) malloc(sizeof(char *) * m_textHeight);
   for (i = 0; i < m_textHeight; i++) {
      m_textList[i] = (char *) malloc(sizeof(char) * m_textWidth);
      strcpy(m_textList[i], "");
   }

   m_displayList = (unsigned int **) malloc(sizeof(unsigned int *) * m_textHeight);
   for (i = 0; i < m_textHeight; i++)
      m_displayList[i] = (unsigned int *) malloc(sizeof(unsigned int) * m_textWidth);

   m_length = (int *) malloc(sizeof(int) * m_textHeight);
   for (i = 0; i < m_textHeight; i++)
      m_length[i] = -1;

   m_updated = (bool *) malloc(sizeof(bool) * m_textHeight);
   for (i = 0; i < m_textHeight; i++)
      m_updated[i] = false;

   m_textLine = 0;
}

/* LogText::log: store log text */
void LogText::log(const char *format, ...)
{
   char *p, *save;
   char buff[LOGTEXT_MAXBUFLEN];
   va_list args;

   if (m_textList == NULL) return;

   va_start(args, format);
   vsprintf(buff, format, args);
   for (p = MMDAgent_strtok(buff, "\n", &save); p; p = MMDAgent_strtok(NULL, "\n", &save)) {
      strncpy(m_textList[m_textLine], p, m_textWidth - 1);
      m_textList[m_textLine][m_textWidth-1] = '\0';
      m_updated[m_textLine] = true;
      if (++m_textLine >= m_textHeight)
         m_textLine = 0;
   }
   va_end(args);
}

/* LogText::render: render log text */
void LogText::render()
{
   int i, j;
   float x, y, z, w, h;

   if (m_textList == NULL) return;

   x = m_textX;
   y = m_textY;
   z = m_textZ;
   w = 0.5f * (float) (m_textWidth) * 0.85f + 1.0f;
   h = 1.0f * (float) (m_textHeight) * 0.85f + 1.0f;

   glPushMatrix();
   glDisable(GL_CULL_FACE);
   glDisable(GL_LIGHTING);
   glScalef(m_textScale, m_textScale, m_textScale);
   glNormal3f(0.0f, 1.0f, 0.0f);
   glColor4f(LOGTEXT_BGCOLOR);
   glBegin(GL_QUADS);
   glVertex3f(x    , y    , z);
   glVertex3f(x + w, y    , z);
   glVertex3f(x + w, y + h, z);
   glVertex3f(x    , y + h, z);
   glEnd();
   glTranslatef(x + 0.5f, y + h - 0.4f, z + 0.05f);
   for (i = 0; i < m_textHeight; i++) {
      glTranslatef(0.0f, -0.85f, 0.0f);
      j = m_textLine + i;
      if (j >= m_textHeight)
         j -= m_textHeight;
      if (MMDAgent_strlen(m_textList[j]) > 0) {
         glColor4f(LOGTEXT_COLOR);
         glPushMatrix();
      #ifdef _WIN32
         if (m_updated[j]) {
            /* cache display list array */
            m_length[j] = m_textRenderer->getDisplayListArrayOfString(m_textList[j], m_displayList[j], m_textWidth);
            m_updated[j] = false;
         }
         if (m_length[j] >= 0)
            m_textRenderer->renderDisplayListArrayOfString(m_displayList[j], m_length[j]);
      #else
         m_textRenderer->drawString(m_textList[j]);
      #endif /* _WIN32 */
         glPopMatrix();
      }
   }
   glEnable(GL_LIGHTING);
   glEnable(GL_CULL_FACE);
   glPopMatrix();
}
