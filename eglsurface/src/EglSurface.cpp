#include <iostream>
#include "EglSurface.h"
#include <vector>
using namespace std;

EglSurface::EglSurface()
: mEGLDisplay(EGL_NO_DISPLAY)
, mEGLConfig()
, mEGLContext(EGL_NO_CONTEXT)
, mEGLSurface(EGL_NO_SURFACE)
{
}

EglSurface::~EglSurface()
{

}

 bool EglSurface::CreateSurface(EGLNativeWindowType nativeWindow, EGLNativeDisplayType nativeDisplay)
 {
    // 获取与本地窗口系统关联的EGL显示连接
    mEGLDisplay = eglGetDisplay(nativeDisplay);
    if (EGL_NO_DISPLAY == mEGLDisplay) 
    {
        cout << "eglGetDisplay failed" << endl;
        return false;
    }

    EGLint eglMajorVersion = 0;
    EGLint eglMinorVersion = 0;
    // 初始化EGL库，验证驱动兼容性，并获取EGL版本号
    if (!eglInitialize(mEGLDisplay, &eglMajorVersion, &eglMinorVersion)) {
        cout << "eglInitialize failed" << endl;
        return false;
    }
    cout << "eglInitialize EGL version: " << eglMajorVersion << ", " << eglMinorVersion << endl;

    // 绑定渲染API（如OpenGL ES、OpenGL）
    (void)eglBindAPI(EGL_OPENGL_ES_API);

    std::vector<EGLint> configAttrib =
    {
        EGL_SURFACE_TYPE,       EGL_WINDOW_BIT,
        EGL_RENDERABLE_TYPE,    EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE,           8,
        EGL_GREEN_SIZE,         8,
        EGL_BLUE_SIZE,          8,
        EGL_ALPHA_SIZE,         8,
        EGL_DEPTH_SIZE,         8,
        // EGL_SAMPLES,            4,
        EGL_NONE
    };

    EGLint configNum = 0;
    // 选择匹配的帧缓冲配置（颜色格式、深度缓冲等）
    if (!eglChooseConfig(mEGLDisplay, configAttrib.data(), &mEGLConfig, 1, &configNum) || 0 == configNum)
    {
        cout << "eglChooseConfig failed" << endl;;
        return false;
    }

    const EGLint surfaceAttribList[] = {EGL_RENDER_BUFFER, EGL_BACK_BUFFER, EGL_NONE};
    // 创建与原生窗口关联的渲染表面
    mEGLSurface = eglCreateWindowSurface(mEGLDisplay, mEGLConfig, nativeWindow, surfaceAttribList);
    if (EGL_NO_SURFACE == mEGLSurface) {
        cout << "eglCreateWindowSurface failed" << endl;
        return false;
    }
 }

 void EglSurface::DestroySurface()
 {
    if (!StopMakeCurrent()) 
    {
        cout << "eglMakeCurrent nosurface nocontext failed" << endl;
    }

    // 销毁通过 eglCreateWindowSurface 创建的渲染表面
    if (!eglDestroySurface(mEGLDisplay, mEGLSurface)) 
    {
        cout << "eglDestroySurface failed" << endl;
    }
    else 
    {
        mEGLSurface = EGL_NO_SURFACE;
    }

    // 销毁通过 eglCreateContext 创建的OpenGL/OpenGL ES上下文
    if (!eglDestroyContext(mEGLDisplay, mEGLContext)) {
        cout << "eglDestroyContext failed" << endl;
    }
    else {
        mEGLContext = EGL_NO_CONTEXT;
    }

    // 终止与显示连接（EGLDisplay）相关的所有资源
    (void)eglTerminate(mEGLDisplay);
    mEGLDisplay = EGL_NO_DISPLAY;

    // 释放当前线程与EGL相关的资源
    (void)eglReleaseThread();

    cout << "EGL Surface destroy success" << endl;
 }

 bool EglSurface::CreateEglContext()
 {
    if(EGL_NO_CONTEXT != mEGLContext) 
    {
        return true;
    }
    const EGLint contextAttribList[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};
    // EGL API 中用于创建 OpenGL/OpenGL ES 渲染上下文的核心函数
    mEGLContext = eglCreateContext(mEGLDisplay, mEGLConfig, NULL, contextAttribList);
    if (EGL_NO_CONTEXT == mEGLContext) 
    {
        cout << "eglCreateContext failed" << endl;;
        return false;
    }
 }

 bool EglSurface::StartMakeCurrent()
 {
    // 将指定的 ‌渲染上下文（EGLContext）‌ 和 ‌表面（EGLSurface）‌ 绑定到当前线程，使得后续 OpenGL ES 指令在此上下文中执行‌
    if (!eglMakeCurrent(mEGLDisplay, mEGLSurface, mEGLSurface, mEGLContext)) 
    {
        cout << "StartMakeCurrent failed" << endl;
        return false;
    }

    return true;
 }

 bool EglSurface::StopMakeCurrent()
 {
    if (!eglMakeCurrent(mEGLDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT)) 
    {
        cout << "StopMakeCurrent failed" << endl;
        return false;
    }

    return true;
 }

 bool EglSurface::SwapBuffers()
 {
    // 交换 ‌前缓冲区（显示内容）‌ 和 ‌后缓冲区（渲染目标）‌，将渲染结果提交到屏幕‌
    if (!eglSwapBuffers(mEGLDisplay, mEGLSurface)) 
    {
        cout << "eglSwapBuffers failed" << endl;
        return false;
    }

    return true;
 }

 void EglSurface::GetSurfaceSize(int& width, int& height)
 {
    // 查询 ‌表面属性‌ 的当前值
    (void)eglQuerySurface(mEGLDisplay, mEGLSurface, EGL_WIDTH, &width);
    (void)eglQuerySurface(mEGLDisplay, mEGLSurface, EGL_HEIGHT, &height);
 }

 void EglSurface::ShowSurface()
 {

 }

void EglSurface::HideSurface()
{

}

EGLDisplay EglSurface::GetEGLDisplay() const
{
    return mEGLDisplay;
}