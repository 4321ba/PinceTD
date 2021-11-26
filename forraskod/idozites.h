#ifndef IDOZITES_H
#define IDOZITES_H

#define GYORSITOTT_SEBESSEG 3.0
#define NORMAL_SEBESSEG 1.0

// beállít, és visszaad egy adott értéket, használhatók a fentebbi makrók
void szimulacio_sebesseg_allitas(double sebesseg);
double szerez_szimulacio_sebesseg(void);

// megadja az előző hívás (!) óta eltelt időt ezredmásodpercben,
// nem számolja bele a szimuláció sebességgel való szorzást
Uint32 szerez_eltelt_ido(void);

// meghívandó, ha egy kép létrejött, másodpercenként frissíti az ablak címét
// a kiszámolt kps = kép per másodperc (= fps) értéket beleírva
// eltelt idő ezredmásodpercben adandó meg
void kps_egy_kep_letrejott(Uint32 eltelt_ido);

#endif // IDOZITES_H
