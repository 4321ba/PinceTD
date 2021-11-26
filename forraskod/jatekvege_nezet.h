#ifndef JATEKVEGE_NEZET_H
#define JATEKVEGE_NEZET_H

#include <SDL2/SDL.h>

// egy nézet szokásos függvényei
void jatekvege_nezet_inicializacio(void);
void jatekvege_nezet_destrukcio(void);
void jatekvege_nezet_frissites(double delta);
void jatekvege_nezet_bemenet(SDL_Event esemeny);

#endif // JATEKVEGE_NEZET_H
