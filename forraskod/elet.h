#ifndef ELET_H
#define ELET_H
#include <stdbool.h>

// vissza a maximum életre
void elet_ujraallit(void);

// ha a játékos meghal a levonás hatására, akkor meghívja a nézetváltást (!)
void elet_levon(int mennyiseg);

// visszaadja, hogy mennyi élete van a játékosnak
int szerez_elet(void);

#endif // ELET_H
