#ifndef DICSOSEGLISTA_NEZET_H
#define DICSOSEGLISTA_NEZET_H

#include <SDL2/SDL.h>

// egy nézet szokásos függvényei
void dicsoseglista_nezet_inicializacio(void);
void dicsoseglista_nezet_destrukcio(void);
void dicsoseglista_nezet_frissites(double delta);
void dicsoseglista_nezet_bemenet(SDL_Event esemeny);

#endif // DICSOSEGLISTA_NEZET_H
