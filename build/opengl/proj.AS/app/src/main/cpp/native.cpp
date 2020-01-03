//
// Created by MaxllMa on 2019/11/8.
//
#include <jni.h>
#include <android/log.h>
#include <GLES3/gl3.h>

#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cstring>

#include "Matrix.h"
#include "Quaternions.h"

#include <android/log.h>
#define LOG_TAG "Native"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

extern "C"{
JNIEXPORT void JNICALL Java_com_boyaa_glesdemo_NativeLibrary_init(
        JNIEnv* env,
        jclass jclass1,
        jint width,
        jint height);

JNIEXPORT void JNICALL Java_com_boyaa_glesdemo_NativeLibrary_rumCmd(
        JNIEnv* env,
        jclass jclass1,
        jstring s1);

JNIEXPORT void JNICALL Java_com_boyaa_glesdemo_NativeLibrary_step(
        JNIEnv* env,
        jclass jclass1);

JNIEXPORT void JNICALL Java_com_boyaa_glesdemo_NativeLibrary_uninit(
        JNIEnv* env,
        jclass jclass1);
}


GLint loadShader(GLenum shaderType,const char* shaderSource){
    GLuint shader = glCreateShader(shaderType);
    if(shader){
        glShaderSource(shader,1,&shaderSource,NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader,GL_COMPILE_STATUS,&compiled);
        if(!compiled){
            GLint infoLen = 0;
            glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&infoLen);
            if(infoLen){
                char* buf = (char*)std::malloc(infoLen);
                if(buf){
                    glGetShaderInfoLog(shader,infoLen,NULL,buf);
                    LOGE("Could not Compile Shader %d:\n%s\n", shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLint createProgram(const char* vertexSource,const char* fragmentSource){
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER,vertexSource);
    if(!vertexShader){
        return 0;
    }
    GLuint fragmentShader = loadShader(GL_FRAGMENT_SHADER,fragmentSource);
    if(!fragmentShader){
        return 0;
    }
    GLuint program = glCreateProgram();
    if(program){
        glAttachShader(program,vertexShader);
        glAttachShader(program,fragmentShader);
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program,GL_LINK_STATUS,&linkStatus);
        if(linkStatus != GL_TRUE){
            GLint bufLength = 0;
            glGetProgramiv(program,GL_INFO_LOG_LENGTH,&bufLength);
            if(bufLength){
                char* buf = (char*)std::malloc(bufLength);
                if(buf){
                    glGetProgramInfoLog(program,bufLength,NULL,buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}


static int select = 0;
static int viewWidth,viewHeight;
void show1(){
    static bool init = false;
    static GLuint simpleTriangleProgram;
    static GLuint vPosition;
    static GLfloat triangleVertices[] = {
            0.0f, 1.0f,0.0f,
            -1.0f, -1.0f,0.0f,
            1.0f, -1.0f,0.0f
    };
    if(!init){
        const char glVertexShader[] =
                "attribute vec4 vPosition;\n"
                "void main()\n"
                "{\n"
                "  gl_Position = vPosition;\n"
                "}\n";

        const char glFragmentShader[] =
                "precision mediump float;\n"
                "void main()\n"
                "{\n"
                "  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
                "}\n";
        simpleTriangleProgram = createProgram(glVertexShader,glFragmentShader);
        if(!simpleTriangleProgram){
            LOGE ("Could not create program");
        }
        vPosition = glGetAttribLocation(simpleTriangleProgram,"vPosition");
    }
    init = true;
    glClearColor(0,0,0,0);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glUseProgram(simpleTriangleProgram);
    glVertexAttribPointer(vPosition,3,GL_FLOAT,GL_FALSE,0,triangleVertices);
    glEnableVertexAttribArray(vPosition);
    glDrawArrays(GL_TRIANGLES,0,3);
}


void show2(){
    static bool init = false;
    static GLuint simpleTriangleProgram;
    static GLuint vPosition;
    static GLfloat triangleVertices[] = {
            -0.5f, 1.0f,1.0,
            -1.0f, 0.0f,1.0,
            0.0f, 0.0f,1.0,

            0.5f, 1.0f,0.0,
            0.0f, 0.0f,0.0,
            1.0f, 0.0f,0.0,

            -0.5f, 0.0f,2.0,
            -1.0f, -1.0f,-1.0,
            0.0f, -1.0f,-1.0,

            0.5f, 0.0f,-2.0,
            0.0f, -1.0f,1.0,
            1.0f, -1.0f,1.0,
    };
    if(!init){
        const char glVertexShader[] =
                "attribute vec4 vPosition;\n"
                "void main()\n"
                "{\n"
                "  gl_Position = vPosition;\n"
                "}\n";

        const char glFragmentShader[] =
                "precision mediump float;\n"
                "void main()\n"
                "{\n"
                "  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0);\n"
                "}\n";
        simpleTriangleProgram = createProgram(glVertexShader,glFragmentShader);
        if(!simpleTriangleProgram){
            LOGE ("Could not create program");
        }
        vPosition = glGetAttribLocation(simpleTriangleProgram,"vPosition");
    }
    init = true;
    glClearColor(0,0,0,0);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    glUseProgram(simpleTriangleProgram);
    glVertexAttribPointer(vPosition,3,GL_FLOAT,GL_FALSE,0,triangleVertices);
    glEnableVertexAttribArray(vPosition);
    glDrawArrays(GL_TRIANGLES,0,3 * 4);
}


void showMVPX(){
    static bool init = false;
    static GLuint program;
    static GLuint vertexLocation;
    static GLuint vertexColourLocation;
    static GLuint projectionLocation;
    static GLuint modelViewLocation;
    static Matrix projectMatrix = Matrix::matrixPerspective(45.0f,(float)viewWidth / (float)viewHeight,0.1f,100);;
    Matrix modelViewMatrix = Matrix::identityMatrix;
    static float angle = 0;
    static GLfloat cubeVertices[] = {
            -1.0f,  1.0f, -1.0f, /* Back. */
             1.0f,  1.0f, -1.0f,
             -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             -1.0f,  1.0f,  1.0f, /* Front. */
             1.0f,  1.0f,  1.0f,
             -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
             -1.0f,  1.0f, -1.0f, /* Left. */
             -1.0f, -1.0f, -1.0f,
             -1.0f, -1.0f,  1.0f,
             -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f, /* Right. */
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             -1.0f, -1.0f, -1.0f, /* Top. */
             -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f, -1.0f, -1.0f,
             -1.0f,  1.0f, -1.0f, /* Bottom. */
             -1.0f,  1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f
    };
    static GLfloat colour[] = {
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f
    };
    GLushort indices[] = {0, 2, 3, 0, 1, 3,
                          4, 6, 7, 4, 5, 7,
                          8, 9, 10, 11, 8, 10,
                          12, 13, 14, 15, 12, 14,
                          16, 17, 18, 16, 19, 18,
                          20, 21, 22, 20, 23, 22};
    if(!init){
        static const char glVertexShader[] =
                "attribute vec4 vertexPosition;\n"
                "attribute vec3 vertexColour;\n"
                "varying vec3 fragColour;\n"
                "uniform mat4 projection;\n"
                "uniform mat4 modelView;\n"
                "void main()\n"
                "{\n"
                "    gl_Position = projection * modelView * vertexPosition;\n"
                "    fragColour = vertexColour;\n"
                "}\n";

        static const char glFragmentShader[] =
                "precision mediump float;\n"
                "varying vec3 fragColour;\n"
                "void main()\n"
                "{\n"
                "    gl_FragColor = vec4(fragColour, 1.0);\n"
                "}\n";
        program = createProgram(glVertexShader,glFragmentShader);
        if(!program){
            LOGE ("Could not create program");
        }
        vertexLocation = glGetAttribLocation(program,"vertexPosition");
        vertexColourLocation = glGetAttribLocation(program, "vertexColour");
        projectionLocation = glGetUniformLocation(program, "projection");
        modelViewLocation = glGetUniformLocation(program, "modelView");

        glEnable(GL_DEPTH_TEST);
    }
    init = true;
    glClearColor(0,0,0,0);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

    modelViewMatrix = Matrix::createRotationX(angle) * modelViewMatrix;
//    modelViewMatrix = Matrix::createRotationY(angle) * modelViewMatrix;
//    modelViewMatrix = Matrix::createRotationZ(angle) * modelViewMatrix;
    modelViewMatrix = Matrix::createTranslation(0.0f,0.0f,-5.0f - (angle / 100.0f)) * modelViewMatrix;

    glUseProgram(program);

    glVertexAttribPointer(vertexLocation,3,GL_FLOAT,GL_FALSE,0,cubeVertices);
    glEnableVertexAttribArray(vertexLocation);
    glVertexAttribPointer(vertexColourLocation,3,GL_FLOAT,GL_FALSE,0,colour);
    glEnableVertexAttribArray(vertexColourLocation);

    glUniformMatrix4fv(projectionLocation,1,GL_FALSE,projectMatrix.getAsArray());
    glUniformMatrix4fv(modelViewLocation,1,GL_FALSE,modelViewMatrix.getAsArray());

    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,indices);

    angle += 1;
    if (angle > 360)
    {
        angle -= 360;
    }
}

void showMVPY(){
    static bool init = false;
    static GLuint program;
    static GLuint vertexLocation;
    static GLuint vertexColourLocation;
    static GLuint projectionLocation;
    static GLuint modelViewLocation;
    static Matrix projectMatrix = Matrix::matrixPerspective(45.0f,(float)viewWidth / (float)viewHeight,0.1f,100);;
    Matrix modelViewMatrix = Matrix::identityMatrix;
    static float angle = 0;
    static GLfloat cubeVertices[] = {
            -1.0f,  1.0f, -1.0f, /* Back. */
            1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f, /* Front. */
            1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f, /* Left. */
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f, /* Right. */
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f, /* Top. */
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f, /* Bottom. */
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f
    };
    static GLfloat colour[] = {
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f
    };
    GLushort indices[] = {0, 2, 3, 0, 1, 3,
                          4, 6, 7, 4, 5, 7,
                          8, 9, 10, 11, 8, 10,
                          12, 13, 14, 15, 12, 14,
                          16, 17, 18, 16, 19, 18,
                          20, 21, 22, 20, 23, 22};
    if(!init){
        static const char glVertexShader[] =
                "attribute vec4 vertexPosition;\n"
                "attribute vec3 vertexColour;\n"
                "varying vec3 fragColour;\n"
                "uniform mat4 projection;\n"
                "uniform mat4 modelView;\n"
                "void main()\n"
                "{\n"
                "    gl_Position = projection * modelView * vertexPosition;\n"
                "    fragColour = vertexColour;\n"
                "}\n";

        static const char glFragmentShader[] =
                "precision mediump float;\n"
                "varying vec3 fragColour;\n"
                "void main()\n"
                "{\n"
                "    gl_FragColor = vec4(fragColour, 1.0);\n"
                "}\n";
        program = createProgram(glVertexShader,glFragmentShader);
        if(!program){
            LOGE ("Could not create program");
        }
        vertexLocation = glGetAttribLocation(program,"vertexPosition");
        vertexColourLocation = glGetAttribLocation(program, "vertexColour");
        projectionLocation = glGetUniformLocation(program, "projection");
        modelViewLocation = glGetUniformLocation(program, "modelView");

        glEnable(GL_DEPTH_TEST);
    }
    init = true;
    glClearColor(0,0,0,0);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

//    modelViewMatrix = Matrix::createRotationX(angle) * modelViewMatrix;
    modelViewMatrix = Matrix::createRotationY(angle) * modelViewMatrix;
//    modelViewMatrix = Matrix::createRotationZ(angle) * modelViewMatrix;
    modelViewMatrix = Matrix::createTranslation(0.0f,0.0f,-5.0f - (angle / 100.0f)) * modelViewMatrix;

    glUseProgram(program);

    glVertexAttribPointer(vertexLocation,3,GL_FLOAT,GL_FALSE,0,cubeVertices);
    glEnableVertexAttribArray(vertexLocation);
    glVertexAttribPointer(vertexColourLocation,3,GL_FLOAT,GL_FALSE,0,colour);
    glEnableVertexAttribArray(vertexColourLocation);

    glUniformMatrix4fv(projectionLocation,1,GL_FALSE,projectMatrix.getAsArray());
    glUniformMatrix4fv(modelViewLocation,1,GL_FALSE,modelViewMatrix.getAsArray());

    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,indices);

    angle += 1;
    if (angle > 360)
    {
        angle -= 360;
    }
}

void showMVPZ(){
    static bool init = false;
    static GLuint program;
    static GLuint vertexLocation;
    static GLuint vertexColourLocation;
    static GLuint projectionLocation;
    static GLuint modelViewLocation;
    static Matrix projectMatrix = Matrix::matrixPerspective(45.0f,(float)viewWidth / (float)viewHeight,0.1f,100);;
    Matrix modelViewMatrix = Matrix::identityMatrix;
    static float angle = 0;
    static GLfloat cubeVertices[] = {
            -1.0f,  1.0f, -1.0f, /* Back. */
            1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f, /* Front. */
            1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f, /* Left. */
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f, /* Right. */
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f, /* Top. */
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f, /* Bottom. */
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f
    };
    static GLfloat colour[] = {
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f
    };
    GLushort indices[] = {0, 2, 3, 0, 1, 3,
                          4, 6, 7, 4, 5, 7,
                          8, 9, 10, 11, 8, 10,
                          12, 13, 14, 15, 12, 14,
                          16, 17, 18, 16, 19, 18,
                          20, 21, 22, 20, 23, 22};
    if(!init){
        static const char glVertexShader[] =
                "attribute vec4 vertexPosition;\n"
                "attribute vec3 vertexColour;\n"
                "varying vec3 fragColour;\n"
                "uniform mat4 projection;\n"
                "uniform mat4 modelView;\n"
                "void main()\n"
                "{\n"
                "    gl_Position = projection * modelView * vertexPosition;\n"
                "    fragColour = vertexColour;\n"
                "}\n";

        static const char glFragmentShader[] =
                "precision mediump float;\n"
                "varying vec3 fragColour;\n"
                "void main()\n"
                "{\n"
                "    gl_FragColor = vec4(fragColour, 1.0);\n"
                "}\n";
        program = createProgram(glVertexShader,glFragmentShader);
        if(!program){
            LOGE ("Could not create program");
        }
        vertexLocation = glGetAttribLocation(program,"vertexPosition");
        vertexColourLocation = glGetAttribLocation(program, "vertexColour");
        projectionLocation = glGetUniformLocation(program, "projection");
        modelViewLocation = glGetUniformLocation(program, "modelView");

        glEnable(GL_DEPTH_TEST);
    }
    init = true;
    glClearColor(0,0,0,0);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

//    modelViewMatrix = Matrix::createRotationX(angle) * modelViewMatrix;
//    modelViewMatrix = Matrix::createRotationY(angle) * modelViewMatrix;
    modelViewMatrix = Matrix::createRotationZ(angle) * modelViewMatrix;
    modelViewMatrix = Matrix::createTranslation(0.0f,0.0f,-5.0f - (angle / 100.0f)) * modelViewMatrix;

    glUseProgram(program);

    glVertexAttribPointer(vertexLocation,3,GL_FLOAT,GL_FALSE,0,cubeVertices);
    glEnableVertexAttribArray(vertexLocation);
    glVertexAttribPointer(vertexColourLocation,3,GL_FLOAT,GL_FALSE,0,colour);
    glEnableVertexAttribArray(vertexColourLocation);

    glUniformMatrix4fv(projectionLocation,1,GL_FALSE,projectMatrix.getAsArray());
    glUniformMatrix4fv(modelViewLocation,1,GL_FALSE,modelViewMatrix.getAsArray());

    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,indices);

    angle += 1;
    if (angle > 360)
    {
        angle -= 360;
    }
}


void showMVPXYZ(){
    static bool init = false;
    static GLuint program;
    static GLuint vertexLocation;
    static GLuint vertexColourLocation;
    static GLuint projectionLocation;
    static GLuint modelViewLocation;
    static Matrix projectMatrix = Matrix::matrixPerspective(45.0f,(float)viewWidth / (float)viewHeight,0.1f,100);;
    Matrix modelViewMatrix = Matrix::identityMatrix;
    static float angle = 0;
    static GLfloat cubeVertices[] = {
            -1.0f,  1.0f, -1.0f, /* Back. */
            1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f, /* Front. */
            1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f, /* Left. */
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f, /* Right. */
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f, /* Top. */
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f, /* Bottom. */
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f
    };
    static GLfloat colour[] = {
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f
    };
    GLushort indices[] = {0, 2, 3, 0, 1, 3,
                          4, 6, 7, 4, 5, 7,
                          8, 9, 10, 11, 8, 10,
                          12, 13, 14, 15, 12, 14,
                          16, 17, 18, 16, 19, 18,
                          20, 21, 22, 20, 23, 22};
    if(!init){
        static const char glVertexShader[] =
                "attribute vec4 vertexPosition;\n"
                "attribute vec3 vertexColour;\n"
                "varying vec3 fragColour;\n"
                "uniform mat4 projection;\n"
                "uniform mat4 modelView;\n"
                "void main()\n"
                "{\n"
                "    gl_Position = projection * modelView * vertexPosition;\n"
                "    fragColour = vertexColour;\n"
                "}\n";

        static const char glFragmentShader[] =
                "precision mediump float;\n"
                "varying vec3 fragColour;\n"
                "void main()\n"
                "{\n"
                "    gl_FragColor = vec4(fragColour, 1.0);\n"
                "}\n";
        program = createProgram(glVertexShader,glFragmentShader);
        if(!program){
            LOGE ("Could not create program");
        }
        vertexLocation = glGetAttribLocation(program,"vertexPosition");
        vertexColourLocation = glGetAttribLocation(program, "vertexColour");
        projectionLocation = glGetUniformLocation(program, "projection");
        modelViewLocation = glGetUniformLocation(program, "modelView");

        glEnable(GL_DEPTH_TEST);
    }
    init = true;
    glClearColor(0,0,0,0);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

    modelViewMatrix = Matrix::createRotationX(angle) * modelViewMatrix;
    modelViewMatrix = Matrix::createRotationY(angle) * modelViewMatrix;
    modelViewMatrix = Matrix::createRotationZ(angle) * modelViewMatrix;
    modelViewMatrix = Matrix::createTranslation(0.0f,0.0f,-5.0f - (angle / 100.0f)) * modelViewMatrix;

    glUseProgram(program);

    glVertexAttribPointer(vertexLocation,3,GL_FLOAT,GL_FALSE,0,cubeVertices);
    glEnableVertexAttribArray(vertexLocation);
    glVertexAttribPointer(vertexColourLocation,3,GL_FLOAT,GL_FALSE,0,colour);
    glEnableVertexAttribArray(vertexColourLocation);

    glUniformMatrix4fv(projectionLocation,1,GL_FALSE,projectMatrix.getAsArray());
    glUniformMatrix4fv(modelViewLocation,1,GL_FALSE,modelViewMatrix.getAsArray());

    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,indices);

    angle += 1;
    if (angle > 360)
    {
        angle -= 360;
    }
}

void showMVPXYZQuaternions(){
    static bool init = false;
    static GLuint program;
    static GLuint vertexLocation;
    static GLuint vertexColourLocation;
    static GLuint projectionLocation;
    static GLuint modelViewLocation;
    static Matrix projectMatrix = Matrix::matrixPerspective(45.0f,(float)viewWidth / (float)viewHeight,0.1f,100);;
    Matrix modelViewMatrix = Matrix::identityMatrix;
    static float angle = 0;
    static GLfloat cubeVertices[] = {
            -1.0f,  1.0f, -1.0f, /* Back. */
            1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f, /* Front. */
            1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f, /* Left. */
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f, /* Right. */
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f, /* Top. */
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f, /* Bottom. */
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f
    };
    static GLfloat colour[] = {
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f
    };
    GLushort indices[] = {0, 2, 3, 0, 1, 3,
                          4, 6, 7, 4, 5, 7,
                          8, 9, 10, 11, 8, 10,
                          12, 13, 14, 15, 12, 14,
                          16, 17, 18, 16, 19, 18,
                          20, 21, 22, 20, 23, 22};
    if(!init){
        static const char glVertexShader[] =
                "attribute vec4 vertexPosition;\n"
                "attribute vec3 vertexColour;\n"
                "varying vec3 fragColour;\n"
                "uniform mat4 projection;\n"
                "uniform mat4 modelView;\n"
                "void main()\n"
                "{\n"
                "    gl_Position = projection * modelView * vertexPosition;\n"
                "    fragColour = vertexColour;\n"
                "}\n";

        static const char glFragmentShader[] =
                "precision mediump float;\n"
                "varying vec3 fragColour;\n"
                "void main()\n"
                "{\n"
                "    gl_FragColor = vec4(fragColour, 1.0);\n"
                "}\n";
        program = createProgram(glVertexShader,glFragmentShader);
        if(!program){
            LOGE ("Could not create program");
        }
        vertexLocation = glGetAttribLocation(program,"vertexPosition");
        vertexColourLocation = glGetAttribLocation(program, "vertexColour");
        projectionLocation = glGetUniformLocation(program, "projection");
        modelViewLocation = glGetUniformLocation(program, "modelView");

        glEnable(GL_DEPTH_TEST);
    }
    init = true;
    glClearColor(0,0,0,0);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

    Quaternion qx = Quaternions::construct_quaternion(1,0,0,angle);
    Quaternion qy = Quaternions::construct_quaternion(0,1,0,angle);
    Quaternion qz = Quaternions::construct_quaternion(0,0,1,angle);
    Quaternion qzyx = Quaternions::multiply_quaternions(Quaternions::multiply_quaternions(qx,qy),qz);
    Matrix qzyxMatrix;
    Quaternions::construct_modelview_matrix(qzyx,qzyxMatrix.getAsArray());
//    Quaternions::construct_modelview_matrix(qx,qzyxMatrix.getAsArray());
    modelViewMatrix = qzyxMatrix * modelViewMatrix;
    modelViewMatrix = Matrix::createTranslation(0.0f,0.0f,-5.0f - (angle / 100.0f)) * modelViewMatrix;

    glUseProgram(program);

    glVertexAttribPointer(vertexLocation,3,GL_FLOAT,GL_FALSE,0,cubeVertices);
    glEnableVertexAttribArray(vertexLocation);
    glVertexAttribPointer(vertexColourLocation,3,GL_FLOAT,GL_FALSE,0,colour);
    glEnableVertexAttribArray(vertexColourLocation);

    glUniformMatrix4fv(projectionLocation,1,GL_FALSE,projectMatrix.getAsArray());
    glUniformMatrix4fv(modelViewLocation,1,GL_FALSE,modelViewMatrix.getAsArray());

    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,indices);

    angle += 1;
    if (angle > 360)
    {
        angle -= 360;
    }
}


void renderFrame(){
    switch (select){
        case 1:
            show1();
            break;
        case 2:
            show2();
            break;
        case 3:
            showMVPX();
            break;
        case 4:
            showMVPY();
            break;
        case 5:
            showMVPZ();
            break;
        case 6:
            showMVPXYZ();
            break;
        case 7:
            showMVPXYZQuaternions();
            break;
        case 8:
            break;
        default:
            break;
    }
}

static void command(const char* content){
    if(std::strcmp(content,"show1") == 0){
        select = 1;
    }else if(std::strcmp(content,"show2") == 0){
        select = 2;
    }else if(std::strcmp(content,"show3") == 0){
        select = 3;
    }else if(std::strcmp(content,"show4") == 0){
        select = 4;
    }else if(std::strcmp(content,"show5") == 0){
        select = 5;
    }else if(std::strcmp(content,"show6") == 0){
        select = 6;
    }else if(std::strcmp(content,"show7") == 0){
        select = 7;
    }else if(std::strcmp(content,"show8") == 0){
        select = 8;
    }
}

JNIEXPORT void JNICALL Java_com_boyaa_glesdemo_NativeLibrary_init(
        JNIEnv* env,
        jclass jclass1,
        jint width,
        jint height){
    viewWidth = width;
    viewHeight = height;
    glViewport(0,0,width,height);
}

JNIEXPORT void JNICALL Java_com_boyaa_glesdemo_NativeLibrary_rumCmd(
        JNIEnv* env,
        jclass jclass1,
        jstring s1){
    auto cmd = env->GetStringUTFChars(s1,NULL);
    LOGI("%s",cmd);
    command(cmd);
    env->ReleaseStringUTFChars(s1,cmd);
}

JNIEXPORT void JNICALL Java_com_boyaa_glesdemo_NativeLibrary_step(
        JNIEnv* env,
        jclass jclass1){
    renderFrame();
}

JNIEXPORT void JNICALL Java_com_boyaa_glesdemo_NativeLibrary_uninit(
        JNIEnv* env,
        jclass jclass1){

}




