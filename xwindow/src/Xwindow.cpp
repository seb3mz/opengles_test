#include "Xwindow.h"
#include "iostream"
using namespace std;

XWindow::XWindow()
{

}

XWindow::~XWindow()
{
    
}

bool XWindow::CreateWindow(const char *title, int w, int h)
{
    Window root;
    XSetWindowAttributes swa;
    XSetWindowAttributes xattr;
    XVisualInfo vinfo;
    // 建立与 X 服务器的连接，返回 Display 结构体指针。参数 display_name 若为 NULL，则使用环境变量 DISPLAY 连接本地服务器
    m_pcXdisplay = XOpenDisplay(NULL);
    // 获取默认根窗口（root window），通常作为父窗口用于创建子窗口‌
    root = DefaultRootWindow(m_pcXdisplay);

    swa.event_mask = KeyPressMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask;
    // 根据屏幕、颜色深度和视觉类型（如 TrueColor）匹配可用的视觉信息，返回 XVisualInfo 结构体。用于确保窗口支持特定颜色模式‌
    XMatchVisualInfo(m_pcXdisplay, DefaultScreen(m_pcXdisplay), 32, TrueColor, &vinfo);

    // 为窗口创建颜色映射表（Colormap），管理颜色分配。通常与 XAllocColor 配合使用，定义颜色资源‌
    swa.colormap = XCreateColormap(m_pcXdisplay, RootWindow(m_pcXdisplay, vinfo.screen), vinfo.visual, AllocNone);

    // 创建新窗口，需指定父窗口、位置、尺寸、边框宽度和颜色等参数
    mWin = XCreateWindow(m_pcXdisplay, RootWindow(m_pcXdisplay, vinfo.screen),
                             0, 0, w, h, 0, vinfo.depth, InputOutput,
                             vinfo.visual,
                             CWBorderPixel | CWColormap | CWEventMask, &swa);

    swa.border_pixel = 0;
    xattr.override_redirect = GL_FALSE;
    xattr.border_pixel = 20;

    // 修改窗口属性，如背景色、边框颜色或事件处理掩码。需通过 XSetWindowAttributes 结构体指定新属性‌
    XChangeWindowAttributes(m_pcXdisplay, mWin, CWOverrideRedirect, &xattr);

    XWindowAttributes newattr;
    XWindowAttributes rootattr;
    XGetWindowAttributes(m_pcXdisplay, mWin, &newattr);
    XGetWindowAttributes(m_pcXdisplay, root, &rootattr);

    // 设置窗口管理器提示（如最小/最大尺寸、缩放策略），通过 XSizeHints 结构体定义窗口行为约束‌
    XSizeHints sizehints;
    sizehints.flags = PPosition|PSize;//|PseudoColor
    sizehints.x = 0;
    sizehints.y = 0;
    sizehints.width = w;
    sizehints.height = h;
    XSetWMNormalHints(m_pcXdisplay, mWin, &sizehints);

    // 将窗口映射到屏幕上，使其可见。未映射的窗口不会显示‌
    XMapWindow(m_pcXdisplay, mWin);

    // 设置窗口标题（显示在标题栏或任务栏中）
    XStoreName(m_pcXdisplay, mWin, title);

    // 获取原子（Atom），用于标识协议或属性名称
    m_wmDeleteMessage = XInternAtom(m_pcXdisplay, "WM_DELETE_WINDOW", GL_FALSE);

    // 设置窗口管理器协议（如 WM_DELETE_WINDOW），以响应特定事件（如用户点击关闭按钮）。需与 XInternAtom 配合使用‌
    XSetWMProtocols(m_pcXdisplay, mWin, &m_wmDeleteMessage, 1);

    cout << "Create Window success!!!" << endl;
    return true;
}

void XWindow::DestroyWindow()
{
    XDestroyWindow(m_pcXdisplay, mWin);
    XFlush(m_pcXdisplay);
    XCloseDisplay(m_pcXdisplay);
}

EGLNativeWindowType XWindow::GetEGLNativeWindow() const
{
    return (EGLNativeWindowType)mWin;
}