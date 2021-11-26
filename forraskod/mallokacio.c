#include <stdio.h>
#include <stdlib.h>

#include "mallokacio.h"

void *mallokal(size_t meret) {
    void *m = malloc(meret);
    if (m == NULL) {
        fprintf(stderr, "Nem kaptunk %lu méretű dinamikus memóriát, kilépés...\n", (long unsigned int)meret);
        exit(2);
    }
    return m;
}
