#include "vektor2e.h"

Vektor2e v2e(int x, int y) {
    return (Vektor2e){x, y};
}

Vektor2e v2e_nyujt(Vektor2e v, int skalar) {
    v.x *= skalar;
    v.y *= skalar;
    return v;
}

Vektor2e v2e_normalas(Vektor2e v, int irany) {
    int ideigl = v.x;
    v.x = -v.y;
    v.y = ideigl;
    v = v2e_nyujt(v, irany);
    return v;
}

Vektor2e v2e_osszead(Vektor2e v1, Vektor2e v2) {
    v1.x += v2.x;
    v1.y += v2.y;
    return v1;
}

Vektor2e v2e_kivon(Vektor2e v1, Vektor2e v2) {
    v1.x -= v2.x;
    v1.y -= v2.y;
    return v1;
}
