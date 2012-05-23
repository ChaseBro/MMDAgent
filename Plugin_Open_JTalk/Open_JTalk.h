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

#define OPENJTALK_MGCWIN1   "mgc.win1"
#define OPENJTALK_MGCWIN2   "mgc.win2"
#define OPENJTALK_MGCWIN3   "mgc.win3"
#define OPENJTALK_LF0WIN1   "lf0.win1"
#define OPENJTALK_LF0WIN2   "lf0.win2"
#define OPENJTALK_LF0WIN3   "lf0.win3"
#define OPENJTALK_LPFWIN1   "lpf.win1"
#define OPENJTALK_GVSWITCH  "gv-switch.inf"
#define OPENJTALK_DURTREE   "tree-dur.inf"
#define OPENJTALK_MGCTREE   "tree-mgc.inf"
#define OPENJTALK_LF0TREE   "tree-lf0.inf"
#define OPENJTALK_LPFTREE   "tree-lpf.inf"
#define OPENJTALK_DURPDF    "dur.pdf"
#define OPENJTALK_MGCPDF    "mgc.pdf"
#define OPENJTALK_LF0PDF    "lf0.pdf"
#define OPENJTALK_LPFPDF    "lpf.pdf"
#define OPENJTALK_MGCGVTREE "tree-gv-mgc.inf"
#define OPENJTALK_LF0GVTREE "tree-gv-lf0.inf"
#define OPENJTALK_MGCGVPDF  "gv-mgc.pdf"
#define OPENJTALK_LF0GVPDF  "gv-lf0.pdf"

#define OPENJTALK_MINLF0VAL log(10.0)

#define OPENJTALK_GAMMA         0
#define OPENJTALK_LOGGAIN       false
#define OPENJTALK_SAMPLINGRATE  48000
#define OPENJTALK_FPERIOD       240
#define OPENJTALK_HALFTONE      0.0
#define OPENJTALK_ALPHA         0.55
#define OPENJTALK_VOLUME        1.0
#define OPENJTALK_AUDIOBUFFSIZE 4800

#define OPENJTALK_MAXFPERIOD  48000
#define OPENJTALK_MINFPERIOD  1
#define OPENJTALK_MAXHALFTONE 24.0
#define OPENJTALK_MINHALFTONE -24.0
#define OPENJTALK_MAXALPHA    1.0
#define OPENJTALK_MINALPHA    0.0
#define OPENJTALK_MAXVOLUME   10.0
#define OPENJTALK_MINVOLUME   0.0

/* Open_JTalk: Japanese TTS system */
class Open_JTalk
{
private:

   Mecab m_mecab;       /* text analyzer */
   NJD m_njd;           /* container for Naist Japanese Dictionary */
   JPCommon m_jpcommon; /* dictionary-independent container */
   HTS_Engine m_engine; /* speech synthesizer */

   double m_f0Shift;       /* pitch parameter */
   int m_numModels;        /* number of models */
   double *m_styleWeights; /* weights of speaking styles */
   int m_numStyles;        /* number of speaking styles */

   /* initialize: initialize system */
   void initialize();

   /* clear: free system */
   void clear();

public:

   /* Open_JTalk: constructor */
   Open_JTalk();

   /* ~Open_JTalk: destructor */
   ~Open_JTalk();

   /* load: load dictionary and models */
   bool load(const char *dicDir, char **modelDir, int numModels, double *styleWeights, int numStyles);

   /* prepare: text analysis, decision of state durations, and parameter generation */
   void prepare(const char *str);

   /* getPhonemeSequence: get phoneme sequence */
   void getPhonemeSequence(char *str);

   /* synthesis: speech synthesis */
   void synthesis();

   /* stop: stop speech synthesis */
   void stop();

   /* setStyle: set style interpolation weight */
   bool setStyle(int val);
};
