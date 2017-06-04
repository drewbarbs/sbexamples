/*
 * Adapted from OpenGL X11 context creation tutorial at
 * https://www.opengl.org/wiki/Tutorial:_OpenGL_3.0_Context_Creation_%28GLX%29
 *
 */
#include <atomic>
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>


#include <GL/gl3w.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <GL/glx.h>

#include "../simpletri/simpletri.h"

typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

template <typename T>
class scope_display_obj {
    Display *d;
    T t;
    std::function<void(Display*, T)> deleter;
public:
    scope_display_obj(Display *d, T t, std::function<void(Display*, T)> deleter): d(d), t(t), deleter(deleter) {}
    ~scope_display_obj() {
        deleter(d, t);
    }

    T get() {
        return t;
    }

    explicit operator bool() {
        return !!t;
    }
};

int main(int argc, char* argv[])
{
    auto display = std::unique_ptr<Display, decltype(&XCloseDisplay)>(XOpenDisplay(nullptr), XCloseDisplay);

    if (!display) {
        std::cerr << "Failed to open X display" << std::endl;
        return 1;
    }

    int glx_major, glx_minor;
    if (!glXQueryVersion(display.get(), &glx_major, &glx_minor)) {
        std::cerr << "Couldn't query glx version" << std::endl;
        return 1;
    }

    // Get a matching FB config
    static int visual_attribs[] =
            {
                    GLX_X_RENDERABLE    , True,
                    GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
                    GLX_RENDER_TYPE     , GLX_RGBA_BIT,
                    GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
                    GLX_RED_SIZE        , 8,
                    GLX_GREEN_SIZE      , 8,
                    GLX_BLUE_SIZE       , 8,
                    GLX_ALPHA_SIZE      , 8,
                    GLX_DEPTH_SIZE      , 24,
                    GLX_STENCIL_SIZE    , 8,
                    GLX_DOUBLEBUFFER    , True,
                    None
            };

    int nfbc = 0;
    auto fbc = std::unique_ptr<GLXFBConfig[], decltype(&XFree)>(glXChooseFBConfig(display.get(), 0, visual_attribs, &nfbc), XFree);
    if (!fbc || nfbc <= 0) {
        std::cerr << "Failed to find frame buffer configuration matching required attributes";
        return 1;
    }

    ;
    auto visinfo = std::unique_ptr<XVisualInfo, decltype(&XFree)> (glXGetVisualFromFBConfig(display.get(), fbc[0]), XFree);
    if (!visinfo) {
        std::cerr << "Failed to find matching visual for TrueColor 32bpp" << std::endl;
        return 1;
    }

    XSetWindowAttributes swa;
    auto cmap = scope_display_obj<Colormap>(display.get(), XCreateColormap(display.get(),
                                                            RootWindow(display.get(), visinfo->screen),
                                                            visinfo->visual, AllocNone), XFreeColormap);
    swa.colormap = cmap.get();
    swa.background_pixmap = None;
    swa.border_pixel = 0;
    swa.event_mask = StructureNotifyMask;

    auto win = scope_display_obj<Window>(display.get(),
                            XCreateWindow(display.get(), RootWindow(display.get(), visinfo->screen),
                                          0, 0, 800, 600, 0, visinfo->depth, InputOutput,
                                          visinfo->visual,
                                          CWBorderPixel | CWColormap | CWEventMask, &swa), XDestroyWindow);
    if (!win) {
        std::cerr << "Failed to create window." << std::endl;
        return 1;
    }

    XWindowAttributes windowAttributes;
    XGetWindowAttributes(display.get(), win.get(), &windowAttributes);

    XStoreName(display.get(), win.get(), "GLX Window");
    XMapWindow(display.get(), win.get());


    /* Install an X error handler so the application won't exit if GL 3.0
       context allocation fails.

       Note this error handler is global.  All display connections in all threads
       of a process use the same error handler, so be sure to guard against other
       threads issuing X commands while this code is running.
    */
    int (*oldHandler)(Display*, XErrorEvent*) =
            XSetErrorHandler([] (Display *dpy, XErrorEvent *ev) {
                std::cerr << "X error occurred" << std::endl;
                return 0;
            });

    // NOTE: It is not necessary to create or make current to a context before
    // calling glXGetProcAddressARB
    auto glXCreateContextAttribsARB = reinterpret_cast<glXCreateContextAttribsARBProc>(gl3wGetProcAddress("glXCreateContextAttribsARB"));
    if (!glXCreateContextAttribsARB) {
        std::cerr << "glXCreateContextAttribsARB() not found, cannot create core OpenGL context" << std::endl;
        return 1;
    }
    const int context_attribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
            GLX_CONTEXT_MINOR_VERSION_ARB, 5,
            None
    };
    auto ctx = scope_display_obj<GLXContext> (display.get(),
                                              glXCreateContextAttribsARB(display.get(),
                                                                         fbc[0], 0, True, context_attribs),
                                              glXDestroyContext);
    // Sync to ensure any errors generated are processed.
    XSync(display.get(), False);
    // Restore the original error handler
    XSetErrorHandler(oldHandler);
    if (!ctx) {
        printf( "Failed to create an OpenGL context\n" );
        return 1;
    }
    gl3wInit();

    // Verifying that context is a direct context
    if (!glXIsDirect(display.get(), ctx.get())) {
        printf("Indirect GLX rendering context obtained\n");
    }
    else {
        printf("Direct GLX rendering context obtained\n");
    }

    printf("Making context current\n");
    glXMakeCurrent(display.get(), win.get(), ctx.get());

    // Make window fullscreen
    XClientMessageEvent xclient = {0};
    xclient.type = ClientMessage;
    xclient.window = win.get();
    xclient.message_type = XInternAtom(display.get(), "_NET_WM_STATE", False);
    xclient.format = 32;
    xclient.data.l[0] = 1;
    xclient.data.l[1] = XInternAtom(display.get(), "_NET_WM_STATE_FULLSCREEN", False);
    xclient.data.l[2] = 0;

    XSendEvent(display.get(),
               windowAttributes.root,
               False,
               SubstructureRedirectMask | SubstructureNotifyMask,
               (XEvent *) &xclient);

    // register interest in the delete window message. WM will produce an event and allow
    // us to gracefully clean up after ourselves, rather than go ahead and destroy the window,
    // close the display as soon as window is closed by user
    // http://stackoverflow.com/a/22612855/756104
    // http://stackoverflow.com/a/1186544/756104
    Atom wm_delete_message = XInternAtom(display.get(), "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display.get(), win.get(), &wm_delete_message, 2);

    // Event loop, "inspired by"
    // http://stackoverflow.com/a/8592969/756104
    // We'll get the X display socket and select() with a timeout so
    // we'll swap monitors every 5 seconds, or immediately break out
    // of sleep and exit the program when we get an X event (in this
    // case, we're interested in WM_DELETE_WINDOW)
    auto x11_fd = ConnectionNumber(display.get());
    fd_set ev_fds;
    FD_ZERO(&ev_fds);
    FD_SET(x11_fd, &ev_fds);
    struct timeval tv;
    tv.tv_sec = 0;
    tv.tv_usec = 50000;
    XEvent ev;
    simpletri instance;
    auto start_time = std::chrono::steady_clock::now();
    auto last_mon_switch = std::chrono::steady_clock::now();
    auto last_mon = 1;
    while (1) {
        select(x11_fd + 1, &ev_fds, nullptr, nullptr, &tv);
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = now - start_time;
        std::chrono::duration<double> since_last_mon_switch = now - last_mon_switch;
        if (since_last_mon_switch > std::chrono::seconds{5}) {
            auto mon = (last_mon + 1) % 2;
            XClientMessageEvent xclient = {0};
            xclient.type = ClientMessage;
            xclient.window = win.get();
            xclient.message_type = XInternAtom(display.get(), "_NET_WM_FULLSCREEN_MONITORS", False);
            xclient.format = 32;
            xclient.data.l[0] = mon;
            xclient.data.l[1] = mon;
            xclient.data.l[2] = mon;
            xclient.data.l[3] = mon;
            xclient.data.l[4] = 1;

            XSendEvent(display.get(),
                       windowAttributes.root,
                       False,
                       SubstructureRedirectMask | SubstructureNotifyMask,
                       (XEvent *) &xclient);
            last_mon = mon;
            last_mon_switch = now;
        }
        instance.render(elapsed.count());
        glXSwapBuffers(display.get(), win.get());
        while (XPending(display.get())) {
            XNextEvent(display.get(), &ev);
            if (ev.xclient.data.l[0] == wm_delete_message) {
                break;
            }
        }
    }
    return 0;
}
