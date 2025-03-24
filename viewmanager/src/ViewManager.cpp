#include <iostream>
#include "ViewManager.h"
#include <EGL/egl.h>
#include "Texture.h"
//#include <GLES2/gl2.h>


#include "Triangle.h"

using namespace std;


#define IMG_WIDTH 1152
#define IMG_HEIGHT 884
    

ViewManager::ViewManager(XWindow *win, EglSurface *sur)
{
    window = win;
    surface = sur;
    xshader = new Shader();
}

ViewManager::~ViewManager()
{
}

void ViewManager::PreRender()
{
    //创建窗口
    window->CreateWindow("opengl test", IMG_WIDTH, IMG_HEIGHT);
    //创建 surface
    surface->CreateSurface(window->GetEGLNativeWindow());
    //创建 context
    surface->CreateEglContext();
    surface->StartMakeCurrent();

    cout << "Begin PreRender" << endl;
    glViewport(0, 0, IMG_WIDTH, IMG_HEIGHT);
}

void ViewManager::Render()
{
    //DrawTriangle();
    DrawTexture();
}

void ViewManager::DrawTriangle()
{
    Triangle *triangle = new Triangle(surface);
    triangle->Draw();
}

void ViewManager::DrawTexture()
{
    Texture *texture = new Texture(surface);
    texture->Draw();
}