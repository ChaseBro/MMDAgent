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

#ifdef _WIN32
#include <windows.h>
#endif /* _WIN32 */

#ifdef __APPLE__
#include <Carbon/Carbon.h>
#include <AudioToolbox/ExtendedAudioFile.h>
#include <audiounit/AudioUnit.h>
#endif /* __APPLE__ */

#include "MMDAgent.h"
#include "Audio_Thread.h"

#ifdef _WIN32
typedef struct _Audio {
   char buf[MMDAGENT_MAXBUFLEN];
   char ret[MMDAGENT_MAXBUFLEN];
} Audio;

static void Audio_initialize(Audio *audio)
{
}

static void Audio_clear(Audio *audio)
{
}

static bool Audio_openAndStart(Audio *audio, const char *alias, char *file)
{
   bool first = true;

   /* wait */
   sprintf(audio->buf, "open \"%s\" alias _%s wait", file, alias);
   if (mciSendStringA(audio->buf, NULL, 0, 0) != 0) {
      return false;
   }

   /* enqueue */
   sprintf(audio->buf, "cue _%s output wait", alias);
   if (mciSendStringA(audio->buf, NULL, 0, 0) != 0) {
      sprintf(audio->buf, "close _%s wait", alias);
      mciSendStringA(audio->buf, NULL, 0, 0);
      return false;
   }

   /* start */
   sprintf(audio->buf, "play _%s", alias);
   if (mciSendStringA(audio->buf, NULL, 0, 0) != 0) {
      sprintf(audio->buf, "close _%s wait", alias);
      mciSendStringA(audio->buf, NULL, 0, 0);
      return false;
   }

   /* wait till sound starts */
   do {
      if(first == true)
         first = false;
      else
         MMDAgent_sleep(AUDIOTHREAD_STARTSLEEPMS);
      /* check sound start */
      sprintf(audio->buf, "status _%s mode wait", alias);
      mciSendStringA(audio->buf, audio->ret, sizeof(audio->ret), NULL);
   } while(MMDAgent_strequal(audio->ret, "playing") == false);

   return true;
}

static void Audio_waitToStop(Audio *audio, const char *alias, bool *m_playing)
{
   do {
      /* check user's stop */
      if(*m_playing == false) {
         sprintf(audio->buf, "stop _%s wait", alias);
         mciSendStringA(audio->buf, NULL, 0, NULL);
         break;
      }
      MMDAgent_sleep(AUDIOTHREAD_ENDSLEEPMS);
      /* check end of sound */
      sprintf(audio->buf, "status _%s mode wait", alias);
      mciSendStringA(audio->buf, audio->ret, sizeof(audio->ret), NULL);
   } while(MMDAgent_strequal(audio->ret, "playing") == true);
}

static void Audio_close(Audio *audio, const char *alias)
{
   sprintf(audio->buf, "close _%s wait", alias);
   mciSendStringA(audio->buf, NULL, 0, NULL);
}
#endif /* _WIN32 */

#ifdef __APPLE__
typedef struct _Audio {
   bool end;
   ExtAudioFileRef file;
   AudioUnit device;
   AudioBufferList buff;
} Audio;

static OSStatus renderCallback(void *inRefCon, AudioUnitRenderActionFlags *ioActionFlags, const AudioTimeStamp *inTimeStamp, UInt32 inBusNumber, UInt32 inNumberFrames, AudioBufferList *ioData)
{
   UInt32 i;
   Audio *audio = (Audio*) inRefCon;
   OSStatus err;
   float *outL, *outR, *wave;

   /* initialize buffer */
   if(audio->buff.mBuffers[0].mDataByteSize < inNumberFrames * 2 * sizeof(float)) {
      if(audio->buff.mBuffers[0].mData != NULL)
         free(audio->buff.mBuffers[0].mData);
      audio->buff.mNumberBuffers = 1;
      audio->buff.mBuffers[0].mNumberChannels = 2;
      audio->buff.mBuffers[0].mData = malloc(inNumberFrames * 2 * sizeof(float) );
      audio->buff.mBuffers[0].mDataByteSize = inNumberFrames * 2 * sizeof(float);
   }

   /* read */
   err = ExtAudioFileRead(audio->file, &inNumberFrames, &audio->buff);
   if (err) {
      audio->end = true;
      return err;
   }

   /* end of file */
   if ( inNumberFrames == 0 ) {
      audio->end = true;
      return noErr;
   }

   /* copy */
   outL = (float*) ioData->mBuffers[0].mData;
   outR = (float*) ioData->mBuffers[1].mData;
   wave = (float*) audio->buff.mBuffers[0].mData;
   for (i = 0; i < inNumberFrames; i++) {
      *outL++ = *wave++;
      *outR++ = *wave++;
   }

   return noErr;
}

static void Audio_initialize(Audio *audio)
{
   memset(audio, 0, sizeof(Audio));
}

static void Audio_clear(Audio *audio)
{
   if(audio->buff.mBuffers[0].mData != NULL) {
      free(audio->buff.mBuffers[0].mData);
   }
   memset(audio, 0, sizeof(Audio));
}

static bool Audio_openAndStart(Audio *audio, const char *alias, char *file)
{
   UInt32 size;

   OSStatus err1;
   OSErr err2;
   ComponentResult err3;

   char *buff;
   FSRef path;
   AudioStreamBasicDescription format;
   ComponentDescription findComp;
   Component comp;
   AURenderCallbackStruct input;

   buff = MMDAgent_pathdup(file);

   /* convert file name */
   err1 = FSPathMakeRef ((const UInt8 *) buff, &path, NULL);
   free(buff);
   if(err1) {
      return false;
   }

   /* open audio file */
   err1 = ExtAudioFileOpen(&path, &audio->file);
   if(err1) {
      return false;
   }

   /* get audio file format */
   size = sizeof(AudioStreamBasicDescription);
   err1 = ExtAudioFileGetProperty(audio->file, kExtAudioFileProperty_FileDataFormat, &size, &format);
   if(err1) {
      ExtAudioFileDispose(audio->file);
      return false;
   }

   /* convert audio format to pcm (32bit stereo with the same sampling rate) */
   format.mSampleRate = format.mSampleRate;
   format.mFormatID = kAudioFormatLinearPCM;
   format.mFormatFlags = kAudioFormatFlagsNativeFloatPacked;
   format.mBytesPerPacket = 4 * 2;
   format.mFramesPerPacket = 1;
   format.mBytesPerFrame = 4 * 2;
   format.mChannelsPerFrame = 2;
   format.mBitsPerChannel = 32;
   format.mReserved = format.mReserved;
   size = sizeof(AudioStreamBasicDescription);
   err1 = ExtAudioFileSetProperty(audio->file, kExtAudioFileProperty_ClientDataFormat, size, &format);
   if(err1) {
      ExtAudioFileDispose(audio->file);
      return false;
   }

   /* open audio device */
   findComp.componentType = kAudioUnitType_Output;
   findComp.componentSubType = kAudioUnitSubType_DefaultOutput;
   findComp.componentManufacturer = kAudioUnitManufacturer_Apple;
   findComp.componentFlags = 0;
   findComp.componentFlagsMask = 0;
   comp = FindNextComponent(NULL, &findComp);
   if(comp == 0) {
      ExtAudioFileDispose(audio->file);
      return false;
   }
   err2 = OpenAComponent(comp, &audio->device);
   if(err2) {
      ExtAudioFileDispose(audio->file);
      return false;
   }

   /* set callback func. */
   input.inputProc = renderCallback;
   input.inputProcRefCon = audio;
   size = sizeof(AURenderCallbackStruct);
   err3 = AudioUnitSetProperty(audio->device, kAudioUnitProperty_SetRenderCallback, kAudioUnitScope_Input, 0, &input, size);
   if(err3) {
      CloseComponent(audio->device);
      ExtAudioFileDispose(audio->file);
      return false;
   }

   /* prepare audio device */
   err3 = AudioUnitInitialize(audio->device);
   if(err3) {
      CloseComponent(audio->device);
      ExtAudioFileDispose(audio->file);
      return false;
   }

   /* start */
   err3 = AudioOutputUnitStart(audio->device);
   if(err3) {
      AudioUnitUninitialize(audio->device);
      CloseComponent(audio->device);
      ExtAudioFileDispose(audio->file);
      return false;
   }

   return true;
}

static void Audio_waitToStop(Audio *audio, const char *alias, bool *m_playing)
{
   /* wait */
   while(*m_playing == true && audio->end == false) {
      MMDAgent_sleep(AUDIOTHREAD_ENDSLEEPMS);
   }

   /* stop */
   AudioOutputUnitStop(audio->device);
}

static void Audio_close(Audio *audio, const char *alias)
{
   /* uninitialize audio device */
   AudioUnitUninitialize(audio->device);

   /* close audio device */
   CloseComponent(audio->device);

   /* close audio file */
   ExtAudioFileDispose(audio->file);
}
#endif /* __APPLE__ */

/* mainThread: main thread */
static void mainThread(void *param)
{
   Audio_Thread *audio_thread = (Audio_Thread *) param;
   audio_thread->run();
}

/* Audio_Thread::initialize: initialize thread */
void Audio_Thread::initialize()
{
   m_mmdagent = NULL;

   m_mutex = NULL;
   m_cond = NULL;
   m_thread = -1;

   m_count = 0;

   m_playing = false;
   m_kill = false;

   m_alias = NULL;
   m_file = NULL;
}

/* Audio_Thread::clear: free thread */
void Audio_Thread::clear()
{
   stop();
   m_kill = true;

   /* wait */
   if(m_cond != NULL)
      glfwSignalCond(m_cond);

   if(m_mutex != NULL || m_cond != NULL || m_thread >= 0) {
      if(m_thread >= 0) {
         glfwWaitThread(m_thread, GLFW_WAIT);
         glfwDestroyThread(m_thread);
      }
      if(m_cond != NULL)
         glfwDestroyCond(m_cond);
      if(m_mutex != NULL)
         glfwDestroyMutex(m_mutex);
      glfwTerminate();
   }

   if(m_alias) free(m_alias);
   if(m_file) free(m_file);

   initialize();
}

/* Audio_Thread::Audio_Thread: thread constructor */
Audio_Thread::Audio_Thread()
{
   initialize();
}

/* Audio_Thread::~Audio_Thread: thread destructor */
Audio_Thread::~Audio_Thread()
{
   clear();
}

/* Audio_Thread::setupAndStart: setup and start thread */
void Audio_Thread::setupAndStart(MMDAgent *mmdagent)
{
   m_mmdagent = mmdagent;

   glfwInit();
   m_mutex = glfwCreateMutex();
   m_cond = glfwCreateCond();
   m_thread = glfwCreateThread(mainThread, this);
   if(m_mutex == NULL || m_cond == NULL || m_thread < 0) {
      clear();
      return;
   }
}

/* Audio_Thread::stopAndRelease: stop thread and free Open JTalk */
void Audio_Thread::stopAndRelease()
{
   clear();
}

/* Audio_Thread::run: main thread loop for TTS */
void Audio_Thread::run()
{
   Audio audio;
   char *alias, *file;

   while (m_kill == false) {
      /* wait event */
      glfwLockMutex(m_mutex);
      while(m_count <= 0) {
         glfwWaitCond(m_cond, m_mutex, GLFW_INFINITY);
         if(m_kill == true)
            return;
      }
      alias = MMDAgent_strdup(m_alias);
      file = MMDAgent_strdup(m_file);
      m_count--;
      glfwUnlockMutex(m_mutex);

      m_playing = true;

      /* open and start audio */
      Audio_initialize(&audio);
      if(Audio_openAndStart(&audio, alias, file) == true) {

         /* send SOUND_EVENT_START */
         m_mmdagent->sendEventMessage(AUDIOTHREAD_EVENTSTART, alias);

         /* wait to stop audio */
         Audio_waitToStop(&audio, alias, &m_playing);

         /* send SOUND_EVENT_STOP */
         m_mmdagent->sendEventMessage(AUDIOTHREAD_EVENTSTOP, alias);

         /* close audio file */
         Audio_close(&audio, alias);
      }

      if(alias) free(alias);
      if(file) free(file);
      Audio_clear(&audio);
      m_playing = false;
   }
}

/* Audio_Thread::isRunning: check running */
bool Audio_Thread::isRunning()
{
   if(m_kill == true || m_mutex == NULL || m_cond == NULL || m_thread < 0)
      return false;
   else
      return true;
}

/* Audio_Thread::isPlaying: check playing */
bool Audio_Thread::isPlaying()
{
   return m_playing;
}

/* checkAlias: check playing alias */
bool Audio_Thread::checkAlias(const char *alias)
{
   bool ret;

   /* check */
   if(isRunning() == false)
      return false;

   /* wait buffer mutex */
   glfwLockMutex(m_mutex);

   /* save character name, speaking style, and text */
   ret = MMDAgent_strequal(m_alias, alias);

   /* release buffer mutex */
   glfwUnlockMutex(m_mutex);

   return ret;
}

/* Audio_Thread::play: start playing */
void Audio_Thread::play(const char *alias, const char *file)
{
   /* check */
   if(isRunning() == false)
      return;
   if(MMDAgent_strlen(alias) <= 0 || MMDAgent_strlen(file) <= 0)
      return;

   /* wait buffer mutex */
   glfwLockMutex(m_mutex);

   /* save character name, speaking style, and text */
   if(m_alias) free(m_alias);
   if(m_file) free(m_file);
   m_alias = MMDAgent_strdup(alias);
   m_file = MMDAgent_strdup(file);
   m_count++;

   /* start playing thread */
   if(m_count <= 1)
      glfwSignalCond(m_cond);

   /* release buffer mutex */
   glfwUnlockMutex(m_mutex);
}

/* Audio_Thread::stop: stop playing */
void Audio_Thread::stop()
{
   if(isRunning() == true)
      m_playing = false;
}
