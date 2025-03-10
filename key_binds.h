#ifndef KEY_BINDS_H
#define KEY_BINDS_H

#include <X11/X.h>

#define MAX_KEYBINDS 32

typedef void (*KeyHandlerFunc)(void);

typedef struct{
  KeySym sym;
  unsigned int mods;
  KeyHandlerFunc handler;
  char* description; // don't know if need
} KeyBind;


extern KeyBind keyBinds[];
extern int numKeyBinds;

int initKeyBinds(void);
void launchApplication(const char* command);

#endif // !KEY_BINDS_H
