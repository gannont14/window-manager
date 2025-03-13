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

// cycling through windows
void cycleWindowLeft(void) { focusNextWindowTiled(activeWorkspaceId, DIRECTION_LEFT); }
void cycleWindowRight(void) { focusNextWindowTiled(activeWorkspaceId, DIRECTION_RIGHT); }
void cycleWindowUp(void) { focusNextWindowTiled(activeWorkspaceId, DIRECTION_UP); }
void cycleWindowDown(void) { focusNextWindowTiled(activeWorkspaceId, DIRECTION_DOWN); }
