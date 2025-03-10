#include "key_binds.h"
#include "bind_handlers.h"
#include <X11/X.h>
#include <X11/Xlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include "window_manager.h"

KeyBind keyBinds[MAX_KEYBINDS];
int numKeyBinds = 0;


int initKeyBinds(void)
{

  // create the key bindings here
  keyBinds[numKeyBinds].sym = XStringToKeysym("t");
  keyBinds[numKeyBinds].mods = ControlMask;
  keyBinds[numKeyBinds].handler = launchTerminal;
  keyBinds[numKeyBinds].description = "launches a terminal";
  numKeyBinds++;

  return EXIT_SUCCESS;

}

void launchApplication(const char* command)
{
  XSync(dsp, False);

  pid_t pid = fork();

  if(pid < 0) fprintf(stderr, "Error Launching application: %s", command);
  else if (pid == 0) { // child proccess
    close(1);
    close(2);
    // close display connection in child
    puts("Hello from child");
    if(dsp)
    {
      puts("Disply not closed, closing");
      /*XCloseDisplay(dsp);*/
      dsp = NULL;
    }

    puts("Display has been reset");

    setsid();
    puts("setsid");

    // Execute the command
    execl("/bin/sh", "sh", "-c", command, NULL);

    // failure, exit
    fprintf(stderr, "Failed to execute: %s\n", command);
    exit(EXIT_FAILURE);
  }
  else{
    printf("Successfully launched application: %s", command);
  }
}
