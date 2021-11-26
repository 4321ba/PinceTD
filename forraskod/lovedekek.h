#ifndef LOVEDEKEK_H
#define LOVEDEKEK_H

#include <stdbool.h>

#include "vektor2.h"

typedef enum Lovedekfajta {
    NYIL,
    DARDA,
    VIZESUVEG,
} Lovedekfajta;

// ide is illik, mert a lövedéknek kell ez a struktúra a predikcióhoz
// ha pedig az ellensegek.h-ba kerülne, akkor a fejlécfájlok egymást körbeincludeolnák, amit a fordító vmiért nem szeret
// egy jól irányított lövedék lövéséhez szükséges adatok
typedef struct EllensegPredikcio {
    // pozicio nagyjából redundáns mert megtett_utból ki lehetne számolni,
    // de ha már korábban úgyis ki van számolva, akkor ne kelljen ismét kiszámolni
    // illetve nem teljesen biztos, hogy a megtett_ut nem túl nagy (ekkor nem sikerülne a pozicio lekérdezés),
    // és akkor is legyen hová célozni
    Vektor2d pozicio;
    double megtett_ut;
    double sebesseg;
} EllensegPredikcio;

// letörli az összes lövedéket
void lovedekek_destrukcio(void);

// frissíti a lövedékek pozícióját, sebességét, kirajzolja őket, és teszteli, hogy lelőttek-e egy ellenséget
void lovedekek_frissites(double delta);

// az ellenség adatai a pozíció jóslásához kellenek, visszatér, hogy balra kellett-e lőni (torony forgatásához)
// mindenképp kilő egy lövedéket, akkor is, ha az valami miatt nem fogja az ellenséget eltalálni
// akkor is a lehető legközelebb lő
bool lovedek_celozva(Lovedekfajta fajta, Vektor2d kiindulas, EllensegPredikcio ep);

// a létrejövő dísz jobbra mutat
void lovedek_disz_letrehoz(Lovedekfajta fajta, Vektor2e csp);

#endif // LOVEDEKEK_H
