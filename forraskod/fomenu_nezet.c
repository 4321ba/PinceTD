#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "fomenu_nezet.h"
#include "nezetek.h"
#include "ablak.h"

#include "vektor2e.h"
#include "ellensegek.h"
#include "tornyok.h"
#include "lovedekek.h"
#include "palya.h"
#include "elet.h"
#include "keprajzolo.h"
#include "hullamok.h"

static SDL_Rect const jatek_gomb = {12 * CSEMPEMERET, 8 * CSEMPEMERET, 7 * CSEMPEMERET, CSEMPEMERET};
static SDL_Rect const iranyitas_gomb = {12 * CSEMPEMERET, 9 * CSEMPEMERET, 7 * CSEMPEMERET, CSEMPEMERET};
static SDL_Rect const dicsoseglista_gomb = {12 * CSEMPEMERET, 10 * CSEMPEMERET, 7 * CSEMPEMERET, CSEMPEMERET};
static SDL_Rect const kilepes_gomb = {12 * CSEMPEMERET, 11 * CSEMPEMERET, 7 * CSEMPEMERET, CSEMPEMERET};
static SDL_Rect const k1234ab_gomb = {2 * CSEMPEMERET, 9 * CSEMPEMERET, 4 * CSEMPEMERET, CSEMPEMERET};
static SDL_Rect const g0x72_gomb = {2 * CSEMPEMERET, 11 * CSEMPEMERET, 4 * CSEMPEMERET, CSEMPEMERET};

// SDL_MOUSEMOTION-nél frissítjük, kell a gombok hátterének színezéséhez
// SDL_GetMouseState() sokkal nehezebben használható, mert SDL_RenderSetLogicalSize()-t nem veszi figyelembe
static SDL_Point eger_poz;

void fomenu_nezet_inicializacio(void) {
    hullamok_ujraallit(); // azért, hogy a háttérben bejövő ellenségek sebessége normál legyen (szerez_gyorsitas()==1 legyen)
    eger_poz = (SDL_Point){-1, -1};
    kezdoajto_allapotvaltas(true);
    torony_letrehoz(IJASZ, v2e(3, 5), false);
    torony_letrehoz(IJASZ, v2e(14, 6), false);
    torony_letrehoz(LOVAG, v2e(10, 6), false);
    torony_letrehoz(VARAZSLO, v2e(10, 3), false);
}

void fomenu_nezet_destrukcio(void) {
    ellensegek_destrukcio();
    tornyok_destrukcio();
    lovedekek_destrukcio();
}

// kirajzol egy szöveget egy területre, bekeretezi, és sötétít mögötte ha az egér benne van ( = gomb)
static void gomb_kirajzol(SDL_Rect const *terulet, char const *szoveg, SDL_Color const *szin, SDL_Color const *hatterszin) {
    SDL_Renderer *megjelenito = szerez_megjelenito();
    Vektor2e sarok1 = v2e(terulet->x + 1, terulet->y + 1);
    Vektor2e sarok2 = v2e(terulet->x + terulet->w - 1, terulet->y + terulet->h - 1);
    if (SDL_PointInRect(&eger_poz, terulet))
        boxRGBA(megjelenito, sarok1.x, sarok1.y, sarok2.x, sarok2.y, hatterszin->r, hatterszin->g, hatterszin->b, hatterszin->a);
    rectangleRGBA(megjelenito, sarok1.x, sarok1.y, sarok2.x, sarok2.y, szin->r, szin->g, szin->b, szin->a);
    // középre igazítva függőlegesen is
    szoveg_kirajzol(szoveg, v2e(terulet->x + terulet->w / 2, terulet->y + (terulet->h - KARAKTER) / 2), szin, true);
}

void fomenu_nezet_frissites(double delta) {
    // 3 másodpercenként idézzünk egy ellenséget
    static double const idozites = 3.0;
    static double eltelt_ido = 0.0;
    eltelt_ido += delta;
    if (eltelt_ido >= idozites) {
        eltelt_ido -= idozites;
        // véletlenszerű ellenség az 5 közül
        ellenseg_idezes(rand() % 5);
    }
    
    elet_ujraallit(); // hogy biztosan ne haljon meg
    palya_kirajzol();
    ellensegek_frissites(delta);
    tornyok_frissites(delta);
    lovedekek_frissites(delta);
    
    static SDL_Color const hatterszin = {17, 17, 17, 128};
    static SDL_Color const sotetites = {17, 17, 17, 192};
    static SDL_Color const szurke = {211, 191, 169, 255};
    // szürkíteni a háttértevékenységet:
    boxRGBA(szerez_megjelenito(), 0, 0, ABLAK_X, ABLAK_Y, hatterszin.r, hatterszin.g, hatterszin.b, hatterszin.a);
    
    szoveg_kirajzol("PinceTD", v2e(ABLAK_X / 2, 3), &szurke, true);
    int y_poz = 8 * CSEMPEMERET + (CSEMPEMERET - KARAKTER) / 2;
    szoveg_kirajzol("K\x82sz\xa1tette", v2e(4 * CSEMPEMERET, y_poz), &szurke, true);
    szoveg_kirajzol("Grafika", v2e(4 * CSEMPEMERET, y_poz + 2 * CSEMPEMERET), &szurke, true);
    
    gomb_kirajzol(&jatek_gomb, "\x8cj j\xa0t\x82k", &szurke, &sotetites);
    gomb_kirajzol(&iranyitas_gomb, "Ir\xa0ny\xa1t\xa0s", &szurke, &sotetites);
    gomb_kirajzol(&dicsoseglista_gomb, "Dics\x93s\x82glista", &szurke, &sotetites);
    gomb_kirajzol(&kilepes_gomb, "Kil\x82p\x82s", &szurke, &sotetites);
    gomb_kirajzol(&k1234ab_gomb, "1234ab", &szurke, &sotetites);
    gomb_kirajzol(&g0x72_gomb, "0x72", &szurke, &sotetites);
}

// megnyit egy weblapot, vagy kiírja stdoutra a linket
static void weblap_megnyit(char const *webcim) {
    // SDL_OpenURL-t csak 2.0.14-ben adták hozzá, ami elég friss és nem szeretném,
    // hogy csak amiatt ne lehessen fordítani kicsit régebbi környezetekben
#if SDL_VERSION_ATLEAST(2, 0, 14)
    if (SDL_OpenURL(webcim) != 0)
        fprintf(stderr, "Nem nyitható meg a weblap \"%s\": %s\n", webcim, SDL_GetError());
#else // sdl verzió < 2.0.14
    printf("Weblap: %s\n", webcim);
#endif // SDL_VERSION_ATLEAST
}

void fomenu_nezet_bemenet(SDL_Event esemeny) {
    switch (esemeny.type) {
        case SDL_MOUSEMOTION:
            eger_poz.x = esemeny.motion.x;
            eger_poz.y = esemeny.motion.y;
            break;
        case SDL_MOUSEBUTTONDOWN:
            ;
            SDL_Point kattintas_poz = {esemeny.button.x, esemeny.button.y};
            if (SDL_PointInRect(&kattintas_poz, &jatek_gomb))
                nezetvaltas(JATEK_NEZET);
            else if (SDL_PointInRect(&kattintas_poz, &iranyitas_gomb))
                nezetvaltas(IRANYITAS_NEZET);
            else if (SDL_PointInRect(&kattintas_poz, &dicsoseglista_gomb))
                nezetvaltas(DICSOSEGLISTA_NEZET);
            else if (SDL_PointInRect(&kattintas_poz, &kilepes_gomb)) {
                SDL_Event es;
                es.type = SDL_QUIT;
                SDL_PushEvent(&es);
            } else if (SDL_PointInRect(&kattintas_poz, &k1234ab_gomb))
                weblap_megnyit("https://github.com/4321ba/PinceTD");
            else if (SDL_PointInRect(&kattintas_poz, &g0x72_gomb))
                weblap_megnyit("https://0x72.itch.io/dungeontileset-ii");
            break;
    }
}
