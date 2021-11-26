#include <stdio.h>
#include <stdbool.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "keprajzolo.h"
#include "vektor2.h"
#include "ablak.h"

static KepInformacio kep_informaciok[161+1]; // 161 kép van a csempekészletben,
// de biztos ami biztos akkor sem akarunk rossz pointert (túlindexelt tömböt)
// adni az fscanf-nek, amikor EOF-ot kéne beolvasnia

static SDL_Texture *kepek_textura;

// fájlkezelés a honlapról származik: https://infoc.eet.bme.hu/fajlkezeles/

// beolvassa a kep_informaciok-ba a koordinátákat a keplista.txt fájlból
static void keplista_beolvas(void) {
    FILE* am = fopen("tartozekok/keplista.txt", "r");
    if (am == NULL) {
        perror("Nem sikerült megnyitni a keplista.txt fájlt");
        exit(3);
    }
    int i = 0;
    int beolvasott_db = 0;
    while (beolvasott_db != EOF) {
        beolvasott_db = fscanf(am, "%s %d %d %d %d %d",
                                kep_informaciok[i].nev,
                               &kep_informaciok[i].kezdopont.x,
                               &kep_informaciok[i].kezdopont.y,
                               &kep_informaciok[i].nagysag.x,
                               &kep_informaciok[i].nagysag.y,
                               &kep_informaciok[i].ismetlesszam
                              );
        // az ismétlésszám nem mindig van ott, ezért ha az utolsót nem sikerült beolvasni,
        // akkor tudjuk, hogy nem volt ott
        if (beolvasott_db == 5)
            kep_informaciok[i].ismetlesszam = 1;
        // vannak üres sorok, ezért nem biztos, hogy minden beolvasás sikeres lesz
        if (beolvasott_db > 0)
            ++i;
    }
    fclose(am);
}

// betölti a textúrát
static void kepkeszlet_betolt(void) {
    kepek_textura = IMG_LoadTexture(szerez_megjelenito(), "tartozekok/kepkeszlet.png");
    if (kepek_textura == NULL) {
        fprintf(stderr, "Nem nyitható meg a kepkeszlet.png fájl: %s\n", IMG_GetError());
        exit(3);
    }
}

void keprajzolo_inicializacio(void) {
    keplista_beolvas();
    kepkeszlet_betolt();
}

void keprajzolo_destrukcio(void) {
    SDL_DestroyTexture(kepek_textura);
}

void kep_kirajzol_egesz(Kep kep, Vektor2e pozicio, int animacio_kepkocka) {
    KepInformacio info = kep_informaciok[kep];
    if (info.ismetlesszam <= animacio_kepkocka)
        fprintf(stderr, "A megadott animáció képkocka már nem ehhez a képhez tartozik!\n");
    SDL_Rect forras = { info.kezdopont.x + animacio_kepkocka*info.nagysag.x, info.kezdopont.y, info.nagysag.x, info.nagysag.y };
    // milyen koordinátára másoljuk:
    SDL_Rect cel = { pozicio.x, pozicio.y, info.nagysag.x, info.nagysag.y };
    SDL_RenderCopy(szerez_megjelenito(), kepek_textura, &forras, &cel);
}

void kep_kirajzol_lebegopontos(Kep kep, Vektor2d pozicio, int animacio_kepkocka, double szog, bool tukros) {
    KepInformacio info = kep_informaciok[kep];
    if (info.ismetlesszam <= animacio_kepkocka)
        fprintf(stderr, "A megadott animáció képkocka már nem ehhez a képhez tartozik!\n");
    SDL_Rect forras = { info.kezdopont.x + animacio_kepkocka*info.nagysag.x, info.kezdopont.y, info.nagysag.x, info.nagysag.y };
    // milyen koordinátára másoljuk:
    SDL_FRect cel = { pozicio.x, pozicio.y, info.nagysag.x, info.nagysag.y };
    SDL_RenderCopyExF(szerez_megjelenito(), kepek_textura, &forras, &cel, szog, NULL, (tukros ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
}

KepInformacio const *szerez_kepinformacio(Kep k) {
    return kep_informaciok+k;
}

void szoveg_kirajzol(char const *szoveg, Vektor2e poz, SDL_Color const *s, bool vizszintesen_kozepre_igazitott) {
    if (vizszintesen_kozepre_igazitott)
        poz.x -= strlen(szoveg) * KARAKTER / 2;
    stringRGBA(szerez_megjelenito(), poz.x, poz.y, szoveg, s->r, s->g, s->b, s->a);
}

