//
// Created by MaxllMa on 2019/11/8.
//

#include "LoadAssetFile.h"

#include <cstdlib>

#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "JNILoad.h"

#include <android/log.h>
#define LOG_TAG "LoadAssetFile"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

jobject LoadAssetFile::g_assManager = nullptr;

void* LoadAssetFile::loadFile(const char *fileName, int *fileSize) {
    *fileSize = 0;
    JNIEnv* env = JNILoad::getJNIEnv();
    if(!env){
        return NULL;
    }
    if(g_assManager == nullptr){
        jclass actClass = nullptr;
        jobject am = nullptr;
        actClass = env->FindClass("com/boyaa/glesdemo/MainActivity");
        if(env->ExceptionCheck()){
            env->ExceptionClear();
            LOGE("cannot find class");
            return NULL;
        }
        if(!actClass){
            LOGE("cannot find class2");
            return NULL;
        }
        jmethodID getAM = env->GetStaticMethodID(actClass,"getAM","()Landroid/content/res/AssetManager;");
        if(env->ExceptionCheck()){
            env->ExceptionClear();
            LOGE("cannot find method getAM");
            env->DeleteLocalRef(actClass);
            return NULL;
        }
        if(!getAM){
            LOGE("cannot find method getAM2");
            env->DeleteLocalRef(actClass);
            return NULL;
        }
        am = env->CallStaticObjectMethod(actClass, getAM);
        if(env->ExceptionCheck()){
            env->ExceptionClear();
            LOGE("cannot get am");
            env->DeleteLocalRef(actClass);
            return NULL;
        }
        if(!am){
            LOGE("cannot get am2");
            env->DeleteLocalRef(actClass);
            return NULL;
        }

        g_assManager = env->NewGlobalRef(am);
        env->DeleteLocalRef(am);
        env->DeleteLocalRef(actClass);
    }
    AAssetManager* assetManager = AAssetManager_fromJava(env,g_assManager);
    if(!assetManager){
        LOGE("cannot get AssetManager AAssetManager_fromJava");
        return NULL;
    }
    AAsset* asset = AAssetManager_open(assetManager,fileName,AASSET_MODE_UNKNOWN);
    if(!asset){
        LOGE("AAssetManager_open file error");
        return NULL;
    }
    off64_t size = AAsset_getLength64(asset);
    char* buff = (char*)std::malloc(size);
    int readNum = AAsset_read(asset,buff,size);
    if(readNum != size){
        LOGE("load file error %d %ld",readNum,size);
    }
    *fileSize = readNum;
    AAsset_close(asset);
    return buff;
}