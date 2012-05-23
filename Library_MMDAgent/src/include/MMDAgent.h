/* ----------------------------------------------------------------- */
/*           Toolkit for Building Voice Interaction Systems          */
/*           MMDAgent developed by MMDAgent Project Team             */
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

#ifndef __mmdagent_h__
#define __mmdagent_h__

/* definitions */

#define MMDAGENT_MAXBUFLEN    MMDFILES_MAXBUFLEN
#define MMDAGENT_DIRSEPARATOR MMDFILES_DIRSEPARATOR
#define MMDAGENT_MAXNCOMMAND  10

#define MMDAGENT_SYSDATADIR "AppData"
#define MMDAGENT_PLUGINDIR  "Plugins"

#define MMDAGENT_COMMAND_MODELADD       "MODEL_ADD"
#define MMDAGENT_COMMAND_MODELDELETE    "MODEL_DELETE"
#define MMDAGENT_COMMAND_MODELCHANGE    "MODEL_CHANGE"
#define MMDAGENT_COMMAND_MOTIONADD      "MOTION_ADD"
#define MMDAGENT_COMMAND_MOTIONDELETE   "MOTION_DELETE"
#define MMDAGENT_COMMAND_MOTIONCHANGE   "MOTION_CHANGE"
#define MMDAGENT_COMMAND_MOVESTART      "MOVE_START"
#define MMDAGENT_COMMAND_MOVESTOP       "MOVE_STOP"
#define MMDAGENT_COMMAND_TURNSTART      "TURN_START"
#define MMDAGENT_COMMAND_TURNSTOP       "TURN_STOP"
#define MMDAGENT_COMMAND_ROTATESTART    "ROTATE_START"
#define MMDAGENT_COMMAND_ROTATESTOP     "ROTATE_STOP"
#define MMDAGENT_COMMAND_STAGE          "STAGE"
#define MMDAGENT_COMMAND_LIGHTCOLOR     "LIGHTCOLOR"
#define MMDAGENT_COMMAND_LIGHTDIRECTION "LIGHTDIRECTION"
#define MMDAGENT_COMMAND_LIPSYNCSTART   "LIPSYNC_START"
#define MMDAGENT_COMMAND_LIPSYNCSTOP    "LIPSYNC_STOP"
#define MMDAGENT_COMMAND_CAMERA         "CAMERA"
#define MMDAGENT_COMMAND_PLUGINENABLE   "PLUGIN_ENABLE"
#define MMDAGENT_COMMAND_PLUGINDISABLE  "PLUGIN_DISABLE"

#define MMDAGENT_EVENT_MODELADD      "MODEL_EVENT_ADD"
#define MMDAGENT_EVENT_MODELDELETE   "MODEL_EVENT_DELETE"
#define MMDAGENT_EVENT_MODELCHANGE   "MODEL_EVENT_CHANGE"
#define MMDAGENT_EVENT_MOTIONADD     "MOTION_EVENT_ADD"
#define MMDAGENT_EVENT_MOTIONDELETE  "MOTION_EVENT_DELETE"
#define MMDAGENT_EVENT_MOTIONCHANGE  "MOTION_EVENT_CHANGE"
#define MMDAGENT_EVENT_MOVESTART     "MOVE_EVENT_START"
#define MMDAGENT_EVENT_MOVESTOP      "MOVE_EVENT_STOP"
#define MMDAGENT_EVENT_TURNSTART     "TURN_EVENT_START"
#define MMDAGENT_EVENT_TURNSTOP      "TURN_EVENT_STOP"
#define MMDAGENT_EVENT_ROTATESTART   "ROTATE_EVENT_START"
#define MMDAGENT_EVENT_ROTATESTOP    "ROTATE_EVENT_STOP"
#define MMDAGENT_EVENT_LIPSYNCSTART  "LIPSYNC_EVENT_START"
#define MMDAGENT_EVENT_LIPSYNCSTOP   "LIPSYNC_EVENT_STOP"
#define MMDAGENT_EVENT_PLUGINENABLE  "PLUGIN_EVENT_ENABLE"
#define MMDAGENT_EVENT_PLUGINDISABLE "PLUGIN_EVENT_DISABLE"
#define MMDAGENT_EVENT_DRAGANDDROP   "DRAGANDDROP"
#define MMDAGENT_EVENT_KEY           "KEY"

/* headers */

#include "MMDFiles.h"

#include "GL/glfw.h"

class MMDAgent;

#include "MMDAgent_utils.h"

#include "TextRenderer.h"
#include "LogText.h"
#include "LipSync.h"
#include "PMDObject.h"

#include "Option.h"
#include "ScreenWindow.h"
#include "Message.h"
#include "TileTexture.h"
#include "Stage.h"
#include "Render.h"
#include "Timer.h"
#include "Plugin.h"
#include "MotionStocker.h"

/* MMDAgent: MMDAgent class */
class MMDAgent
{
private:
   bool m_enable;

   char *m_configFileName; /* config file name */
   char *m_configDirName;  /* directory name of config file */
   char *m_appDirName;     /* directory name of application data */

   Option *m_option;        /* user options */
   ScreenWindow *m_screen;  /* screen window */
   Message *m_message;      /* message queue */
   BulletPhysics *m_bullet; /* Bullet Physics */
   Plugin *m_plugin;        /* plugins */
   Stage *m_stage;          /* stage */
   SystemTexture *m_systex; /* system texture */
   LipSync *m_lipSync;      /* system default lipsync */
   Render *m_render;        /* render */
   Timer *m_timer;          /* timer */
   TextRenderer *m_text;    /* text render */
   LogText *m_logger;       /* logger */

   PMDObject *m_model;      /* models */
   short *m_renderOrder;    /* model rendering order */
   int m_numModel;          /* number of models */
   MotionStocker *m_motion; /* motions */

   CameraController m_camera; /* camera controller */
   bool m_cameraControlled;   /* true when camera is controlled by motion */

   bool m_keyCtrl;           /* true if Ctrl-key is on */
   bool m_keyShift;          /* true if Shift-key is on */
   int m_selectedModel;      /* model ID selected by mouse */
   int m_highLightingModel;
   bool m_doubleClicked;     /* true if double clicked */
   int m_mousePosX;
   int m_mousePosY;
   bool m_leftButtonPressed;
   double m_restFrame;

   bool m_enablePhysicsSimulation; /* true if physics simulation is on */
   bool m_dispLog;                 /* true if log window is shown */
   bool m_dispBulletBodyFlag;      /* true if bullet body is shown */
   bool m_dispModelDebug;          /* true if model debugger is on */
   bool m_holdMotion;              /* true if holding motion */

   /* getNewModelId: return new model ID */
   int getNewModelId();

   /* removeRelatedModels: delete a model */
   void removeRelatedModels(int modelId);

   /* updateLight: update light */
   void updateLight();

   /* setHighLight: set high-light of selected model */
   void setHighLight(int modelId);

   /* updateScene: update the whole scene */
   bool updateScene();

   /* renderScene: render the whole scene */
   void renderScene();

   /* addModel: add model */
   bool addModel(const char *modelAlias, const char *fileName, btVector3 *pos, btQuaternion *rot, const char *baseModelAlias, const char *baseBoneName);

   /* changeModel: change model */
   bool changeModel(const char *modelAlias, const char *fileName);

   /* deleteModel: delete model */
   bool deleteModel(const char *modelAlias);

   /* addMotion: add motion */
   bool addMotion(const char *modelAlias, const char *motionAlias, const char *fileName, bool full, bool once, bool enableSmooth, bool enableRePos, float priority);

   /* changeMotion: change motion */
   bool changeMotion(const char *modelAlias, const char *motionAlias, const char *fileName);

   /* deleteMotion: delete motion */
   bool deleteMotion(const char *modelAlias, const char *motionAlias);

   /* startMove: start moving */
   bool startMove(const char *modelAlias, btVector3 *pos, bool local, float speed);

   /* stopMove: stop moving */
   bool stopMove(const char *modelAlias);

   /* startTurn: start turn */
   bool startTurn(const char *modelAlias, btVector3 *pos, bool local, float speed);

   /* stopTurn: stop turn */
   bool stopTurn(const char *modelAlias);

   /* startRotation: start rotation */
   bool startRotation(const char *modelAlias, btQuaternion *rot, bool local, float spped);

   /* stopRotation: stop rotation */
   bool stopRotation(const char *modelAlias);

   /* setFloor: set floor image */
   bool setFloor(const char *fileName);

   /* setBackground: set background image */
   bool setBackground(const char *fileName);

   /* setStage: set stage */
   bool setStage(const char *fileName);

   /* changeCamera: change camera setting */
   bool changeCamera(const char *pos, const char *rot, const char *distance, const char *fovy, const char *time);

   /* changeLightColor: change light color */
   bool changeLightColor(float r, float g, float b);

   /* changeLightDirection: change light direction */
   bool changeLightDirection(float x, float y, float z);

   /* startLipSync: start lip sync */
   bool startLipSync(const char *modelAlias, const char *seq);

   /* stopLipSync: stop lip sync */
   bool stopLipSync(const char *modelAlias);

   /* initialize: initialize MMDAgent */
   void initialize();

   /* clear: free MMDAgent */
   void clear();

public:

   /* MMDAgent: constructor */
   MMDAgent();

   /* ~MMDAgent: destructor */
   ~MMDAgent();

   /* setup: initialize and setup MMDAgent */
   bool setup(int argc, char **argv, const char *title);

   /* updateAndRender: update and render the whole scene */
   void updateAndRender();

   /* drawString: draw string */
   void drawString(const char *str);

   /* resetAdjustmentTimer: reset adjustment timer */
   void resetAdjustmentTimer();

   /* sendCommandMessage: send command message */
   void sendCommandMessage(const char *type, const char *format, ...);

   /* sendEventMessage: send event message */
   void sendEventMessage(const char *type, const char *format, ...);

   /* showLogMessage: show log message */
   void showLogMessage(const char *format, ...);

   /* findModelAlias: find a model with the specified alias */
   int findModelAlias(const char *alias);

   /* getMoelList: get model list */
   PMDObject *getModelList();

   /* getNumModel: get number of models */
   short getNumModel();

   /* getMousePosition:: get mouse position */
   void getMousePosition(int *x, int *y);

   /* getScreenPointPosition: convert screen position to object position */
   void getScreenPointPosition(btVector3 *dst, btVector3 *src);

   /* MMDAgent::getWindowSize: get window size */
   void getWindowSize(int *w, int *h);

   /* getConfigFileName: get config file name for plugin */
   char *getConfigFileName();

   /* getConfigDirName: get directory of config file for plugin */
   char *getConfigDirName();

   /* getAppDirName: get application directory name for plugin */
   char *getAppDirName();

   /* procWindowDestroyMessage: process window destroy message */
   void procWindowDestroyMessage();

   /* procMouseLeftButtonDoubleClickMessage: process mouse left button double click message */
   void procMouseLeftButtonDoubleClickMessage(int x, int y);

   /* procMouseLeftButtonDownMessage: process mouse left button down message */
   void procMouseLeftButtonDownMessage(int x, int y, bool withCtrl, bool withShift);

   /* procMouseLeftButtonUpMessage: process mouse left button up message */
   void procMouseLeftButtonUpMessage();

   /* procMouseWheel: process mouse wheel message */
   void procMouseWheelMessage(bool zoomup, bool withCtrl, bool withShift);

   /* procMousePosMessage: process mouse position message */
   void procMousePosMessage(int x, int y, bool withCtrl, bool withShift);

   /* procMouseRightButtonDownMessage: process mouse right button down message */
   void procMouseRightButtonDownMessage();

   /* procFullScreenMessage: process full screen message */
   void procFullScreenMessage();

   /* procInfoStringMessage: process information string message */
   void procInfoStringMessage();

   /* procVSyncMessage: process vsync message */
   void procVSyncMessage();

   /* procShadowMappingMessage: process shadow mapping message */
   void procShadowMappingMessage();

   /* procShadowMappingOrderMessage: process shadow mapping order message */
   void procShadowMappingOrderMessage();

   /* procDisplayRigidBodyMessage: process display rigid body message */
   void procDisplayRigidBodyMessage();

   /* procDisplayWireMessage: process display wire message */
   void procDisplayWireMessage();

   /* procDisplayBoneMessage: process display bone message */
   void procDisplayBoneMessage();

   /* procCartoonEdgeMessage: process cartoon edge message */
   void procCartoonEdgeMessage(bool plus);

   /* procTimeAdjustMessage: process time adjust message */
   void procTimeAdjustMessage(bool plus);

   /* procHorizontalRotateMessage: process horizontal rotate message */
   void procHorizontalRotateMessage(bool right);

   /* procVerticalRotateMessage: process vertical rotate message */
   void procVerticalRotateMessage(bool up);

   /* procHorizontalMoveMessage: process horizontal move message */
   void procHorizontalMoveMessage(bool right);

   /* procVerticalMoveMessage: process vertical move message */
   void procVerticalMoveMessage(bool up);

   /* procDeleteModelMessage: process delete model message */
   void procDeleteModelMessage();

   /* procPhysicsMessage: process physics message */
   void procPhysicsMessage();

   /* procDisplayLogMessage: process display log message */
   void procDisplayLogMessage();

   /* procHoldMessage: process hold message */
   void procHoldMessage();

   /* procWindowSizeMessage: process window size message */
   void procWindowSizeMessage(int x, int y);

   /* procKeyMessage: process key message */
   void procKeyMessage(char c);

   /* procCommandMessage: process command message */
   void procCommandMessage(const char *type, const char *value);

   /* procEventMessage: process event message */
   void procEventMessage(const char *type, const char *value);

   /* procLogMessage: process log message */
   void procLogMessage(const char *log);

   /* procDropFileMessage: process file drops message */
   void procDropFileMessage(const char *file, int x, int y);
};

#endif /* __mmdagent_h__ */
