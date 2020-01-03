//
// Created by MaxllMa on 2019/11/8.
//

#ifndef GLESDEMO_MATRIX_H
#define GLESDEMO_MATRIX_H

#include <cmath>

inline float degreesToRadians(float degrees)
{
    return M_PI * degrees / 180.0f;
}

inline float radiansToDegrees(float radians)
{
    return radians * 180.0f / M_PI;
}

class Vec3f{
public:
    float x,y,z;
    static float dot(Vec3f& vector1,Vec3f& vector2);

    void normalize();

    static Vec3f cross(const Vec3f& vector1, const Vec3f& vector2);
};

class Vec4f{
public:
    float x,y,z,w;

    void normalize();
};


class Matrix {
private:
    float elements[16];
    static Matrix multiply(Matrix *left, Matrix *right);
    static const float biasArray[];
    static const float identityArray[];
public:
    Matrix(void);
    float& operator[] (unsigned element);
    Matrix operator* (Matrix right);
    Matrix& operator=(const Matrix &another);
    Matrix(const float* array);
    static Matrix biasMatrix;
    static Matrix identityMatrix;
    static Matrix createRotationX(float angleInDegrees);
    static Matrix createRotationY(float angleInDegrees);
    static Matrix createRotationZ(float angleInDegrees);
    static Matrix createScaling(float x, float y, float z);
    static Matrix createTranslation(float x, float y, float z);
    float* getAsArray(void);
    static float matrixDeterminant(float *matrix);
    static float matrixDeterminant(Matrix *matrix);
    static Matrix matrixInvert(Matrix *matrix);
    static Matrix matrixLookAt(Vec3f eye, Vec3f center, Vec3f up);
    static Matrix matrixPerspective(float FOV, float ratio, float zNear, float zFar);
    static Matrix matrixScale(Matrix *matrix, float scale);
    static void matrixTranspose(Matrix *matrix);
    static Vec4f vertexTransform(Vec4f *vector, Matrix *matrix);
};


#endif //GLESDEMO_MATRIX_H
