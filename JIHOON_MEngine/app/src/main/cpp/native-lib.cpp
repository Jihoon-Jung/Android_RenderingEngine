#include <jni.h>
#include <string>
#include <android/native_window_jni.h> // ANativeWindow_fromSurface
#include "EGLRenderer.h"
#include "Shaders.h"

static EGLRenderer renderer;
extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jihoon_1mengine_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}


extern "C"
JNIEXPORT jboolean JNICALL
Java_com_example_jihoon_1mengine_MainActivity_nativeInit(JNIEnv *env, jobject thiz, jobject surface) {
    ANativeWindow* window = ANativeWindow_fromSurface(env, surface);

    return renderer.initialize(window);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_jihoon_1mengine_MainActivity_nativeRender(JNIEnv* env, jobject thiz) {
    renderer.renderFrame(); // 이 함수는 EGLRenderer::renderFrame에서 화면 clear + swapBuffers만 해도 OK
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_jihoon_1mengine_MainActivity_nativeOnSurfaceChanged(JNIEnv *env, jobject thiz, jint width, jint height) {
    renderer.setViewportSize(width, height);
}