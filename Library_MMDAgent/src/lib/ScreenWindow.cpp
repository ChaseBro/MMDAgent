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

/* ScreenWindow::initialize: initialize screen */
void ScreenWindow::initialize()
{
   m_enable = false;

   m_vsync = true;
   m_numMultiSampling = 0;
   m_showMouse = true;
   m_mouseActiveLeftFrame = 0.0;
   m_fullScreen = false;
}

/* ScreenWindow::clear: free screen */
void ScreenWindow::clear()
{
   if(m_enable == true)
      glfwTerminate();

   initialize();
}

/* ScreenWindow::ScreenWindow: constructor */
ScreenWindow::ScreenWindow()
{
   initialize();
}

/* ScreenWindow::ScreenWindow: destructor */
ScreenWindow::~ScreenWindow()
{
   clear();
}

/* ScreenWindow::setup: create window */
bool ScreenWindow::setup(const int *size, const char *title, int maxMultiSampling)
{
   clear();

   if(glfwInit() == GL_FALSE)
      return false;

   /* try to set number of multi-sampling */
   glfwOpenWindowHint(GLFW_FSAA_SAMPLES, maxMultiSampling);

   /* create window */
   if(glfwOpenWindow(size[0], size[1], 24, 24, 24, 8, 24, 8, GLFW_WINDOW) == GL_FALSE) {
      glfwTerminate();
      return false;
   }

   /* store number of multi-sampling */
   m_numMultiSampling = glfwGetWindowParam(GLFW_FSAA_SAMPLES);

   /* set title */
   glfwSetWindowTitle(title);

   /* set vertical sync. */
   glfwSwapInterval(1);

   m_enable = true;
   return true;
}

/* ScreenWindow::swapBuffers: swap buffers */
void ScreenWindow::swapBuffers()
{
   if(m_enable == false)
      return;

   glfwSwapBuffers();
}

/* ScreenWindow::getVSync: get vertical sync. flag */
bool ScreenWindow::getVSync()
{
   return m_vsync;
}

/* ScreenWindow::toggleVSync: toggle vertical sync. flag */
void ScreenWindow::toggleVSync()
{
   if(m_enable == false)
      return;

   if(m_vsync == true) {
      glfwSwapInterval(0);
      m_vsync = false;
   } else {
      glfwSwapInterval(1);
      m_vsync = true;
   }
}

/* ScreenWindow::getNumMultiSmapling: get number of multi-sampling */
int ScreenWindow::getNumMultiSampling()
{
   return m_numMultiSampling;
}

/* ScreenWindow::setMouseActiveTime: set mouse active time */
void ScreenWindow::setMouseActiveTime(double frame)
{
   if(m_enable == false)
      return;

   m_mouseActiveLeftFrame = frame;

   /* if full screen, disable mouse cursor */
   if (m_fullScreen == true && m_showMouse == false) {
      m_showMouse = true;
      glfwEnable(GLFW_MOUSE_CURSOR);
   }
}

/* ScreenWindow::updateMouseActiveTime: update mouse active time */
void ScreenWindow::updateMouseActiveTime(double frame)
{
   if(m_enable == false)
      return;

   if(m_mouseActiveLeftFrame == 0.0)
      return;

   m_mouseActiveLeftFrame -= frame;
   if (m_mouseActiveLeftFrame <= 0.0) {
      m_mouseActiveLeftFrame = 0.0;
      /* if full screen, disable mouse cursor */
      if (m_fullScreen == true && m_showMouse == true) {
         m_showMouse = false;
         glfwDisable(GLFW_MOUSE_CURSOR);
      }
   }
}

/* ScreenWindow::setFullScreen: set fullscreen */
void ScreenWindow::setFullScreen()
{
   if(m_enable == false)
      return;

   /* set full screen */
   if(m_fullScreen == false) {
      glfwEnableFullScreen();
      m_fullScreen = true;
   }

   /* disable mouse cursor */
   if (m_showMouse == true) {
      m_showMouse = false;
      glfwDisable(GLFW_MOUSE_CURSOR);
   }
}

/* ScreenWindow::exitFullScreen: exit fullscreen */
void ScreenWindow::exitFullScreen()
{
   if(m_enable == false)
      return;

   /* exit full screen */
   if(m_fullScreen == true) {
      glfwDisableFullScreen();
      m_fullScreen = false;
   }

   /* enable mouse cursor */
   if (m_showMouse == false) {
      m_showMouse = true;
      glfwEnable(GLFW_MOUSE_CURSOR);
   }
}
