#include "workspace.h"
#include "window_manager.h"
#include "window_utils.h"
#include <X11/Xlib.h>
#include <X11/X.h>
#include <stdio.h>
#include <math.h>
#include <sys/types.h>

Workspace workspaces[MAX_NUM_WORKSPACES];
int numWorkspaces = 0;
int activeWorkspaceId;

void focusNextWindowTiled(int workspaceId, FocusDirection direction)
{
  printf("Focusing next window\n");
  // find the current workspace 
  Workspace *ws = NULL;
  for(int i = 0; i < numWorkspaces; i++)
  {
    if(workspaces[i].id == workspaceId)
    {
      ws = &workspaces[i];
      break;
    }
  }

  if(ws == NULL)
  {
    // workspace not found, do nothing (shouldn't ever reach this?)
    return;
  }

  Window currentFocusedWindow = getCurrentlyFocusedWindow();
  int focusedWindowIndex = -1;
  // find the index of the selected window
  for(int i = 0; i < ws->numWindows; i++)
  {
    if (ws->workspaceWindows[i] == currentFocusedWindow)
    {
      focusedWindowIndex = i;
      break;
    }
  }

  if(focusedWindowIndex == -1)
  {
    // can't find the window
    return;
  }


  // this logic will probabyl ahve to be changed
  // for right now up and right are going to be the same thing
  int newIndex;
  switch (direction) {
    case DIRECTION_UP:
    case DIRECTION_RIGHT:
      newIndex = focusedWindowIndex + 1 % ws->numWindows;
      break;
    case DIRECTION_DOWN:
    case DIRECTION_LEFT:
      newIndex = (focusedWindowIndex - 1 + ws->numWindows) % ws->numWindows;
      break;
    default:
      puts("No window to cycle to\n");
      break;
  }

  // focus the window
  XSetInputFocus(dsp, ws->workspaceWindows[newIndex],
                 RevertToPointerRoot, CurrentTime);
}

// TODO
void applyMonocoleLayout(int numWindows, Window windows[]);

void applyTiledLayout(int numWindows, Window windows[])
{
  puts("Applying the tiling layout");
  if(numWindows == 0) return;

  int screen = DefaultScreen(dsp);
  int screen_width = DisplayWidth(dsp, screen);
  int screen_height = DisplayHeight(dsp, screen);

  // Calculate tile dimensions
  int rows = (int)sqrt(numWindows);
  int cols = (numWindows + rows - 1) / rows;

  int cell_width = screen_width / cols;
  int cell_height = screen_height / rows;
    
  // Apply layout
  int row = 0, col = 0;
  for (int i = 0; i < numWindows; i++) {
      int x = col * cell_width;
      int y = row * cell_height;

    printf("Moving window to [%d %d]\n with h/w:[%d, %d]",
           x, y,
           cell_height, cell_width);
      XMoveResizeWindow(dsp, windows[i], x, y, 
                        cell_width - 2, cell_height - 2);

      col++;
      if (col >= cols) {
        col = 0;
        row++;
      }
  }

}

int addWorkSpace(int id)
{
  if(numWorkspaces == MAX_NUM_WORKSPACES)
  {
    printf("Could not add new workspace, as we are at max capacity\n");
    return -1;
  }

  workspaces[numWorkspaces].id = id;
  workspaces[numWorkspaces].layout = LAYOUT_TILED; // whatever default is
  workspaces[numWorkspaces].numWindows = 0;

  return numWorkspaces++;
}

int addWindowToWorkspace(int workspaceId, Window window)
{
  // find the workspace with given id
  int workspaceInd = -1;
  for(int i = 0; i < numWorkspaces; i++)
  {
    if(workspaces[i].id == workspaceId)
    {
      workspaceInd = i;
      break;
    }
  }

  if(workspaceInd == -1) // couldn't find workspace
  {
    printf("Couldn't find workspace with id: %d\n", workspaceId);
    return -1;
  }

  Workspace* ws = &workspaces[workspaceInd];

  if(ws->numWindows == MAX_WINDOWS) // workspace has max # of windows
  {
    printf("Workspace %d has max num of windows\n", workspaceId);
    return -1;
  }

  // success, window added
  ws->workspaceWindows[ws->numWindows] = window;
  ws->numWindows++;
  return 0;
}

void applyWorkspaceLayout(int workspaceId)
{
  // find the workspace with given id
  int workspaceInd = -1;
  for(int i = 0; i < numWorkspaces; i++)
  {
    if(workspaces[i].id == workspaceId)
    {
      workspaceInd = i;
      break;
    }
  }

  if(workspaceInd == -1) // couldn't find workspace
  {
    printf("Couldn't find workspace with id: %d\n", workspaceId);
    return;
  }

  Workspace ws = workspaces[workspaceInd];
  switch (ws.layout) {
    case LAYOUT_TILED:
      applyTiledLayout(ws.numWindows, ws.workspaceWindows);
      break;
    case LAYOUT_MONOCLE:
      /*applyMonocleLayout(ws.numWindows, ws.workspaceWindows);*/
      break;
    default:
      printf("Invalid layout | workspace\n");
      return;
  }

}




