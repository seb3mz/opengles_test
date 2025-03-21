#ifndef X_WINDOW_H
#define X_WINDOW_H

#include <X11/Xlib.h>
#include <X11/X.h>
#include <X11/Xutil.h>
#include <GL/gl.h>
#include <EGL/egl.h>

class XWindow
{
public:
    XWindow();
    ~XWindow();

    bool CreateWindow(const char *title, int w, int h);
    void DestroyWindow();

    EGLNativeWindowType GetEGLNativeWindow() const;

private:
    Window     mWin;
    Display *m_pcXdisplay;
    Atom m_wmDeleteMessage;
};

#endif //X_WINDOW_H