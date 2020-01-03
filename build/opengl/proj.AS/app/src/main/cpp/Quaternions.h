//
// Created by MaxllMa on 2019/11/8.
//

#ifndef GLESDEMO_QUATERNIONS_H
#define GLESDEMO_QUATERNIONS_H

typedef struct Quaternion
{
    /** Real part. */
    float w;
    /** X imaginary part. */
    float x;
    /** Y imaginary part. */
    float y;
    /** Z imaginary part. */
    float z;
} Quaternion;

class Quaternions {
public:
    static Quaternion construct_quaternion(float x, float y, float z, float degs);
    static void construct_modelview_matrix(Quaternion quaternion, float* mat);
    static Quaternion multiply_quaternions(Quaternion a, Quaternion b);

};


#endif //GLESDEMO_QUATERNIONS_H
