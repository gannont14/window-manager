#include <X11/Xlib.h>
#include<stdio.h>
#include "key_binds.h"
#include "window_manager.h"

void launchTerminal(void)
{
  puts("Launching terminal!");
  XFlush(dsp);
  launchApplication("xterm -fn fixed");
  XFlush(dsp);
}
