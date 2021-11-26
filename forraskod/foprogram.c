#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "modositott_szovegtipus.h"
#include "ablak.h"
#include "keprajzolo.h"
#include "palya.h"
#include "hullamok.h"
#include "nezetek.h"
#include "idozites.h"

// kezeli a parancssori argumentumokat, inicializál, destruktál sok mindent, illetve itt található a főciklus
// az időzítést is ő kezeli az idozites.c segítségével
int main(int argc, char *argv[]) {
    MegjTipus megjelenites = HARDVER_MEGJ_FSSEL;
    bool kps_kijelzes = true;
    if (argc > 1) {
        char const *opciok = argv[1]+1; // +1 mivel a kötőjel nem érdekel minket
        // ha 2 vagy több argumentumot írtak be, akkor nem tudják, hogy milyen formátumban kell megadni
        // az argumentumokat, tehát írjuk ki nekik a segítséget
        // hasonlóképp ha nem '-'szal kezdték, vagy több '-'t írtak, vagy segítséget kérnek
        if (argc > 2 || argv[1][0] != '-' || strchr(opciok, '-') != NULL || strchr(opciok, 'h') != NULL) {
            printf( "Használat:\n"
                    "%s -opciok\n"
                    "ahol a következő opciók lehetségesek:\n"
                    "-h: segítség\n"
                    "-s: szoftveres megjelenítés\n"
                    "-v: függőleges szinkronizáció (vsync) nélküli hardveres megjelenítés\n"
                    "-f: kép per másodperc (fps) kijelzésének kikapcsolása az ablak címében\n"
                    "a kapcsolók/opciók csak az első argumentumként érvényesek, \"%s -abcd\" formátumban\n",
                    argv[0], argv[0]
                  );
            return -1;
        }
        if (strchr(opciok, 's') != NULL)
            megjelenites = SZOFTVER_MEGJ;
        else if (strchr(opciok, 'v') != NULL)
            megjelenites = HARDVER_MEGJ_FS_NELKUL;
        if (strchr(opciok, 'f') != NULL)
            kps_kijelzes = false;
    }
    
    srand(time(NULL));
    // beállítjuk, hogy a saját, modositott_szovegtipus.h-ban definiált,
    // összes ékezetes karaktert tartalmazó szövegtípust használja a stringRGBA
    gfxPrimitivesSetFont(modositott_szovegtipus_adat, 8, 8);
    
    sdl_inicializacio(ABLAK_X*2, ABLAK_Y*2, megjelenites);
    keprajzolo_inicializacio();
    palya_inicializacio();
    hullamok_inicializacio();
    
    nezetvaltas(FOMENU_NEZET);
    // nézetinicializáció csak az első frissítésnél
    
    // köszönet ennek az SDL konzinak az időzítésért és a főciklus felépítéséért:
    // https://www.youtube.com/watch?v=u__Z_OrgJ0U
    bool kilepes = false;
    while (!kilepes) {
        
        // események feldolgozása minden kép kirajzolás előtt
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                kilepes = true;
            else
                nezetek_bemenet(event);
            }
        
        Uint32 eltelt_ido = szerez_eltelt_ido();
        if (kps_kijelzes)
            kps_egy_kep_letrejott(eltelt_ido);
        
        // letörlünk mindent, és újrarajzoljuk az egészet
        // amilyen gyorsan csak tudjuk
        // illetve hardveres megjelenítésnél valahol a renderpresent környékén
        // amúgy is vár a vsyncre
        SDL_Renderer *megjelenito = szerez_megjelenito();
        static SDL_Color const hatterszin = {17, 17, 17, 255};
        SDL_SetRenderDrawColor(megjelenito, hatterszin.r, hatterszin.g, hatterszin.b, hatterszin.a);
        SDL_RenderClear(megjelenito);
        nezetek_frissites(eltelt_ido * szerez_szimulacio_sebesseg() / 1000.0);
        SDL_RenderPresent(megjelenito);
    }
    
    nezetvaltas(EGYIK_NEZET_SEM);
    nezetek_frissites(0.0); // hogy megtörténjen a nézet destrukció
    
    hullamok_destrukcio();
    keprajzolo_destrukcio();
    sdl_destrukcio();

    return 0;
}

