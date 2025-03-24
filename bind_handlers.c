#include <X11/Xlib.h>
#include<stdio.h>
#include "key_binds.h"
#include "window_manager.h"
#include "workspace.h"

void launchTerminal(void)
{
  puts("Launching terminal!");
  launchApplication("xterm -fn fixed");
  /*launchApplication("ghostty");*/
}

void launchFirefox(void)
{
  puts("Launching firefox!");
  /*launchApplication("export DISPLAY:=1 && firefox");*/
}

void closeWindow(void)
{
  Window focusedWindow;
  int revertTo;

  // get the right focus
  XGetInputFocus(dsp, &focusedWindow, &revertTo);

  // make sure that it is a closeable window (i.e. not the root)
  if(focusedWindow != None && focusedWindow != root)
  {
    closeCurrentWindow(focusedWindow);
  }

}

// cycling through windows
void cycleWindowLeft(void) { focusNextWindowTiled(activeWorkspaceId, DIRECTION_LEFT); }
void cycleWindowRight(void) { focusNextWindowTiled(activeWorkspaceId, DIRECTION_RIGHT); }
void cycleWindowUp(void) { focusNextWindowTiled(activeWorkspaceId, DIRECTION_UP); }
void cycleWindowDown(void) { focusNextWindowTiled(activeWorkspaceId, DIRECTION_DOWN); }
