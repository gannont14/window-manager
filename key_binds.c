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
  unsigned int UserModKey = Mod4Mask;
  //  #################################
  //  KEYBINDINGS
  //  #################################

  //Terminal
  keyBinds[numKeyBinds].sym = XStringToKeysym("t");
  keyBinds[numKeyBinds].mods = UserModKey ;
  keyBinds[numKeyBinds].handler = launchTerminal;
  keyBinds[numKeyBinds].description = "launches a terminal";
  numKeyBinds++;

  //Firefox                                                     Doesn't seem to wrok well with XEPHYR session
  keyBinds[numKeyBinds].sym = XStringToKeysym("b");
  keyBinds[numKeyBinds].mods = UserModKey;
  keyBinds[numKeyBinds].handler = launchFirefox;
  keyBinds[numKeyBinds].description = "launches firefox";
  numKeyBinds++;

  // Cycle through windows keybinds
  // cycle right
  keyBinds[numKeyBinds].sym = XStringToKeysym("l");
  keyBinds[numKeyBinds].mods = UserModKey;
  keyBinds[numKeyBinds].handler = cycleWindowRight;
  keyBinds[numKeyBinds].description = "Cycles window the the right";
  numKeyBinds++;

  // cycle left
  keyBinds[numKeyBinds].sym = XStringToKeysym("h");
  keyBinds[numKeyBinds].mods = UserModKey;
  keyBinds[numKeyBinds].handler = cycleWindowLeft;
  keyBinds[numKeyBinds].description = "Cycles window the the left";
  numKeyBinds++;

  // cycle up
  keyBinds[numKeyBinds].sym = XStringToKeysym("k");
  keyBinds[numKeyBinds].mods = UserModKey;
  keyBinds[numKeyBinds].handler = cycleWindowUp;
  keyBinds[numKeyBinds].description = "Cycles window Up";
  numKeyBinds++;
  
  // cycle down
  keyBinds[numKeyBinds].sym = XStringToKeysym("j");
  keyBinds[numKeyBinds].mods = UserModKey;
  keyBinds[numKeyBinds].handler = cycleWindowDown;
  keyBinds[numKeyBinds].description = "Cycles window down";
  numKeyBinds++;

  printf("Num keybinds loaded: %d\n", numKeyBinds);
  return EXIT_SUCCESS;

}

void launchApplication(const char* command)
{
  XSync(dsp, False);

  pid_t pid = fork();

  if(pid < 0) fprintf(stderr, "Error Launching application: %s", command);
  else if (pid == 0) { // child proccess
    /*close(1);*/
    /*close(2);*/
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
