#ifndef ELLENSEGEK_H
#define ELLENSEGEK_H

#include "vektor2.h"
#include "lovedekek.h"

typedef enum Ellensegfajta {
    LELKECSKE,
    KIS_OGRE,
    KIS_DEMON,
    NAGY_OGRE,
    NAGY_DEMON,
    EGYIK_ELLENSEG_SEM = -1,
} Ellensegfajta;

// letörli az összes ellenséget
void ellensegek_destrukcio(void);

// frissíti az ellenségek pozícióját, és kirajzolja őket
// levon életet, ha beért a célba
void ellensegek_frissites(double delta);

// leidéz egy ellenséget a kiindulóponthoz
void ellenseg_idezes(Ellensegfajta fajta);

// leidéz egy ellenséget, amin az álló animáció fog játszódni, és nem fog semmit sem csinálni (dísz)
void ellenseg_disz_idezes(Ellensegfajta fajta, Vektor2e csp);

// visszaadja a célhoz legközelebbi, de a körben még benne levő ellenség predikcióhoz szükséges adatait, illetve, hogy talált-e
// preferálja a lövedék által lelőhető ellenfeleket, de ha nem talál, akkor a nem lelőhetőt is visszaadja
bool ellenseg_celzando_keres(Vektor2d kozeppont, double sugar, Lovedekfajta lovedek, EllensegPredikcio *ep);

// igaz, ha talált a lövés
bool ellenseg_leloves(Vektor2d lovedek_poz, Lovedekfajta lovedek);

#endif // ELLENSEGEK_H
