#ifndef PALYA_H
#define PALYA_H

#include <stdbool.h>
#include "vektor2.h"

// beolvassa a fájlból a pálya adatait, és kitalálja az útvonalat
void palya_inicializacio(void);

// kirajzolja a pályát
void palya_kirajzol(void);

// a kezdőpontban levő ajtót kinyitja (igaz), vagy becsukja (hamis)
void kezdoajto_allapotvaltas(bool nyitva);

// visszaadja, hogy a csempe pozícióra lerakható-e torony
bool csempe_toronykompatibilis_e(Vektor2e csp);

// hamist ad vissza, ha az út végére ért, igazat ha nem
// hamis esetén a kapott mutatót nem változtatja,
// igaz esetén beleírja az új pozíciót
bool pozicio_megtett_utbol(double megtett_ut, Vektor2d *pozicio);

#endif // PALYA_H
