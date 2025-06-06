//
// Created by jihoon jung on 2025. 6. 5..
//

#include "EGLRenderer.h"
EGLRenderer::EGLRenderer() : _display(EGL_NO_DISPLAY), _surface(EGL_NO_SURFACE), _context(EGL_NO_CONTEXT)
{

}

EGLRenderer::~EGLRenderer() {
    cleanup();
}
// ** 흐름 요약 **
//  - eglGetDisplay → 디바이스 연결
//  - eglInitialize → EGL 초기화
//  - eglChooseConfig → 설정 조건에 맞는 Config 선택
//  - eglCreateWindowSurface → 실제 화면에 쓸 Surface 생성
//  - eglCreateContext → 렌더링용 OpenGL 환경 생성
//  - eglMakeCurrent → 현재 쓰레드에 Context와 Surface 바인딩 (→ glXXX 함수 사용 가능)
//  - eglSwapBuffers → Surface 내용 갱신 (화면에 보여짐)
bool EGLRenderer::initialize(EGLNativeWindowType window) {
    LOGI("EGLRenderer initialzie");

    _display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (_display == EGL_NO_DISPLAY) {
        LOGE("Failed to get EGL display");
        return false;
    }

    if (!eglInitialize(_display, nullptr, nullptr)) {
        LOGE("Failed to initialize EGL");
        return false;
    }

    const EGLint configAttribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_DEPTH_SIZE, 16,
            EGL_NONE
    };

    EGLConfig config;
    EGLint numConfigs;
    if (!eglChooseConfig(_display, configAttribs, &config, 1, &numConfigs) || numConfigs < 1) {
        LOGE("Failed to choose EGL config");
        return false;
    }

    _surface = eglCreateWindowSurface(_display, config, window, nullptr);
    if (_surface == EGL_NO_SURFACE) {
        LOGE("Failed to create EGL surface");
        return false;
    }

    const EGLint contextAttribs[] = {
            EGL_CONTEXT_CLIENT_VERSION, 3, // OpenGL ES 3.0
            EGL_NONE
    };
    _context = eglCreateContext(_display, config, EGL_NO_CONTEXT, contextAttribs);
    if (_context == EGL_NO_CONTEXT) {
        LOGE("Failed to create EGL context");
        return false;
    }

    if (!eglMakeCurrent(_display, _surface, _surface, _context)) {
        LOGE("Failed to make EGL context current");
        return false;
    }

    LOGI("EGL context and surface initialized successfully");

    shaders = make_shared<Shaders>();
    if (!shaders->CreateShaderProgram())
    {
        LOGE("Shader complie Error");
        return false;
    }

    _camera = make_shared<Camera>();
    _camera->initialize(ANativeWindow_getWidth(window),ANativeWindow_getHeight(window));

    _renderObject = make_shared<RenderObject>();
    return true;
}

void EGLRenderer::cleanup() {
    LOGI("EGLRenderer cleanup");
    if (_display != EGL_NO_DISPLAY) {
        eglMakeCurrent(_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

        if (_context != EGL_NO_CONTEXT) {
            eglDestroyContext(_display, _context);
            _context = EGL_NO_CONTEXT;
        }

        if (_surface != EGL_NO_SURFACE) {
            eglDestroySurface(_display, _surface);
            _surface = EGL_NO_SURFACE;
        }

        eglTerminate(_display);
        _display = EGL_NO_DISPLAY;
    }
}


void EGLRenderer::renderFrame() {

    if (_display == EGL_NO_DISPLAY || _surface == EGL_NO_SURFACE || _context == EGL_NO_CONTEXT) {
        LOGE("EGL not initialized or surface/context lost");
        return;
    }

    if (_hasPendingImage) {
        // 텍스처 생성 코드
        if (_textureId == 0)
            glGenTextures(1, &_textureId);
        glBindTexture(GL_TEXTURE_2D, _textureId);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _pendingWidth, _pendingHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, _pendingImageData.data());
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            LOGE("OpenGL error after glTexImage2D: 0x%x", err);
        } else {
            LOGE("Texture uploaded successfully: id=%u, size=%dx%d", _textureId, _pendingWidth, _pendingHeight);
        }
        _hasPendingImage = false;
    }
    if (_textureId == 0) {
        LOGE("No texture available for rendering");
        return;
    }

    _camera->Update();
    _renderObject->Update();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    unsigned int shaderProgram = shaders->GetShaderProgram("SimpleRed");

    Eigen::Matrix4f mvp = _camera->GetProjectionViewMat() * _renderObject->GetTransform()->GetModelMatrix();

    glUseProgram(shaderProgram);

    // 1. uMVP 설정
    GLint mvpLoc = glGetUniformLocation(shaderProgram, "uMVP");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, mvp.data());

    // 텍스처 바인딩 전 에러 체크
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGE("OpenGL error before texture binding: 0x%x", err);
    }

    // 텍스처 바인딩
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _textureId);

    // 텍스처 바인딩 후 에러 체크
    err = glGetError();
    if (err != GL_NO_ERROR) {
        LOGE("OpenGL error after texture binding: 0x%x", err);
    }

    GLint texLoc = glGetUniformLocation(shaderProgram, "uTexture");
    if (texLoc == -1) {
        LOGE("Failed to get uniform location for uTexture");
    }
    glUniform1i(texLoc, 0);

    // 5. 정점 배열 및 드로우 호출
    glBindVertexArray(_renderObject->GetGeometry()->GetVAO());
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

    if (!eglSwapBuffers(_display, _surface)) {
        LOGE("eglSwapBuffers failed: 0x%x", eglGetError());
    }
}


void EGLRenderer::setViewportSize(int width, int height) {
    LOGI("EGLRenderer setViewportSize");
    _surfaceWidth = width;
    _surfaceHeight = height;
    _camera->setViewportSize(width, height);
    glViewport(0, 0, _surfaceWidth, _surfaceHeight);
}

void EGLRenderer::onTouchDelta(float dx, float dy) {
    if (_renderObject)
    {
        float sensitivity = 0.5f;
        float yaw = dx * sensitivity;
        float pitch = dy * sensitivity;
        LOGE(" yaw : %f, pitch : %f\n", dy, dx);
        _renderObject->GetTransform()->RotateByEulerDelta(pitch, yaw);
    }
}

void EGLRenderer::setImageData(int width, int height, void *pixelData) {
    _pendingImageData.assign((unsigned char*)pixelData, (unsigned char*)pixelData + width * height * 4);
    _pendingWidth = width;
    _pendingHeight = height;
    _hasPendingImage = true;
}
