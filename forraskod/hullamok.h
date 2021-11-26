#ifndef HULLAMOK_H
#define HULLAMOK_H

// beolvassa a hulláminformációkat a fájlból
void hullamok_inicializacio(void);

// meghívandó időintervallumonként, megidéz ellenséget, ha esedékes
void hullamok_frissites(double delta);

// felszabadítja a memóriát
void hullamok_destrukcio(void);

// elindít egy hullámot, ha éppen nem ment hullám
void hullam_indit(void);

// beállítja/újraállítja a kiinduló állapotot, használat előtt meghívandó
void hullamok_ujraallit(void);

// visszaadja, hogy hányadik hullám megy éppen, 0-tól kezdve
int szerez_hullamszam(void);

// visszaadja, hogy nehezítés céljából mennyire kell az ellenségeket gyorsítani
double szerez_gyorsitas(void);

#endif // HULLAMOK_H
