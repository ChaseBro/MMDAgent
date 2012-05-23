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

#define JULIUSLOGGER_ADINMAXVOLUMEUPDATEFRAME 6.0
#define JULIUSLOGGER_DECAYFRAME               15.0
#define JULIUSLOGGER_DECAYDECREASECOEF        0.95f
#define JULIUSLOGGER_MAXARCS                  1200
#define JULIUSLOGGER_BARWIDTH                 8.0f
#define JULIUSLOGGER_BARHEIGHT                1.0f
#define JULIUSLOGGER_BARMARGIN                0.1f
#define JULIUSLOGGER_BARINDICATORZOFFSET      0.02f
#define JULIUSLOGGER_FRAMESINBAR              200.0f
#define JULIUSLOGGER_ADINOVERFLOWTHRES        32000

/* Julius_Logger: display debug information with OpenGL */
class Julius_Logger
{
private :

   bool m_active;               /* draw the log only when true */
   bool m_recognizing;          /* true when recognition is running */
   TRELLIS_ATOM* m_lastTrellis; /* current word trellis edge */
   double m_decayFrame;         /* rest frames for last decaying */
   int m_currentMaxAdIn;        /* maximum level at last input segment */
   int m_maxAdIn;               /* snapped maximum level */
   double m_adInFrameStep;      /* current frame for adin level calculation */
   int m_numWord;               /* number of words in recognition dictionary */
   int m_levelThreshold;        /* audio trigger level threshold */

   float m_pos[JULIUSLOGGER_MAXARCS*3];          /* position array for recognition process drawing */
   unsigned int m_index[JULIUSLOGGER_MAXARCS*2]; /* index array for recognition process drawing */
   int m_numPos;                                 /* length of m_posArray */
   int m_numIndex;                               /* length of m_index */

   /* initialize: initialize data */
   void initialize();

   /* clear: free data */
   void clear();

public :

   /* Julius_Logger: constructor */
   Julius_Logger();

   /* ~Julius_Logger: destructor  */
   ~Julius_Logger();

   /* setup: setup for logging */
   void setup(Recog *recog);

   /* setRecognitionFlag: mark recognition start and end */
   void setRecognitionFlag(bool flag);

   /* setLastTrellis: store the last best word trellis while recognition */
   void setLastTrellis(TRELLIS_ATOM *atom);

   /* updateMaxVol: update maximum volume */
   void updateMaxVol(SP16 *buf, int len);

   /* startDecay: start decaying the search status bar */
   void startDecay();

   /* setActiveFlag: set active flag */
   void setActiveFlag(bool flag);

   /* getActiveFlag: get active flag */
   bool getActiveFlag();

   /* update: update log view per step */
   void update(double frame);

   /* render: render log view */
   void render();
};
