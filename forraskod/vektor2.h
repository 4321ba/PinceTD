#ifndef VEKTOR2_H
#define VEKTOR2_H

#include "vektor2e.h"
#include "vektor2d.h"

// konverzió a típusok között
Vektor2e v2e_v2dbol(Vektor2d v);
Vektor2d v2d_v2ebol(Vektor2e v);

// visszaadja, hogy v pixel pozíció melyik csempe pozícióban van
Vektor2e csempepozza(Vektor2d v);

// visszaadja a csempe pozíció csempéjének a közepét pixel pozícióban
Vektor2d csempe_kozepe(Vektor2e csp);

#endif // VEKTOR2_H
