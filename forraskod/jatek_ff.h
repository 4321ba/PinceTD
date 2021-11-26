// játék felhasználói felülete (ui = ff)
#ifndef JATEK_FF_H
#define JATEK_FF_H

#include "ellensegek.h"

// leidézi az ffhez tartozó díszeket
// destrukciójukról az adott modul tud gondoskodni
// ellenségnél az ellenseg.c, stb.
void jatek_ff_inicializacio(void);

// kirajzol egy képet
void jatek_ff_frissites(double delta);

// forgat egy fél fordulatot a pénzérmén az elkövetkezendő
// kb. fél másodperc folyamán
void penz_animal(void);

// akkor legyen meghívva, ha új ellenség lépett a pályára,
// akit alulra ki kellene rajzolni, hogy a felhasználó tudja,
// hogy melyik torony tudja lelőni
// egy ellenségre csak egyszer hívandó meg!
void jatek_ffhez_uj_ellenseg(Ellensegfajta fajta);

#endif // JATEK_FF_H
