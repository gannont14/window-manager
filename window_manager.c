#include<stdio.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include <stdlib.h>
#include "key_binds.h"
#include "workspace.h"

Display *dsp;
Window root;

// exit with failure and message
void panic(char* message)
{
  puts(message);
  exit(EXIT_FAILURE);
}

void handleConfigureRequest(XEvent e)
{
  XConfigureRequestEvent *req = &e.xconfigurerequest;
  XWindowChanges changes;

  changes.x = req->x;
  changes.y = req->y;

  changes.width = req->width;
  changes.height = req->height;

  /*changes.width = 1000;*/
  /*changes.height = 500;*/
  changes.border_width = req->border_width;
  changes.sibling = req->above;
  changes.stack_mode = req->detail;

  XConfigureWindow(dsp, req->window, req->value_mask, &changes);
  printf("window configured: %lu\n", req->window); 
}

void handleMapRequest(XEvent e)
{
  puts("handling map request");
  Window window = e.xmaprequest.window;

  XWindowAttributes attr;
  XGetWindowAttributes(dsp, window, &attr);

  XMapWindow(dsp, window);

  XRaiseWindow(dsp, window); // raise the window to the top, mostly for floating

  XSetInputFocus(dsp,
                 window,
                 RevertToPointerRoot,
                 CurrentTime); // set focus to the new window
  
  XSync(dsp, False);
  addWindowToWorkspace(activeWorkspaceId, window);
  applyWorkspaceLayout(activeWorkspaceId);

  printf("Window mapped: %lu\n", window);
}

void handleDestroyNotify(XEvent e)
{
  // find the window that was destroyed
  Window destroyedWindow = e.xdestroywindow.window; 
  removeWindowFromWorkspace(destroyedWindow);
  applyWorkspaceLayout(activeWorkspaceId);
}

void handleKeyPress(XEvent e)
{
  KeySym sym = XLookupKeysym(&e.xkey, 0);
  unsigned int mods = e.xkey.state;

  // look up in our keyBinds
  printf("Sym: %s\n", XKeysymToString(sym));
  for(int i = 0; i < numKeyBinds; i++)
  {
    KeyBind bind = keyBinds[i];
    if(sym == bind.sym && mods == bind.mods)
    {
      // found the key press, execute the function
      printf("Executing: %s\n", bind.description);
      bind.handler();
      return;
    }

    /*printf("No Handler found for this bind\n");*/
  }
}

void grabAllKeys(void)
{
  // ungrabs keys for config reloading
  XUngrabKey(dsp, AnyKey, AnyModifier, root);
  for(int i = 0; i < numKeyBinds;i++)
  {
    KeyBind bind = keyBinds[i];
    KeyCode code = XKeysymToKeycode(dsp, bind.sym);
    if(code == 0)
    {
      printf("Invalid key code for key %lu\n", bind.sym);
    }

    XGrabKey(dsp, code, bind.mods, root, False, GrabModeAsync, GrabModeAsync);
    printf("Grabbed %s (keycode %d, mods %u)\n", 
           bind.description, code,
           bind.mods);
  }

  XSync(dsp, 0);
}

int main(void)
{
  dsp = XOpenDisplay(NULL); 
  if(dsp == NULL) panic("Dispay not found");

  root = DefaultRootWindow(dsp);

  // Creatign our simple test window
  /*Window testWindow = createSimpleWindow(100, 100, 200, 300);*/
  /*setWindowProperties(testWindow, "Testing testing 123");*/
  /*showWindow(testWindow);*/
  /*drawInWindow(testWindow);*/

  // Requests X server to send related events
  XSelectInput(dsp, root, 
               SubstructureNotifyMask | SubstructureRedirectMask | KeyPressMask | ButtonPressMask);
  XSync(dsp, 0); // Sync changes

  // Creating and defining the cursor
  Cursor cursor = XCreateFontCursor(dsp, XC_left_ptr);
  XDefineCursor(dsp, root, cursor);
  XSync(dsp, False);

  // Mouse Bindings
  XGrabButton(dsp, Button1, 0, root, 0, ButtonPressMask,
             GrabModeSync, GrabModeAsync, 0, 0); // left click

  // Grab all of our defined keybindings
  initKeyBinds();
  grabAllKeys();

  // generate the first workspace
  int ret;
  ret = addWorkSpace(1);
  printf("Creating workspace: %d\n", ret);
  activeWorkspaceId = 1;

  XEvent e;
  for(;;)
  {
    XNextEvent(dsp, &e);
    printf("Received event of type: %d\n", e.type);
    switch (e.type) 
    {
      case MapRequest:
        {
          handleMapRequest(e);
          break;
        }
      case DestroyNotify:
        {
          printf("Handling window destruction\n");
          handleDestroyNotify(e);
          break;
        }
      case ConfigureRequest:
        {
          handleConfigureRequest(e);
          break;
        }
      case ButtonPress:
        {
          XAllowEvents(dsp, ReplayPointer, CurrentTime);
          XSync(dsp, 0);
          Window mainWindow = e.xbutton.window;
          Window subWindow = e.xbutton.subwindow;
          /*printf("Pressed key in %lu window and %lu wubwindow\n", */
          /*       mainWindow, subWindow);*/
          /*puts("button pressed");*/
          break;
        }
      case KeyPress:
        {
          // don't need the XAllowEvents because it is async
          handleKeyPress(e);
          break;
        }
      default:
        /*puts("Unexpected event");*/
        break;
    }

    XSync(dsp, 0);
  }

  XCloseDisplay(dsp);

  return 0;
}
