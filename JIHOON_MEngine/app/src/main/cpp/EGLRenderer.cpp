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

    _surfaceWidth = ANativeWindow_getWidth(window);
    _surfaceHeight = ANativeWindow_getHeight(window);
    _tmp = make_shared<RenderObject>();
    _tmp->GetGeometry()->createCube_Geometry();
    _tmp->GetTransform()->SetPosition(glm::vec3 (-3.0f, 0.0f, -2.0f));
    _scene =  make_shared<Scene>();
    _camera = make_shared<Camera>();
    _camera->initialize(_surfaceWidth,_surfaceHeight);
    _camera->getTransform()->SetPosition(glm::vec3 (0.0f, 3.0f, 7.0f));
    _camera->getTransform()->RotateByEulerDelta(-10.0f, 0.0f);
    _light = make_shared<Light>();
    _light->getTransform()->SetPosition(glm::vec3 (0.0f, 3.0f, 3.0f));
    _light->getTransform()->RotateByEulerDelta(-30.0f, -30.0f);

    makeShadowMapTexture();

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

    renderScene();
}


void EGLRenderer::setViewportSize(int width, int height) {
    LOGI("EGLRenderer setViewportSize");
    _surfaceWidth = width;
    _surfaceHeight = height;
    _camera->setViewportSize(width, height);
    glViewport(0, 0, _surfaceWidth, _surfaceHeight);
}

void EGLRenderer::onTouchDelta(float dx, float dy) {
    if (_scene->getRenderObject())
    {
        float sensitivity = 0.5f;
        float yaw = dx * sensitivity;
        float pitch = dy * sensitivity;
        _scene->getRenderObject()->GetTransform()->RotateByEulerDelta(pitch, yaw);
    }
}

void EGLRenderer::resetScene() {
    LOGI("EGLRenderer -> resetScene()");
    _scene.reset();
    _scene = make_shared<Scene>();
}

void EGLRenderer::makeShadowMapTexture() {

    glGenFramebuffers(1, &_shadowMapFBO);   CHECK_GL_ERROR();

    glGenTextures(1, &_shadowMap);   CHECK_GL_ERROR();
    glBindTexture(GL_TEXTURE_2D, _shadowMap);   CHECK_GL_ERROR();

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);   CHECK_GL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);   CHECK_GL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   CHECK_GL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);   CHECK_GL_ERROR();
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);   CHECK_GL_ERROR();

    glBindFramebuffer(GL_FRAMEBUFFER, _shadowMapFBO);   CHECK_GL_ERROR();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _shadowMap, 0);   CHECK_GL_ERROR();

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        LOGE("Shadow map framebuffer is not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);   CHECK_GL_ERROR();

}


void EGLRenderer::renderScene() {
    if (_display == EGL_NO_DISPLAY || _surface == EGL_NO_SURFACE || _context == EGL_NO_CONTEXT) {
        LOGE("EGL not initialized or surface/context lost");
        return;
    }

    shared_ptr<RenderObject> renderObj = _scene->getRenderObject();
    shared_ptr<RenderObject> defaultPlane = _scene->getDefaultPlane();
    renderObj->GetTransform()->SetPosition(glm::vec3(-10.0f, 0.0f, 0.0f));
    defaultPlane->GetTransform()->SetPosition(glm::vec3 (-10.0f, -0.5f, -10.0f));
    _tmp->GetTransform()->RotateByEulerDelta(0.0f, 1.0f);
    //_camera->getTransform()->RotateByEulerDelta(0.0f, 1.0f);
    _light->Update();
    _camera->Update();
    drawShadowMap(renderObj, defaultPlane);


    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    CHECK_GL_ERROR();

//    DebugShadowMap();

    drawRenderObject(renderObj, "DefaultShader");
    drawRenderObject(defaultPlane, "DefaultShader");
    drawTmp(_tmp, "SimpleWhiteShader",false);
//    drawLight(_light,"SimpleWhiteShader");

    if (!eglSwapBuffers(_display, _surface)) {
        LOGE("eglSwapBuffers failed: 0x%x", eglGetError());
    }
    CHECK_GL_ERROR();


}

void EGLRenderer::drawRenderObject(shared_ptr<RenderObject> renderObj, string shaderName, bool isDrawShadowMap) {

    unsigned int textureId = 0;
    shared_ptr<Texture> texture = renderObj->GetTexture();

    textureId = renderObj->GetTexture()->getTextureId();
    if (textureId == 0) {
        return;
    }


    unsigned int shaderProgram = shaders->GetShaderProgram(shaderName);

    glUseProgram(shaderProgram);
    CHECK_GL_ERROR();


    GLint modelLoc = glGetUniformLocation(shaderProgram, "uModel");
    const glm::mat4& model = renderObj->GetTransform()->getModelMatrix();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    CHECK_GL_ERROR();

    GLint viewLoc = glGetUniformLocation(shaderProgram, "uView");
    GLint projLoc = glGetUniformLocation(shaderProgram, "uProj");

    if (_isToggleOn)
    {
        Eigen::Matrix4f lightView = _light->getLightViewMat();
        Eigen::Matrix4f lightProj = _light->getLightProjMat();

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, lightView.data());
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, lightProj.data());
    }
    else
    {
        if (isDrawShadowMap) {
            Eigen::Matrix4f lightView = _light->getLightViewMat();
            Eigen::Matrix4f lightProj = _light->getLightProjMat();

            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, lightView.data());
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, lightProj.data());
        }
        else {
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(_camera->getViewMat()));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(_camera->getProjMat()));
        }
    }

    CHECK_GL_ERROR();

    GLint lightSpaceViewProjLoc = glGetUniformLocation(shaderProgram, "uLightSpaceMat");
    Eigen::Matrix4f lightSpaceViewProMat = _light->getLightProjMat() * _light->getLightViewMat();
    glUniformMatrix4fv(lightSpaceViewProjLoc, 1, GL_FALSE, lightSpaceViewProMat.data());
    CHECK_GL_ERROR();

    if (!isDrawShadowMap)
    {
        // 텍스처 바인딩
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        CHECK_GL_ERROR();

        GLint texLoc = glGetUniformLocation(shaderProgram, "uTexture");
        if (texLoc == -1) {
            //LOGE("Failed to get uniform location for uTexture");
        }
        glUniform1i(texLoc, 0);
        CHECK_GL_ERROR();

        // shadowmap 바인딩
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, _shadowMap);
        CHECK_GL_ERROR();

        GLint shadowLoc = glGetUniformLocation(shaderProgram, "uShadowMap");
        if (shadowLoc == -1) {
           // LOGE("Failed to get uniform location for uShadowMap");
        }
        glUniform1i(shadowLoc, 1);  // ← GL_TEXTURE1에 해당하는 1번 유닛 지정
        CHECK_GL_ERROR();
    }


    glm::vec3 cameraPos = _camera->getTransform()->getPosition();
    GLint viewPosLoc = glGetUniformLocation(shaderProgram, "uViewPos");
    glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
    CHECK_GL_ERROR();

    GLint lightPosLoc = glGetUniformLocation(shaderProgram, "uLightPos");
    glUniform3f(lightPosLoc, _light->getTransform()->getPosition().x, _light->getTransform()->getPosition().y,
                _light->getTransform()->getPosition().z);
    CHECK_GL_ERROR();

    // 5. 정점 배열 및 드로우 호출
    glBindVertexArray(renderObj->GetGeometry()->getVAO());
    CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, renderObj->GetGeometry()->getIndexCount(), GL_UNSIGNED_INT, 0);
    CHECK_GL_ERROR();
}

void EGLRenderer::drawShadowMap(shared_ptr<RenderObject> renderObj, shared_ptr<RenderObject> defaultPlane) {

    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);   CHECK_GL_ERROR();
    glBindFramebuffer(GL_FRAMEBUFFER, _shadowMapFBO);   CHECK_GL_ERROR();

    glClear(GL_DEPTH_BUFFER_BIT);

    drawRenderObject(renderObj, "ShadowMapShader", true);
    drawRenderObject(defaultPlane, "ShadowMapShader", true);
    drawTmp(_tmp, "ShadowMapShader", true);
    resetViewport();
}

void EGLRenderer::resetViewport() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, _surfaceWidth, _surfaceHeight);
}

void EGLRenderer::onJoystickMoved(float x, float y) {

    if (!_camera) return;
    shared_ptr<Transform> cameraTransform = _camera->getTransform();
    float cameraPosX = cameraTransform->getPosition().x;
    float cameraPosY = cameraTransform->getPosition().y;
    float newPosX = cameraPosX + x;
    float newPosY = cameraPosY - y;
    _camera->getTransform()->SetPosition(glm::vec3 (newPosX, newPosY, cameraTransform->getPosition().z));
//    if (_fpsCtrl)
//        _fpsCtrl->SetJoystick(x, -y);   // 필요하면 Y축 뒤집기
}

void EGLRenderer::onTwoFingerTouchDelta(float dx, float dy) {
    if (_camera) {
        float sensitivity = 0.1f;
        float yaw = dx * sensitivity;
        float pitch = dy * sensitivity;

        _camera->getTransform()->RotateByEulerDelta(pitch, yaw);
    }
//    if (_fpsCtrl)
//        _fpsCtrl->AddViewDelta(dx, dy);
}

void EGLRenderer::drawLight(shared_ptr<Light> renderObj, string shaderName) {

    unsigned int shaderProgram = shaders->GetShaderProgram(shaderName);

    glUseProgram(shaderProgram);
    CHECK_GL_ERROR();


    GLint modelLoc = glGetUniformLocation(shaderProgram, "uModel");
    const glm::mat4& model = renderObj->GetTransform()->getModelMatrix();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    CHECK_GL_ERROR();

    GLint viewLoc = glGetUniformLocation(shaderProgram, "uView");
    GLint projLoc = glGetUniformLocation(shaderProgram, "uProj");

    if (_isToggleOn)
    {
        Eigen::Matrix4f lightView = _light->getLightViewMat();
        Eigen::Matrix4f lightProj = _light->getLightProjMat();

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, lightView.data());
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, lightProj.data());
    }
    else
    {
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(_camera->getViewMat()));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(_camera->getProjMat()));
    }

    CHECK_GL_ERROR();

    // 5. 정점 배열 및 드로우 호출
    glBindVertexArray(renderObj->GetGeometry()->getVAO());
    CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, renderObj->GetGeometry()->getIndexCount(), GL_UNSIGNED_INT, 0);
    CHECK_GL_ERROR();
}

void EGLRenderer::drawTmp(shared_ptr<RenderObject> renderObj, string shaderName, bool isDrawShadowMap) {
    unsigned int shaderProgram = shaders->GetShaderProgram(shaderName);

    glUseProgram(shaderProgram);
    CHECK_GL_ERROR();


    GLint modelLoc = glGetUniformLocation(shaderProgram, "uModel");
    const glm::mat4& model = renderObj->GetTransform()->getModelMatrix();
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
    CHECK_GL_ERROR();

    GLint viewLoc = glGetUniformLocation(shaderProgram, "uView");
    GLint projLoc = glGetUniformLocation(shaderProgram, "uProj");

    if (_isToggleOn)
    {
        Eigen::Matrix4f lightView = _light->getLightViewMat();
        Eigen::Matrix4f lightProj = _light->getLightProjMat();

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, lightView.data());
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, lightProj.data());
    }
    else
    {
        if (isDrawShadowMap) {
            Eigen::Matrix4f lightView = _light->getLightViewMat();
            Eigen::Matrix4f lightProj = _light->getLightProjMat();

            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, lightView.data());
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, lightProj.data());
        }
        else {
            glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(_camera->getViewMat()));
            glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(_camera->getProjMat()));
        }
    }
    CHECK_GL_ERROR();

    GLint lightSpaceViewProjLoc = glGetUniformLocation(shaderProgram, "uLightSpaceMat");
    Eigen::Matrix4f lightSpaceViewProMat = _light->getLightProjMat() * _light->getLightViewMat();
    glUniformMatrix4fv(lightSpaceViewProjLoc, 1, GL_FALSE, lightSpaceViewProMat.data());
    CHECK_GL_ERROR();

    if (!isDrawShadowMap)
    {
        // shadowmap 바인딩
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _shadowMap);
        CHECK_GL_ERROR();

        GLint shadowLoc = glGetUniformLocation(shaderProgram, "uShadowMap");
        if (shadowLoc == -1) {
            // LOGE("Failed to get uniform location for uShadowMap");
        }
        glUniform1i(shadowLoc, 0);
        CHECK_GL_ERROR();
    }


    glm::vec3 cameraPos = _camera->getTransform()->getPosition();
    GLint viewPosLoc = glGetUniformLocation(shaderProgram, "uViewPos");
    glUniform3f(viewPosLoc, cameraPos.x, cameraPos.y, cameraPos.z);
    CHECK_GL_ERROR();

    GLint lightPosLoc = glGetUniformLocation(shaderProgram, "uLightPos");
    glUniform3f(lightPosLoc, _light->getTransform()->getPosition().x, _light->getTransform()->getPosition().y,
                _light->getTransform()->getPosition().z);
    CHECK_GL_ERROR();

    // 5. 정점 배열 및 드로우 호출
    glBindVertexArray(renderObj->GetGeometry()->getVAO());
    CHECK_GL_ERROR();
    glDrawElements(GL_TRIANGLES, renderObj->GetGeometry()->getIndexCount(), GL_UNSIGNED_INT, 0);
    CHECK_GL_ERROR();
}

void EGLRenderer::DebugShadowMap()
{
    unsigned int shaderProgram = shaders->GetShaderProgram("DebugShadowMapShader");
    glUseProgram(shaderProgram);
    CHECK_GL_ERROR();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _shadowMap);
    CHECK_GL_ERROR();

    GLint shadowLoc = glGetUniformLocation(shaderProgram, "uShadowMap");
    if (shadowLoc == -1) {
        // LOGE("Failed to get uniform location for uShadowMap");
    }
    glUniform1i(shadowLoc, 0);  // ← GL_TEXTURE1에 해당하는 1번 유닛 지정
    CHECK_GL_ERROR();

    if (debug_VAO == 0)
    {
        float quadVertices[] = {
                // positions        // texture Coords
                -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };
        // setup plane VAO
        glGenVertexArrays(1, &debug_VAO);
        glGenBuffers(1, &debug_VBO);
        glBindVertexArray(debug_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, debug_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
    glBindVertexArray(debug_VAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
};

void EGLRenderer::onToggleChanged(bool isOn) {
    _isToggleOn = isOn;
    // 여기서 토글 상태에 따른 처리를 수행
    // 예: 로그 출력
    LOGI("Toggle state changed to: %s", isOn ? "ON" : "OFF");
}