#include <stdlib.h>
#include <string.h>
#include <SDL2/SDL.h>

#include "ablak.h"
#include "mallokacio.h"

static SDL_Renderer *megjelenito;
static SDL_Window *ablak;
static char const eredeti_cim[] = "PinceTD";

// az itteni és a többi sdl-kezelő kód jelentős része a honlapról származik: https://infoc.eet.bme.hu/sdl/

// get_renderer csak magyarul
SDL_Renderer *szerez_megjelenito(void) {
    return megjelenito;
}

// ablakadabra! - ablak megnyitása
void sdl_inicializacio(int szelesseg, int magassag, MegjTipus megjelenites) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Nem indítható az SDL: %s\n", SDL_GetError());
        exit(1);
    }
    ablak = SDL_CreateWindow(eredeti_cim, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, szelesseg, magassag, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
    if (ablak == NULL) {
        fprintf(stderr, "Nem hozható létre az ablak: %s\n", SDL_GetError());
        exit(1);
    }
    Uint32 kapcsolok = 0;
    switch (megjelenites) {
        case SZOFTVER_MEGJ:
            kapcsolok = SDL_RENDERER_SOFTWARE;
            break;
        case HARDVER_MEGJ_FS_NELKUL:
            kapcsolok = SDL_RENDERER_ACCELERATED;
            break;
        case HARDVER_MEGJ_FSSEL:
            kapcsolok = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
            break;
    }
    megjelenito = SDL_CreateRenderer(ablak, -1, kapcsolok);
    if (megjelenito == NULL) {
        fprintf(stderr, "Nem hozható létre a megjelenítő: %s\n", SDL_GetError());
        exit(1);
    }
    
    SDL_RendererInfo info;
    if (SDL_GetRendererInfo(megjelenito, &info) == 0)
        printf("Megjelenítés: %s\n", info.name);
    
    // ennek a beállításával a programnak csak egyféle felbontással kell tudnia dolgozni
    // és az SDL fogja felnagyítani az ablakot
    SDL_RenderSetLogicalSize(megjelenito, ABLAK_X, ABLAK_Y);
    // viszont a csempék között keskeny csíkok / lukak lesznek, ha nem egésszer nagyítja fel
    SDL_RenderSetIntegerScale(megjelenito, SDL_TRUE);
}

void sdl_destrukcio(void) {
    SDL_DestroyRenderer(megjelenito);
    SDL_DestroyWindow(ablak);
    SDL_Quit();
}

void beallit_ablakcim(char *hozzafuzendo) {
    if (hozzafuzendo == NULL) {
        SDL_SetWindowTitle(ablak, eredeti_cim);
        return;
    }
    static char const koztes[] = " - ";
    int uj_cim_hossz = strlen(eredeti_cim) + strlen(koztes) + strlen(hozzafuzendo) + 1;
    char *uj_cim = mallokal(uj_cim_hossz * sizeof(char));
    sprintf(uj_cim, "%s%s%s", eredeti_cim, koztes, hozzafuzendo);
    SDL_SetWindowTitle(ablak, uj_cim);
    free(uj_cim);
}
