#ifndef WINDOW_LAYOUT_H
#define WINDOW_LAYOUT_H

#include <X11/X.h>

#define MAX_WINDOWS 10
#define MAX_NUM_WORKSPACES 10

typedef enum{
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT,
} FocusDirection;

typedef enum{
  LAYOUT_TILED,
  LAYOUT_MONOCLE
} Layout;

typedef struct{
  int id;
  Layout layout;
  int numWindows;
  Window workspaceWindows[MAX_WINDOWS];
} Workspace;

extern int activeWorkspaceId;

int addWorkSpace(int id);
int addWindowToWorkspace(int workspaceId, Window window);
void applyWorkspaceLayout(int workspaceId);
void focusNextWindowTiled(int workspaceId,
                          FocusDirection direction);
void removeWindowFromWorkspace(Window destroyedWindow);

#endif // !WINDOW_LAYOUT_H
