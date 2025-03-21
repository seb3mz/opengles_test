#ifndef EGL_SURFACE_H
#define EGL_SURFACE_H

#include <EGL/egl.h>


// Context Surface 
class EglSurface
{
public:
    EglSurface();
    ~EglSurface();

    bool CreateSurface(EGLNativeWindowType nativeWindow, EGLNativeDisplayType nativeDisplay = EGL_DEFAULT_DISPLAY);
    void DestroySurface();

    bool StartMakeCurrent();
    bool StopMakeCurrent();
    bool SwapBuffers();

    void GetSurfaceSize(int& width, int& height);
    void ShowSurface();
    void HideSurface();

    EGLDisplay GetEGLDisplay() const;
    bool CreateEglContext();

private:
    // 表示与 ‌本地窗口系统（Native Window System）的连接
    EGLDisplay mEGLDisplay;
    EGLConfig  mEGLConfig;
    // 表示一个 ‌OpenGL/OpenGL ES 渲染上下文‌，包含所有 GPU 状态（如着色器、纹理绑定等）
    EGLContext mEGLContext;
    // 表示一个 ‌渲染目标（Render Target）‌，即 OpenGL ES 指令的绘制目标
    EGLSurface mEGLSurface;

private:

};

#endif //EGL_SURFACE_H