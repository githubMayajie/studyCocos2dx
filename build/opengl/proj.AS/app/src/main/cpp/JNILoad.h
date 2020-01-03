//
// Created by MaxllMa on 2019/11/8.
//

#ifndef GLESDEMO_JNILOAD_H
#define GLESDEMO_JNILOAD_H

#include <jni.h>
class JNILoad {
public:
    static JavaVM* getJavaVM();
    static JNIEnv* getJNIEnv();
    static JavaVM* g_jvm;
    static JNIEnv* g_jenv;
};


#endif //GLESDEMO_JNILOAD_H
