#include <stdbool.h>
#include <math.h>

#include "vektor2d.h"

Vektor2d v2d(double x, double y) {
    return (Vektor2d){x, y};
}

Vektor2d v2d_polar(double hossz, double szog) {
    return (Vektor2d){hossz * cos(szog), hossz * sin(szog)};
}

double v2d_irany(Vektor2d v) {
    return atan2(v.y, v.x);
}

// gyökvonás drága és esetleg elkerülhető
double v2d_hossznegyzet(Vektor2d v) {
    return v.x*v.x + v.y*v.y;
}

// ha nem
double v2d_hossz(Vektor2d v) {
    return sqrt(v2d_hossznegyzet(v));
}
Vektor2d v2d_nyujt(Vektor2d v, double skalar) {
    v.x *= skalar;
    v.y *= skalar;
    return v;
}
Vektor2d v2d_osszead(Vektor2d v1, Vektor2d v2) {
    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}

Vektor2d v2d_kivon(Vektor2d v1, Vektor2d v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
    return v1;
}

bool v2d_benne_van_e(Vektor2d v1, Vektor2d v2, double sugar) {
    return v2d_hossznegyzet(v2d_kivon(v1, v2)) <= sugar*sugar; 
}

Vektor2d v2d_interpolal(Vektor2d v1, Vektor2d v2, double suly) {
    // suly=0 esetén v1, suly=1 esetén v2, suly=0.3 esetén 0.7v1+0.3v2 stb.
    // működik suly<0 és suly>1 értékekkel is,
    // ekkor nem v1 és v2 között van a visszaadott vektor, de velük egyvonalban
    v1.x = v1.x*(1-suly) + v2.x*suly;
    v1.y = v1.y*(1-suly) + v2.y*suly;
    return v1;
}
