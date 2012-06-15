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

#include <FTGL/ftgl.h>

/* definitions */

#define TEXTRENDERER_FONT      "Arial Unicode MS"
#define TEXTRENDERER_ASCIISIZE 256

/* CharDispList: display list for character */
typedef struct _CharDispList {
   unsigned long c;
   unsigned int id;
   struct _CharDispList *next;
} CharDispList;

/* TextRenderer: text renderer */
class TextRenderer
{
private:

#ifdef _WIN32
   HDC m_hDC;                    /* device context */
   HFONT m_outlineFont;          /* outline font */
   unsigned int m_outlineFontID; /* first 256 ID in ASCII font (outline) */
   unsigned int m_bitmapFontID;  /* first 256 ID in ASCII font (bitmatp) */

   CharDispList *m_list;

   /* getID: get display list of character */
   bool getID(unsigned long mbc, unsigned int *id);
#else
   FTPixmapFont *m_pixFont;       /* Pixmap font for rendering */
   FTTextureFont *m_texFont;      /* Texture font for renderint */
#endif /* _WIN32 */

   /* initialize: initialize text renderer */
   void initialize();

   /* clear: free text renderer */
   void clear();

public:

   /* TextRenderer: constructor */
   TextRenderer();

   /* ~TextRender: destructor */
   ~TextRenderer();

   /* setup: initialize and setup text renderer */
   void setup();

   /* drawAsciiStringBitmap: draw ascii string (bitmap) */
   void drawAsciiStringBitmap(const char *str);

   /* getDisplayListArrayOfString: get array of display list indices draw any string (outline) */
   int getDisplayListArrayOfString(const char *str, unsigned int *idList, int maxlen);

   /* renderSispayListArrayOfString: render the obtained array of display lists for a string */
   void renderDisplayListArrayOfString(const unsigned int *idList, int n);

   /* drawString: draw any string (outline, slow) */
   void drawString(const char *str);
};
