###########################################################################
##                                                                       ##
##                  Language Technologies Institute                      ##
##                     Carnegie Mellon University                        ##
##                         Copyright (c) 2010                            ##
##                        All Rights Reserved.                           ##
##                                                                       ##
##  Permission is hereby granted, free of charge, to use and distribute  ##
##  this software and its documentation without restriction, including   ##
##  without limitation the rights to use, copy, modify, merge, publish,  ##
##  distribute, sublicense, and/or sell copies of this work, and to      ##
##  permit persons to whom this work is furnished to do so, subject to   ##
##  the following conditions:                                            ##
##   1. The code must retain the above copyright notice, this list of    ##
##      conditions and the following disclaimer.                         ##
##   2. Any modifications must be clearly marked as such.                ##
##   3. Original authors' names are not deleted.                         ##
##   4. The authors' names are not used to endorse or promote products   ##
##      derived from this software without specific prior written        ##
##      permission.                                                      ##
##                                                                       ##
##  CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK         ##
##  DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING      ##
##  ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT   ##
##  SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE      ##
##  FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES    ##
##  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN   ##
##  AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,          ##
##  ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF       ##
##  THIS SOFTWARE.                                                       ##
##                                                                       ##
###########################################################################
##                                                                       ##
##  Author: Alok Parlikar (aup@cs.cmu.edu)                               ##
##                                                                       ##
##  Android support for Flite                                            ##
##                                                                       ##
##  Confirgure:                                                          ##
##    Install Android NDK from                                           ##
##       http://developer.android.com/sdk/ndk/index.html                 ##
##    Install Android SDK from                                           ##
##       http://developer.android.com/sdk/index.html                     ##
##    Set environment variable ANDROID_NDK to your local installation    ## 
##    export ANDROID_NDK=/opt/android-ndk-r3                             ##
##    export ANDROID_SDK=/opt/android-sdk                                ##
##    In Flite directory, run                                            ##
##    ./configure --target=arm-android                                   ##
##                                                                       ##
###########################################################################
TOP = ..
DIRNAME = android
BUILD_DIRS = 
ALL_DIRS = $(BUILD_DIRS) 
APP_CODE = app_code/LICENSE \
	   app_code/res/layout/download_voice_data.xml \
           app_code/res/layout/main.xml \
	   app_code/res/xml/engine_settings.xml \
           app_code/res/drawable/icon.png \
           app_code/res/values/strings.xml \
           app_code/default.properties \
           app_code/src/edu/cmu/cs/speech/tts/flite/EngineSettings.java \
           app_code/src/edu/cmu/cs/speech/tts/flite/FileDownloader.java \
           app_code/src/edu/cmu/cs/speech/tts/flite/DownloadVoiceData.java \
           app_code/src/edu/cmu/cs/speech/tts/flite/CheckVoiceData.java \
           app_code/src/edu/cmu/cs/speech/tts/flite/GetSampleText.java \
           app_code/src/edu/cmu/cs/speech/tts/flite/Flite.java \
           app_code/src/edu/cmu/cs/speech/tts/flite/Utility.java \
           app_code/build.xml \
           app_code/AndroidManifest.xml \
           app_code/jni/edu_cmu_cs_speech_tts_fliteVoices.hh \
           app_code/jni/tts/tts.h \
           app_code/jni/Android.mk \
           app_code/jni/edu_cmu_cs_speech_tts_String.cpp \
           app_code/jni/edu_cmu_cs_speech_tts_fliteVoices.cpp \
           app_code/jni/edu_cmu_cs_speech_tts_String.hh \
           app_code/jni/edu_cmu_cs_speech_tts_Common.hh \
           app_code/jni/Application.mk \
           app_code/jni/edu_cmu_cs_speech_tts_fliteEngine.cpp \
           app_code/README
H = 
SRCS = 
OBJS = $(SRCS:.c=.o)
IMGS = 
OTHER = 
FILES = Makefile README $(APP_CODE)

include $(TOP)/config/common_make_rules

all: app

app: $(ANDROID_NDK)/ndk-build $(ANDROID_SDK)/tools/adb $(FLITEDIR)/android/Makefile
	@ echo "Building Android app."
	@ $(ANDROID_NDK)/ndk-build --directory=$(TOP)/android/app_code
	@ cd $(TOP)/android/app_code; echo "sdk.dir=$(ANDROID_SDK)" > local.properties; ant debug
	@ echo ""
	@ echo "The built app is located at: $(TOP)/android/app_code/bin/FliteEngine-debug.apk"

$(ANDROID_SDK)/tools/adb:
	$(error Please set the ANDROID_SDK variable appropriately.)

$(ANDROID_NDK)/ndk-build:
	$(error Please set the ANDROID_NDK variable appropriately. You need release-4.)

$(FLITEDIR)/android/Makefile:
	$(error Please set the FLITEDIR variable to point to absolute path of the flite root directory)

local_clean:
	@ echo "Cleaning up app code directory"
	@ cd $(TOP)/android/app_code; rm -f local.properties; rm -rf bin/ gen/ libs/
