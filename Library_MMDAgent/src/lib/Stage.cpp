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

/* Stage::makeFloorBody: create a rigid body for floor */
void Stage::makeFloorBody(float width, float depth)
{
   btVector3 localInertia(0, 0, 0);
   btScalar mass = 0.0f;
   btCollisionShape *colShape;
   btTransform startTransform;
   btDefaultMotionState* myMotionState;

   releaseFloorBody();

   colShape = new btBoxShape(btVector3(width, 10.0f, depth)); /* <- can free memory ? */
   if (mass != 0.0f)
      colShape->calculateLocalInertia(mass, localInertia);
   startTransform.setIdentity();
   startTransform.setOrigin(btVector3(0.0f, -9.99f, 0.0f));
   myMotionState = new btDefaultMotionState(startTransform);
   btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
   rbInfo.m_linearDamping = 0.5f;
   rbInfo.m_angularDamping = 0.5f;
   rbInfo.m_restitution = 0.8f;
   rbInfo.m_friction = 0.5f;
   m_floorBody = new btRigidBody(rbInfo);

   if (m_bullet)
      m_bullet->getWorld()->addRigidBody(m_floorBody);
}

/* Stage::releaseFloorBody: release rigid body for floor */
void Stage::releaseFloorBody()
{
   if (m_floorBody) {
      if(m_floorBody->getCollisionShape())
         delete m_floorBody->getCollisionShape();
      if (m_floorBody->getMotionState())
         delete m_floorBody->getMotionState();
      if (m_bullet)
         m_bullet->getWorld()->removeCollisionObject(m_floorBody);
      delete m_floorBody;
      m_floorBody = NULL;
   }
}

/* Stage::initialize: initialize stage */
void Stage::initialize()
{
   int i, j;

   m_hasPMD = false;
   m_listIndexPMD = 0;
   m_listIndexPMDValid = false;
   m_bullet = NULL;
   m_floorBody = NULL;
   for (i = 0; i < 4 ; i++)
      for (j = 0; j < 4; j++)
         m_floorShadow[i][j] = 0.0f;
}

/* Stage::clear: free stage */
void Stage::clear()
{
   releaseFloorBody();
   initialize();
}

/* Stage::Stage: constructor */
Stage::Stage()
{
   initialize();
}

/* Stage::~Stage: destructor */
Stage::~Stage()
{
   clear();
}

/* Stage::setSize: set size of floor and background */
void Stage::setSize(const float *size, float numx, float numy)
{
   m_floor.setSize(-size[0], 0.0f, size[1],
                   size[0], 0.0f, size[1],
                   size[0], 0.0f, -size[1],
                   -size[0], 0.0f, -size[1],
                   numx, numy);
   m_background.setSize(-size[0], 0.0f, -size[1],
                        size[0], 0.0f, -size[1],
                        size[0], size[2], -size[1],
                        -size[0], size[2], -size[1],
                        numx, numy);
   makeFloorBody(size[0], size[1]);
}

/* Stage::loadFloor: load floor image */
bool Stage::loadFloor(const char *file, BulletPhysics *bullet)
{
   if (m_bullet == NULL)
      m_bullet = bullet;

   if(m_floor.load(file) == false)
      return false;

   if (m_hasPMD) {
      m_pmd.release();
      m_hasPMD = false;
   }

   return true;
}

/* Stage::loadBackground: load background image */
bool Stage::loadBackground(const char *file, BulletPhysics *bullet)
{
   if (m_bullet == NULL)
      m_bullet = bullet;

   if(m_background.load(file) == false)
      return false;

   if (m_hasPMD) {
      m_pmd.release();
      m_hasPMD = false;
   }

   return true;
}

/* Stage::loadStagePMD: load stage pmd */
bool Stage::loadStagePMD(const char *file, BulletPhysics *bullet, SystemTexture *systex)
{
   if (m_bullet == NULL)
      m_bullet = bullet;

   if(m_pmd.load(file, m_bullet, systex) == false)
      return false;

   m_pmd.setToonFlag(false);
   m_pmd.updateSkin();
   m_hasPMD = true;
   if (m_listIndexPMDValid) {
      glDeleteLists(m_listIndexPMD, 1);
      m_listIndexPMDValid = false;
   }

   return true;
}

/* Stage::renderFloor: render the floor */
void Stage::renderFloor()
{
   const float normal[3] = {0.0f, 1.0f, 0.0f};

   if (m_hasPMD)
      renderPMD();
   else
      m_floor.render(false, normal);
}

/* Stage::renderBackground: render the background */
void Stage::renderBackground()
{
   const float normal[3] = {0.0f, 0.0f, 1.0f};

   if (!m_hasPMD)
      m_background.render(true, normal);
}

/* Stage::renderPMD: render the stage pmd */
void Stage::renderPMD()
{
   if (m_listIndexPMDValid) {
      glCallList(m_listIndexPMD);
      return;
   }

   m_listIndexPMD = glGenLists(1);
   glNewList(m_listIndexPMD, GL_COMPILE);
   glPushMatrix();
   m_pmd.renderModel();
   glPopMatrix();
   glEndList();
   m_listIndexPMDValid = true;
}

/* Stage::updateShadowMatrix: update shadow projection matrix */
void Stage::updateShadowMatrix(const float *lightDirection)
{
   GLfloat dot;
   GLfloat floorPlane[4];
   GLfloat vec0x, vec0y, vec0z, vec1x, vec1y, vec1z;

   /* need 2 vectors to find cross product */
   vec0x = m_floor.getSize(2, 0) - m_floor.getSize(1, 0);
   vec0y = m_floor.getSize(2, 1) - m_floor.getSize(1, 1);
   vec0z = m_floor.getSize(2, 2) - m_floor.getSize(1, 2);

   vec1x = m_floor.getSize(3, 0) - m_floor.getSize(1, 0);
   vec1y = m_floor.getSize(3, 1) - m_floor.getSize(1, 1);
   vec1z = m_floor.getSize(3, 2) - m_floor.getSize(1, 2);

   /* find cross product to get A, B, and C of plane equation */
   floorPlane[0] =   vec0y * vec1z - vec0z * vec1y;
   floorPlane[1] = -(vec0x * vec1z - vec0z * vec1x);
   floorPlane[2] =   vec0x * vec1y - vec0y * vec1x;
   floorPlane[3] = -(floorPlane[0] * m_floor.getSize(1, 0) + floorPlane[1] * m_floor.getSize(1, 1) + floorPlane[2] * m_floor.getSize(1, 2));

   /* find dot product between light position vector and ground plane normal */
   dot = floorPlane[0] * lightDirection[0] +
         floorPlane[1] * lightDirection[1] +
         floorPlane[2] * lightDirection[2] +
         floorPlane[3] * lightDirection[3];

   m_floorShadow[0][0] = dot - lightDirection[0] * floorPlane[0];
   m_floorShadow[1][0] = 0.f - lightDirection[0] * floorPlane[1];
   m_floorShadow[2][0] = 0.f - lightDirection[0] * floorPlane[2];
   m_floorShadow[3][0] = 0.f - lightDirection[0] * floorPlane[3];

   m_floorShadow[0][1] = 0.f - lightDirection[1] * floorPlane[0];
   m_floorShadow[1][1] = dot - lightDirection[1] * floorPlane[1];
   m_floorShadow[2][1] = 0.f - lightDirection[1] * floorPlane[2];
   m_floorShadow[3][1] = 0.f - lightDirection[1] * floorPlane[3];

   m_floorShadow[0][2] = 0.f - lightDirection[2] * floorPlane[0];
   m_floorShadow[1][2] = 0.f - lightDirection[2] * floorPlane[1];
   m_floorShadow[2][2] = dot - lightDirection[2] * floorPlane[2];
   m_floorShadow[3][2] = 0.f - lightDirection[2] * floorPlane[3];

   m_floorShadow[0][3] = 0.f - lightDirection[3] * floorPlane[0];
   m_floorShadow[1][3] = 0.f - lightDirection[3] * floorPlane[1];
   m_floorShadow[2][3] = 0.f - lightDirection[3] * floorPlane[2];
   m_floorShadow[3][3] = dot - lightDirection[3] * floorPlane[3];
}

/* Stage::getShadowMatrix: get shadow projection matrix */
GLfloat *Stage::getShadowMatrix()
{
   return (GLfloat *) m_floorShadow;
}

