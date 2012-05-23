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

#define MAIN_TITLE         "MMDAgent - Toolkit for building voice interaction systems"
#define MAIN_DOUBLECLICKMS 200

/* headers */

#ifdef _WIN32
#include <locale.h>
#include <windows.h>
#else
#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#else
#include <limits.h>
#include <iconv.h>
#endif /* __APPLE__ */
#endif /* _WIN32 */
#include "MMDAgent.h"

/* MMDAgent */

MMDAgent *mmdagent;
bool enable;

/* Key */

bool shiftKeyL;
bool shiftKeyR;
bool ctrlKeyL;
bool ctrlKeyR;

/* Mouse */

double mouseLastClick;
int mousePosX;
int mousePosY;
int mouseLastWheel;

/* procWindowSizeMessage: process window resize message */
void GLFWCALL procWindowSizeMessage(int w, int h)
{
   if(enable == false)
      return;

   mmdagent->procWindowSizeMessage(w, h);
   mmdagent->updateAndRender();
}

/* procDropFileMessage: process drop files message */
void GLFWCALL procDropFileMessage(const char *file, int x, int y)
{
#ifdef __APPLE__
   CFStringRef cfs;
   size_t len;
   char *buff;

   if(enable == false)
      return;

   mousePosX = x;
   mousePosY = y;

   cfs = CFStringCreateWithCString(NULL, file, kCFStringEncodingUTF8);
   len = CFStringGetMaximumSizeForEncoding(CFStringGetLength(cfs), kCFStringEncodingDOSJapanese) + 1;
   buff = (char *) malloc(len);
   CFStringGetCString(cfs, buff, len, kCFStringEncodingDOSJapanese);
   CFRelease(cfs);
   mmdagent->procDropFileMessage(buff,  mousePosX, mousePosY);
   free(buff);
#else
   if(enable == false)
      return;

   mmdagent->procDropFileMessage(file, mousePosX, mousePosY);
#endif /* __APPLE__ */
}

/* procKeyMessage: process key message */
void GLFWCALL procKeyMessage(int key, int action)
{
   if(enable == false)
      return;

   if(action == GLFW_PRESS) {
      switch(key) {
      case GLFW_KEY_LSHIFT:
         shiftKeyL = true;
         break;
      case GLFW_KEY_RSHIFT:
         shiftKeyR = true;
         break;
      case GLFW_KEY_LCTRL:
         ctrlKeyL = true;
         break;
      case GLFW_KEY_RCTRL:
         ctrlKeyR = true;
         break;
      case GLFW_KEY_DEL:
         mmdagent->procDeleteModelMessage();
         break;
      case GLFW_KEY_ESC:
         enable = false;
         break;
      case GLFW_KEY_LEFT:
         if(ctrlKeyL == true || ctrlKeyR == true)
            mmdagent->procTimeAdjustMessage(false);
         else if(shiftKeyL == true || shiftKeyR == true)
            mmdagent->procHorizontalMoveMessage(false);
         else
            mmdagent->procHorizontalRotateMessage(false);
         break;
      case GLFW_KEY_RIGHT:
         if(ctrlKeyL == true || ctrlKeyR == true)
            mmdagent->procTimeAdjustMessage(true);
         else if(shiftKeyL == true || shiftKeyR == true)
            mmdagent->procHorizontalMoveMessage(true);
         else
            mmdagent->procHorizontalRotateMessage(true);
         break;
      case GLFW_KEY_UP:
         if(shiftKeyL == true || shiftKeyR == true)
            mmdagent->procVerticalMoveMessage(true);
         else
            mmdagent->procVerticalRotateMessage(true);
         break;
      case GLFW_KEY_DOWN:
         if(shiftKeyL == true || shiftKeyR == true)
            mmdagent->procVerticalMoveMessage(false);
         else
            mmdagent->procVerticalRotateMessage(false);
         break;
      default:
         break;
      }
   } else {
      switch(key) {
      case GLFW_KEY_LSHIFT:
         shiftKeyL = false;
         break;
      case GLFW_KEY_RSHIFT:
         shiftKeyR = false;
         break;
      case GLFW_KEY_LCTRL:
         ctrlKeyL = false;
         break;
      case GLFW_KEY_RCTRL:
         ctrlKeyR = false;
         break;
      default:
         break;
      }
   }
}

/* procCharMessage: process char message */
void GLFWCALL procCharMessage(int key, int action)
{
   if(enable == false)
      return;

   if(action == GLFW_RELEASE)
      return;

   switch(key) {
   case 'd':
   case 'D':
      mmdagent->procDisplayLogMessage();
      break;
   case 'f':
      mmdagent->procFullScreenMessage();
      break;
   case 's':
      mmdagent->procInfoStringMessage();
      break;
   case '+':
      mmdagent->procMouseWheelMessage(true, false, false);
      break;
   case '-':
      mmdagent->procMouseWheelMessage(false, false, false);
      break;
   case 'x':
      mmdagent->procShadowMappingMessage();
      break;
   case 'X':
      mmdagent->procShadowMappingOrderMessage();
      break;
   case 'W':
      mmdagent->procDisplayRigidBodyMessage();
      break;
   case 'w':
      mmdagent->procDisplayWireMessage();
      break;
   case 'b':
      mmdagent->procDisplayBoneMessage();
      break;
   case 'e':
      mmdagent->procCartoonEdgeMessage(true);
      break;
   case 'E':
      mmdagent->procCartoonEdgeMessage(false);
      break;
   case 'p':
      mmdagent->procPhysicsMessage();
      break;
   case 'h':
      mmdagent->procHoldMessage();
      break;
   case 'V':
      mmdagent->procVSyncMessage();
      break;
   default:
      break;
   }

   mmdagent->procKeyMessage((char) key);
}

/* procMouseButtonMessage: process mouse button message */
void GLFWCALL procMouseButtonMessage(int button, int action)
{
   if(enable == false)
      return;

   if(action == GLFW_PRESS) {
      switch(button) {
      case GLFW_MOUSE_BUTTON_LEFT:
         if(MMDAgent_diffTime(MMDAgent_getTime(), mouseLastClick) <= MAIN_DOUBLECLICKMS)
            mmdagent->procMouseLeftButtonDoubleClickMessage(mousePosX, mousePosY);
         else
            mmdagent->procMouseLeftButtonDownMessage(mousePosX, mousePosY, ctrlKeyL == true || ctrlKeyR == true ? true : false, shiftKeyL == true || shiftKeyR == true ? true : false);
         mouseLastClick = MMDAgent_getTime();
         break;
      case GLFW_MOUSE_BUTTON_RIGHT:
         mmdagent->procMouseRightButtonDownMessage();
         break;
      default:
         break;
      }
   } else {
      switch(button) {
      case GLFW_MOUSE_BUTTON_LEFT:
         mmdagent->procMouseLeftButtonUpMessage();
         break;
      default:
         break;
      }
   }
}

/* procMousePosMessage: process mouse position message */
void GLFWCALL procMousePosMessage(int x, int y, int shift, int ctrl)
{
   if(enable == false)
      return;

   mousePosX = x;
   mousePosY = y;
   shiftKeyL = shiftKeyR = shift == GLFW_PRESS ? true : false;
   ctrlKeyL = ctrlKeyR = ctrl == GLFW_PRESS ? true : false;

   mmdagent->procMousePosMessage(mousePosX, mousePosY, ctrlKeyL == true || ctrlKeyR == true ? true : false, shiftKeyL == true || shiftKeyR == true ? true : false);
}

/* procMouseWheelMessage: process mouse wheel message */
void GLFWCALL procMouseWheelMessage(int x)
{
   if(enable == false)
      return;

   mmdagent->procMouseWheelMessage(x > mouseLastWheel ? true : false, ctrlKeyL == true || ctrlKeyR == true ? true : false, shiftKeyL == true || shiftKeyR == true ? true : false);
   mouseLastWheel = x;
}

/* commonMain: common main function */
int commonMain(int argc, char **argv)
{
   enable = false;

   shiftKeyL = false;
   shiftKeyR = false;
   ctrlKeyL = false;
   ctrlKeyR = false;

   mouseLastClick = 0;
   mousePosX = 0;
   mousePosY = 0;
   mouseLastWheel = 0;

   /* create MMDAgent window */
   mmdagent = new MMDAgent();
   if(mmdagent->setup(argc, argv, MAIN_TITLE) == false) {
      delete mmdagent;
      glfwTerminate();
      return -1;
   }

   /* window */
   glfwSetWindowSizeCallback(procWindowSizeMessage);

   /* drag and drop */
   glfwSetDropFileCallback(procDropFileMessage);

   /* key */
   glfwSetKeyCallback(procKeyMessage);
   glfwEnable(GLFW_KEY_REPEAT);

   /* char */
   glfwSetCharCallback(procCharMessage);

   /* mouse */
   glfwSetMouseButtonCallback(procMouseButtonMessage);
   glfwSetMousePosCallback(procMousePosMessage);
   glfwSetMouseWheelCallback(procMouseWheelMessage);

   /* main loop */
   enable = true;
   while(enable == true && glfwGetWindowParam(GLFW_OPENED) == GL_TRUE) {
      mmdagent->updateAndRender();
   }

   /* free */
   mmdagent->procWindowDestroyMessage();
   delete mmdagent;
   return 0;
}

/* main: main function */
#ifdef _WIN32
#ifndef __MINGW32__
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
   int i;
   size_t len;
   int argc;
   wchar_t **wargv;
   char **argv;
   int result;

   setlocale(LC_CTYPE, "japanese");

   wargv = CommandLineToArgvW(GetCommandLineW(), &argc);
   if(argc < 1) return 0;
   argv = (char **) malloc(sizeof(char *) * argc);
   for(i = 0; i < argc; i++) {
      argv[i] = (char *) malloc(sizeof(char) * MMDAGENT_MAXBUFLEN);
      wcstombs_s(&len, argv[i], MMDAGENT_MAXBUFLEN, wargv[i], _TRUNCATE);
   }
   result = commonMain(argc, argv);
   for(i = 0; i < argc; i++)
      free(argv[i]);
   free(argv);

   return result;
}
#else
int main(int argc, char **argv)
{
   return commonMain(argc, argv);
}
#endif /* !__MIGW32__ */
#else
#ifdef __APPLE__
int main(int argc, char **argv)
{
   int i, j;
   char inBuff[PATH_MAX+1];
   CFStringRef cfs;
   size_t len;
   char **newArgv;
   int result;

   newArgv = (char **) malloc(sizeof(char *) * argc);
   for(i = 0; i < argc; i++) {
      /* prepare buffer */
      memset(inBuff, 0, PATH_MAX + 1);
      realpath(argv[i], inBuff);

      cfs = CFStringCreateWithCString(NULL, inBuff, kCFStringEncodingUTF8);
      len = CFStringGetMaximumSizeForEncoding(CFStringGetLength(cfs), kCFStringEncodingDOSJapanese) + 1;
      newArgv[i] = (char *) malloc(len);
      CFStringGetCString(cfs, newArgv[i], len, kCFStringEncodingDOSJapanese);
      CFRelease(cfs);
   }
   result = commonMain(argc, newArgv);
   for(i = 0; i < argc; i++) {
      free(newArgv[i]);
   }
   free(newArgv);

   return result;
}
#else
int main(int argc, char **argv)
{
   int i;
   iconv_t ic;
   char **newArgv;
   char inBuff[PATH_MAX+1], outBuff[PATH_MAX+1];
   char *inStr, *outStr;
   size_t inLen, outLen;
   int result = 0;

   ic = iconv_open("SHIFT-JIS", MAIN_CHARSET);
   if(ic < 0)
      return -1;

   newArgv = (char **) malloc(sizeof(char *) * argc);
   for(i = 0; i < argc; i++) {
      /* prepare buffer */
      memset(inBuff, 0, PATH_MAX + 1);
      memset(outBuff, 0, PATH_MAX + 1);
      realpath(argv[i], inBuff);

      inStr = &inBuff[0];
      outStr = &outBuff[0];

      inLen = MMDAgent_strlen(inStr);
      outLen = MMDAGENT_MAXBUFLEN;

      if(iconv(ic, &inStr, &inLen, &outStr, &outLen) < 0) {
         result = -1;
         strcpy(outBuff, "");
      }

      newArgv[i] = MMDAgent_strdup(outBuff);
   }

   iconv_close(ic);

   if(result >= 0)
      result = commonMain(argc, newArgv);

   for(i = 0; i < argc; i++) {
      if(newArgv[i] != NULL)
         free(newArgv[i]);
   }
   free(newArgv);

   return result;
}
#endif /* __APPLE__ */
#endif /* _WIN32 */
