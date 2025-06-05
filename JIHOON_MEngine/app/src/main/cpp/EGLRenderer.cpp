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

//    _camera = make_shared<Camera>();
//    _camera->initialize(ANativeWindow_getWidth(window),ANativeWindow_getHeight(window));
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
    LOGI("EGLRenderer renderFrame");
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    unsigned int shaderProgram = shaders->GetShaderProgram("SimpleRed");

    // Define cube vertices (Eigen)
    std::vector<Eigen::Vector3f> cubeVertices = {
            {-0.5f, -0.5f, -0.5f}, { 0.5f, -0.5f, -0.5f}, { 0.5f,  0.5f, -0.5f}, { -0.5f,  0.5f, -0.5f},
            {-0.5f, -0.5f,  0.5f}, { 0.5f, -0.5f,  0.5f}, { 0.5f,  0.5f,  0.5f}, { -0.5f,  0.5f,  0.5f},
    };

    // Define indices for 12 triangles (two per face)
    std::vector<unsigned int> indices = {
            0,1,2, 2,3,0,  4,5,6, 6,7,4,
            4,5,1, 1,0,4,  7,6,2, 2,3,7,
            4,0,3, 3,7,4,  5,1,2, 2,6,5
    };

    // Flattened vertex array for OpenGL
    std::vector<float> flatVerts;
    for (auto& v : cubeVertices) {
        flatVerts.push_back(v.x());
        flatVerts.push_back(v.y());
        flatVerts.push_back(v.z());
    }

    // Generate buffers
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, flatVerts.size() * sizeof(float), flatVerts.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Create MVP matrix
    Eigen::Matrix4f model = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f view = Eigen::Matrix4f::Identity();
    Eigen::Matrix4f proj = Eigen::Matrix4f::Identity();

    // Rotate around Y
    static auto start = std::chrono::steady_clock::now();
    float t = std::chrono::duration<float>(std::chrono::steady_clock::now() - start).count();
    // model.block<3,3>(0,0) = ...
    // 이 부분은 model이라는 4×4 행렬의 왼쪽 상단 3×3 블록 (즉 회전, 스케일 부분)에 값을 대입한다는 뜻입니다.
    model.block<3,3>(0,0) = Eigen::AngleAxisf(t, Eigen::Vector3f::UnitY()).toRotationMatrix();

    // View: simple translation (camera back from origin)
    Eigen::Vector3f eye(0.0f, 0.0f, 3.0f);
    Eigen::Vector3f at(0.0f, 0.0f, 0.0f);
    Eigen::Vector3f up(0.0f, 1.0f, 0.0f);

    Eigen::Vector3f zaxis = (eye - at).normalized();           // forward
    Eigen::Vector3f xaxis = up.cross(zaxis).normalized();          // right
    Eigen::Vector3f yaxis = zaxis.cross(xaxis);                    // true up

    view <<  xaxis.x(), yaxis.x(), zaxis.x(), -xaxis.dot(eye),
            xaxis.y(), yaxis.y(), zaxis.y(), -yaxis.dot(eye),
            xaxis.z(), yaxis.z(), zaxis.z(), -zaxis.dot(eye),
            0,         0,         0,         1;



    // Projection: perspective
    float aspect = (float)_surfaceWidth / (float)_surfaceHeight;
    float fov = 45.0f * M_PI / 180.0f;
    float near = 0.1f, far = 100.0f;
    float f = 1.0f / tan(fov / 2);
    proj << f / aspect, 0, 0, 0,
            0, f, 0, 0,
            0, 0, (far+near)/(near-far), (2*far*near)/(near-far),
            0, 0, -1, 0;

    Eigen::Matrix4f mvp = proj * view * model;

    glUseProgram(shaderProgram);
    GLint mvpLoc = glGetUniformLocation(shaderProgram, "uMVP");
    glUniformMatrix4fv(mvpLoc, 1, GL_FALSE, mvp.data());

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    eglSwapBuffers(_display, _surface);
}

void EGLRenderer::setViewportSize(int width, int height) {
    LOGI("EGLRenderer setViewportSize");
    _surfaceWidth = width;
    _surfaceHeight = height;

    glViewport(0, 0, _surfaceWidth, _surfaceHeight);
}