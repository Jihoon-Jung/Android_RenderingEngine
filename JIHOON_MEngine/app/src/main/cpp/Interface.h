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

// glm
#define GLM_ENABLE_EXPERIMENTAL
#include <../../../external/glm/glm/glm.hpp>
#include <../../../external/glm/glm/gtc/matrix_transform.hpp>
#include <../../../external/glm/glm/gtx/quaternion.hpp>
#include <../../../external/glm/glm/gtc/type_ptr.hpp>
#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

using namespace std;

inline const char* GlErrorToString(GLenum err) {
    switch (err) {
        case GL_NO_ERROR:          return "GL_NO_ERROR";
        case GL_INVALID_ENUM:      return "GL_INVALID_ENUM";
        case GL_INVALID_VALUE:     return "GL_INVALID_VALUE";
        case GL_INVALID_OPERATION: return "GL_INVALID_OPERATION";
        case GL_OUT_OF_MEMORY:     return "GL_OUT_OF_MEMORY";
        case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
        default:                   return "UNKNOWN_GL_ERROR";
    }
}

/*
 < 에러 체크 사용 가능한 gl함수들 >
- 셰이더/프로그램 관련
    glCreateShader, glShaderSource, glCompileShader, glCreateProgram, glAttachShader, glLinkProgram, glUseProgram
- 버퍼/VAO/VBO/EBO 관련
    glGenBuffers, glBindBuffer, glBufferData, glGenVertexArrays, glBindVertexArray, glVertexAttribPointer, glEnableVertexAttribArray
- 텍스처 관련
    glGenTextures, glBindTexture, glTexImage2D, glTexParameteri, glActiveTexture
- 렌더링 관련
    glClearColor, glClear, glDrawArrays, glDrawElements
- 기타
    glViewport, glEnable, glDisable, glUniform*, glGetUniformLocation, glGetAttribLocation
 */
#define CHECK_GL_ERROR() \
    { \
        GLenum err = glGetError(); \
        if (err != GL_NO_ERROR) { \
            LOGE("[GL_ERROR] %s:%d (%s) - 0x%x (%s)", \
                __FILE__, __LINE__, __func__, err, GlErrorToString(err)); \
        } \
    }
#endif //JIHOON_MENGINE_INTERFACE_H
