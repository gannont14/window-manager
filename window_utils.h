#ifndef WINDOW_UTILS_H
#define WINDOW_UTILS_H

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>

Window createSimpleWindow(int x, int y, unsigned int width,
                          unsigned int height);

void setWindowProperties(Window win, const char* title);

void drawInWindow(Window win);

void showWindow(Window win);

Window getCurrentlyFocusedWindow(void);

#endif // !WINDOW_UTILS_H
