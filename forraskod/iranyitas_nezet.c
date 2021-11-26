#include <SDL2/SDL.h>

#include "iranyitas_nezet.h"
#include "vektor2e.h"
#include "nezetek.h"
#include "keprajzolo.h"

void iranyitas_nezet_inicializacio(void) {
}

void iranyitas_nezet_destrukcio(void) {
}

static char const *kiirando[] = {
    "Ir\xa0ny\xa1t\xa0s",
    "",
    "      \x10 Bal eg\x82rgomb:",
    "  \x10 Kopony\xa0ra kattintva:",
    "Vissza a f\x93men\x81\x62\x65, nincs ment\x82s!",
    "  \x10 Alulra kattintva:",
    "A veend\x93 torony kiv\xa0laszt\xa0sa",
    "  \x10 Ezut\xa0n a p\xa0ly\xa0ra:",
    "A kiv\xa0lasztott torony lerak\xa0sa",
    "  \x10 \x8cjra alulra, vagy Escape:",
    "Torony kiv\xa0laszt\xa0s t\x94rl\x82se",
    "",
    "      \x10 Jobb eg\x82rgomb:",
    "Torony t\x94rl\x82se",
    "",
    "      \x10 K\x94z\x82ps\x93 eg\x82rgomb, vagy Sz\xa2k\x94z:",
    "K\x94vetkez\x93 hull\xa0m ind\xa1t\xa0sa",
    "",
    "      \x10 1, 2, 3 gombok:",
    "A veend\x93 torony kiv\xa0laszt\xa0sa",
    "",
    "      \x10 Tabul\xa0tor:",
    "Gyors\xa1t\xa0s \x82s visszalass\xa1t\xa0s",
    NULL
};

void iranyitas_nezet_frissites(double delta) {
    (void)delta;
    static SDL_Color const szurke = {211, 191, 169, 255};
    for (size_t i = 0; kiirando[i] != NULL; ++i)
        szoveg_kirajzol(kiirando[i], v2e(KARAKTER / 2, KARAKTER / 2 + i * KARAKTER * 5/4), &szurke, false);
}

void iranyitas_nezet_bemenet(SDL_Event esemeny) {
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
