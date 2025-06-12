#include <jni.h>
#include <string>
#include <android/native_window_jni.h> // ANativeWindow_fromSurface
#include "EGLRenderer.h"
#include "Shaders.h"
#include <android/bitmap.h>

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

extern "C"
JNIEXPORT void JNICALL
Java_com_example_jihoon_1mengine_MainActivity_nativeOnTouchDelta(JNIEnv *env, jobject thiz, jfloat dx, jfloat dy) {
    renderer.onTouchDelta(dx, dy); // 회전 입력 전달
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_jihoon_1mengine_MainActivity_nativeSetImage(JNIEnv *env, jobject thiz, jobject bitmap) {
    AndroidBitmapInfo info;
    void* pixels = nullptr;

    if (AndroidBitmap_getInfo(env, bitmap, &info) != ANDROID_BITMAP_RESULT_SUCCESS ||
        info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Unsupported bitmap format");
        return;
    }

    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("Failed to lock pixels");
        return;
    }

    // renderer에 이미지 데이터 전달
    renderer.getScene()->setTextureToRenderObject(info.width, info.height, pixels);
    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_jihoon_1mengine_MainActivity_nativeRelease(JNIEnv* env, jobject thiz) {
    renderer.cleanup();
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_jihoon_1mengine_MainActivity_nativeSetMeshType(JNIEnv* env, jobject thiz, jstring s) {
    const char* cstr = env->GetStringUTFChars(s, nullptr); // Java string → C-style string
    std::string str(cstr);                                // C-style string → std::string
    env->ReleaseStringUTFChars(s, cstr);                  // 메모리 해제

    renderer.getScene()->setGeometryToRenderObject(str);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_jihoon_1mengine_MainActivity_nativeResetScene(JNIEnv* env, jobject thiz) {
    renderer.resetScene();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_jihoon_1mengine_MainActivity_nativeSetAssetImage(JNIEnv *env, jobject thiz, jobject bitmap) {
    AndroidBitmapInfo info;
    void* pixels = nullptr;

    if (AndroidBitmap_getInfo(env, bitmap, &info) != ANDROID_BITMAP_RESULT_SUCCESS ||
        info.format != ANDROID_BITMAP_FORMAT_RGBA_8888) {
        LOGE("Unsupported bitmap format");
        return;
    }

    if (AndroidBitmap_lockPixels(env, bitmap, &pixels) != ANDROID_BITMAP_RESULT_SUCCESS) {
        LOGE("Failed to lock pixels");
        return;
    }
    renderer.getScene()->getDefaultPlane()->GetTexture()->setTextureData(info.width, info.height, pixels);
    // 또는 원하는 Texture 인스턴스에 전달

    AndroidBitmap_unlockPixels(env, bitmap);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_jihoon_1mengine_MainActivity_nativeOnJoystickMoved(JNIEnv *env, jobject thiz, jfloat x, jfloat y) {
    renderer.onJoystickMoved(x, y);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_example_jihoon_1mengine_MainActivity_nativeOnTwoFingerTouchDelta(JNIEnv *env, jobject thiz, jfloat dx, jfloat dy) {
    renderer.onTwoFingerTouchDelta(dx, dy);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_jihoon_1mengine_MainActivity_nativeOnToggleChanged(
        JNIEnv *env, jobject thiz, jboolean is_on) {
    renderer.onToggleChanged(is_on);
}