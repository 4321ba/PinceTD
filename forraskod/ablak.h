#ifndef ABLAK_H
#define ABLAK_H

#include <SDL2/SDL.h>

// ez az a méret, ahogy a program látja az ablakot
enum { ABLAK_X = 320, ABLAK_Y = 240 };

// a létrehozandó megjelenítő típusai, fs: függőleges szinkronizáció
typedef enum MegjTipus {
    SZOFTVER_MEGJ,
    HARDVER_MEGJ_FS_NELKUL,
    HARDVER_MEGJ_FSSEL,
} MegjTipus;

// megnyitják, illetve bezárják az ablakot és a megjelenítőt
// inicializáció kiírja stdoutra a használt megjelenítés fajtáját
void sdl_inicializacio(int szelesseg, int magassag, MegjTipus megjelenites);
void sdl_destrukcio(void);
// visszaadja a megjelenítőt, hogy ne kelljen az összes függvény fejlécében átadni
SDL_Renderer *szerez_megjelenito(void);

// az eredeti ablakcímhez hozzáfűz valamit (pl.: PinceTD -> PinceTD - kutyafüle)
// visszaírja eredetire, ha hozzafuzendo==NULL
void beallit_ablakcim(char *hozzafuzendo);

#endif // ABLAK_H
