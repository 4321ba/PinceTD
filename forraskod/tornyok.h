#ifndef TORNYOK_H
#define TORNYOK_H

#include "vektor2e.h"

typedef enum Toronyfajta {
    IJASZ,
    LOVAG,
    VARAZSLO,
    EGYIK_TORONY_SEM = -1,
} Toronyfajta;

// letörli az összes tornyot
void tornyok_destrukcio(void);

// kirajzolja a tornyokat, és megpróbál velük lőni
void tornyok_frissites(double delta);

// meghívás előtt tesztelni kell, hogy a hely jó-e!
void torony_letrehoz(Toronyfajta fajta, Vektor2e csp, bool disz);

// visszatér, hogy mit törölt le, EGYIK_TORONY_SEM, ha semmit sem törölt
Toronyfajta torony_letorol(Vektor2e csp);

// visszaadja, hogy a lerakandó torony megfelel-e a látótáv követelménynek
// azaz nem lesz-e olyan torony, ami látja a másikat
bool latotav_szabad_e(Vektor2e csp, Toronyfajta fajta);

// kirajzol az összes torony köré 1-1 karikát a látótávolságukkal,
// még az összes tornyon kívül extrában kirajzol egy karikát és tornyot is,
// hasznos új torony lerakáskor az egér pozícióhoz rajzoláshoz
// az új karikát az alapján színezi, hogy a torony lerakható-e
void tornyok_latotav_kirajzol(Vektor2e uj_csp, Toronyfajta uj_toronyfajta);

#endif // TORNYOK_H
