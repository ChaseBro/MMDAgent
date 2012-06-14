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

#include <ctype.h>
#include "MMDAgent.h"

#ifdef _WIN32
/* TextRenderer::getID: get display list of character */
bool TextRenderer::getID(unsigned long mbc, unsigned int *id)
{
   GLYPHMETRICSFLOAT gmf;
   HGDIOBJ oldfont;
   CharDispList *tmp1, *tmp2 = NULL;

   /* return cache one */
   for(tmp1 = m_list; tmp1; tmp1 = tmp1->next) {
      if(tmp1->c == mbc) {
         if(tmp1 == m_list) {
            *id = tmp1->id;
         } else {
            if(tmp2)
               tmp2->next = tmp1->next;
            tmp1->next = m_list;
            m_list = tmp1;
            *id = tmp1->id;
         }
         return true;
      }
      tmp2 = tmp1;
   }

   /* make new display list */
   *id = glGenLists(1);
   if (id == 0)
      return false;
   /* get font outline to the display list */
   oldfont = SelectObject(m_hDC, m_outlineFont);
   if (!oldfont)
      return false;
   if (wglUseFontOutlinesA(m_hDC, mbc, 1, *id, 0.0f, 0.1f, WGL_FONT_POLYGONS, &gmf) == false)
      return false;
   SelectObject(m_hDC, oldfont);
   /* make id as relative to base id */
   *id -= m_outlineFontID;

   tmp1 = (CharDispList *) malloc(sizeof(CharDispList));
   tmp1->c = mbc;
   tmp1->id = *id;
   tmp1->next = m_list;
   m_list = tmp1;

   return true;
}
#else
#include <FTGL/ftgl.h>
#endif /* _WIN32 */

/* TextRenderer::initialize: initialize text renderer */
void TextRenderer::initialize()
{
#ifdef _WIN32
   m_hDC = NULL;
   m_outlineFont = NULL;
   m_outlineFontID = 0;
   m_bitmapFontID = 0;
   m_list = NULL;
#endif /* _WIN32 */
}

/* TextRenderer::clear: clear text renderer */
void TextRenderer::clear()
{
#ifdef _WIN32
   CharDispList *tmp1, *tmp2;

   for(tmp1 = m_list; tmp1; tmp1 = tmp2) {
      tmp2 = tmp1->next;
      glDeleteLists(tmp1->id, 1);
      free(tmp1);
   }
   if (m_bitmapFontID != 0)
      glDeleteLists(m_bitmapFontID, TEXTRENDERER_ASCIISIZE);
   if (m_outlineFontID != 0)
      glDeleteLists(m_outlineFontID, TEXTRENDERER_ASCIISIZE);
   if (m_outlineFont)
      DeleteObject(m_outlineFont);

   initialize();
#endif /* _WIN32 */
}

/* TextRenderer::TextRenderer: constructor */
TextRenderer::TextRenderer()
{
   initialize();
}

/* TextRender:: destructor */
TextRenderer::~TextRenderer()
{
   clear();
}

/* TextRenderer::setup: initialize and setup text renderer */
void TextRenderer::setup()
{
#ifdef _WIN32
   HGDIOBJ oldfont;
   GLYPHMETRICSFLOAT gmf[TEXTRENDERER_ASCIISIZE];

   clear();

   /* store device context */
   m_hDC = glfwGetDeviceContext();

   /* set TEXTRENDERER_ASCIISIZE bitmap font for ASCII */
   m_bitmapFontID = glGenLists(TEXTRENDERER_ASCIISIZE);
   if (m_bitmapFontID == 0) {
      clear();
      return;
   }

   /* get system font */
   oldfont = SelectObject(m_hDC, GetStockObject(SYSTEM_FONT));
   if (!oldfont) {
      clear();
      return;
   }
   if (wglUseFontBitmaps(m_hDC, 0, TEXTRENDERER_ASCIISIZE, m_bitmapFontID) == false) {
      clear();
      return;
   }
   SelectObject(m_hDC, oldfont);

   /* set TEXTRENDERER_ASCIISIZE outline font for ASCII */
   m_outlineFontID = glGenLists(TEXTRENDERER_ASCIISIZE);
   if (m_outlineFontID == 0) {
      clear();
      return;
   }

   /* get outline font */
   m_outlineFont = CreateFontA(25, 0, 0, 0, FW_NORMAL, false, false, false,
                               SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
                               (LPCSTR) TEXTRENDERER_FONT);
   if (!m_outlineFont) {
      clear();
      return;
   }

   oldfont = SelectObject(m_hDC, m_outlineFont);
   if (!oldfont) {
      clear();
      return;
   }
   if (wglUseFontOutlinesA(m_hDC, 0, TEXTRENDERER_ASCIISIZE, m_outlineFontID, 0.0f, 0.1f, WGL_FONT_POLYGONS, gmf) == false) {
      clear();
      return;
   }
   SelectObject(m_hDC, oldfont);
#else
   m_pixFont = new FTPixmapFont("/usr/share/fonts/truetype/msttcorefonts/arial.ttf");
   m_texFont = new FTGLTextureFont("/usr/share/fonts/truetype/msttcorefonts/arial.ttf");

   if (m_pixFont->Error() || m_texFont->Error())
      return;

   m_texFont->FaceSize(72,36);
   m_pixFont->FaceSize(12);
#endif /* _WIN32 */
}

/* TextRenderer::drawAsciiStringBitmap: draw ascii string (bitmap) */
void TextRenderer::drawAsciiStringBitmap(const char *str)
{
#ifdef _WIN32
   GLsizei size;

   if(!m_hDC)
      return;

   size = MMDAgent_strlen(str);
   if(size > 0) {
      glListBase(m_bitmapFontID);
      glCallLists(size, GL_UNSIGNED_BYTE, (const GLvoid*) str);
   }
#else
   if (m_pixFont->Error())
      return;

   m_pixFont->Render(str);
#endif /* _WIN32 */
}

/* TextRenderer::getDisplayListArrayOfString: get array of display list indices Draw any string (outline, slow) */
int TextRenderer::getDisplayListArrayOfString(const char *str, unsigned int *idList, int maxlen)
{
#ifdef _WIN32
   int i;
   int n = 0;
   unsigned int id = 0;
   unsigned char c1;
   unsigned char c2;
   unsigned long mbc;
   char size;
   int len;

   if(!m_hDC)
      return 0;

   len = MMDAgent_strlen(str);
   for (i = 0; i < len && n < maxlen;) {
      size = MMDAgent_getcharsize(&str[i]);
      if(size <= 0) {
         break;
      } else if(size == 1 && isascii(str[i])) {
         /* ascii, use display list whose id number equals to ascii code */
         idList[n] = (unsigned int) str[i];
         n++;
         i++;
      } else if(size <= 2) {
         if(size == 1) {
            c1 = (unsigned char) 0;
            c2 = (unsigned char) str[i];
         } else {
            c1 = (unsigned char) str[i];
            c2 = (unsigned char) str[i+1];
         }
         mbc = (c1 << 8) | c2;
         /* non-ascii look for already allocated display lists */
         if(getID(mbc, &id)) {
            idList[n] = id;
            n++;
         }
         i += size;
      } else {
         break; /* unknown character */
      }
   }
   return n;
#else
   return 0;
#endif /* _WIN32 */
}

/* TextRenderer::renderSispayListArrayOfString: render the obtained array of display lists for a string */
void TextRenderer::renderDisplayListArrayOfString(const unsigned int *idList, int n)
{
#ifdef _WIN32
   if(!m_hDC)
      return;

   glListBase(m_outlineFontID);
   glCallLists((GLsizei) n, GL_UNSIGNED_INT, (const GLvoid*) idList);
   glFrontFace(GL_CCW);
#endif /* _WIN32 */
}

/* TextRenderer::drawString: draw any string (outline, slow) */
void TextRenderer::drawString(const char *str)
{
#ifdef _WIN32
   unsigned int *idList;
   int len;
   int n;

   if(!m_hDC)
      return;

   len = MMDAgent_strlen(str);
   if(len > 0) {
      idList = (unsigned int *) malloc(sizeof(unsigned int) * len);
      n = getDisplayListArrayOfString(str, idList, len);
      if (n > 0)
         renderDisplayListArrayOfString(idList, n);
      free(idList);
   }
#else
   if (m_texFont->Error())
      return;

   glPushMatrix();
   glScaled(.01,.01,.01);
   m_texFont->Render(str);
   glPopMatrix();
#endif /* _WIN32 */
}
