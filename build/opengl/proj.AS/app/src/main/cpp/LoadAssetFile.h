//
// Created by MaxllMa on 2019/11/8.
//

#ifndef GLESDEMO_LOADASSETFILE_H
#define GLESDEMO_LOADASSETFILE_H

#include <jni.h>

class LoadAssetFile {
public:
    static void* loadFile(const char* fileName,int* fileSize);

private:
    static jobject g_assManager;
};


#endif //GLESDEMO_LOADASSETFILE_H
