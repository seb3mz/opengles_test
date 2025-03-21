#include <iostream>
#include <unistd.h>
#include "ViewManager.h"
#include "Xwindow.h"
#include "EglSurface.h"


using namespace std;

int main()
{
    //窗口
    XWindow *win = new XWindow();
    //Display、Surface、Context
    EglSurface *surface = new EglSurface();
    //OpengL ES 渲染
    ViewManager * view = new ViewManager(win, surface);

    //渲染
    view->PreRender();
    view->Render();

    sleep(-1);
    return 0;
}