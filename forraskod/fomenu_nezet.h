#ifndef FOMENU_NEZET_H
#define FOMENU_NEZET_H

#include <SDL2/SDL.h>

// egy nézet szokásos függvényei
void fomenu_nezet_inicializacio(void);
void fomenu_nezet_destrukcio(void);
void fomenu_nezet_frissites(double delta);
void fomenu_nezet_bemenet(SDL_Event esemeny);

#endif // FOMENU_NEZET_H
