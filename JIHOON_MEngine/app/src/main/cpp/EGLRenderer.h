//
// Created by jihoon jung on 2025. 6. 5..
//

#ifndef JIHOON_MENGINE_EGLRENDERER_H
#define JIHOON_MENGINE_EGLRENDERER_H


#include "Interface.h"
#include "Shaders.h"
//#include "RenderObject.h"
//#include "Camera.h"

class EGLRenderer {
public:
    EGLRenderer();
    ~EGLRenderer();

    bool initialize(EGLNativeWindowType window);
    void renderFrame();
    void cleanup();
    void setViewportSize(int width, int height);

private:
    EGLDisplay _display; // OpenglES 명령을 보낼 수 있는 "물리적 출력 장치"에 대한 핸들, HDMI포트 같은 느낌이다.
    EGLSurface _surface; // 실제 픽셀을 렌더링할 대상 (보통 화면 Surface), GPU가 렌더링한 결과를 출력하는 버퍼
    EGLContext _context; // OpenglES 산채를 담고있는 객체(렌더링 설정, 셰이더, 텍스처 등), OpenGL을 쓰기 위한 작업 환경이나 작업 책상
    // 렌더링에 필요한 모든 상태와 객체 (셰이더, 텍스처, VBO 등)을 갖고 있는 GPU 명령 환경
    // GL 함수를 호출하면 context 내 자원에 작용함
    // 한 쓰레드당 하나의 context만 current 상태가 될 수 있음

    shared_ptr<Shaders> shaders;

    int _surfaceWidth;
    int _surfaceHeight;


//    shared_ptr<RenderObject> _renderObject;
//    shared_ptr<Camera> _camera;

};


#endif //JIHOON_MENGINE_EGLRENDERER_H
