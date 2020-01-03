//
// Created by MaxllMa on 2019/11/8.
//

#include "Quaternions.h"
#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <android/log.h>
#define LOG_TAG "Quaternions"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

Quaternion Quaternions::construct_quaternion(float x, float y, float z, float degs) {
    Quaternion init = { 0.0f, 0.0f, 0.0f, 0.0f };

    float ang = (float) M_PI * degs / 180.0f;
    float res = std::sin(ang / 2.0f);

    init.w = std::cos(ang / 2.0f);
    init.x = x * res;
    init.y = y * res;
    init.z = z * res;

    return init;
}

void Quaternions::construct_modelview_matrix(Quaternion quaternion, float *mat) {
    if(mat == nullptr){
        LOGE("Pointer to a modelview matrix points to NULL.\n");
        return;
    }
    mat[ 0] = 1.0f - 2.0f * (quaternion.y * quaternion.y + quaternion.z * quaternion.z);
    mat[ 1] = 2.0f        * (quaternion.x * quaternion.y + quaternion.z * quaternion.w);
    mat[ 2] = 2.0f        * (quaternion.x * quaternion.z - quaternion.y * quaternion.w);
    mat[ 3] = 0.0f;
    mat[ 4] = 2.0f        * (quaternion.x * quaternion.y - quaternion.z * quaternion.w);
    mat[ 5] = 1.0f - 2.0f * (quaternion.x * quaternion.x + quaternion.z * quaternion.z);
    mat[ 6] = 2.0f        * (quaternion.z * quaternion.y + quaternion.x * quaternion.w);
    mat[ 7] = 0.0f;
    mat[ 8] = 2.0f        * ( quaternion.x * quaternion.z + quaternion.y * quaternion.w);
    mat[ 9] = 2.0f        * ( quaternion.y * quaternion.z - quaternion.x * quaternion.w);
    mat[10] = 1.0f - 2.0f * ( quaternion.x * quaternion.x + quaternion.y * quaternion.y);
    mat[11] = 0.0f;
    mat[12] = 0.0f;
    mat[13] = 0.0f;
    mat[14] = 0.0f;
    mat[15] = 1.0f;
}

Quaternion Quaternions::multiply_quaternions(Quaternion a, Quaternion b) {
    Quaternion res;

    res.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    res.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    res.y = a.w * b.y + a.y * b.w + a.z * b.x - a.x * b.z;
    res.z = a.w * b.z + a.z * b.w + a.x * b.y - a.y * b.x;

    return res;
}