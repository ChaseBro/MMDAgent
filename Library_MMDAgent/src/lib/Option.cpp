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

/* Option::initialize: initialize options */
void Option::initialize()
{
   m_useCartoonRendering = OPTION_USECARTOONRENDERING_DEF;
   m_useMMDLikeCartoon = OPTION_USEMMDLIKECARTOON_DEF;
   m_cartoonEdgeWidth = OPTION_CARTOONEDGEWIDTH_DEF;
   m_cartoonEdgeStep = OPTION_CARTOONEDGESTEP_DEF;
   m_cartoonEdgeSelectedColor[0] = OPTION_CARTOONEDGESELECTEDCOLORR_DEF;
   m_cartoonEdgeSelectedColor[1] = OPTION_CARTOONEDGESELECTEDCOLORG_DEF;
   m_cartoonEdgeSelectedColor[2] = OPTION_CARTOONEDGESELECTEDCOLORB_DEF;
   m_cartoonEdgeSelectedColor[3] = OPTION_CARTOONEDGESELECTEDCOLORA_DEF;

   m_cameraRotation[0] = OPTION_CAMERAROTATIONX_DEF;
   m_cameraRotation[1] = OPTION_CAMERAROTATIONY_DEF;
   m_cameraRotation[2] = OPTION_CAMERAROTATIONZ_DEF;
   m_cameraTransition[0] = OPTION_CAMERATRANSITIONX_DEF;
   m_cameraTransition[1] = OPTION_CAMERATRANSITIONY_DEF;
   m_cameraTransition[2] = OPTION_CAMERATRANSITIONZ_DEF;
   m_cameraDistance = OPTION_CAMERADISTANCE_DEF;
   m_cameraFovy = OPTION_CAMERAFOVY_DEF;

   m_stageSize[0] = OPTION_STAGESIZEW_DEF;
   m_stageSize[1] = OPTION_STAGESIZED_DEF;
   m_stageSize[2] = OPTION_STAGESIZEH_DEF;

   m_showFps = OPTION_SHOWFPS_DEF;
   m_fpsPosition[0] = OPTION_FPSPOSITIONX_DEF;
   m_fpsPosition[1] = OPTION_FPSPOSITIONY_DEF;
   m_fpsPosition[2] = OPTION_FPSPOSITIONZ_DEF;

   m_windowSize[0] = OPTION_WINDOWSIZEW_DEF;
   m_windowSize[1] = OPTION_WINDOWSIZEH_DEF;
   m_fullScreen = OPTION_FULLSCREEN_DEF;

   m_logSize[0] = OPTION_LOGSIZEW_DEF;
   m_logSize[1] = OPTION_LOGSIZEH_DEF;
   m_logPosition[0] = OPTION_LOGPOSITIONX_DEF;
   m_logPosition[1] = OPTION_LOGPOSITIONY_DEF;
   m_logPosition[2] = OPTION_LOGPOSITIONZ_DEF;
   m_logScale = OPTION_LOGSCALE_DEF;

   m_lightDirection[0] = OPTION_LIGHTDIRECTIONX_DEF;
   m_lightDirection[1] = OPTION_LIGHTDIRECTIONY_DEF;
   m_lightDirection[2] = OPTION_LIGHTDIRECTIONZ_DEF;
   m_lightDirection[3] = OPTION_LIGHTDIRECTIONI_DEF;
   m_lightIntensity = OPTION_LIGHTINTENSITY_DEF;
   m_lightColor[0] = OPTION_LIGHTCOLORR_DEF;
   m_lightColor[1] = OPTION_LIGHTCOLORG_DEF;
   m_lightColor[2] = OPTION_LIGHTCOLORB_DEF;

   m_campusColor[0] = OPTION_CAMPUSCOLORR_DEF;
   m_campusColor[1] = OPTION_CAMPUSCOLORG_DEF;
   m_campusColor[2] = OPTION_CAMPUSCOLORB_DEF;

   m_maxMultiSampling = OPTION_MAXMULTISAMPLING_DEF;

   m_motionAdjustFrame = OPTION_MOTIONADJUSTFRAME_DEF;

   m_bulletFps = OPTION_BULLETFPS_DEF;

   m_rotateStep = OPTION_ROTATESTEP_DEF;
   m_translateStep = OPTION_TRANSLATESTEP_DEF;
   m_distanceStep = OPTION_DISTANCESTEP_DEF;
   m_fovyStep = OPTION_FOVYSTEP_DEF;

   m_useShadowMapping = OPTION_USESHADOWMAPPING_DEF;
   m_shadowMapTextureSize = OPTION_SHADOWMAPPINGTEXTURESIZE_DEF;
   m_shadowMapSelfDensity = OPTION_SHADOWMAPPINGSELFDENSITY_DEF;
   m_shadowMapFloorDensity = OPTION_SHADOWMAPPINGFLOORDENSITY_DEF;
   m_shadowMapLightFirst = OPTION_SHADOWMAPPINGLIGHTFIRST_DEF;

   m_displayCommentFrame = OPTION_DISPLAYCOMMENTFRAME_DEF;

   m_maxNumModel = OPTION_MAXNUMMODEL_DEF;
}

/* Option::Option: constructor */
Option::Option()
{
   initialize();
}

/* Option::load: load options from config file */
bool Option::load(const char *file)
{
   FILE *fp;
   char buf[OPTION_MAXBUFLEN];
   int len;
   char *p1;

   int ivec2[2];
   float fvec3[3];
   float fvec4[4];

   fp = MMDAgent_fopen(file, "r");
   if (fp == NULL)
      return false;

   while (fgets(buf, OPTION_MAXBUFLEN, fp)) {
      len = MMDAgent_strlen(buf);
      if(len <= 0) continue;
      p1 = &(buf[len-1]);
      while (p1 >= &(buf[0]) && (*p1 == '\n' || *p1 == '\r' || *p1 == '\t' || *p1 == ' ')) {
         *p1 = L'\0';
         p1--;
      }
      p1 = &(buf[0]);
      if (*p1 == '#') continue;
      while (*p1 != L'=' && *p1 != L'\0') p1++;
      if (*p1 == L'\0') continue;
      *p1 = L'\0';
      p1++;

      /* overwrite option values */
      if(MMDAgent_strequal(buf, OPTION_USECARTOONRENDERING_STR)) {
         setUseCartoonRendering(MMDAgent_str2bool(p1));
      } else if(MMDAgent_strequal(buf, OPTION_USEMMDLIKECARTOON_STR)) {
         setUseMMDLikeCartoon(MMDAgent_str2bool(p1));
      } else if(MMDAgent_strequal(buf, OPTION_CARTOONEDGEWIDTH_STR)) {
         setCartoonEdgeWidth(MMDAgent_str2float(p1));
      } else if(MMDAgent_strequal(buf, OPTION_CARTOONEDGESTEP_STR)) {
         setCartoonEdgeStep(MMDAgent_str2float(p1));
      } else if(MMDAgent_strequal(buf, OPTION_CARTOONEDGESELECTEDCOLOR_STR)) {
         if(MMDAgent_str2fvec(p1, fvec4, 4))
            setCartoonEdgeSelectedColor(fvec4);
      } else if(MMDAgent_strequal(buf, OPTION_CAMERAROTATION_STR)) {
         if(MMDAgent_str2fvec(p1, fvec3, 3))
            setCameraRotation(fvec3);
      } else if(MMDAgent_strequal(buf, OPTION_CAMERATRANSITION_STR)) {
         if(MMDAgent_str2fvec(p1, fvec3, 3))
            setCameraTransition(fvec3);
      } else if(MMDAgent_strequal(buf, OPTION_CAMERADISTANCE_STR)) {
         setCameraDistance(MMDAgent_str2float(p1));
      } else if(MMDAgent_strequal(buf, OPTION_CAMERAFOVY_STR)) {
         setCameraFovy(MMDAgent_str2float(p1));
      } else if(MMDAgent_strequal(buf, OPTION_STAGESIZE_STR)) {
         if(MMDAgent_str2fvec(p1, fvec3, 3))
            setStageSize(fvec3);
      } else if(MMDAgent_strequal(buf, OPTION_SHOWFPS_STR)) {
         setShowFps(MMDAgent_str2bool(p1));
      } else if(MMDAgent_strequal(buf, OPTION_FPSPOSITION_STR)) {
         if(MMDAgent_str2fvec(p1, fvec3, 3))
            setFpsPosition(fvec3);
      } else if(MMDAgent_strequal(buf, OPTION_WINDOWSIZE_STR)) {
         if(MMDAgent_str2ivec(p1, ivec2, 2))
            setWindowSize(ivec2);
      } else if(MMDAgent_strequal(buf, OPTION_FULLSCREEN_STR)) {
         setFullScreen(MMDAgent_str2bool(p1));
      } else if(MMDAgent_strequal(buf, OPTION_LOGSIZE_STR)) {
         if(MMDAgent_str2ivec(p1, ivec2, 2))
            setLogSize(ivec2);
      } else if(MMDAgent_strequal(buf, OPTION_LOGPOSITION_STR)) {
         if(MMDAgent_str2fvec(p1, fvec3, 3))
            setLogPosition(fvec3);
      } else if(MMDAgent_strequal(buf, OPTION_LOGSCALE_STR)) {
         setLogScale(MMDAgent_str2float(p1));
      } else if(MMDAgent_strequal(buf, OPTION_LIGHTDIRECTION_STR)) {
         if(MMDAgent_str2fvec(p1, fvec4, 4))
            setLightDirection(fvec4);
      } else if(MMDAgent_strequal(buf, OPTION_LIGHTINTENSITY_STR)) {
         setLightIntensity(MMDAgent_str2float(p1));
      } else if(MMDAgent_strequal(buf, OPTION_LIGHTCOLOR_STR)) {
         if(MMDAgent_str2fvec(p1, fvec3, 3))
            setLightColor(fvec3);
      } else if(MMDAgent_strequal(buf, OPTION_CAMPUSCOLOR_STR)) {
         if(MMDAgent_str2fvec(p1, fvec3, 3))
            setCampusColor(fvec3);
      } else if(MMDAgent_strequal(buf, OPTION_MAXMULTISAMPLING_STR)) {
         setMaxMultiSampling(MMDAgent_str2int(p1));
      } else if(MMDAgent_strequal(buf, OPTION_MOTIONADJUSTFRAME_STR)) {
         setMotionAdjustFrame(MMDAgent_str2int(p1));
      } else if(MMDAgent_strequal(buf, OPTION_BULLETFPS_STR)) {
         setBulletFps(MMDAgent_str2int(p1));
      } else if(MMDAgent_strequal(buf, OPTION_ROTATESTEP_STR)) {
         setRotateStep(MMDAgent_str2float(p1));
      } else if(MMDAgent_strequal(buf, OPTION_TRANSLATESTEP_STR)) {
         setTranslateStep(MMDAgent_str2float(p1));
      } else if(MMDAgent_strequal(buf, OPTION_DISTANCESTEP_STR)) {
         setDistanceStep(MMDAgent_str2float(p1));
      } else if(MMDAgent_strequal(buf, OPTION_FOVYSTEP_STR)) {
         setFovyStep(MMDAgent_str2float(p1));
      } else if(MMDAgent_strequal(buf, OPTION_USESHADOWMAPPING_STR)) {
         setUseShadowMapping(MMDAgent_str2bool(p1));
      } else if(MMDAgent_strequal(buf, OPTION_SHADOWMAPPINGTEXTURESIZE_STR)) {
         setShadowMappingTextureSize(MMDAgent_str2int(p1));
      } else if(MMDAgent_strequal(buf, OPTION_SHADOWMAPPINGSELFDENSITY_STR)) {
         setShadowMappingSelfDensity(MMDAgent_str2float(p1));
      } else if(MMDAgent_strequal(buf, OPTION_SHADOWMAPPINGFLOORDENSITY_STR)) {
         setShadowMappingFloorDensity(MMDAgent_str2float(p1));
      } else if(MMDAgent_strequal(buf, OPTION_SHADOWMAPPINGLIGHTFIRST_STR)) {
         setShadowMappingLightFirst(MMDAgent_str2bool(p1));
      } else if(MMDAgent_strequal(buf, OPTION_DISPLAYCOMMENTFRAME_STR)) {
         setDisplayCommentFrame(MMDAgent_str2float(p1));
      } else if(MMDAgent_strequal(buf, OPTION_MAXNUMMODEL_STR)) {
         setMaxNumModel(MMDAgent_str2int(p1));
      }
   }
   fclose(fp);

   return true;
}

/* Option::getUseCartoonRendering: get cartoon rendering flag */
bool Option::getUseCartoonRendering()
{
   return m_useCartoonRendering;
}

/* Option::setUseCartoonRendering: set cartoon rendering flag */
void Option::setUseCartoonRendering(bool b)
{
   m_useCartoonRendering = b;
}

/* Option::getUseMMDLikeCartoon: get MMD like cartoon flag */
bool Option::getUseMMDLikeCartoon()
{
   return m_useMMDLikeCartoon;
}

/* Option::setUseMMDLikeCartoon: set MMD like cartoon flag */
void Option::setUseMMDLikeCartoon(bool b)
{
   m_useMMDLikeCartoon = b;
}

/* Option::getCartoonEdgeWidth: get edge width for catoon */
float Option::getCartoonEdgeWidth()
{
   return m_cartoonEdgeWidth;
}

/* Option::setCartoonEdgeWidth: set edge width for catoon */
void Option::setCartoonEdgeWidth(float f)
{
   if(OPTION_CARTOONEDGEWIDTH_MAX < f)
      m_cartoonEdgeWidth = OPTION_CARTOONEDGEWIDTH_MAX;
   else if(OPTION_CARTOONEDGEWIDTH_MIN > f)
      m_cartoonEdgeWidth = OPTION_CARTOONEDGEWIDTH_MIN;
   else
      m_cartoonEdgeWidth = f;
}

/* Option::getCartoonEdgeStep: get cartoon edge step */
float Option::getCartoonEdgeStep()
{
   return m_cartoonEdgeStep;
}

/* Option::setCartoonEdgeStep: set cartoon edge step */
void Option::setCartoonEdgeStep(float f)
{
   if(OPTION_CARTOONEDGESTEP_MAX < f)
      m_cartoonEdgeStep = OPTION_CARTOONEDGESTEP_MAX;
   else if(OPTION_CARTOONEDGESTEP_MIN > f)
      m_cartoonEdgeStep = OPTION_CARTOONEDGESTEP_MIN;
   else
      m_cartoonEdgeStep = f;
}

/* Option::getCartoonEdgeSelectedColor: get cartoon edge selected color */
float *Option::getCartoonEdgeSelectedColor()
{
   return m_cartoonEdgeSelectedColor;
}

/* Option::setCartoonEdgeSelectedColor: set cartoon edge selected color */
void Option::setCartoonEdgeSelectedColor(const float *f)
{
   if(OPTION_CARTOONEDGESELECTEDCOLOR_MAX < f[0])
      m_cartoonEdgeSelectedColor[0] = OPTION_CARTOONEDGESELECTEDCOLOR_MAX;
   else if(OPTION_CARTOONEDGESELECTEDCOLOR_MIN > f[0])
      m_cartoonEdgeSelectedColor[0] = OPTION_CARTOONEDGESELECTEDCOLOR_MIN;
   else
      m_cartoonEdgeSelectedColor[0] = f[0];

   if(OPTION_CARTOONEDGESELECTEDCOLOR_MAX < f[1])
      m_cartoonEdgeSelectedColor[1] = OPTION_CARTOONEDGESELECTEDCOLOR_MAX;
   else if(OPTION_CARTOONEDGESELECTEDCOLOR_MIN > f[1])
      m_cartoonEdgeSelectedColor[1] = OPTION_CARTOONEDGESELECTEDCOLOR_MIN;
   else
      m_cartoonEdgeSelectedColor[1] = f[1];

   if(OPTION_CARTOONEDGESELECTEDCOLOR_MAX < f[2])
      m_cartoonEdgeSelectedColor[2] = OPTION_CARTOONEDGESELECTEDCOLOR_MAX;
   else if(OPTION_CARTOONEDGESELECTEDCOLOR_MIN > f[2])
      m_cartoonEdgeSelectedColor[2] = OPTION_CARTOONEDGESELECTEDCOLOR_MIN;
   else
      m_cartoonEdgeSelectedColor[2] = f[2];

   if(OPTION_CARTOONEDGESELECTEDCOLOR_MAX < f[3])
      m_cartoonEdgeSelectedColor[3] = OPTION_CARTOONEDGESELECTEDCOLOR_MAX;
   else if(OPTION_CARTOONEDGESELECTEDCOLOR_MIN > f[3])
      m_cartoonEdgeSelectedColor[3] = OPTION_CARTOONEDGESELECTEDCOLOR_MIN;
   else
      m_cartoonEdgeSelectedColor[3] = f[3];
}

/* Option::getCameraRotation: get camera rotation */
float *Option::getCameraRotation()
{
   return m_cameraRotation;
}

/* Option::setCameraRotation: set camera rotation */
void Option::setCameraRotation(const float *f)
{
   if(OPTION_CAMERAROTATION_MAX < f[0])
      m_cameraRotation[0] = OPTION_CAMERAROTATION_MAX;
   else if(OPTION_CAMERAROTATION_MIN > f[0])
      m_cameraRotation[0] = OPTION_CAMERAROTATION_MIN;
   else
      m_cameraRotation[0] = f[0];

   if(OPTION_CAMERAROTATION_MAX < f[1])
      m_cameraRotation[1] = OPTION_CAMERAROTATION_MAX;
   else if(OPTION_CAMERAROTATION_MIN > f[1])
      m_cameraRotation[1] = OPTION_CAMERAROTATION_MIN;
   else
      m_cameraRotation[1] = f[1];

   if(OPTION_CAMERAROTATION_MAX < f[2])
      m_cameraRotation[2] = OPTION_CAMERAROTATION_MAX;
   else if(OPTION_CAMERAROTATION_MIN > f[2])
      m_cameraRotation[2] = OPTION_CAMERAROTATION_MIN;
   else
      m_cameraRotation[2] = f[2];
}

/* Option::getCameraTransition: get camera transition */
float *Option::getCameraTransition()
{
   return m_cameraTransition;
}

/* Option::setCameraTransition: set camera transition */
void Option::setCameraTransition(const float *f)
{
   if(OPTION_CAMERATRANSITION_MAX < f[0])
      m_cameraTransition[0] = OPTION_CAMERATRANSITION_MAX;
   else if(OPTION_CAMERATRANSITION_MIN > f[0])
      m_cameraTransition[0] = OPTION_CAMERATRANSITION_MIN;
   else
      m_cameraTransition[0] = f[0];

   if(OPTION_CAMERATRANSITION_MAX < f[1])
      m_cameraTransition[1] = OPTION_CAMERATRANSITION_MAX;
   else if(OPTION_CAMERATRANSITION_MIN > f[1])
      m_cameraTransition[1] = OPTION_CAMERATRANSITION_MIN;
   else
      m_cameraTransition[1] = f[1];

   if(OPTION_CAMERATRANSITION_MAX < f[2])
      m_cameraTransition[2] = OPTION_CAMERATRANSITION_MAX;
   else if(OPTION_CAMERATRANSITION_MIN > f[2])
      m_cameraTransition[2] = OPTION_CAMERATRANSITION_MIN;
   else
      m_cameraTransition[2] = f[2];
}

/* Option::getCameraDistance: get camera distance */
float Option::getCameraDistance()
{
   return m_cameraDistance;
}

/* Option::setCameraDistance: set camera distance */
void Option::setCameraDistance(float f)
{
   if(OPTION_CAMERADISTANCE_MAX < f)
      m_cameraDistance = OPTION_CAMERADISTANCE_MAX;
   else if(OPTION_CAMERADISTANCE_MIN > f)
      m_cameraDistance = OPTION_CAMERADISTANCE_MIN;
   else
      m_cameraDistance = f;
}

/* Option::getCameraFovy: get camera fovy */
float Option::getCameraFovy()
{
   return m_cameraFovy;
}

/* Option::setCameraFovy: set camera fovy */
void Option::setCameraFovy(float f)
{
   if(OPTION_CAMERAFOVY_MAX < f)
      m_cameraFovy = OPTION_CAMERAFOVY_MAX;
   else if(OPTION_CAMERAFOVY_MIN > f)
      m_cameraFovy = OPTION_CAMERAFOVY_MIN;
   else
      m_cameraFovy = f;
}

/* Option::getStageSize: get stage size */
float *Option::getStageSize()
{
   return m_stageSize;
}

/* Option::setStageSize: set stage size */
void Option::setStageSize(const float *f)
{
   if(OPTION_STAGESIZE_MAX < f[0])
      m_stageSize[0] = OPTION_STAGESIZE_MAX;
   else if(OPTION_STAGESIZE_MIN > f[0])
      m_stageSize[0] = OPTION_STAGESIZE_MIN;
   else
      m_stageSize[0] = f[0];

   if(OPTION_STAGESIZE_MAX < f[1])
      m_stageSize[1] = OPTION_STAGESIZE_MAX;
   else if(OPTION_STAGESIZE_MIN > f[1])
      m_stageSize[1] = OPTION_STAGESIZE_MIN;
   else
      m_stageSize[1] = f[1];

   if(OPTION_STAGESIZE_MAX < f[2])
      m_stageSize[2] = OPTION_STAGESIZE_MAX;
   else if(OPTION_STAGESIZE_MIN > f[2])
      m_stageSize[2] = OPTION_STAGESIZE_MIN;
   else
      m_stageSize[2] = f[2];
}

/* Option::getShowFps: get fps flag */
bool Option::getShowFps()
{
   return m_showFps;
}

/* Option::setShowFps: set fps flag */
void Option::setShowFps(bool b)
{
   m_showFps = b;
}

/* Option::getFpsPosition: get fps position */
float *Option::getFpsPosition()
{
   return m_fpsPosition;
}

/* Option::setFpsPosition: set fps position */
void Option::setFpsPosition(const float *f)
{
   m_fpsPosition[0] = f[0];
   m_fpsPosition[1] = f[1];
   m_fpsPosition[2] = f[2];
}

/* Option::getWindowSize: get window size */
int *Option::getWindowSize()
{
   return m_windowSize;
}

/* Option::setWindowSize: set window size */
void Option::setWindowSize(const int *i)
{
   if(OPTION_WINDOWSIZE_MAX < i[0])
      m_windowSize[0] = OPTION_WINDOWSIZE_MAX;
   else if(OPTION_WINDOWSIZE_MIN > i[0])
      m_windowSize[0] = OPTION_WINDOWSIZE_MIN;
   else
      m_windowSize[0] = i[0];

   if(OPTION_WINDOWSIZE_MAX < i[1])
      m_windowSize[1] = OPTION_WINDOWSIZE_MAX;
   else if(OPTION_WINDOWSIZE_MIN > i[1])
      m_windowSize[1] = OPTION_WINDOWSIZE_MIN;
   else
      m_windowSize[1] = i[1];
}

/* Option::getFullScreen: get full screen flag */
bool Option::getFullScreen()
{
   return m_fullScreen;
}

/* Option::setFullScreen: set full screen flag */
void Option::setFullScreen(bool b)
{
   m_fullScreen = b;
}

/* Option::getLogSize: get log window size */
int *Option::getLogSize()
{
   return m_logSize;
}

/* Option::setLogSize: set log window size */
void Option::setLogSize(const int *i)
{
   if(OPTION_LOGSIZE_MAX < i[0])
      m_logSize[0] = OPTION_LOGSIZE_MAX;
   else if(OPTION_LOGSIZE_MIN > i[0])
      m_logSize[0] = OPTION_LOGSIZE_MIN;
   else
      m_logSize[0] = i[0];

   if(OPTION_LOGSIZE_MAX < i[1])
      m_logSize[1] = OPTION_LOGSIZE_MAX;
   else if(OPTION_LOGSIZE_MIN > i[1])
      m_logSize[1] = OPTION_LOGSIZE_MIN;
   else
      m_logSize[1] = i[1];
}

/* Option::getLogPosition: get log window position */
float *Option::getLogPosition()
{
   return m_logPosition;
}

/* Option::setLogPosition: set log window position */
void Option::setLogPosition(const float *f)
{
   m_logPosition[0] = f[0];
   m_logPosition[1] = f[1];
   m_logPosition[2] = f[2];
}

/* Option::getLogScale: get log window scale */
float Option::getLogScale()
{
   return m_logScale;
}

/* Option::setLogScale: set log window scale */
void Option::setLogScale(float f)
{
   if(OPTION_LOGSCALE_MAX < f)
      m_logScale = OPTION_LOGSCALE_MAX;
   else if(OPTION_LOGSCALE_MIN > f)
      m_logScale = OPTION_LOGSCALE_MIN;
   else
      m_logScale = f;
}

/* Option::getLogDirection: get light direction */
float *Option::getLightDirection()
{
   return m_lightDirection;
}

/* Option::setLogDirection: set light direction */
void Option::setLightDirection(const float *f)
{
   m_lightDirection[0] = f[0];
   m_lightDirection[1] = f[1];
   m_lightDirection[2] = f[2];
   m_lightDirection[3] = f[3];
}

/* Option::getLogIntensity: get light intensity */
float Option::getLightIntensity()
{
   return m_lightIntensity;
}

/* Option::setLogIntensity: set light intensity */
void Option::setLightIntensity(float f)
{
   if(OPTION_LIGHTINTENSITY_MAX < f)
      m_lightIntensity = OPTION_LIGHTINTENSITY_MAX;
   else if(OPTION_LIGHTINTENSITY_MIN > f)
      m_lightIntensity = OPTION_LIGHTINTENSITY_MIN;
   else
      m_lightIntensity = f;
}

/* Option::getLightColor: get light color */
float *Option::getLightColor()
{
   return m_lightColor;
}

/* Option::setLightColor: set light color */
void Option::setLightColor(const float *f)
{
   if(OPTION_LIGHTCOLOR_MAX < f[0])
      m_lightColor[0] = OPTION_LIGHTCOLOR_MAX;
   else if(OPTION_LIGHTCOLOR_MIN > f[0])
      m_lightColor[0] = OPTION_LIGHTCOLOR_MIN;
   else
      m_lightColor[0] = f[0];

   if(OPTION_LIGHTCOLOR_MAX < f[1])
      m_lightColor[1] = OPTION_LIGHTCOLOR_MAX;
   else if(OPTION_LIGHTCOLOR_MIN > f[1])
      m_lightColor[1] = OPTION_LIGHTCOLOR_MIN;
   else
      m_lightColor[1] = f[1];

   if(OPTION_LIGHTCOLOR_MAX < f[2])
      m_lightColor[2] = OPTION_LIGHTCOLOR_MAX;
   else if(OPTION_LIGHTCOLOR_MIN > f[2])
      m_lightColor[2] = OPTION_LIGHTCOLOR_MIN;
   else
      m_lightColor[2] = f[2];
}

/* Option::getCampusColor: get campus color */
float *Option::getCampusColor()
{
   return m_campusColor;
}

/* Option::setCampusColor: set campus color */
void Option::setCampusColor(const float *f)
{
   if(OPTION_CAMPUSCOLOR_MAX < f[0])
      m_campusColor[0] = OPTION_CAMPUSCOLOR_MAX;
   else if(OPTION_CAMPUSCOLOR_MIN > f[0])
      m_campusColor[0] = OPTION_CAMPUSCOLOR_MIN;
   else
      m_campusColor[0] = f[0];

   if(OPTION_CAMPUSCOLOR_MAX < f[1])
      m_campusColor[1] = OPTION_CAMPUSCOLOR_MAX;
   else if(OPTION_CAMPUSCOLOR_MIN > f[1])
      m_campusColor[1] = OPTION_CAMPUSCOLOR_MIN;
   else
      m_campusColor[1] = f[1];

   if(OPTION_CAMPUSCOLOR_MAX < f[2])
      m_campusColor[2] = OPTION_CAMPUSCOLOR_MAX;
   else if(OPTION_CAMPUSCOLOR_MIN > f[2])
      m_campusColor[2] = OPTION_CAMPUSCOLOR_MIN;
   else
      m_campusColor[2] = f[2];
}

/* Option::getMaxMultiSampling: get max number of multi sampling */
int Option::getMaxMultiSampling()
{
   return m_maxMultiSampling;
}

/* Option::setMaxMultiSampling: set max number of multi sampling */
void Option::setMaxMultiSampling(int i)
{
   if(OPTION_MAXMULTISAMPLING_MAX < i)
      m_maxMultiSampling = OPTION_MAXMULTISAMPLING_MAX;
   else if(OPTION_MAXMULTISAMPLING_MIN > i)
      m_maxMultiSampling = OPTION_MAXMULTISAMPLING_MIN;
   else
      m_maxMultiSampling = i;
}

/* Option::getMotionAdjustFrame: get motion adjust frame */
int Option::getMotionAdjustFrame()
{
   return m_motionAdjustFrame;
}

/* Option::setMotionAdjustFrame: set motion adjust frame */
void Option::setMotionAdjustFrame(int i)
{
   m_motionAdjustFrame = i;
}

/* Option::getBulletFps: get bullet fps */
int Option::getBulletFps()
{
   return m_bulletFps;
}

/* Option::setBulletFps: set bullet fps */
void Option::setBulletFps(int i)
{
   if(OPTION_BULLETFPS_MAX < i)
      m_bulletFps = OPTION_BULLETFPS_MAX;
   else if(OPTION_BULLETFPS_MIN > i)
      m_bulletFps = OPTION_BULLETFPS_MIN;
   else
      m_bulletFps = i;
}

/* Option::getRotateStep: get rotate step */
float Option::getRotateStep()
{
   return m_rotateStep;
}

/* Option::setRotateStep: set rotate step */
void Option::setRotateStep(float f)
{
   if(OPTION_ROTATESTEP_MAX < f)
      m_rotateStep = OPTION_ROTATESTEP_MAX;
   else if(OPTION_ROTATESTEP_MIN > f)
      m_rotateStep = OPTION_ROTATESTEP_MIN;
   else
      m_rotateStep = f;
}

/* Option::getTranslateStep: get translate step */
float Option::getTranslateStep()
{
   return m_translateStep;
}

/* Option::setTranslateStep: set translate step */
void Option::setTranslateStep(float f)
{
   if(OPTION_TRANSLATESTEP_MAX < f)
      m_translateStep = OPTION_TRANSLATESTEP_MAX;
   else if(OPTION_TRANSLATESTEP_MIN > f)
      m_translateStep = OPTION_TRANSLATESTEP_MIN;
   else
      m_translateStep = f;
}

/* Option::getDistanceStep: get distance step */
float Option::getDistanceStep()
{
   return m_distanceStep;
}

/* Option::setDistanceStep: set distance step */
void Option::setDistanceStep(float f)
{
   if(OPTION_DISTANCESTEP_MAX < f)
      m_distanceStep = OPTION_DISTANCESTEP_MAX;
   else if(OPTION_DISTANCESTEP_MIN > f)
      m_distanceStep = OPTION_DISTANCESTEP_MIN;
   else
      m_distanceStep = f;
}

/* Option::getFovyStep: get fovy step */
float Option::getFovyStep()
{
   return m_fovyStep;
}

/* Option::setFovyStep: set fovy step */
void Option::setFovyStep(float f)
{
   if(OPTION_FOVYSTEP_MAX < f)
      m_fovyStep = OPTION_FOVYSTEP_MAX;
   else if(OPTION_FOVYSTEP_MIN > f)
      m_fovyStep = OPTION_FOVYSTEP_MIN;
   else
      m_fovyStep = f;
}

/* Option::getUseShadowMapping: get shadow mapping flag */
bool Option::getUseShadowMapping()
{
   return m_useShadowMapping;
}

/* Option::setUseShadowMapping: set shadow mapping flag */
void Option::setUseShadowMapping(bool b)
{
   m_useShadowMapping = b;
}

/* Option::getShadowMappingTextureSize: get texture size of shadow mapping */
int Option::getShadowMappingTextureSize()
{
   return m_shadowMapTextureSize;
}

/* Option::setShadowMappingTextureSize: set texture size of shadow mapping */
void Option::setShadowMappingTextureSize(int i)
{
   if(OPTION_SHADOWMAPPINGTEXTURESIZE_MAX < i)
      m_shadowMapTextureSize = OPTION_SHADOWMAPPINGTEXTURESIZE_MAX;
   else if(OPTION_SHADOWMAPPINGTEXTURESIZE_MIN > i)
      m_shadowMapTextureSize = OPTION_SHADOWMAPPINGTEXTURESIZE_MIN;
   else
      m_shadowMapTextureSize = i;
}

/* Option::getShadowMappingSelfDensity: get self density of shadow mapping */
float Option::getShadowMappingSelfDensity()
{
   return m_shadowMapSelfDensity;
}

/* Option::setShadowMappingSelfDensity: set self density of shadow mapping */
void Option::setShadowMappingSelfDensity(float f)
{
   if(OPTION_SHADOWMAPPINGSELFDENSITY_MAX < f)
      m_shadowMapSelfDensity = OPTION_SHADOWMAPPINGSELFDENSITY_MAX;
   else if(OPTION_SHADOWMAPPINGSELFDENSITY_MIN > f)
      m_shadowMapSelfDensity = OPTION_SHADOWMAPPINGSELFDENSITY_MIN;
   else
      m_shadowMapSelfDensity = f;
}

/* Option::getShadowMappingFloorDensity: get floor density of shadow mapping */
float Option::getShadowMappingFloorDensity()
{
   return m_shadowMapFloorDensity;
}

/* Option::setShadowMappingFloorDensity: set floor density of shadow mapping */
void Option::setShadowMappingFloorDensity(float f)
{
   if(OPTION_SHADOWMAPPINGFLOORDENSITY_MAX < f)
      m_shadowMapFloorDensity = OPTION_SHADOWMAPPINGFLOORDENSITY_MAX;
   else if(OPTION_SHADOWMAPPINGFLOORDENSITY_MIN > f)
      m_shadowMapFloorDensity = OPTION_SHADOWMAPPINGFLOORDENSITY_MIN;
   else
      m_shadowMapFloorDensity = f;
}

/* Option::getShadowMappingLightFirst: get first light flag */
bool Option::getShadowMappingLightFirst()
{
   return m_shadowMapLightFirst;
}

/* Option::setShadowMappingLightFirst: set first light flag */
void Option::setShadowMappingLightFirst(bool b)
{
   m_shadowMapLightFirst = b;
}

/* Option::getDisplayCommentFrame: get display comment frame */
float Option::getDisplayCommentFrame()
{
   return m_displayCommentFrame;
}

/* Option::setDisplayCommentFrame: set display comment frame */
void Option::setDisplayCommentFrame(float f)
{
   if(OPTION_DISPLAYCOMMENTFRAME_MAX < f)
      m_displayCommentFrame = OPTION_DISPLAYCOMMENTFRAME_MAX;
   else if(OPTION_DISPLAYCOMMENTFRAME_MIN > f)
      m_displayCommentFrame = OPTION_DISPLAYCOMMENTFRAME_MIN;
   else
      m_displayCommentFrame = f;
}

/* Option::getMaxNumModel: get maximum number of models */
int Option::getMaxNumModel()
{
   return m_maxNumModel;
}

/* Option::setMaxNumModel: set maximum number of models */
void Option::setMaxNumModel(int i)
{
   if(OPTION_MAXNUMMODEL_MAX < i)
      m_maxNumModel = OPTION_MAXNUMMODEL_MAX;
   else if(OPTION_MAXNUMMODEL_MIN > i)
      m_maxNumModel = OPTION_MAXNUMMODEL_MIN;
   else
      m_maxNumModel = i;
}
