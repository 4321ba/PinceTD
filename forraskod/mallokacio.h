#ifndef MALLOKACIO_H
#define MALLOKACIO_H

#include <stdlib.h>

// kikommentelendő, ha a tesztelést nem szeretnénk végezni
// ezen az egyetlen helyen van includeolva, elég itt kikommentelni
// ezért a mallokal hívást nem, de free hívást tartalmazó
// moduloknak is includeolnia kell ezt a fejlécfájlt
#include "debugmalloc.h"

// segédfüggvény malloc-hoz, ami kilép, ha sikertelen a foglalás
void *mallokal(size_t meret);

#endif // MALLOKACIO_H
