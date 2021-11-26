#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "hullamok.h"
#include "mallokacio.h"
#include "ellensegek.h"
#include "jatek_ff.h"
#include "palya.h"

typedef struct Hullamok {
    // az összes hullám '\0'-kkal elválasztva
    char *adatok;
    // pointerek az összes hullám első elemére
    char **hullamok;
    int darab;
} Hullamok;

typedef struct Allapot {
    // ha nincs elindítva hullám, akkor ezen kívül ellensegszam legyen -1
    bool hullam_elinditva;
    // utolsó ellenség idézése óta eltelt idő
    double eltelt_ido;
    // a kör indításakor frissül, ezért kezdetben -1
    int aktualis_hullam;
    // hányadik ellenséget kell idézni, ha nincs kör, akkor -1
    int aktualis_ellenseg;
    // Ellensegfajta-val legyen indexelve, szükséges az alsó, dísz ellenségek idézéséhez
    bool mar_idezett_fajtak[5];
} Allapot;

// csak az inicializációkor változik
static Hullamok hullamok;

// futás közben változik, vissza kell állítani a mezőit
static Allapot allapot;

void hullamok_inicializacio(void) {
    FILE *am = fopen("tartozekok/hullamok.txt", "r");
    if (am == NULL) {
        perror("Nem sikerült megnyitni a hullamok.txt fájlt");
        return;
    }
    // megnézzük a hosszát, és az egészet egyszerre beolvassuk
    fseek(am, 0, SEEK_END);
    hullamok.adatok = mallokal((ftell(am) + 1) * sizeof(char));
    rewind(am);
    fscanf(am, "%[loOdD \n]", hullamok.adatok);
    // ha nem a fájl vége miatt állt meg az fscanf,
    // akkor valamilyen érvénytelen karakter miatt
    if (!feof(am))
        fprintf(stderr, "A hullam.txt fájlban van nem megengedett karakter is!\n");
    fclose(am);
    
    hullamok.darab = 0;
    for (char *m = hullamok.adatok; *m != '\0'; ++m)
        if (*m == '\n')
            ++hullamok.darab;
    // itt számítunk arra, hogy az utolsó sor is sortöréssel zárul
    
    // feldolgozzuk: csinálunk egy indexelő tömböt
    // és a sortöréseket lecseréljük '\0'-ra
    hullamok.hullamok = mallokal(hullamok.darab * sizeof(char *));
    int index = 0;
    hullamok.hullamok[index++] = hullamok.adatok;
    for (char *m = hullamok.adatok; *m != '\0'; ++m) {
        if (m[0] == '\n') {
            m[0] = '\0';
            // ha a tömb végén vagyunk, akkor ne rakjuk be a következő hullám eleje pointert
            if (m[1] != '\0')
                hullamok.hullamok[index++] = m+1;
        }
    }
}

void hullamok_destrukcio(void) {
    free(hullamok.adatok);
    free(hullamok.hullamok);
}


// végtelenítő logika, megkapja a hullám számát 0-tól végtelenig
// illetve, hogy az adott hullámban a hányadik ellenséget kell idézni (0-tól kezdődik)
// visszaadja az idézendő karaktert, '\0'-t ha végére ért a hullám

// a végtelenítő logika a következőképp néz ki:
// 0. hullám - n-1-edik hullám (n darab hullámmal a fájlban): önmaguk
// n-edik hullám - 2n-1-edik hullám: először a k-adik hullám ellenségei (ahol k 0-tól n-1-ig fut fel),
// aztán az n-k-1-edik hullám ellenségei, majd ismét a k-adikéi
// 2n-edik hullám - 3n-1-edik hullám: k-adik, n-k-1-edik, k-adik, n-k-1-edik, majd k-adik
// stb. tovább
// tehát például ha előre 20 hullám van definiálva a fájlban (0..19), akkor a 37. hullám úgy néz ki,
// hogy először jön a 17. hullám, majd a 2. hullám, majd ismét a 17. hullám
// az 51. pedig: 11., 8., 11., 8., 11.
// 59.: 19., 0., 19., 0., 19.
// 60.: 0., 19., 0., 19., 0., 19., 0.
// 61.: 1., 18., 1., 18., 1., 18., 1.
static char idezendo(int hullamszam, int ellensegszam) {
    // picit pazarló ezeket minden egyes ellenség idézésénél kiszámolni, de így átláthatóbb lesz
    // hullam_darab*2+1 darab hullámból kell összetákolni a mostanit:
    int const hullam_darab = hullamszam / hullamok.darab;
    // a fenti mesében a k szerepét tölti be:
    int const pszeudo_hullamszam = hullamszam % hullamok.darab;
    // a k-adik hullám, amit eggyel többször kell bemásolni, mint b-t:
    char const *a_felhullam = hullamok.hullamok[pszeudo_hullamszam];
    // az n-k-1-edik hullám:
    char const *b_felhullam = hullamok.hullamok[hullamok.darab - pszeudo_hullamszam - 1];
    int const a_ellensegszam = strlen(a_felhullam);
    int const b_ellensegszam = strlen(b_felhullam);
    
    if (ellensegszam >= hullam_darab * (a_ellensegszam + b_ellensegszam) + a_ellensegszam)
        return '\0';
    
    ellensegszam %= a_ellensegszam + b_ellensegszam;
    if (ellensegszam >= a_ellensegszam)
        return b_felhullam[ellensegszam - a_ellensegszam];
    return a_felhullam[ellensegszam];
}

// leidézi a karakterhez rendelt ellenséget, pluszban díszt is idéz, ha új ellenség jött
static void idezes(char k) {
    Ellensegfajta fajta = EGYIK_ELLENSEG_SEM;
    
    switch (k) {
        case 'l':
            fajta = LELKECSKE;
            break;
        case 'o':
            fajta = KIS_OGRE;
            break;
        case 'O':
            fajta = NAGY_OGRE;
            break;
        case 'd':
            fajta = KIS_DEMON;
            break;
        case 'D':
            fajta = NAGY_DEMON;
            break;
        case ' ':
            fajta = EGYIK_ELLENSEG_SEM;
            break;
    }
    
    if (fajta != EGYIK_ELLENSEG_SEM) {
        ellenseg_idezes(fajta);
        
        if (!allapot.mar_idezett_fajtak[fajta]) {
            jatek_ffhez_uj_ellenseg(fajta);
            allapot.mar_idezett_fajtak[fajta] = true;
        }
    }
}

void hullamok_frissites(double delta) {
    if (!allapot.hullam_elinditva)
        return;
    
    // hány másodpercenként idézzen
    double idozites = 0.2 / szerez_gyorsitas();
    
    // ha éppen most lett elindítva a hullám
    if (allapot.aktualis_ellenseg == -1) {
        allapot.aktualis_ellenseg = 0;
        allapot.eltelt_ido = 0.0;
        ++allapot.aktualis_hullam;
        kezdoajto_allapotvaltas(true);
    }
    
    allapot.eltelt_ido += delta;
    if (allapot.eltelt_ido >= idozites) {
        allapot.eltelt_ido -= idozites;
        char k = idezendo(allapot.aktualis_hullam, allapot.aktualis_ellenseg);
        if (k != '\0') {
            // ha még nincs vége a hullámnak
            idezes(k);
            ++allapot.aktualis_ellenseg;
        } else {
            // ha épp most lett vége
            allapot.aktualis_ellenseg = -1;
            allapot.hullam_elinditva = false;
            kezdoajto_allapotvaltas(false);
        }
    }
}

void hullam_indit(void) {
    allapot.hullam_elinditva = true;
}

void hullamok_ujraallit(void) {
    allapot.hullam_elinditva = false;
    allapot.eltelt_ido = 0.0;
    allapot.aktualis_hullam = -1;
    allapot.aktualis_ellenseg = -1;
    for (size_t i = 0; i < sizeof(allapot.mar_idezett_fajtak) / sizeof(*allapot.mar_idezett_fajtak); ++i)
        allapot.mar_idezett_fajtak[i] = false;
}

int szerez_hullamszam(void) {
    // mielőtt még bármi elkezdődött (aktualis_hullam==-1), 0+1-et adjunk vissza és ne -1+1-et
    // +1, mert minden, ami ezt használja (dicsőséglista, játékff), azoknak 1-gyel kezdve kell
    return (allapot.aktualis_hullam >= 0 ? allapot.aktualis_hullam : 0) + 1;
}

double szerez_gyorsitas(void) {
    // minden adag (pl 26) hullám után +30% sebesség, ellenségeknek és az ellenségidézésnek is
    return 1.0 + 0.3 * (allapot.aktualis_hullam / hullamok.darab);
}
