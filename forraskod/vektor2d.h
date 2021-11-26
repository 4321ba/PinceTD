#ifndef VEKTOR2D_H
#define VEKTOR2D_H

#include <stdbool.h>

// d mint dupla (double)
typedef struct Vektor2d {
    double x, y;
} Vektor2d;

// a szögek radiánban vannak,
// pozitív az óramutatóval megegyező(!) mert +xtengelytől +ytengely irányába forog
// jobbra = 0

// könnyítik a struktúra létrehozását
Vektor2d v2d(double x, double y);
Vektor2d v2d_polar(double hossz, double szog);

// szöget ad vissza
double v2d_irany(Vektor2d v);

// hosszt ad vissza
double v2d_hossznegyzet(Vektor2d v);
double v2d_hossz(Vektor2d v);

// vektorműveletek
Vektor2d v2d_nyujt(Vektor2d v, double skalar);
Vektor2d v2d_osszead(Vektor2d v1, Vektor2d v2);
Vektor2d v2d_kivon(Vektor2d v1, Vektor2d v2);

// megmondja, hogy v2 benne van-e egy v1 középpontú, sugar sugarú körben
// avagy v1 és v2 távolsága kisebb-e mint sugar
bool v2d_benne_van_e(Vektor2d v1, Vektor2d v2, double sugar);

// lineárisan interpolál v1 és v2 között, súly lehet <0 és >1 is
Vektor2d v2d_interpolal(Vektor2d v1, Vektor2d v2, double suly);

#endif // VEKTOR2D_H
