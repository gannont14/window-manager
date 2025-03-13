#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <X11/Xutil.h>
#include "window_manager.h"

Window createSimpleWindow(int x, int y, unsigned int width,
                          unsigned int height)
{
  Window win = XCreateSimpleWindow(
    dsp, 
    root, 
    x, y,
    width, height,
    3,
    BlackPixel(dsp, 0),
    WhitePixel(dsp, 0)
  );

  XSelectInput(dsp, win, 
               ExposureMask |
               ButtonPressMask | 
               ButtonReleaseMask | 
               KeyPressMask);

  return win;
}

void setWindowProperties(Window win, const char* title)
{
  XStoreName(dsp, win, title);

  XClassHint class_hint;
  class_hint.res_name = "winder";
  class_hint.res_class = "WindowManager";
  XSetClassHint(dsp, win, &class_hint);

  Atom wm_delete = XInternAtom(dsp, "WM_DELTE_WINDOW", False);
  XSetWMProtocols(dsp, win, &wm_delete, 1);
}

void drawInWindow(Window win)
{
  GC gc = XCreateGC(dsp, win, 0, NULL);

  XSetForeground(dsp, gc, BlackPixel(dsp, 0));

  XDrawLine(dsp, win, gc, 10, 10, 110, 60);

  XDrawString(dsp, win, gc, 30, 40, "Hello World", 11);

  XFreeGC(dsp, gc);
}

void showWindow(Window win)
{
  XMapWindow(dsp, win);

  XFlush(dsp);
}

Window getCurrentlyFocusedWindow(void)
{
  Window focused = None;
  int revert_to;

  XGetInputFocus(dsp, &focused, &revert_to);

  return focused;
}

