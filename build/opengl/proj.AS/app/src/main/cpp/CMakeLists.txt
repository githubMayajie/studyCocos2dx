cmake_minimum_required(VERSION 3.5)

find_library(
    log-lib
    log
)
find_library(
    GLESv2-lib
    GLESv2
)
find_library(
    android-lib
    android
)

set(ANDROID_TOOLCHAIN clang)
set(ANDROID_STL c++_static)
set(ANDROID_ARM_MODE arm)
set(ANDROID_PLATFORM_LEVEL 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(ANDROID_CPP_FEATURES "exceptions")


set(NATIVE_NAME Native)
file(GLOB NATIVE_FILES
        JNILoad.cpp
        LoadAssetFile.cpp
        Matrix.cpp
        Quaternions.cpp
        native.cpp
        )

set(NATIVE_INCLUDES cpp)
add_library(
        ${NATIVE_NAME}
        SHARED
        ${NATIVE_FILES}
)
target_include_directories(
        ${NATIVE_NAME}
        PRIVATE
        ${NATIVE_INCLUDES}
)
target_link_libraries(
        ${NATIVE_NAME}
        PRIVATE
        ${log-lib}
        ${android-lib}
        ${GLESv2-lib}
)