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

#define OPTION_MAXBUFLEN 2048

#define OPTION_USECARTOONRENDERING_STR "use_cartoon_rendering"
#define OPTION_USECARTOONRENDERING_DEF true

#define OPTION_USEMMDLIKECARTOON_STR "use_mmd_like_cartoon"
#define OPTION_USEMMDLIKECARTOON_DEF true

#define OPTION_CARTOONEDGEWIDTH_STR "cartoon_edge_width"
#define OPTION_CARTOONEDGEWIDTH_DEF 0.7f
#define OPTION_CARTOONEDGEWIDTH_MAX 1000.0f
#define OPTION_CARTOONEDGEWIDTH_MIN 0.001f

#define OPTION_CARTOONEDGESTEP_STR "cartoon_edge_step"
#define OPTION_CARTOONEDGESTEP_DEF 1.2f
#define OPTION_CARTOONEDGESTEP_MAX 10.0f
#define OPTION_CARTOONEDGESTEP_MIN 1.0f

#define OPTION_CARTOONEDGESELECTEDCOLOR_STR  "cartoon_edge_selected_color"
#define OPTION_CARTOONEDGESELECTEDCOLORR_DEF 1.0f
#define OPTION_CARTOONEDGESELECTEDCOLORG_DEF 0.0f
#define OPTION_CARTOONEDGESELECTEDCOLORB_DEF 0.0f
#define OPTION_CARTOONEDGESELECTEDCOLORA_DEF 1.0f
#define OPTION_CARTOONEDGESELECTEDCOLOR_MAX  1.0f
#define OPTION_CARTOONEDGESELECTEDCOLOR_MIN  0.0f

#define OPTION_CAMERAROTATION_STR  "camera_rotation"
#define OPTION_CAMERAROTATIONX_DEF 0.0f
#define OPTION_CAMERAROTATIONY_DEF 0.0f
#define OPTION_CAMERAROTATIONZ_DEF 0.0f
#define OPTION_CAMERAROTATION_MAX  1000.0f
#define OPTION_CAMERAROTATION_MIN  0.001f

#define OPTION_CAMERATRANSITION_STR  "camera_transition"
#define OPTION_CAMERATRANSITIONX_DEF 0.0f
#define OPTION_CAMERATRANSITIONY_DEF 13.0f
#define OPTION_CAMERATRANSITIONZ_DEF 0.0f
#define OPTION_CAMERATRANSITION_MAX  10000.0f
#define OPTION_CAMERATRANSITION_MIN  -10000.0f

#define OPTION_CAMERADISTANCE_STR "camera_distance"
#define OPTION_CAMERADISTANCE_DEF 100.0f
#define OPTION_CAMERADISTANCE_MAX 100000.0f
#define OPTION_CAMERADISTANCE_MIN 0.0f

#define OPTION_CAMERAFOVY_STR "camera_fovy"
#define OPTION_CAMERAFOVY_DEF 16.0f
#define OPTION_CAMERAFOVY_MAX 180.0f
#define OPTION_CAMERAFOVY_MIN 0.0f

#define OPTION_STAGESIZE_STR  "stage_size"
#define OPTION_STAGESIZEW_DEF 25.0f
#define OPTION_STAGESIZED_DEF 25.0f
#define OPTION_STAGESIZEH_DEF 40.0f
#define OPTION_STAGESIZE_MAX  1000.0f
#define OPTION_STAGESIZE_MIN  0.001f

#define OPTION_SHOWFPS_STR "show_fps"
#define OPTION_SHOWFPS_DEF true

#define OPTION_FPSPOSITION_STR "fps_position"
#define OPTION_FPSPOSITIONX_DEF -2.5f
#define OPTION_FPSPOSITIONY_DEF 22.0f
#define OPTION_FPSPOSITIONZ_DEF 3.0f

#define OPTION_WINDOWSIZE_STR "window_size"
#define OPTION_WINDOWSIZEW_DEF 600
#define OPTION_WINDOWSIZEH_DEF 600
#define OPTION_WINDOWSIZE_MAX  4096
#define OPTION_WINDOWSIZE_MIN  1

#define OPTION_FULLSCREEN_STR "full_screen"
#define OPTION_FULLSCREEN_DEF false

#define OPTION_LOGSIZE_STR  "log_size"
#define OPTION_LOGSIZEW_DEF 80
#define OPTION_LOGSIZEH_DEF 30
#define OPTION_LOGSIZE_MAX  4096
#define OPTION_LOGSIZE_MIN  1

#define OPTION_LOGPOSITION_STR  "log_position"
#define OPTION_LOGPOSITIONX_DEF -17.5f
#define OPTION_LOGPOSITIONY_DEF 3.0f
#define OPTION_LOGPOSITIONZ_DEF -15.0f

#define OPTION_LOGSCALE_STR "log_scale"
#define OPTION_LOGSCALE_DEF 1.0f
#define OPTION_LOGSCALE_MAX 1000.0f
#define OPTION_LOGSCALE_MIN 0.001f

#define OPTION_LIGHTDIRECTION_STR  "light_direction"
#define OPTION_LIGHTDIRECTIONX_DEF 0.5f
#define OPTION_LIGHTDIRECTIONY_DEF 1.0f
#define OPTION_LIGHTDIRECTIONZ_DEF 0.5f
#define OPTION_LIGHTDIRECTIONI_DEF 0.0f

#define OPTION_LIGHTINTENSITY_STR "light_intensity"
#define OPTION_LIGHTINTENSITY_DEF 0.6f
#define OPTION_LIGHTINTENSITY_MAX 1.0f
#define OPTION_LIGHTINTENSITY_MIN 0.0f

#define OPTION_LIGHTCOLOR_STR  "light_color"
#define OPTION_LIGHTCOLORR_DEF 1.0f
#define OPTION_LIGHTCOLORG_DEF 1.0f
#define OPTION_LIGHTCOLORB_DEF 1.0f
#define OPTION_LIGHTCOLOR_MAX  1.0f
#define OPTION_LIGHTCOLOR_MIN  0.0f

#define OPTION_CAMPUSCOLOR_STR  "campus_color"
#define OPTION_CAMPUSCOLORR_DEF 0.0f
#define OPTION_CAMPUSCOLORG_DEF 0.0f
#define OPTION_CAMPUSCOLORB_DEF 0.2f
#define OPTION_CAMPUSCOLOR_MAX  1.0f
#define OPTION_CAMPUSCOLOR_MIN  0.0f

#define OPTION_MAXMULTISAMPLING_STR "max_multi_sampling"
#define OPTION_MAXMULTISAMPLING_DEF 4
#define OPTION_MAXMULTISAMPLING_MAX 32
#define OPTION_MAXMULTISAMPLING_MIN 0

#define OPTION_MOTIONADJUSTFRAME_STR "motion_adjust_frame"
#define OPTION_MOTIONADJUSTFRAME_DEF 0

#define OPTION_BULLETFPS_STR "bullet_fps"
#define OPTION_BULLETFPS_DEF 120
#define OPTION_BULLETFPS_MAX 120
#define OPTION_BULLETFPS_MIN 1

#define OPTION_ROTATESTEP_STR "rotate_step"
#define OPTION_ROTATESTEP_DEF 4.5f
#define OPTION_ROTATESTEP_MAX 180.0f
#define OPTION_ROTATESTEP_MIN 0.001f

#define OPTION_TRANSLATESTEP_STR "translate_step"
#define OPTION_TRANSLATESTEP_DEF 0.5f
#define OPTION_TRANSLATESTEP_MAX 1000.0f
#define OPTION_TRANSLATESTEP_MIN 0.001f

#define OPTION_DISTANCESTEP_STR "distance_step"
#define OPTION_DISTANCESTEP_DEF 4.0f
#define OPTION_DISTANCESTEP_MAX 1000.0f
#define OPTION_DISTANCESTEP_MIN 0.001f

#define OPTION_FOVYSTEP_STR "fovy_step"
#define OPTION_FOVYSTEP_DEF 1.0f
#define OPTION_FOVYSTEP_MAX 1000.0f
#define OPTION_FOVYSTEP_MIN 0.001f

#define OPTION_USESHADOWMAPPING_STR "use_shadow_mapping"
#define OPTION_USESHADOWMAPPING_DEF false

#define OPTION_SHADOWMAPPINGTEXTURESIZE_STR "shadow_mapping_texture_size"
#define OPTION_SHADOWMAPPINGTEXTURESIZE_DEF 1024
#define OPTION_SHADOWMAPPINGTEXTURESIZE_MAX 8192
#define OPTION_SHADOWMAPPINGTEXTURESIZE_MIN 1

#define OPTION_SHADOWMAPPINGSELFDENSITY_STR "shadow_mapping_self_density"
#define OPTION_SHADOWMAPPINGSELFDENSITY_DEF 1.0f
#define OPTION_SHADOWMAPPINGSELFDENSITY_MAX 1.0f
#define OPTION_SHADOWMAPPINGSELFDENSITY_MIN 0.0f

#define OPTION_SHADOWMAPPINGFLOORDENSITY_STR "shadow_mapping_floor_density"
#define OPTION_SHADOWMAPPINGFLOORDENSITY_DEF 0.5f
#define OPTION_SHADOWMAPPINGFLOORDENSITY_MAX 1.0f
#define OPTION_SHADOWMAPPINGFLOORDENSITY_MIN 0.0f

#define OPTION_SHADOWMAPPINGLIGHTFIRST_STR "shadow_mapping_light_first"
#define OPTION_SHADOWMAPPINGLIGHTFIRST_DEF true

#define OPTION_DISPLAYCOMMENTFRAME_STR "display_comment_frame"
#define OPTION_DISPLAYCOMMENTFRAME_DEF 150.0f
#define OPTION_DISPLAYCOMMENTFRAME_MAX 1000.0f
#define OPTION_DISPLAYCOMMENTFRAME_MIN 0.0f

#define OPTION_MAXNUMMODEL_STR "max_num_model"
#define OPTION_MAXNUMMODEL_DEF 10
#define OPTION_MAXNUMMODEL_MAX 1024
#define OPTION_MAXNUMMODEL_MIN 1

/* Option: user options */
class Option
{
private:

   /* cartoon rendering */
   bool m_useCartoonRendering;
   bool m_useMMDLikeCartoon;
   float m_cartoonEdgeWidth;
   float m_cartoonEdgeStep;
   float m_cartoonEdgeSelectedColor[4];

   /* camera viewpoint parameters */
   float m_cameraRotation[3];
   float m_cameraTransition[3];
   float m_cameraDistance;
   float m_cameraFovy;

   /* stage */
   float m_stageSize[3];

   /* fps */
   bool m_showFps;
   float m_fpsPosition[3];

   /* window */
   int m_windowSize[2];
   bool m_fullScreen;

   /* log */
   int m_logSize[2];
   float m_logPosition[3];
   float m_logScale;

   /* light */
   float m_lightDirection[4];
   float m_lightIntensity;
   float m_lightColor[3];

   /* campus */
   float m_campusColor[3];

   /* OpenGL */
   int m_maxMultiSampling;

   /* motion */
   int m_motionAdjustFrame;

   /* bullet physics */
   int m_bulletFps;

   /* move */
   float m_rotateStep;
   float m_translateStep;
   float m_distanceStep;
   float m_fovyStep;

   /* shadow mapping */
   bool m_useShadowMapping;
   int m_shadowMapTextureSize;
   float m_shadowMapSelfDensity;
   float m_shadowMapFloorDensity;
   bool m_shadowMapLightFirst;

   /* comment */
   float m_displayCommentFrame;

   /* model */
   int m_maxNumModel;

   /* initialize: initialize options */
   void initialize();

public:

   /* Option: constructor */
   Option();

   /* load: load options */
   bool load(const char *file);

   /* getUseCartoonRendering: get cartoon rendering flag */
   bool getUseCartoonRendering();

   /* setUseCartoonRendering: set cartoon rendering flag */
   void setUseCartoonRendering(bool b);

   /* getUseMMDLikeCartoon: get MMD like cartoon flag */
   bool getUseMMDLikeCartoon();

   /* setUseMMDLikeCartoon: set MMD like cartoon flag */
   void setUseMMDLikeCartoon(bool b);

   /* getCartoonEdgeWidth: get edge width for catoon */
   float getCartoonEdgeWidth();

   /* setCartoonEdgeWidth: set edge width for catoon */
   void setCartoonEdgeWidth(float f);

   /* getCartoonEdgeStep: get cartoon edge step */
   float getCartoonEdgeStep();

   /* setCartoonEdgeStep: set cartoon edge step */
   void setCartoonEdgeStep(float f);

   /* getCartoonEdgeSelectedColor: get cartoon edge seleceted color */
   float *getCartoonEdgeSelectedColor();

   /* setCartoonEdgeSelectedColor: set cartoon edge seleceted color */
   void setCartoonEdgeSelectedColor(const float *f);

   /* getCameraRotation: get camera rotation */
   float *getCameraRotation();

   /* setCameraRotation: set camera rotation */
   void setCameraRotation(const float *f);

   /* getCameraTransition: get camera transition */
   float *getCameraTransition();

   /* setCameraTransition: set camera transition */
   void setCameraTransition(const float *f);

   /* getCameraDistance: get camera distance */
   float getCameraDistance();

   /* setCameraDistance: set camera distance */
   void setCameraDistance(float f);

   /* getCameraFovy: get camera fovy */
   float getCameraFovy();

   /* setCameraFovy: set camera fovy */
   void setCameraFovy(float f);

   /* getStageSize: get stage size */
   float *getStageSize();

   /* setStageSize: set stage size */
   void setStageSize(const float *f);

   /* getShowFps: get fps flag */
   bool getShowFps();

   /* setShowFps: set fps flag */
   void setShowFps(bool b);

   /* getFpsPosition: get fps position */
   float *getFpsPosition();

   /* setFpsPosition: set fps position */
   void setFpsPosition(const float *f);

   /* getWindowSize: get window size */
   int *getWindowSize();

   /* setWindowSize: set window size */
   void setWindowSize(const int *i);

   /* getFullScreen: get full screen flag */
   bool getFullScreen();

   /* setFullScreen: set full screen flag */
   void setFullScreen(bool b);

   /* getLogSize: get log window size */
   int* getLogSize();

   /* setLogSize: set log window size */
   void setLogSize(const int *i);

   /* getLogPosition: get log window position */
   float *getLogPosition();

   /* setLogPosition: set log window position */
   void setLogPosition(const float *f);

   /* getLogScale: get log window scale */
   float getLogScale();

   /* setLogScale: set log window scale */
   void setLogScale(float f);

   /* getLogDirection: get light direction */
   float *getLightDirection();

   /* setLogDirection: set light direction */
   void setLightDirection(const float *f);

   /* getLogIntensity: get light intensity */
   float getLightIntensity();

   /* setLogIntensity: set light intensity */
   void setLightIntensity(float f);

   /* getLightColor: get light color */
   float *getLightColor();

   /* setLightColor: set light color */
   void setLightColor(const float *f);

   /* getCampusColor: get campus color */
   float *getCampusColor();

   /* setCampusColor: set campus color */
   void setCampusColor(const float *f);

   /* getMaxMultiSampling: get max number of multi sampling */
   int getMaxMultiSampling();

   /* setMaxMultiSampling: set max number of multi sampling */
   void setMaxMultiSampling(int i);

   /* getMotionAdjustFrame: get motion adjust frame */
   int getMotionAdjustFrame();

   /* setMotionAdjustFrame: set motion adjust frame */
   void setMotionAdjustFrame(int i);

   /* getBulletFps: get bullet fps */
   int getBulletFps();

   /* setBulletFps: set bullet fps */
   void setBulletFps(int i);

   /* getRotateStep: get rotate step */
   float getRotateStep();

   /* setRotateStep: set rotate step */
   void setRotateStep(float f);

   /* getTranslateStep: get translate step */
   float getTranslateStep();

   /* setTranslateStep: set translate step */
   void setTranslateStep(float f);

   /* getDistanceStep: get distance step */
   float getDistanceStep();

   /* setDistanceStep: set distance step */
   void setDistanceStep(float f);

   /* getFovyStep: get fovy step */
   float getFovyStep();

   /* setFovyStep: set fovy step */
   void setFovyStep(float f);

   /* getUseShadowMapping: get shadow mapping flag */
   bool getUseShadowMapping();

   /* setUseShadowMapping: set shadow mapping flag */
   void setUseShadowMapping(bool b);

   /* getShadowMappingTextureSize: get texture size of shadow mapping */
   int getShadowMappingTextureSize();

   /* setShadowMappingTextureSize: set texture size of shadow mapping */
   void setShadowMappingTextureSize(int i);

   /* getShadowMappingSelfDensity: get self density of shadow mapping */
   float getShadowMappingSelfDensity();

   /* setShadowMappingSelfDensity: set self density of shadow mapping */
   void setShadowMappingSelfDensity(float f);

   /* getShadowMappingFloorDensity: get floor density of shadow mapping */
   float getShadowMappingFloorDensity();

   /* setShadowMappingFloorDensity: set floor density of shadow mapping */
   void setShadowMappingFloorDensity(float f);

   /* getShadowMappingLightFirst: get first light flag */
   bool getShadowMappingLightFirst();

   /* setShadowMappingLightFirst: set first light flag */
   void setShadowMappingLightFirst(bool b);

   /* getDisplayCommentFrame: get display comment frame */
   float getDisplayCommentFrame();

   /* setDisplayCommentFrame: set display comment frame */
   void setDisplayCommentFrame(float f);

   /* getMaxNumModel: get maximum number of models */
   int getMaxNumModel();

   /* setMaxNumModel: set maximum number of models */
   void setMaxNumModel(int i);
};
