// játék felhasználói felülete (ui = ff)
// csak a felület kirajzolásáért felel, az események kezelését a jatek_nezet végzi
#include <stdbool.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "jatek_ff.h"
#include "ablak.h"
#include "vektor2e.h"
#include "keprajzolo.h"
#include "elet.h"
#include "penz.h"
#include "hullamok.h"
#include "tornyok.h"
#include "ellensegek.h"
#include "lovedekek.h"

static double const penz_animacio_sebesseg = 10.0; // képkocka/másodperc
static double penz_animacio;
static int penz_ennyiszer_animalando;

// az itt megadott pozíciók a díszek helyei a pályán,
// a pálya fájlban ','-k jelzik a helyüket (olyan padló, ahová a felhasználó nem tehet tornyot)
static int const disz_y = 14;

static int const ijasz_ar_x = 1;
static int const ijasz_x = 2;
static int const nyil_x = 3;
static int const lelkecske1_x = 4;

static int const lovag_ar_x = 6;
static int const lovag_x = 7;
static int const darda_x = 8;
static int const lelkecske2_x = 9;
static int const kis_ogre_x = 10;
static int const nagy_ogre_x = 11;

static int const varazslo_ar_x = 13;
static int const varazslo_x = 14;
static int const vizesuveg_x = 15;
static int const kis_demon_x = 16;
static int const nagy_demon_x = 17;


void jatek_ff_inicializacio(void) {
    penz_animacio = 0.0;
    penz_ennyiszer_animalando = 0;
    torony_letrehoz(IJASZ, v2e(ijasz_x, disz_y), true);
    lovedek_disz_letrehoz(NYIL, v2e(nyil_x, disz_y));
    torony_letrehoz(LOVAG, v2e(lovag_x, disz_y), true);
    lovedek_disz_letrehoz(DARDA, v2e(darda_x, disz_y));
    torony_letrehoz(VARAZSLO, v2e(varazslo_x, disz_y), true);
    lovedek_disz_letrehoz(VIZESUVEG, v2e(vizesuveg_x, disz_y));
}

void jatek_ffhez_uj_ellenseg(Ellensegfajta fajta) {
    switch (fajta) {
        case LELKECSKE:
            ellenseg_disz_idezes(LELKECSKE, v2e(lelkecske1_x, disz_y));
            ellenseg_disz_idezes(LELKECSKE, v2e(lelkecske2_x, disz_y));
            break;
        case KIS_OGRE:
            ellenseg_disz_idezes(KIS_OGRE, v2e(kis_ogre_x, disz_y));
            break;
        case NAGY_OGRE:
            ellenseg_disz_idezes(NAGY_OGRE, v2e(nagy_ogre_x, disz_y));
            break;
        case KIS_DEMON:
            ellenseg_disz_idezes(KIS_DEMON, v2e(kis_demon_x, disz_y));
            break;
        case NAGY_DEMON:
            ellenseg_disz_idezes(NAGY_DEMON, v2e(nagy_demon_x, disz_y));
            break;
        case EGYIK_ELLENSEG_SEM:
            break;
    }
}

void jatek_ff_frissites(double delta) {
    
    if (penz_ennyiszer_animalando > 0) {
        penz_animacio += penz_animacio_sebesseg * delta;
        int ismetlesszam = szerez_kepinformacio(COIN_ANIM)->ismetlesszam;
        if (penz_animacio >= ismetlesszam) {
            penz_animacio -= ismetlesszam;
            --penz_ennyiszer_animalando;
            // nem szeretnénk, hogy felgyülemljen az animálandóság
            if (penz_ennyiszer_animalando > 1)
                penz_ennyiszer_animalando = 1;
        }
    }
    
    static SDL_Color const arany = {250, 203, 62, 255};
    static SDL_Color const piros = {218, 78, 56, 255};
    static SDL_Color const szurke = {211, 191, 169, 255};
    
    // ebbe írjuk be az életet, a pénzt és a hullámot, 2^32 10-es szrszerben kb 10 karakter hosszú
    // tehát 15 elég kell, hogy legyen a számnak, +16 a szövegnek: %d. Hullám
    char szam_szovegkent[32];
    
    // pénz
    kep_kirajzol_egesz(COIN_ANIM, v2e(CSEMPEMERET, 2), penz_animacio);
    sprintf(szam_szovegkent, "%d", szerez_penz());
    szoveg_kirajzol(szam_szovegkent, v2e(CSEMPEMERET * 2 - 6, 3), &arany, false);
    
    // élet
    kep_kirajzol_egesz(UI_HEART_FULL, v2e(ABLAK_X - CSEMPEMERET*2 + 2, -2), 0);
    sprintf(szam_szovegkent, "%2d", szerez_elet());
    szoveg_kirajzol(szam_szovegkent, v2e(ABLAK_X - CSEMPEMERET * 3, 3), &piros, false);
    
    // hullámszám
    sprintf(szam_szovegkent, "%d. Hull\xA0m", szerez_hullamszam());
    szoveg_kirajzol(szam_szovegkent, v2e(ABLAK_X / 2, 3), &szurke, true);
    
    // mennyibe kerülnek a tornyok
    // a disz_y-adik sorban a csempe közepére igazítva
    int const ar_y = disz_y * CSEMPEMERET + (CSEMPEMERET - KARAKTER) / 2;
    sprintf(szam_szovegkent, "%2d", szerez_ar(IJASZ));
    szoveg_kirajzol(szam_szovegkent, v2e(ijasz_ar_x * CSEMPEMERET, ar_y), &arany, false);
    sprintf(szam_szovegkent, "%2d", szerez_ar(LOVAG));
    szoveg_kirajzol(szam_szovegkent, v2e(lovag_ar_x * CSEMPEMERET, ar_y), &arany, false);
    sprintf(szam_szovegkent, "%2d", szerez_ar(VARAZSLO));
    szoveg_kirajzol(szam_szovegkent, v2e(varazslo_ar_x * CSEMPEMERET, ar_y), &arany, false);
    
    // vissza a főmenübe gomb: koponya, mert azonnal, mentés nélkül lép ki
    kep_kirajzol_egesz(SKULL, v2e(ABLAK_X - CSEMPEMERET * 7/4, ABLAK_Y - CSEMPEMERET * 9/8), 0);
}

void penz_animal(void) {
    ++penz_ennyiszer_animalando;
}
