#include <stdbool.h>
#include "penz.h"
#include "tornyok.h"

enum {KEZDOPENZ = 15};

static int penz;

void penz_ujraallit(void) {
    penz = KEZDOPENZ;
}

void penz_hozzaad(int mennyiseg) {
    penz += mennyiseg;
}

bool penz_levon(int mennyiseg) {
    if (penz >= mennyiseg) {
        penz -= mennyiseg;
        return true;
    }
    return false;
}

int szerez_penz(void) {
    return penz;
}

int szerez_ar(Toronyfajta fajta) {
    static int const arak[3] = {
        10, // IJASZ
        30, // LOVAG
        50, // VARAZSLO
    };
    return arak[fajta];
}
