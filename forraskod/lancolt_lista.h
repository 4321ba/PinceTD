#ifndef LANCOLT_LISTA_H
#define LANCOLT_LISTA_H

// az (első elem)->elozo és (utolsó elem)->kovi mindenképp legyenek NULL
typedef struct ListaElem {
    struct ListaElem *elozo, *kovi;
    void *adat;
} ListaElem;

// az elso és az utolso csak együtt lehet NULL, és akkor és csak akkor, ha üres a lista
typedef struct Lista {
    ListaElem *elso, *utolso;
} Lista;

// a végére fűzi hozzá
// uj_adat dinamikusan foglalt memóriára mutasson, mert a letorol fgv felszabadítja
// foglal még dinamikusan egy ListaElemet is, amit a letorol vagy a kiurit szintén felszabadít
void hozzafuz(Lista *lista, void *uj_adat);

// felszabadítják a ListaElemeket és az általuk mutatott adatokat
// (a Listát nem, az inkább legyen statikus)
void letorol(Lista *lista, ListaElem *torlendo);
void kiurit(Lista *lista);

#endif // LANCOLT_LISTA_H
