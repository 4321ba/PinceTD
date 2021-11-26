#ifndef VEKTOR2E_H
#define VEKTOR2E_H

// e mint egész (int)
typedef struct Vektor2e {
    int x, y;
} Vektor2e;

Vektor2e v2e(int x, int y);
Vektor2e v2e_nyujt(Vektor2e v, int skalar);
Vektor2e v2e_osszead(Vektor2e v1, Vektor2e v2);
Vektor2e v2e_kivon(Vektor2e v1, Vektor2e v2);

// az irány -1 (90° forgatás, óramutatóval ellenkező),
// vagy 1 (90° forgatás, óramutatóval megegyező)
Vektor2e v2e_normalas(Vektor2e v, int irany);

#endif // VEKTOR2E_H
