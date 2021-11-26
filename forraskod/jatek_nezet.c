#include <SDL2/SDL.h>

#include "jatek_nezet.h"
#include "nezetek.h"
#include "ablak.h"
#include "vektor2.h"

#include "palya.h"
#include "hullamok.h"
#include "ellensegek.h"
#include "tornyok.h"
#include "lovedekek.h"
#include "jatek_ff.h"
#include "penz.h"
#include "elet.h"
#include "keprajzolo.h"
#include "idozites.h"

static Toronyfajta lerakando_torony;
static bool gyorsitott;
static Vektor2e eger_csp;

void jatek_nezet_inicializacio(void) {
    lerakando_torony = EGYIK_TORONY_SEM;
    gyorsitott = false;
    kezdoajto_allapotvaltas(false);
    jatek_ff_inicializacio();
    penz_ujraallit();
    elet_ujraallit();
    hullamok_ujraallit();
}

void jatek_nezet_destrukcio(void) {
    ellensegek_destrukcio();
    tornyok_destrukcio();
    lovedekek_destrukcio();
    szimulacio_sebesseg_allitas(NORMAL_SEBESSEG);
}


void jatek_nezet_frissites(double delta) {
    palya_kirajzol();

    hullamok_frissites(delta);

    if (lerakando_torony != EGYIK_TORONY_SEM)
        tornyok_latotav_kirajzol(eger_csp, lerakando_torony);
    ellensegek_frissites(delta);
    tornyok_frissites(delta);
    lovedekek_frissites(delta);
    jatek_ff_frissites(delta);
}

// balgomb bemenet feldolgozás
static void eger_balkatt_bemenet(Vektor2e poz) {
    // ha a jobb alsó csempe területére kattint, akkor megnyitjuk a játék végét
    if (poz.x >= ABLAK_X - 2*CSEMPEMERET && poz.y >= ABLAK_Y - CSEMPEMERET)
        nezetvaltas(JATEKVEGE_NEZET);
    
    // ha alulra kattint, akkor kiválaszthatja valamelyik tornyot lerakásra
    else if (poz.y > ABLAK_Y - 2*CSEMPEMERET) {
        Toronyfajta kattintott_torony = EGYIK_TORONY_SEM;
        if (poz.x < 6*CSEMPEMERET)
            kattintott_torony = IJASZ;
        else if (poz.x < 13*CSEMPEMERET)
            kattintott_torony = LOVAG;
        else
            kattintott_torony = VARAZSLO;
        
        // ha olyanra kattintunk ami már ki van jelölve, akkor tűnjön el a kijelölés
        lerakando_torony = (kattintott_torony!=lerakando_torony) ? kattintott_torony : EGYIK_TORONY_SEM;
        
    }
    
    // különben megpróbáljuk lerakni a tornyot
    else if (lerakando_torony != EGYIK_TORONY_SEM) {
        Vektor2e csp = csempepozza(v2d_v2ebol(poz));
        if (csempe_toronykompatibilis_e(csp) && latotav_szabad_e(csp, lerakando_torony))
            if (penz_levon(szerez_ar(lerakando_torony)))
                torony_letrehoz(lerakando_torony, csp, false);
    }
}

// billentyű bemenet feldolgozás
static void billentyuzet_bemenet(SDL_Keycode billentyu) {
    enum {SEMMI_VALASZTAS = -2};
    Toronyfajta kattintott_torony = SEMMI_VALASZTAS;
    switch (billentyu) {
        case SDLK_1:
            kattintott_torony = IJASZ;
            break;
        case SDLK_2:
            kattintott_torony = LOVAG;
            break;
        case SDLK_3:
            kattintott_torony = VARAZSLO;
            break;
        case SDLK_ESCAPE:
            kattintott_torony = EGYIK_TORONY_SEM;
            break;
        case SDLK_SPACE:
            hullam_indit();
            break;
        case SDLK_TAB:
            gyorsitott = !gyorsitott;
            szimulacio_sebesseg_allitas(gyorsitott ? GYORSITOTT_SEBESSEG : NORMAL_SEBESSEG);
            break;
    }
    if (kattintott_torony != (Toronyfajta)SEMMI_VALASZTAS)
        // ha olyanra kattintunk ami már ki van jelölve, akkor tűnjön el a kijelölés
        lerakando_torony = (kattintott_torony!=lerakando_torony) ? kattintott_torony : EGYIK_TORONY_SEM;
}

void jatek_nezet_bemenet(SDL_Event esemeny) {
    switch (esemeny.type) {
        case SDL_MOUSEBUTTONDOWN:
            ;
            Vektor2e egerkatt_poz = v2e(esemeny.button.x, esemeny.button.y);
            switch (esemeny.button.button) {
                case SDL_BUTTON_LEFT:
                    eger_balkatt_bemenet(egerkatt_poz);
                    break;
                case SDL_BUTTON_RIGHT:
                    ;
                    Toronyfajta letorolt = torony_letorol(csempepozza(v2d_v2ebol(egerkatt_poz)));
                    if (letorolt != EGYIK_TORONY_SEM) {
                        penz_hozzaad(szerez_ar(letorolt) * VISSZAKAPOTT_PENZ + 0.5); // kerekítés miatt +0.5
                        penz_animal();
                    }
                    break;
                case SDL_BUTTON_MIDDLE:
                    hullam_indit();
            }
            
            break;
        case SDL_MOUSEMOTION:
            eger_csp = v2e(esemeny.motion.x / CSEMPEMERET, esemeny.motion.y / CSEMPEMERET);
            break;
        case SDL_KEYDOWN:
            billentyuzet_bemenet(esemeny.key.keysym.sym);
            break;
    }
}
