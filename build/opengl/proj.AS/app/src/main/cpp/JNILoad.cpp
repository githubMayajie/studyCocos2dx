//
// Created by MaxllMa on 2019/11/8.
//

#include "JNILoad.h"
JavaVM* JNILoad::g_jvm = nullptr;
JNIEnv* JNILoad::g_jenv = nullptr;

JNIEXPORT jint JNI_OnLoad(JavaVM* vm,void* reserved){
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }

    JNILoad::g_jvm = vm;
    JNILoad::g_jenv = env;
    return JNI_VERSION_1_6;
}

JavaVM* JNILoad::getJavaVM() {
    return g_jvm;
}

JNIEnv* JNILoad::getJNIEnv() {
    return g_jenv;
}

