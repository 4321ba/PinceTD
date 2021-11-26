#!/bin/sh
x86_64-w64-mingw32-gcc forraskod/*.c -o PinceTD -IMinGW/include -LMinGW/lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_gfx -lSDL2_image -Wall -Wextra -std=c99 -pedantic
