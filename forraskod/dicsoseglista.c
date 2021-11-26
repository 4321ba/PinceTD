#include <stdio.h>

#include "dicsoseglista.h"
#include "vektor2e.h"
#include "keprajzolo.h"

typedef struct Dicsoseg {
    int pontszam;
    char nev[20 + 1];
} Dicsoseg;

enum {DICSOSEG_ELEMSZAM = 6};
static Dicsoseg dicsoseglista[DICSOSEG_ELEMSZAM];

// beolvassa a fájlból az elemeket a dicsoseglista tömbbe, -1-es pontszámmal tölti ki a nem használt részt
// (mind -1, ha nem tudott semmit sem beolvasni)
void dicsoseglista_beolvas(void) {
    FILE *am = fopen("dicsoseglista.txt", "r");
    if (am == NULL) {
        perror("Nem sikerült olvasni a dicsoseglista.txt fájlt (a program ettől még működőképes lesz)");
        for (int i = 0; i < DICSOSEG_ELEMSZAM; ++i)
            dicsoseglista[i].pontszam = -1;
        return;
    }
    for (int i = 0; i < DICSOSEG_ELEMSZAM; ++i)
        if (fscanf(am, "%d %20[^\n]%*[^\n]", &dicsoseglista[i].pontszam, dicsoseglista[i].nev) != 2)
            dicsoseglista[i].pontszam = -1;
    fclose(am);
}

// kiírja a fájlba a dicsoseglista tartalmát
static void dicsoseglista_lement(void) {
    FILE *am = fopen("dicsoseglista.txt", "w");
    if (am == NULL) {
        perror("Nem sikerült írni a dicsoseglista.txt fájlt (a program ettől még működőképes lesz)");
        return;
    }
    for (int i = 0; i < DICSOSEG_ELEMSZAM && dicsoseglista[i].pontszam != -1; ++i)
        fprintf(am, "%d %s\n", dicsoseglista[i].pontszam, dicsoseglista[i].nev);
    if (fclose(am) != 0)
        perror("Nem sikerült írni a dicsoseglista.txt fájlt (a program ettől még működőképes lesz)");
}

void dicsoseglista_kirajzol(Vektor2e poz, SDL_Color const *szin) {
    if (dicsoseglista[0].pontszam == -1) {
        szoveg_kirajzol("Egy bejegyz\x82s sincs a dics\x93s\x82glist\xa0\x62\x61n.", poz, szin, false);
        return;
    }
    szoveg_kirajzol("Dics\x93s\x82glista:", poz, szin, false);
    szoveg_kirajzol("Hely Hull\xa0m  N\x82v", v2e_osszead(poz, v2e(0, KARAKTER * 5/4)), szin, false);
    for (int i = 0; i < DICSOSEG_ELEMSZAM && dicsoseglista[i].pontszam != -1; ++i) {
        char kiirando[64]; // "1.: pontszám név" elég kell legyen a 64
        sprintf(kiirando, " %d.:%7d  %s", i + 1, dicsoseglista[i].pontszam, dicsoseglista[i].nev);
        szoveg_kirajzol(kiirando, v2e_osszead(poz, v2e(0, (i + 2) * KARAKTER * 5/4)), szin, false);
    }
}

bool dicsoseglista_felkerul_e(int pontszam) {
    dicsoseglista_beolvas();
    // ha -1, akkor még van hely, ha pedig már van pontszám, de kisebb, mint a felteendő, akkor is jó
    return dicsoseglista[DICSOSEG_ELEMSZAM - 1].pontszam < pontszam;
}

void dicsoseglista_feltesz(int pontszam, char const *nev) {
    dicsoseglista_beolvas();
    if (dicsoseglista[DICSOSEG_ELEMSZAM - 1].pontszam >= pontszam)
        return;
    // mindenkit eggyel lejjebb tolunk, akinek a pontszáma rosszabb, mint a hozzáadandó
    int i;
    for (i = DICSOSEG_ELEMSZAM - 1; i > 0 && dicsoseglista[i - 1].pontszam < pontszam; --i)
        dicsoseglista[i] = dicsoseglista[i - 1];
    // amit most felülírunk, az a ciklus belsejében a dicsoseglista[i - 1]-es elem
    // mert a for ciklus fejléce még lefut
    dicsoseglista[i].pontszam = pontszam;
    snprintf(dicsoseglista[i].nev, 20 + 1, "%s", strlen(nev) > 0 ? nev : "N\x82vtelen");
    dicsoseglista_lement();
}
