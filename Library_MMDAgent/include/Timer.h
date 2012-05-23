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

/* Timer: timer */
class Timer
{
private:

   double m_systemStartTime; /* system start time */
   double m_lastUpdateFrameSystem;  /* number of frames from last getTimeInterval function */
   double m_pauseTime;       /* time from system start to last pause function */

   float m_fps;                  /* current frame rate */
   double m_fpsStartTime; /* start time of fps count */
   double m_fpsCount;     /* count of calling countFrame function */

   double m_targetAdjustmentFrame;  /* target frame to sync music */
   double m_currentAdjustmentFrame; /* current frame to sync music */
   bool m_enableAdjustment;         /* switch to sync music */

   double m_userStartTime; /* user start time */

   /* initialize: initialize timer */
   void initialize();

   /* clear: free timer */
   void clear();

public:

   /* Timer: constructor */
   Timer();

   /* ~Timer: destructor */
   ~Timer();

   /* setup: initialize and start timer */
   void setup();

   /* getFrameInterval: return time interval from last call */
   double getFrameInterval();

   /* pause: pause timer */
   void pause();

   /* resume: resume timer */
   void resume();

   /* start: start user timer */
   void start();

   /* ellapsed: return ellapsed time since last call of start() */
   double ellapsed();

   /* countFrame: increment frame count for FPS calculation */
   void countFrame();

   /* getFps: get fps */
   float getFps();

   /* setTargetAdjustmentFrame: set target frame to sync music */
   void setTargetAdjustmentFrame(double frame);

   /* startAdjustment: start to sync music */
   void startAdjustment();

   /* stopAdjustment: stop to sync music */
   void stopAdjustment();

   /* getCurrentAdjustmentFrame: get current frame to sync music */
   double getCurrentAdjustmentFrame();

   /* getAdditionalFrame: get number of additional frames to sync music */
   double getAdditionalFrame(double frame);
};
