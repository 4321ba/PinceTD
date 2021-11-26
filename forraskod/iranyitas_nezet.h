#ifndef IRANYITAS_NEZET_H
#define IRANYITAS_NEZET_H

#include <SDL2/SDL.h>

// egy nézet szokásos függvényei
void iranyitas_nezet_inicializacio(void);
void iranyitas_nezet_destrukcio(void);
void iranyitas_nezet_frissites(double delta);
void iranyitas_nezet_bemenet(SDL_Event esemeny);

#endif // IRANYITAS_NEZET_H
