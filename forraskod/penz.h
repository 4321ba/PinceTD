#ifndef PENZ_H
#define PENZ_H
#include <stdbool.h>
#include "tornyok.h"

// egy torony eladásakor a pénz ennyiszeresét kell visszakapni
#define VISSZAKAPOTT_PENZ 0.8

// visszaállítja a kezdőpénzt
void penz_ujraallit(void);

// növeli a pénzt
void penz_hozzaad(int mennyiseg);

// visszaadja, hogy megtörtént-e a levonás, azaz volt-e elég pénz
bool penz_levon(int mennyiseg);

// visszaadja, hogy mennyi pénz van jelenleg
int szerez_penz(void);

// visszaadja a torony árát
int szerez_ar(Toronyfajta fajta);

#endif // PENZ_H
