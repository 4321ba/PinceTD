#include <stdbool.h>
#include "elet.h"
#include "nezetek.h"

enum {MAX_ELET = 32};

static int elet;

void elet_ujraallit(void) {
    elet = MAX_ELET;
}

void elet_levon(int mennyiseg) {
    if (elet > mennyiseg) {
        elet -= mennyiseg;
        return;
    }
    elet = 0;
    nezetvaltas(JATEKVEGE_NEZET);
    return;
}

int szerez_elet(void) {
    return elet;
}
