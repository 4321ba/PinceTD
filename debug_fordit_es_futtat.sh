#!/bin/sh
gcc forraskod/*.c -o PinceTD `sdl2-config --cflags --libs` -lSDL2_gfx -lSDL2_image -lSDL2_mixer -Wall -Wextra -std=c99 -pedantic -lm -g
gdb PinceTD
