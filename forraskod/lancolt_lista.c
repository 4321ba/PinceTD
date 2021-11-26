#include <stdlib.h>

#include "lancolt_lista.h"
#include "mallokacio.h"

void hozzafuz(Lista *lista, void *uj_adat) {
    ListaElem *uj = mallokal(sizeof(ListaElem));
    uj->adat = uj_adat;
    uj->kovi = NULL;
    
    if (lista->utolso != NULL) // ha a lista nem üres
        lista->utolso->kovi = uj;
    else
        lista->elso = uj;
    
    uj->elozo = lista->utolso;
    lista->utolso = uj;
}

void letorol(Lista *lista, ListaElem *torlendo) {
    if (torlendo != lista->elso)
        torlendo->elozo->kovi = torlendo->kovi;
    else
        lista->elso = torlendo->kovi;
    
    if (torlendo != lista->utolso)
        torlendo->kovi->elozo = torlendo->elozo;
    else
        lista->utolso = torlendo->elozo;
    
    free(torlendo->adat);
    free(torlendo);
}

void kiurit(Lista *lista) {
    while (lista->elso != NULL)
        letorol(lista, lista->elso);
    // a lista első és utolsó elemét a letorol fgv beállítja NULL-ra
}
