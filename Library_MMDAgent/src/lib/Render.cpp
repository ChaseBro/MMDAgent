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
#include <GL/glu.h>

/* compareDepth: qsort function for reordering */
static int compareDepth(const void *a, const void *b)
{
   RenderDepthData *x = (RenderDepthData *) a;
   RenderDepthData *y = (RenderDepthData *) b;

   if (x->dist == y->dist)
      return 0;
   return ( (x->dist > y->dist) ? 1 : -1 );
}

/* Render::updateProjectionMatrix: update view information */
void Render::updateProjectionMatrix()
{
   glViewport(0, 0, m_width, m_height);

   /* camera setting */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   applyProjectionMatrix();
   glMatrixMode(GL_MODELVIEW);
}

/* Render::applyProjectionMatirx: update projection matrix */
void Render::applyProjectionMatrix()
{
   gluPerspective(m_currentFovy, (double) m_width / (double) m_height, RENDER_VIEWPOINTFRUSTUMNEAR, RENDER_VIEWPOINTFRUSTUMFAR);
}

/* Render::updateModelViewMatrix: update model view matrix */
void Render::updateModelViewMatrix()
{
   m_transMatrix.setIdentity();
   m_transMatrix.setRotation(m_currentRot);
   m_transMatrix.setOrigin(m_transMatrix * ( - m_currentTrans) - btVector3(0.0f, 0.0f, m_currentDistance));
   m_transMatrixInv = m_transMatrix.inverse();
   m_transMatrix.getOpenGLMatrix(m_rotMatrix);
   m_transMatrixInv.getOpenGLMatrix(m_rotMatrixInv);
}

/* Render::updateTransRotMatrix:  update trans and rotation matrix */
bool Render::updateTransRotMatrix(double ellapsedTimeForMove)
{
   float diff1, diff2;
   btVector3 trans;
   btQuaternion rot;

   /* if no difference, return */
   if (m_currentRot == m_rot && m_currentTrans == m_trans)
      return false;

   if (m_viewMoveTime == 0 || m_viewControlledByMotion == true) {
      /* immediately apply the target */
      m_currentRot = m_rot;
      m_currentTrans = m_trans;
   } else if (m_viewMoveTime > 0) {
      /* constant move */
      if (ellapsedTimeForMove >= m_viewMoveTime) {
         m_currentRot = m_rot;
         m_currentTrans = m_trans;
      } else {
         m_currentTrans = m_viewMoveStartTrans.lerp(m_trans, (btScalar) (ellapsedTimeForMove / m_viewMoveTime));
         m_currentRot = m_viewMoveStartRot.slerp(m_rot, (btScalar) (ellapsedTimeForMove / m_viewMoveTime));
      }
   } else {
      /* calculate difference */
      trans = m_trans;
      trans -= m_currentTrans;
      diff1 = trans.length2();
      rot = m_rot;
      rot -= m_currentRot;
      diff2 = rot.length2();

      if (diff1 > RENDER_MINMOVEDIFF)
         m_currentTrans = m_currentTrans.lerp(m_trans, 1.0f - RENDER_MOVESPEEDRATE); /* current * 0.9 + target * 0.1 */
      else
         m_currentTrans = m_trans;
      if (diff2 > RENDER_MINSPINDIFF)
         m_currentRot = m_currentRot.slerp(m_rot, 1.0f - RENDER_SPINSPEEDRATE); /* current * 0.9 + target * 0.1 */
      else
         m_currentRot = m_rot;
   }

   return true;
}

/* Render::updateRotationFromAngle: update rotation quaternion from angle */
void Render::updateRotationFromAngle()
{
   m_rot = btQuaternion(btVector3(0, 0, 1), MMDFILES_RAD(m_angle.z()))
           * btQuaternion(btVector3(1, 0, 0), MMDFILES_RAD(m_angle.x()))
           * btQuaternion(btVector3(0, 1, 0), MMDFILES_RAD(m_angle.y()));
}

/* Render::updateDistance: update distance */
bool Render::updateDistance(double ellapsedTimeForMove)
{
   float diff;

   /* if no difference, return */
   if (m_currentDistance == m_distance)
      return false;

   if (m_viewMoveTime == 0 || m_viewControlledByMotion == true) {
      /* immediately apply the target */
      m_currentDistance = m_distance;
   } else if (m_viewMoveTime > 0) {
      /* constant move */
      if (ellapsedTimeForMove >= m_viewMoveTime) {
         m_currentDistance = m_distance;
      } else {
         m_currentDistance = m_viewMoveStartDistance + (m_distance - m_viewMoveStartDistance) * (float)(ellapsedTimeForMove / m_viewMoveTime);
      }
   } else {
      diff = fabs(m_currentDistance - m_distance);
      if (diff < RENDER_MINDISTANCEDIFF) {
         m_currentDistance = m_distance;
      } else {
         m_currentDistance = m_currentDistance * (RENDER_DISTANCESPEEDRATE) + m_distance * (1.0f - RENDER_DISTANCESPEEDRATE);
      }
   }

   return true;
}

/* Render::updateFovy: update distance */
bool Render::updateFovy(double ellapsedTimeForMove)
{
   float diff;

   /* if no difference, return */
   if (m_currentFovy == m_fovy)
      return false;

   if (m_viewMoveTime == 0 || m_viewControlledByMotion == true) {
      /* immediately apply the target */
      m_currentFovy = m_fovy;
   } else if (m_viewMoveTime > 0) {
      /* constant move */
      if (ellapsedTimeForMove >= m_viewMoveTime) {
         m_currentFovy = m_fovy;
      } else {
         m_currentFovy = m_viewMoveStartFovy + (m_fovy - m_viewMoveStartFovy) * (float)(ellapsedTimeForMove / m_viewMoveTime);
      }
   } else {
      diff = fabs(m_currentFovy - m_fovy);
      if (diff < RENDER_MINFOVYDIFF) {
         m_currentFovy = m_fovy;
      } else {
         m_currentFovy = m_currentFovy * (RENDER_FOVYSPEEDRATE) + m_fovy * (1.0f - RENDER_FOVYSPEEDRATE);
      }
   }

   return true;
}

/* Render::initializeShadowMap: initialize OpenGL for shadow mapping */
void Render::initializeShadowMap(int textureSize)
{
   static const GLdouble genfunc[][4] = {
      { 1.0, 0.0, 0.0, 0.0 },
      { 0.0, 1.0, 0.0, 0.0 },
      { 0.0, 0.0, 1.0, 0.0 },
      { 0.0, 0.0, 0.0, 1.0 },
   };

   /* initialize model view matrix */
   glPushMatrix();
   glLoadIdentity();

   /* use 4th texture unit for depth texture, make it current */
   glActiveTextureARB(GL_TEXTURE3_ARB);

   /* prepare a texture object for depth texture rendering in frame buffer object */
   glGenTextures(1, &m_depthTextureID);
   glBindTexture(GL_TEXTURE_2D, m_depthTextureID);

   /* assign depth component to the texture */
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, textureSize, textureSize, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

   /* set texture parameters for shadow mapping */
#ifdef RENDER_SHADOWPCF
   /* use hardware PCF */
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
#else
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
#endif /* RENDER_SHADOWPCF */

   /* tell OpenGL to compare the R texture coordinates to the (depth) texture value */
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

   /* also tell OpenGL to get the compasiron result as alpha value */
   glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_ALPHA);

   /* set texture coordinates generation mode to use the raw texture coordinates (S, T, R, Q) in eye view */
   glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
   glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
   glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
   glTexGeni(GL_Q, GL_TEXTURE_GEN_MODE, GL_EYE_LINEAR);
   glTexGendv(GL_S, GL_EYE_PLANE, genfunc[0]);
   glTexGendv(GL_T, GL_EYE_PLANE, genfunc[1]);
   glTexGendv(GL_R, GL_EYE_PLANE, genfunc[2]);
   glTexGendv(GL_Q, GL_EYE_PLANE, genfunc[3]);

   /* finished configuration of depth texture: unbind the texture */
   glBindTexture(GL_TEXTURE_2D, 0);

   /* allocate a frame buffer object (FBO) for depth buffer rendering */
   glGenFramebuffersEXT(1, &m_fboID);
   /* switch to the newly allocated FBO */
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboID);
   /* bind the texture to the FBO, telling that it should render the depth information to the texture */
   glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, m_depthTextureID, 0);
   /* also tell OpenGL not to draw and read the color buffers */
   glDrawBuffer(GL_NONE);
   glReadBuffer(GL_NONE);
   /* check FBO status */
   if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT) != GL_FRAMEBUFFER_COMPLETE_EXT) {
      /* cannot use FBO */
   }
   /* finished configuration of FBO, now switch to default frame buffer */
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

   /* reset the current texture unit to default */
   glActiveTextureARB(GL_TEXTURE0_ARB);

   /* restore the model view matrix */
   glPopMatrix();
}

/* Render::renderSceneShadowMap: shadow mapping */
void Render::renderSceneShadowMap(PMDObject *objs, short *order, int num, Stage *stage, bool useMMDLikeCartoon, bool useCartoonRendering, float lightIntensity, const float *lightDirection, const float *lightColor, int shadowMappingTextureSize, bool shadowMappingLightFirst, float shadowMappingSelfDensity)
{
   short i;
   GLint viewport[4]; /* store viewport */
   GLdouble modelview[16]; /* store model view transform */
   GLdouble projection[16]; /* store projection transform */

#ifdef RENDER_SHADOWAUTOVIEW
   float eyeDist;
   btVector3 v;
#endif /* RENDER_SHADOWAUTOVIEW */

   static GLfloat lightdim[] = { 0.2f, 0.2f, 0.2f, 1.0f };
   static const GLfloat lightblk[] = { 0.0f, 0.0f, 0.0f, 1.0f };

   /* render the depth texture */
   /* store the current viewport */
   glGetIntegerv(GL_VIEWPORT, viewport);

   /* store the current projection matrix */
   glGetDoublev(GL_PROJECTION_MATRIX, projection);

   /* switch to FBO for depth buffer rendering */
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboID);

   /* clear the buffer */
   /* clear only the depth buffer, since other buffers will not be used */
   glClear(GL_DEPTH_BUFFER_BIT);

   /* set the viewport to the required texture size */
   glViewport(0, 0, shadowMappingTextureSize, shadowMappingTextureSize);

   /* reset the projection matrix */
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   /* set the model view matrix to make the light position as eye point and capture the whole scene in the view */
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

#ifdef RENDER_SHADOWAUTOVIEW
   /* set the distance to cover all the model range */
   eyeDist = m_shadowMapAutoViewRadius / sinf(RENDER_SHADOWAUTOVIEWANGLE * 0.5f * 3.1415926f / 180.0f);
   /* set the perspective */
   gluPerspective(RENDER_SHADOWAUTOVIEWANGLE, 1.0, 1.0, eyeDist + m_shadowMapAutoViewRadius + 50.0f); /* +50.0f is needed to cover the background */
   /* the viewpoint should be at eyeDist far toward light direction from the model center */
   v = m_lightVec * eyeDist + m_shadowMapAutoViewEyePoint;
   gluLookAt(v.x(), v.y(), v.z(), m_shadowMapAutoViewEyePoint.x(), m_shadowMapAutoViewEyePoint.y(), m_shadowMapAutoViewEyePoint.z(), 0.0, 1.0, 0.0);
#else
   /* fixed view */
   gluPerspective(25.0, 1.0, 1.0, 120.0);
   gluLookAt(30.0, 77.0, 30.0, 0.0, 17.0, 0.0, 0.0, 1.0, 0.0);
#endif /* RENDER_SHADOWAUTOVIEW */

   /* keep the current model view for later process */
   glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

   /* do not write into frame buffer other than depth information */
   glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

   /* also, lighting is not needed */
   glDisable(GL_LIGHTING);

   /* disable rendering the front surface to get the depth of back face */
   glCullFace(GL_FRONT);

   /* disable alpha test */
   glDisable(GL_ALPHA_TEST);

   /* we are now writing to depth texture using FBO, so disable the depth texture mapping here */
   glActiveTextureARB(GL_TEXTURE3_ARB);
   glDisable(GL_TEXTURE_2D);
   glActiveTextureARB(GL_TEXTURE0_ARB);

   /* set polygon offset to avoid "moire" */
   glEnable(GL_POLYGON_OFFSET_FILL);
   glPolygonOffset(4.0, 4.0);

   /* render objects for depth */
   /* only objects that wants to drop shadow should be rendered here */
   for (i = 0; i < num; i++) {
      if (objs[order[i]].isEnable() == true) {
         objs[order[i]].getPMDModel()->renderForShadow();
      }
   }

   /* reset the polygon offset */
   glDisable(GL_POLYGON_OFFSET_FILL);

   /* switch to default FBO */
   glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

   /* revert configurations to normal rendering */
   glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
   glMatrixMode(GL_PROJECTION);
   glLoadMatrixd(projection);
   glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
   glEnable(GL_LIGHTING);
   glCullFace(GL_BACK);
   glEnable(GL_ALPHA_TEST);

   /* clear all the buffers */
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   /* render the full scene */
   /* set model view matrix, as the same as normal rendering */
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glMultMatrixf(m_rotMatrix);

   /* render the whole scene */
   if (shadowMappingLightFirst) {
      /* render light setting, later render only the shadow part with dark setting */
      stage->renderBackground();
      stage->renderFloor();
      for (i = 0; i < num; i++) {
         if (objs[order[i]].isEnable() == true) {
            objs[order[i]].getPMDModel()->renderModel();
            objs[order[i]].getPMDModel()->renderEdge();
         }
      }
   } else {
      /* render in dark setting, later render only the non-shadow part with light setting */
      /* light setting for non-toon objects */
      lightdim[0] = lightdim[1] = lightdim[2] = 0.55f - 0.2f * shadowMappingSelfDensity;
      glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdim);
      glLightfv(GL_LIGHT0, GL_AMBIENT, lightdim);
      glLightfv(GL_LIGHT0, GL_SPECULAR, lightblk);

      /* render the non-toon objects (back, floor, non-toon models) */
      stage->renderBackground();
      stage->renderFloor();
      for (i = 0; i < num; i++) {
         if (objs[order[i]].isEnable() == true && objs[order[i]].getPMDModel()->getToonFlag() == false)
            objs[order[i]].getPMDModel()->renderModel();
      }

      /* for toon objects, they should apply the model-defined toon texture color at texture coordinates (0, 0) for shadow rendering */
      /* so restore the light setting */
      if (useCartoonRendering == true)
         updateLight(useMMDLikeCartoon, useCartoonRendering, lightIntensity, lightDirection, lightColor);
      /* render the toon objects */
      for (i = 0; i < num; i++) {
         if (objs[order[i]].isEnable() == true && objs[order[i]].getPMDModel()->getToonFlag() == true) {
            /* set texture coordinates for shadow mapping */
            objs[order[i]].getPMDModel()->updateShadowColorTexCoord(shadowMappingSelfDensity);
            /* tell model to render with the shadow corrdinates */
            objs[order[i]].getPMDModel()->setSelfShadowDrawing(true);
            /* render model and edge */
            objs[order[i]].getPMDModel()->renderModel();
            objs[order[i]].getPMDModel()->renderEdge();
            /* disable shadow rendering */
            objs[order[i]].getPMDModel()->setSelfShadowDrawing(false);
         }
      }
      if (useCartoonRendering == false)
         updateLight(useMMDLikeCartoon, useCartoonRendering, lightIntensity, lightDirection, lightColor);
   }

   /* render the part clipped by the depth texture */
   /* activate the texture unit for shadow mapping and make it current */
   glActiveTextureARB(GL_TEXTURE3_ARB);

   /* set texture matrix (note: matrices should be set in reverse order) */
   glMatrixMode(GL_TEXTURE);
   glLoadIdentity();
   /* move the range from [-1,1] to [0,1] */
   glTranslated(0.5, 0.5, 0.5);
   glScaled(0.5, 0.5, 0.5);
   /* multiply the model view matrix when the depth texture was rendered */
   glMultMatrixd(modelview);
   /* multiply the inverse matrix of current model view matrix */
   glMultMatrixf(m_rotMatrixInv);

   /* revert to model view matrix mode */
   glMatrixMode(GL_MODELVIEW);

   /* enable texture mapping with texture coordinate generation */
   glEnable(GL_TEXTURE_2D);
   glEnable(GL_TEXTURE_GEN_S);
   glEnable(GL_TEXTURE_GEN_T);
   glEnable(GL_TEXTURE_GEN_R);
   glEnable(GL_TEXTURE_GEN_Q);

   /* bind the depth texture rendered at the first step */
   glBindTexture(GL_TEXTURE_2D, m_depthTextureID);

   /* depth texture set up was done, now switch current texture unit to default */
   glActiveTextureARB(GL_TEXTURE0_ARB);

   /* set depth func to allow overwrite for the same surface in the following rendering */
   glDepthFunc(GL_LEQUAL);

   if (shadowMappingLightFirst) {
      /* the area clipped by depth texture by alpha test is dark part */
      glAlphaFunc(GL_GEQUAL, 0.1f);

      /* light setting for non-toon objects */
      lightdim[0] = lightdim[1] = lightdim[2] = 0.55f - 0.2f * shadowMappingSelfDensity;
      glLightfv(GL_LIGHT0, GL_DIFFUSE, lightdim);
      glLightfv(GL_LIGHT0, GL_AMBIENT, lightdim);
      glLightfv(GL_LIGHT0, GL_SPECULAR, lightblk);

      /* render the non-toon objects (back, floor, non-toon models) */
      stage->renderBackground();
      stage->renderFloor();
      for (i = 0; i < num; i++) {
         if (objs[order[i]].isEnable() == true && objs[order[i]].getPMDModel()->getToonFlag() == false)
            objs[order[i]].getPMDModel()->renderModel();
      }

      /* for toon objects, they should apply the model-defined toon texture color at texture coordinates (0, 0) for shadow rendering */
      /* so restore the light setting */
      if (useCartoonRendering == true)
         updateLight(useMMDLikeCartoon, useCartoonRendering, lightIntensity, lightDirection, lightColor);
      /* render the toon objects */
      for (i = 0; i < num; i++) {
         if (objs[order[i]].isEnable() == true && objs[order[i]].getPMDModel()->getToonFlag() == true) {
            /* set texture coordinates for shadow mapping */
            objs[order[i]].getPMDModel()->updateShadowColorTexCoord(shadowMappingSelfDensity);
            /* tell model to render with the shadow corrdinates */
            objs[order[i]].getPMDModel()->setSelfShadowDrawing(true);
            /* render model and edge */
            objs[order[i]].getPMDModel()->renderModel();
            /* disable shadow rendering */
            objs[order[i]].getPMDModel()->setSelfShadowDrawing(false);
         }
      }
      if (useCartoonRendering == false)
         updateLight(useMMDLikeCartoon, useCartoonRendering, lightIntensity, lightDirection, lightColor);
   } else {
      /* the area clipped by depth texture by alpha test is light part */
      glAlphaFunc(GL_GEQUAL, 0.001f);
      stage->renderBackground();
      stage->renderFloor();
      for (i = 0; i < num; i++)
         if (objs[order[i]].isEnable() == true)
            objs[order[i]].getPMDModel()->renderModel();
   }

   /* reset settings */
   glDepthFunc(GL_LESS);
   glAlphaFunc(GL_GEQUAL, 0.05f);

   glActiveTextureARB(GL_TEXTURE3_ARB);
   glDisable(GL_TEXTURE_GEN_S);
   glDisable(GL_TEXTURE_GEN_T);
   glDisable(GL_TEXTURE_GEN_R);
   glDisable(GL_TEXTURE_GEN_Q);
   glDisable(GL_TEXTURE_2D);
   glActiveTextureARB(GL_TEXTURE0_ARB);
}

/* Render::renderScene: render scene */
void Render::renderScene(PMDObject *objs, short *order, int num, Stage *stage, float shadowMappingFloorDensity)
{
   short i;

   /* clear rendering buffer */
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

   glEnable(GL_CULL_FACE);
   glEnable(GL_BLEND);

   /* set model viwe matrix */
   glLoadIdentity();
   glMultMatrixf(m_rotMatrix);

   /* stage and shadhow */
   /* background */
   stage->renderBackground();
   /* enable stencil */
   glEnable(GL_STENCIL_TEST);
   glStencilFunc(GL_ALWAYS, 1, ~0);
   /* make stencil tag true */
   glStencilOp(GL_KEEP, GL_KEEP , GL_REPLACE);
   /* render floor */
   stage->renderFloor();
   /* render shadow stencil */
   glColorMask(0, 0, 0, 0) ;
   glDepthMask(0);
   glEnable(GL_STENCIL_TEST);
   glStencilFunc(GL_EQUAL, 1, ~0);
   /* increment 1 pixel stencil */
   glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
   /* render moodel */
   glDisable(GL_DEPTH_TEST);
   glPushMatrix();
   glMultMatrixf(stage->getShadowMatrix());
   for (i = 0; i < num; i++) {
      if (objs[order[i]].isEnable() == true) {
         objs[order[i]].getPMDModel()->renderForShadow();
      }
   }
   glPopMatrix();
   glEnable(GL_DEPTH_TEST);
   glColorMask(1, 1, 1, 1);
   glDepthMask(1);
   /* if stencil is 2, render shadow with blend on */
   glStencilFunc(GL_EQUAL, 2, ~0);
   glDisable(GL_LIGHTING);
   glColor4f(0.1f, 0.1f, 0.1f, shadowMappingFloorDensity);
   glDisable(GL_DEPTH_TEST);
   stage->renderFloor();
   glEnable(GL_DEPTH_TEST);
   glDisable(GL_STENCIL_TEST);
   glEnable(GL_LIGHTING);

   /* render model */
   for (i = 0; i < num; i++) {
      if (objs[order[i]].isEnable() == true) {
         objs[order[i]].getPMDModel()->renderModel();
         objs[order[i]].getPMDModel()->renderEdge();
      }
   }
}

/* Render::initialize: initialzie Render */
void Render::initialize()
{
   m_width = 0;
   m_height = 0;
   m_trans = btVector3(0.0f, 0.0f, 0.0f);
   m_angle = btVector3(0.0f, 0.0f, 0.0f);
   updateRotationFromAngle();
   m_distance = 100.0f;
   m_fovy = 16.0f;

   m_currentTrans = m_trans;
   m_currentRot = m_rot;
   m_currentDistance = m_distance;
   m_currentFovy = m_fovy;

   m_viewMoveTime = -1;
   m_viewControlledByMotion = false;

   m_transMatrix.setIdentity();
   updateModelViewMatrix();

   m_shadowMapInitialized = false;
   m_lightVec = btVector3(0.0f, 0.0f, 0.0f);
   m_shadowMapAutoViewEyePoint = btVector3(0.0f, 0.0f, 0.0f);
   m_shadowMapAutoViewRadius = 0.0f;

   m_depth = NULL;
}

/* Render::clear: free Render */
void Render::clear()
{
   if(m_depth)
      free(m_depth);
   initialize();
}

/* Render::Render: constructor */
Render::Render()
{
   initialize();
}

/* Render::~Render: destructor */
Render::~Render()
{
   clear();
}

/* Render::setup: initialize and setup Render */
bool Render::setup(const int *size, const float *color, const float *trans, const float *rot, float distance, float fovy, bool useShadowMapping, int shadowMappingTextureSize, bool shadowMappingLightFirst, int maxNumModel)
{
   if(size == NULL || color == NULL || rot == NULL || trans == NULL)
      return false;

   resetCameraView(trans, rot, distance, fovy);
   setViewMoveTimer(0);

   /* set clear color */
   glClearColor(color[0], color[1], color[2], 0.0f);
   glClearStencil(0);

   /* enable depth test */
   glEnable(GL_DEPTH_TEST);

   /* enable texture */
   glEnable(GL_TEXTURE_2D);

   /* enable face culling */
   glEnable(GL_CULL_FACE);
   /* not render the back surface */
   glCullFace(GL_BACK);

   /* enable alpha blending */
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   /* enable alpha test, to avoid zero-alpha surfaces to depend on the rendering order */
   glEnable(GL_ALPHA_TEST);
   glAlphaFunc(GL_GEQUAL, 0.05f);

   /* enable lighting */
   glEnable(GL_LIGHT0);
   glEnable(GL_LIGHTING);

   /* initialization for shadow mapping */
   setShadowMapping(useShadowMapping, shadowMappingTextureSize, shadowMappingLightFirst);

   setSize(size[0], size[1]);

   m_depth = (RenderDepthData *) malloc(sizeof(RenderDepthData) * maxNumModel);

   return true;
}

/* Render::setSize: set size */
void Render::setSize(int w, int h)
{
   if (m_width != w || m_height != h) {
      if (w > 0)
         m_width = w;
      if (h > 0)
         m_height = h;
      updateProjectionMatrix();
   }
}

/* Render::getWidth: get width */
int Render::getWidth()
{
   return m_width;
}

/* Render::getHeight: get height */
int Render::getHeight()
{
   return m_height;
}

/* Render::resetCameraView: reset camera view */
void Render::resetCameraView(const float *trans, const float *angle, float distance, float fovy)
{
   m_angle = btVector3(angle[0], angle[1], angle[2]);
   m_trans = btVector3(trans[0], trans[1], trans[2]);
   m_distance = distance;
   m_fovy = fovy;
   updateRotationFromAngle();
}

/* Render::setCameraParam: set camera view parameter from camera controller */
void Render::setCameraFromController(CameraController *c)
{
   if (c != NULL) {
      c->getCurrentViewParam(&m_distance, &m_trans, &m_angle, &m_fovy);
      updateRotationFromAngle();
      m_viewControlledByMotion = true;
   } else
      m_viewControlledByMotion = false;
}

/* Render::setViewMoveTimer: set timer for rotation, transition, and scale of view */
void Render::setViewMoveTimer(int ms)
{
   m_viewMoveTime = ms;
   if (m_viewMoveTime > 0) {
      m_viewMoveStartRot = m_currentRot;
      m_viewMoveStartTrans = m_currentTrans;
      m_viewMoveStartDistance = m_currentDistance;
      m_viewMoveStartFovy = m_currentFovy;
   }
}

/* Render::isViewMoving: return if view is moving by timer */
bool Render::isViewMoving()
{
   if (m_viewMoveTime > 0 && (m_currentRot != m_rot || m_currentTrans != m_trans || m_currentDistance != m_distance || m_currentFovy != m_fovy))
      return true;
   return false;
}

/* Render::translate: translate */
void Render::translate(float x, float y, float z)
{
   m_trans += btVector3(x, y, z);
}

/* Render::rotate: rotate scene */
void Render::rotate(float x, float y, float z)
{
   m_angle.setX(m_angle.x() + x);
   m_angle.setY(m_angle.y() + y);
   m_angle.setZ(m_angle.z() + z);
   updateRotationFromAngle();
}

/* Render::setDistance: set distance */
void Render::setDistance(float distance)
{
   m_distance = distance;
}

/* Render::getDistance: get distance */
float Render::getDistance()
{
   return m_distance;
}

/* Render::setFovy: set fovy */
void Render::setFovy(float fovy)
{
   m_fovy = fovy;
}

/* Render::getFovy: get fovy */
float Render::getFovy()
{
   return m_fovy;
}

/* Render::setShadowMapping: switch shadow mapping */
void Render::setShadowMapping(bool useShadowMapping, int textureSize, bool shadowMappingLightFirst)
{
   if(useShadowMapping) {
      /* enabled */
      if (!m_shadowMapInitialized) {
         /* initialize now */
         initializeShadowMap(textureSize);
         m_shadowMapInitialized = true;
      }
      /* set how to set the comparison result value of R coordinates and texture (depth) value */
      glActiveTextureARB(GL_TEXTURE3_ARB);
      glBindTexture(GL_TEXTURE_2D, m_depthTextureID);
      if (shadowMappingLightFirst) {
         /* when rendering order is light(full) - dark(shadow part), OpenGL should set the shadow part as true */
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_GEQUAL);
      } else {
         /* when rendering order is dark(full) - light(non-shadow part), OpenGL should set the shadow part as false */
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
      }
      glDisable(GL_TEXTURE_2D);
      glActiveTextureARB(GL_TEXTURE0_ARB);
   } else {
      /* disabled */
      if (m_shadowMapInitialized) {
         /* disable depth texture unit */
         glActiveTextureARB(GL_TEXTURE3_ARB);
         glDisable(GL_TEXTURE_2D);
         glActiveTextureARB(GL_TEXTURE0_ARB);
      }
   }
}

/* Render::getRenderOrder: return rendering order */
void Render::getRenderOrder(short *order, PMDObject *objs, int num)
{
   int i, s;
   btVector3 pos, v;

   if (num == 0)
      return;

   s = 0;
   for (i = 0; i < num; i++) {
      if (objs[i].isEnable() == false || objs[i].allowMotionFileDrop() == false) continue;
      pos = objs[i].getPMDModel()->getCenterBone()->getTransform()->getOrigin();
      pos = m_transMatrix * pos;
      m_depth[s].dist = pos.z();
      m_depth[s].id = i;
      s++;
   }
   qsort(m_depth, s, sizeof(RenderDepthData), compareDepth);
   for (i = 0; i < s; i++)
      order[i] = m_depth[i].id;
   for (i = 0; i < num; i++)
      if (objs[i].isEnable() == false || objs[i].allowMotionFileDrop() == false)
         order[s++] = i;
}

/* Render::render: render all */
void Render::render(PMDObject *objs, short *order, int num, Stage *stage, bool useMMDLikeCartoon, bool useCartoonRendering, float lightIntensity, float *lightDirection, float *lightColor, bool useShadowMapping, int shadowMappingTextureSize, bool shadowMappingLightFirst, float shadowMappingSelfDensity, float shadowMappingFloorDensity, double ellapsedTimeForMove)
{
   bool updated;

   /* update camera view matrices */
   updated = updateDistance(ellapsedTimeForMove);
   updated |= updateTransRotMatrix(ellapsedTimeForMove);
   if (updated == true)
      updateModelViewMatrix();
   if (updateFovy(ellapsedTimeForMove) == true)
      updateProjectionMatrix();

   if (isViewMoving() == false)
      m_viewMoveTime = -1;

   if (useShadowMapping)
      renderSceneShadowMap(objs, order, num, stage, useMMDLikeCartoon, useCartoonRendering, lightIntensity, lightDirection, lightColor, shadowMappingTextureSize, shadowMappingLightFirst, shadowMappingSelfDensity);
   else
      renderScene(objs, order, num, stage, shadowMappingFloorDensity);
}

/* Render::pickModel: pick up a model at the screen position */
int Render::pickModel(PMDObject *objs, int num, int x, int y, int *allowDropPicked)
{
   int i;

   GLuint selectionBuffer[512];
   GLint viewport[4];

   GLint hits;
   GLuint *data;
   GLuint minDepth = 0, minDepthAllowDrop = 0;
   int minID, minIDAllowDrop;
   GLuint depth;
   int id;

   /* get current viewport */
   glGetIntegerv(GL_VIEWPORT, viewport);
   /* set selection buffer */
   glSelectBuffer(512, selectionBuffer);
   /* begin selection mode */
   glRenderMode(GL_SELECT);
   /* save projection matrix */
   glMatrixMode(GL_PROJECTION);
   glPushMatrix();
   /* set projection matrix for picking */
   glLoadIdentity();
   /* apply picking matrix */
   gluPickMatrix(x, viewport[3] - y, 15.0, 15.0, viewport);
   /* apply normal projection matrix */
   applyProjectionMatrix();
   /* switch to model view mode */
   glMatrixMode(GL_MODELVIEW);
   /* initialize name buffer */
   glInitNames();
   glPushName(0);
   /* draw models with selection names */
   for (i = 0; i < num; i++) {
      if (objs[i].isEnable() == true) {
         glLoadName(i);
         objs[i].getPMDModel()->renderForPick();
      }
   }

   /* restore projection matrix */
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   /* switch to model view mode */
   glMatrixMode(GL_MODELVIEW);
   /* end selection mode and get number of hits */
   hits = glRenderMode(GL_RENDER);
   if (hits == 0) return -1;
   data = &(selectionBuffer[0]);
   minID = -1;
   minIDAllowDrop = -1;
   for (i = 0; i < hits; i++) {
      depth = *(data + 1);
      id = *(data + 3);
      if (minID == -1 || minDepth > depth) {
         minDepth = depth;
         minID = id;
      }
      if (allowDropPicked && objs[id].allowMotionFileDrop()) {
         if (minIDAllowDrop == -1 || minDepthAllowDrop > depth) {
            minDepthAllowDrop = depth;
            minIDAllowDrop = id;
         }
      }
      data += *data + 3;
   }
   if (allowDropPicked)
      *allowDropPicked = minIDAllowDrop;

   return minID;
}

/* Render::updateLight: update light */
void Render::updateLight(bool useMMDLikeCartoon, bool useCartoonRendering, float lightIntensity, const float *lightDirection, const float *lightColor)
{
   float fLightDif[4];
   float fLightSpc[4];
   float fLightAmb[4];
   int i;
   float d, a, s;

   if (useMMDLikeCartoon == false) {
      /* MMDAgent original cartoon */
      d = 0.2f;
      a = lightIntensity * 2.0f;
      s = 0.4f;
   } else if (useCartoonRendering) {
      /* like MikuMikuDance */
      d = 0.0f;
      a = lightIntensity * 2.0f;
      s = lightIntensity;
   } else {
      /* no toon */
      d = lightIntensity;
      a = 1.0f;
      s = 1.0f; /* OpenGL default */
   }

   for (i = 0; i < 3; i++)
      fLightDif[i] = lightColor[i] * d;
   fLightDif[3] = 1.0f;
   for (i = 0; i < 3; i++)
      fLightAmb[i] = lightColor[i] * a;
   fLightAmb[3] = 1.0f;
   for (i = 0; i < 3; i++)
      fLightSpc[i] = lightColor[i] * s;
   fLightSpc[3] = 1.0f;

   glLightfv(GL_LIGHT0, GL_POSITION, lightDirection);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, fLightDif);
   glLightfv(GL_LIGHT0, GL_AMBIENT, fLightAmb);
   glLightfv(GL_LIGHT0, GL_SPECULAR, fLightSpc);

   /* update light direction vector */
   m_lightVec = btVector3(lightDirection[0], lightDirection[1], lightDirection[2]);
   m_lightVec.normalize();
}

/* Render::updateDepthTextureViewParam: update center and radius information to get required range for shadow mapping */
void Render::updateDepthTextureViewParam(PMDObject *objList, int num)
{
   int i;
   float d, dmax;
   float *r = new float[num];
   btVector3 *c = new btVector3[num];
   btVector3 cc = btVector3(0.0f, 0.0f, 0.0f);

   for (i = 0; i < num; i++) {
      if (objList[i].isEnable() == false)
         continue;
      r[i] = objList[i].getPMDModel()->calculateBoundingSphereRange(&(c[i]));
      cc += c[i];
   }
   cc /= (float) num;

   dmax = 0.0f;
   for (i = 0; i < num; i++) {
      if (objList[i].isEnable() == false)
         continue;
      d = cc.distance(c[i]) + r[i];
      if (dmax < d)
         dmax = d;
   }

   m_shadowMapAutoViewEyePoint = cc;
   m_shadowMapAutoViewRadius = dmax;

   delete [] r;
   delete [] c;
}

/* Render::getScreenPointPosition: convert screen position to object position */
void Render::getScreenPointPosition(btVector3 *dst, btVector3 *src)
{
   *dst = m_transMatrixInv * (*src);
}

/* Render::getCurrentViewCenterPos: get current view center position */
void Render::getCurrentViewCenterPos(btVector3 *pos)
{
   *pos = m_currentTrans;
}

/* Render::getCurrentViewRotation: get current view translation matrix */
void Render::getCurrentViewTransform(btTransform *tr)
{
   *tr = m_transMatrix;
}

/* Render::getInfoString: store current view parameters to buffer */
void Render::getInfoString(char *buf)
{
   sprintf(buf, "%.2f, %.2f, %.2f | %.2f, %.2f, %.2f | %.2f | %.2f", m_currentTrans.x(), m_currentTrans.y(), m_currentTrans.z(), m_angle.x(), m_angle.y(), m_angle.z(), m_currentDistance, m_currentFovy);
}

