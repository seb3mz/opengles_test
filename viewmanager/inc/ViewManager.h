#ifndef VIEW_MANAGER_H
#define VIEW_MANAGER_H
#include "imgtrans.h"
#include "Shader.h"
#include "EglSurface.h"
#include "Xwindow.h"

class ViewManager
{
public:
    ViewManager(XWindow *win, EglSurface *sur);
    ~ViewManager();
    void PreRender();
    void Render();
    void DrawTriangle();
    void DrawTexture();

private:
    XWindow *window;
    EglSurface *surface;
    Shader *xshader;  
};

#endif //VIEW_MANAGER_H