#ifndef JATEK_NEZET_H
#define JATEK_NEZET_H

#include <SDL2/SDL.h>

// egy nézet szokásos függvényei
void jatek_nezet_inicializacio(void);
void jatek_nezet_destrukcio(void);
void jatek_nezet_frissites(double delta);
void jatek_nezet_bemenet(SDL_Event esemeny);

#endif // JATEK_NEZET_H
