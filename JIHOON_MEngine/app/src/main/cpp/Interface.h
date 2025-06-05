//
// Created by jihoon jung on 2025. 6. 5..
//

#ifndef JIHOON_MENGINE_INTERFACE_H
#define JIHOON_MENGINE_INTERFACE_H

// EGL, Opengl
#include <EGL/egl.h>                        // EGL의 기본 타입 및 함수 (EGLDisplay, EGLContext 등)
#include <EGL/eglext.h>                     // 확장 기능 관련
#include <android/native_window.h>         // ANativeWindow, EGLNativeWindowType 등
#include <android/native_window_jni.h>     // Java의 Surface 객체를 ANativeWindow로 변환할 때 필요
#include <GLES3/gl3.h>


// 기본 stl
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>
#include <queue>
#include <stack>
#include <deque>
#include <list>
#include <algorithm>
#include <cmath>
#include <cstdlib>


// Log
#include <android/log.h>
#define LOG_TAG "JJH Renderer"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_FATAL_ERROR, LOG_TAG, __VA_ARGS__)
#define LOGS(...) __android_log_print(ANDROID_SILENT_ERROR, LOG_TAG, __VA_ARGS__)

// Eigen
#include <../../../external/eigen/Eigen/Dense>
#include <../../../external/eigen/Eigen/Core>
#include <../../../external/eigen/Eigen/Geometry>
// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


using namespace std;
#endif //JIHOON_MENGINE_INTERFACE_H
