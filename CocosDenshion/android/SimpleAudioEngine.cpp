/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "SimpleAudioEngine.h"
#include "jni/SimpleAudioEngineJni.h"

#include "cocos2d.h"
#include <cstring>
#include <android/log.h>
#include <jni/JniHelper.h>
#include <jni.h>

#define  LOG_TAG     "Device Model"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

USING_NS_CC;
/**********************************************************************************
 *   jni
 **********************************************************************************/
#define  CLASS_NAME   "org/cocos2dx/lib/Cocos2dxHelper"
#define  METHOD_NAME  "getDeviceModel"

namespace CocosDenshion {

static std::string getFullPathWithoutAssetsPrefix(const char* pszFilename)
{
    // Changing file path to full path
#ifdef Hunter
    std::string fullPath = CCFileUtils::sharedFileUtils()->MD5fullPathForFilename(pszFilename);
#else
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(pszFilename);
#endif
    // Removing `assets` since it isn't needed for the API of playing sound.
    size_t pos = fullPath.find("assets/");
    if (pos == 0)
    {
        fullPath = fullPath.substr(strlen("assets/"));
    }
    return fullPath;
}

static SimpleAudioEngine *s_pEngine = 0;

SimpleAudioEngine::SimpleAudioEngine()
{
    JniMethodInfo methodInfo;
    jstring jstr;
    if (JniHelper::getStaticMethodInfo(methodInfo, CLASS_NAME, METHOD_NAME, "()Ljava/lang/String;"))
    {
        jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
    }
    methodInfo.env->DeleteLocalRef(methodInfo.classID);
    
    const char* deviceModel = methodInfo.env->GetStringUTFChars(jstr, NULL);
    
    LOGD("SimpleAudioEngine() - deviceModel = %s", deviceModel);
    
    methodInfo.env->ReleaseStringUTFChars(jstr, deviceModel);
    methodInfo.env->DeleteLocalRef(jstr);
}

SimpleAudioEngine::~SimpleAudioEngine()
{
}

SimpleAudioEngine* SimpleAudioEngine::sharedEngine()
{
    if (! s_pEngine)
    {
        s_pEngine = new SimpleAudioEngine();
    }
    
    return s_pEngine;
}

void SimpleAudioEngine::end()
{
    endJNI();
}

void SimpleAudioEngine::preloadBackgroundMusic(const char* pszFilePath)
{
    std::string fullPath = getFullPathWithoutAssetsPrefix(pszFilePath);
    preloadBackgroundMusicJNI(fullPath.c_str());
}

void SimpleAudioEngine::playBackgroundMusic(const char* pszFilePath, bool bLoop)
{
    std::string fullPath = getFullPathWithoutAssetsPrefix(pszFilePath);
    playBackgroundMusicJNI(fullPath.c_str(), bLoop);
}

void SimpleAudioEngine::stopBackgroundMusic(bool bReleaseData)
{
    stopBackgroundMusicJNI();
}

void SimpleAudioEngine::pauseBackgroundMusic()
{
    pauseBackgroundMusicJNI();
}

void SimpleAudioEngine::resumeBackgroundMusic()
{
    resumeBackgroundMusicJNI();
}

void SimpleAudioEngine::rewindBackgroundMusic()
{
    rewindBackgroundMusicJNI();
}

bool SimpleAudioEngine::willPlayBackgroundMusic()
{
    return true;
}

bool SimpleAudioEngine::isBackgroundMusicPlaying()
{
    return isBackgroundMusicPlayingJNI();
}

float SimpleAudioEngine::getBackgroundMusicVolume()
{
    return getBackgroundMusicVolumeJNI();
}

void SimpleAudioEngine::setBackgroundMusicVolume(float volume)
{
    setBackgroundMusicVolumeJNI(volume);
}

float SimpleAudioEngine::getEffectsVolume()
{
    return getEffectsVolumeJNI();
}

void SimpleAudioEngine::setEffectsVolume(float volume)
{
    setEffectsVolumeJNI(volume);
}

unsigned int SimpleAudioEngine::playEffect(const char* pszFilePath, bool bLoop)
{
    std::string fullPath = getFullPathWithoutAssetsPrefix(pszFilePath);
    return playEffectJNI(fullPath.c_str(), bLoop);
}

void SimpleAudioEngine::stopEffect(unsigned int nSoundId)
{
    stopEffectJNI(nSoundId);
}

void SimpleAudioEngine::preloadEffect(const char* pszFilePath)
{
    std::string fullPath = getFullPathWithoutAssetsPrefix(pszFilePath);
    preloadEffectJNI(fullPath.c_str());
}

void SimpleAudioEngine::unloadEffect(const char* pszFilePath)
{
    std::string fullPath = getFullPathWithoutAssetsPrefix(pszFilePath);
    unloadEffectJNI(fullPath.c_str());
}

void SimpleAudioEngine::pauseEffect(unsigned int nSoundId)
{
    pauseEffectJNI(nSoundId);
}

void SimpleAudioEngine::pauseAllEffects()
{
    pauseAllEffectsJNI();
}

void SimpleAudioEngine::resumeEffect(unsigned int nSoundId)
{
    resumeEffectJNI(nSoundId);
}

void SimpleAudioEngine::resumeAllEffects()
{
    resumeAllEffectsJNI();
}

void SimpleAudioEngine::stopAllEffects()
{
    stopAllEffectsJNI();
}

}