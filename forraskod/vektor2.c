#include "vektor2.h"
#include "keprajzolo.h"

Vektor2e v2e_v2dbol(Vektor2d v) {
    return v2e(v.x, v.y);
}

Vektor2d v2d_v2ebol(Vektor2e v) {
    return v2d(v.x, v.y);
}

Vektor2e csempepozza(Vektor2d v) {
    return v2e(v.x / CSEMPEMERET, v.y / CSEMPEMERET);
}

Vektor2d csempe_kozepe(Vektor2e csp) {
    return v2d_v2ebol(v2e_osszead(v2e_nyujt(csp, CSEMPEMERET), v2e(CSEMPEMERET/2, CSEMPEMERET/2)));
}
