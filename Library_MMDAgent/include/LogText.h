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

#define LOGTEXT_MAXBUFLEN 2048
#define LOGTEXT_COLOR     1.0f,0.7f,0.0f,0.7f /* text color */
#define LOGTEXT_BGCOLOR   0.0f,0.0f,0.0f,0.8f /* background color */

/* LogText: log text area behind character */
class LogText
{
private:

   TextRenderer *m_textRenderer; /* link of text renderer */

   int m_textHeight;
   int m_textWidth;
   float m_textX;
   float m_textY;
   float m_textZ;
   float m_textScale;

   char **m_textList;            /* text list */
   unsigned int **m_displayList; /* display list index for rendering */
   int *m_length;                /* length of each line */
   bool *m_updated;              /* true when line is uploaded */
   int m_textLine;               /* current position */

   /* LogText: initialize logger */
   void initialize();

   /* LogText: free logger */
   void clear();

public:

   /* LogText: constructor */
   LogText();

   /* ~LogText: destructor */
   ~LogText();

   /* setup: initialize and setup logger with args */
   void setup(TextRenderer *text, const int *size, const float *position, float scale);

   /* log: store log text */
   void log(const char *format, ...);

   /* render: render text area */
   void render();
};
