#include <SDL2/SDL.h>

#include "nezetek.h"
#include "fomenu_nezet.h"
#include "jatek_nezet.h"
#include "jatekvege_nezet.h"
#include "iranyitas_nezet.h"
#include "dicsoseglista_nezet.h"

static Nezet aktualis_nezet = EGYIK_NEZET_SEM;
static Nezet kovetkezo_nezet = EGYIK_NEZET_SEM;

// meghívja n inicializációját
static void nezetinicializacio(Nezet n) {
    switch (n) {
        case FOMENU_NEZET:
            fomenu_nezet_inicializacio();
            break;
        case JATEK_NEZET:
            jatek_nezet_inicializacio();
            break;
        case JATEKVEGE_NEZET:
            jatekvege_nezet_inicializacio();
            break;
        case IRANYITAS_NEZET:
            iranyitas_nezet_inicializacio();
            break;
        case DICSOSEGLISTA_NEZET:
            dicsoseglista_nezet_inicializacio();
            break;
        case EGYIK_NEZET_SEM:
            break;
    }
}

// meghívja n destrukcióját
static void nezetdestrukcio(Nezet n) {
    switch (n) {
        case FOMENU_NEZET:
            fomenu_nezet_destrukcio();
            break;
        case JATEK_NEZET:
            jatek_nezet_destrukcio();
            break;
        case JATEKVEGE_NEZET:
            jatekvege_nezet_destrukcio();
            break;
        case IRANYITAS_NEZET:
            iranyitas_nezet_destrukcio();
            break;
        case DICSOSEGLISTA_NEZET:
            dicsoseglista_nezet_destrukcio();
            break;
        case EGYIK_NEZET_SEM:
            break;
    }
}

void nezetvaltas(Nezet uj) {
    kovetkezo_nezet = uj;
}

void nezetek_frissites(double delta) {
    // ezt a nézetváltó rendszert meg lehetett volna úgy is oldani, hogy a *_frissites és a *_bemenet
    // visszatérési értéke a megváltozott nézet, de így nem kell csomó függvénynek visszatérési értéket adni
    // ami egyébként elképzelhető, hogy szebb megvalósítás lenne,
    // viszont ez a megvalósítás így általánosabb, kevésbé specifikus,
    // mivel frissitesből és bemenetből is egyformán működik
    if (aktualis_nezet != kovetkezo_nezet) {
        nezetdestrukcio(aktualis_nezet);
        nezetinicializacio(kovetkezo_nezet);
        aktualis_nezet = kovetkezo_nezet;
    }
    switch (aktualis_nezet) {
        case FOMENU_NEZET:
            fomenu_nezet_frissites(delta);
            break;
        case JATEK_NEZET:
            jatek_nezet_frissites(delta);
            break;
        case JATEKVEGE_NEZET:
            jatekvege_nezet_frissites(delta);
            break;
        case IRANYITAS_NEZET:
            iranyitas_nezet_frissites(delta);
            break;
        case DICSOSEGLISTA_NEZET:
            dicsoseglista_nezet_frissites(delta);
            break;
        case EGYIK_NEZET_SEM:
            break;
    }
}

void nezetek_bemenet(SDL_Event esemeny) {
    switch (aktualis_nezet) {
        case FOMENU_NEZET:
            fomenu_nezet_bemenet(esemeny);
            break;
        case JATEK_NEZET:
            jatek_nezet_bemenet(esemeny);
            break;
        case JATEKVEGE_NEZET:
            jatekvege_nezet_bemenet(esemeny);
            break;
        case IRANYITAS_NEZET:
            iranyitas_nezet_bemenet(esemeny);
            break;
        case DICSOSEGLISTA_NEZET:
            dicsoseglista_nezet_bemenet(esemeny);;
            break;
        case EGYIK_NEZET_SEM:
            break;
    }
}
