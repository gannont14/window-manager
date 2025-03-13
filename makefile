all: build

SRC = window_manager.c window_utils.c key_binds.c bind_handlers.c workspace.c

build:
		gcc $(SRC) -o main -Wall -Wextra -std=c17 -lX11 -lm

br: 
		gcc $(SRC) -o main -Wall -Wextra -std=c17 -lX11 && ./main
