#ifndef NEZETEK_H
#define NEZETEK_H

#include <SDL2/SDL.h>

typedef enum Nezet {
    FOMENU_NEZET,
    JATEK_NEZET,
    JATEKVEGE_NEZET,
    IRANYITAS_NEZET,
    DICSOSEGLISTA_NEZET,
    EGYIK_NEZET_SEM = -1,
} Nezet;

// frissíti az aktuális nézetet
void nezetek_frissites(double delta);

// átadja a bemenetet az aktuális nézetnek
void nezetek_bemenet(SDL_Event esemeny);

// berakja a sorba (queue-ol) egy nézetváltást
// a tényleges váltás a következő frissítés
// előtt fog megtörténni
void nezetvaltas(Nezet uj);

#endif // NEZETEK_H
