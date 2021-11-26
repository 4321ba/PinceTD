#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "palya.h"
#include "keprajzolo.h"
#include "vektor2.h"

enum {PALYA_SZELESSEG = 20, PALYA_MAGASSAG = 15};

// egy-egy karakter a fájlban egy-egy csempének (képnek)
// felel meg a lekepezes függvénynek megfelelően
// először x koordinátával, majd y-nal kell indexelni (!)
static Kep palya[PALYA_SZELESSEG][PALYA_MAGASSAG];
// ebben tároljuk el, hogy az adott csempére lehet-e tornyot rakni
// egyszerűbb külön, mert több Képe van a padlónak, amire le lehet tenni (FLOOR_1-8)
// és van olyan is, ami ugyanaz a Kép és nem lehet letenni ('.' és ',')
static bool toronylerakhatosag[PALYA_SZELESSEG][PALYA_MAGASSAG];

// az elméleti maximális úthossz 15*20 = 300
// az útvonal végét utvonal_hossza jelzi
static Vektor2e utvonal[PALYA_SZELESSEG*PALYA_MAGASSAG];
static int utvonal_hossza = 0;

// eltárolja, hogy a kezdőponti ajtó nyitva van-e
static bool kezdoajto_nyitva;

// az út anyaga a tüskés padló első képkockája
enum {UT = FLOOR_SPIKES_ANIM, SEMMI = -1};

void kezdoajto_allapotvaltas(bool nyitva) {
    kezdoajto_nyitva = nyitva;
}

// visszaadja a fájlbeli k-hoz tartozó kirajzolandó Képet
static Kep lekepezes(char k) {
    switch (k) {
        case ' ':
            return SEMMI;
        // '.'-ra lehet tornyot letenni, ','-re nem
        case '.':
        case ',':
            ; // üres állítás, hogy tudjak változót létrehozni
            // a 8-féle FLOOR konstans szomszédos, közülük választunk random
            // de az 1., a sima gyakrabban szerepeljen
            int veletlen = rand() % 64;
            return veletlen >= 8 ? FLOOR_1 : FLOOR_1 + veletlen;
        // u mint út, k mint kezdőpont (pontosan 1 k-nak kell lennie!)
        // az ellenfelek kezdőpontja; és a tesztelése a palya_beolvas-ban van
        // k-ból csak 1 irányba lehet u
        case 'u':
        case 'k':
            return UT;
        // o mint oldalfal, o - bal, O - jobb; csak egy fehér csík a csempe szélén
        case 'o':
            return WALL_SIDE_MID_LEFT;
        case 'O':
            return WALL_SIDE_MID_RIGHT;
        // f mint fal, a középső rész
        case 'f':
            return WALL_MID;
        // s mint sarok (és t mint következő betű),
        // s - felül, t - alul; kisbetű - bal, nagybetű - jobb
        case 's':
            return WALL_SIDE_TOP_LEFT;
        case 'S':
            return WALL_SIDE_TOP_RIGHT;
        case 't':
            return WALL_SIDE_FRONT_LEFT;
        case 'T':
            return WALL_SIDE_FRONT_RIGHT;
    }
    return SEMMI;
}

// visszaadja, hogy a fájlbeli k karakterre lehet-e tornyot letenni
static bool toronylerakhatosag_karakterbol(char k) {
    // '.'-ra lehet egyedül tornyot letenni
    return k == '.';
}

// v a pálya határain belül van-e (v csempe pozíció)
static bool palya_resze_e(Vektor2e v) {
    return v.x>=0 && v.y>=0 && v.x<PALYA_SZELESSEG && v.y<PALYA_MAGASSAG;
}

bool csempe_toronykompatibilis_e(Vektor2e csp) {
    return palya_resze_e(csp) && toronylerakhatosag[csp.x][csp.y];
}

// visszaadja, hogy v csempe pozíción út van-e
static bool ut_e(Vektor2e v) {
    return palya_resze_e(v) && palya[v.x][v.y] == (Kep)UT; // rövidzár fontos, különben túlindexelheti
}


// aktualis_pozicio kiindulóhellyel kezdve feltölti az utvonal tömböt az útvonal csempe koordinátáival
// addig keres, amíg a pálya végét el nem éri
static void utvonal_kitalal(Vektor2e aktualis_pozicio) {
    Vektor2e irany = v2e(1, 0);
    bool talalt_utat = false;
    for (int i = 0; i<4 && !talalt_utat; ++i) {
        irany = v2e_normalas(irany, 1);
        if (ut_e(v2e_osszead(aktualis_pozicio, irany)))
            talalt_utat = true;
    }
    if (!talalt_utat) {
        fprintf(stderr, "Az útkeresés a kezdőpontnál befuccsolt!\n");
        exit(4);
    }
    while (talalt_utat) {
        if (utvonal_hossza >= PALYA_SZELESSEG*PALYA_MAGASSAG) {
            fprintf(stderr, "Az útkeresés során végtelen ciklusba kerültünk!\n");
            exit(4);
        }
        utvonal[utvonal_hossza++] = aktualis_pozicio;
        // egyenesen?
        if (!ut_e(v2e_osszead(aktualis_pozicio, irany))) {
            // ha egyenesen nem, akkor balra?
            if (ut_e(v2e_osszead(aktualis_pozicio, v2e_normalas(irany, 1))))
                irany = v2e_normalas(irany, 1);
            // esetleg jobbra?
            else if (ut_e(v2e_osszead(aktualis_pozicio, v2e_normalas(irany, -1))))
                irany = v2e_normalas(irany, -1);
            else
                talalt_utat = false;
        }
        aktualis_pozicio = v2e_osszead(aktualis_pozicio, irany);
    }
}


void palya_inicializacio(void) {
    FILE *am = fopen("tartozekok/palya.txt", "r");
    if (am == NULL) {
        perror("Nem sikerült megnyitni a palya.txt fájlt");
        exit(3);
    }
    Vektor2e kezdopont;
    bool van_kezdopont = false;
    for (int y = 0; y < PALYA_MAGASSAG; ++y) {
        for (int x = 0; x < PALYA_SZELESSEG; ++x) {
            char k = fgetc(am);
            palya[x][y] = lekepezes(k);
            toronylerakhatosag[x][y] = toronylerakhatosag_karakterbol(k);
            // kezdőpont
            if (k == 'k') {
                if (van_kezdopont)
                    fprintf(stderr, "A pályán több kezdőpont van megadva!\n");
                van_kezdopont = true;
                kezdopont.x = x;
                kezdopont.y = y;
            }
        }
        if (fgetc(am) != '\n')
            fprintf(stderr, "Pálya beolvasásnál %d. sor túl rövid vagy túl hosszú!\n", y);
    }
    if (fgetc(am) != EOF)
        fprintf(stderr, "Pálya beolvasásnál a várt fájl vége jel nem következett!\n");
    fclose(am);
    
    if (!van_kezdopont) {
        fprintf(stderr, "A pályán nincs kezdőpont megadva!\n");
        exit(4);
    }
    utvonal_kitalal(kezdopont);
    
}

// kirajzol egy csukott vagy nyitott ajtót a csempe pozíció fölé, középre igazítva
static void ajto_kirajzol(Vektor2e pozicio, bool nyitott) {
    pozicio.y -= 2;
    pozicio = v2e_nyujt(pozicio, CSEMPEMERET);
    Vektor2e eltolas = v2e(CSEMPEMERET * 3/2, 0);
    kep_kirajzol_egesz(DOORS_FRAME_LEFT, v2e_kivon(pozicio, eltolas), 0);
    kep_kirajzol_egesz(DOORS_FRAME_RIGH, v2e_osszead(pozicio, eltolas), 0);
    pozicio.x -= CSEMPEMERET / 2;
    kep_kirajzol_egesz(nyitott ? DOORS_LEAF_OPEN : DOORS_LEAF_CLOSED, pozicio, 0);
}

void palya_kirajzol(void) {
    for (int y = 0; y < PALYA_MAGASSAG; ++y) {
        for (int x = 0; x < PALYA_SZELESSEG; ++x) {
            if (palya[x][y] != (Kep)SEMMI)
                kep_kirajzol_egesz(palya[x][y], v2e(x*CSEMPEMERET, y*CSEMPEMERET), 0);
            // a falnak kell felső részt rajzolni (az külön csempe),
            // a fölötte levő csempére rá is akár
            if (palya[x][y] == WALL_MID)
                kep_kirajzol_egesz(WALL_TOP_MID, v2e(x*CSEMPEMERET, (y-1)*CSEMPEMERET), 0);
        }
    }
    ajto_kirajzol(utvonal[0], kezdoajto_nyitva);
    ajto_kirajzol(utvonal[utvonal_hossza-1], false);
}


bool pozicio_megtett_utbol(double megtett_ut, Vektor2d *pozicio) {
    // példa (hosszabbitas=0 esetén): megtett_ut = 1.0 esetén az utvonal[1] csempe középpontját adjuk vissza
    // megtett_ut = 1.3 esetén az utvonal[1] és az utvonal[2] középpontok között interpolálunk 0.3 arányban
    int index = megtett_ut;
    double arany = megtett_ut - index;
    
    // azt szeretnénk, ha még a kezdőpont előtt és a végpont után
    // mennének még valamennyit (pl 3 csempényit), hogy az ellenfelek az ajtón sétáljanak be
    // ehhez megtett_ut=hosszabbitas-nál lesz csak az utvonal[0] a visszatérés
    // előtte pedig, illetve az utvonal[utvonal_hossza-1] után a két legközelebbi útdarab (első 2 vagy utsó 2) alapján interpolálunk
    // azaz az arány <0, ill >1 lesz, és ez azt jelenti, hogy pl
    // <0 esetén a végeredmény nem a két pont között lesz, hanem az elsőnél még előrébb
    static int const hosszabbitas = 3;
    index -= hosszabbitas;
    if (index < 0) {
        arany += index;
        index = 0;
    }
    
    if (index >= utvonal_hossza-1) {
        // ha még benne van a hosszabbításban:
        if (index < utvonal_hossza-1 + hosszabbitas) {
            arany += index - (utvonal_hossza - 2);
            // az utolsó előtti érvényes index (hogy azt, ill az utolsó elemet meg lehessen indexelni):
            index = utvonal_hossza - 2;
        } else {
            return false;
        }
    }
    
    *pozicio = v2d_interpolal(csempe_kozepe(utvonal[index]), csempe_kozepe(utvonal[index+1]), arany);
    return true;
}



