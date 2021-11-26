#ifndef DICSOSEGLISTA_H
#define DICSOSEGLISTA_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "vektor2e.h"

// beolvassa a fájlból az adatokat
void dicsoseglista_beolvas(void);

// kirajzolja a memóriában levő adatokat, előtte dicsoseglista_beolvas meghívandó egyszer
void dicsoseglista_kirajzol(Vektor2e poz, SDL_Color const *szin);

// ezek előtt dicsoseglista_beolvast nem kell meghívni,
// mert ezeket nem képkockánként hívják meg
// sőt, helyettesítik a dicsoseglista_beolvas hívását
bool dicsoseglista_felkerul_e(int pontszam);
// a fájlba is kiírja az új listát:
void dicsoseglista_feltesz(int pontszam, char const *nev);

#endif // DICSOSEGLISTA_H
