#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "dicsoseglista_nezet.h"
#include "nezetek.h"
#include "vektor2e.h"
#include "dicsoseglista.h"

void dicsoseglista_nezet_inicializacio(void) {
    dicsoseglista_beolvas();
}

void dicsoseglista_nezet_destrukcio(void) {
}

void dicsoseglista_nezet_frissites(double delta) {
    (void)delta;
    static SDL_Color const szurke = {211, 191, 169, 255};
    dicsoseglista_kirajzol(v2e(4, 80), &szurke);
}

void dicsoseglista_nezet_bemenet(SDL_Event esemeny) {
    // ha egérgomb vagy enter vagy szóköz vagy escape, akkor visszalépünk
    if (esemeny.type == SDL_MOUSEBUTTONDOWN || (
        esemeny.type == SDL_KEYDOWN && (
            esemeny.key.keysym.sym == SDLK_RETURN ||
            esemeny.key.keysym.sym == SDLK_SPACE ||
            esemeny.key.keysym.sym == SDLK_ESCAPE
        )
    ))
        nezetvaltas(FOMENU_NEZET);
}
