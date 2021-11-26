#include <stdlib.h>
#include <stdbool.h>

#include <SDL2/SDL2_gfxPrimitives.h>

#include "tornyok.h"
#include "mallokacio.h"
#include "lancolt_lista.h"
#include "vektor2.h"
#include "keprajzolo.h"
#include "palya.h"
#include "ellensegek.h"
#include "lovedekek.h"
#include "ablak.h"

typedef struct Toronytipus {
    Toronyfajta fajta;
    Kep kep;
    Lovedekfajta lovedek;
    double latosugar; // pixelben
    double loves_sebesseg; // lövés / másodpercben
    double animacio_sebesseg; // animációképkocka / másodpercben
} Toronytipus;

// toronytipusok[Toronyfajta] adja meg a Toronytipus-t
static Toronytipus const toronytipusok[3] = {
    {IJASZ, ELF_M_IDLE_ANIM, NYIL, 55.0, 0.8, 5.0},
    {LOVAG, KNIGHT_M_IDLE_ANIM, DARDA, 35.0, 0.5, 3.0},
    {VARAZSLO, WIZZARD_F_IDLE_ANIM, VIZESUVEG, 45.0, 0.65, 3.5},
};

typedef struct Torony {
    Toronytipus const *tipus;
    
    // ez a csempe koordinátája, amin áll, nem pixel!
    Vektor2e csempe_pozicio;
    
    // melyik irányba néz, tükrös: balra, nem tükrös: jobbra
    bool tukros;
    
    // dupla érték, mert nem minden képkockánál lesz új animációkocka
    // mindig kisebb kell legyen, mint a kép ismétlésszáma
    double animacio;
    
    // lövéskor 1/loves_sebesseg lesz, és 0-ig folyamatosan csökken, értéke másodpercben
    double ujratoltodes;
    
    // ha dísz, akkor nem csinál semmit, csak animálva van
    bool disz;
} Torony;

static Lista tornyok = {NULL, NULL};

void tornyok_destrukcio(void) {
    kiurit(&tornyok);
}

void tornyok_frissites(double delta) {
    for (ListaElem *m = tornyok.elso; m != NULL; m = m->kovi) {
        Torony *t = m->adat;
        
        // animáció
        t->animacio += t->tipus->animacio_sebesseg * delta;
        KepInformacio const *kepinfo = szerez_kepinformacio(t->tipus->kep);
        if (t->animacio >= kepinfo->ismetlesszam)
            t->animacio -= kepinfo->ismetlesszam;
        
        // kirajzolás
        // a tornyok képei 2 csempe magasak, tehát a bal felső saroknak egy csempényivel magasabban kell lennie
        Vektor2d rajz_poz = v2d_v2ebol(v2e_kivon(v2e_nyujt(t->csempe_pozicio, CSEMPEMERET), v2e(0, CSEMPEMERET)));
        kep_kirajzol_lebegopontos(t->tipus->kep, rajz_poz, t->animacio, 0, t->tukros); //animációnál double->int
        
        // lövés
        if (!t->disz) {
            t->ujratoltodes -= delta;
            if (t->ujratoltodes <= 0.0) {
                Vektor2d torony_kozepe = csempe_kozepe(t->csempe_pozicio);
                EllensegPredikcio ep;
                if (ellenseg_celzando_keres(torony_kozepe, t->tipus->latosugar, t->tipus->lovedek, &ep)) {
                    t->tukros = lovedek_celozva(t->tipus->lovedek, torony_kozepe, ep);
                    t->ujratoltodes += 1 / t->tipus->loves_sebesseg;
                } else
                    t->ujratoltodes = 0.0;
            }
        }
        
    }
}

// kirajzol egy karikát
static void latokarika_kirajzol(Vektor2e csp, double sugar, bool lerakhato_e) {
    static SDL_Color const piros = {218, 78, 56, 128};
    static SDL_Color const sotetszurke = {34, 34, 34, 64};
    SDL_Color const s = lerakhato_e ? sotetszurke : piros;
    Vektor2e poz = v2e_v2dbol(csempe_kozepe(csp));
    filledCircleRGBA(szerez_megjelenito(), poz.x, poz.y, sugar, s.r, s.g, s.b, s.a);
}

void tornyok_latotav_kirajzol(Vektor2e uj_csp, Toronyfajta uj_toronyfajta) {
    bool lerakhato_e = csempe_toronykompatibilis_e(uj_csp) && latotav_szabad_e(uj_csp, uj_toronyfajta);
    latokarika_kirajzol(uj_csp, toronytipusok[uj_toronyfajta].latosugar, lerakhato_e);
    for (ListaElem *m = tornyok.elso; m != NULL; m = m->kovi) {
        Torony *t = m->adat;
        if (!t->disz)
            latokarika_kirajzol(t->csempe_pozicio, t->tipus->latosugar, true);
    }
    // lerakandó torony képének kirajzolása:
    // a tornyok képei 2 csempe magasak, tehát a bal felső saroknak egy csempényivel magasabban kell lennie
    Vektor2e rajz_poz = v2e_kivon(v2e_nyujt(uj_csp, CSEMPEMERET), v2e(0, CSEMPEMERET));
    kep_kirajzol_egesz(toronytipusok[uj_toronyfajta].kep, rajz_poz, 0);
}

void torony_letrehoz(Toronyfajta fajta, Vektor2e csp, bool disz) {
    Torony *uj = mallokal(sizeof(Torony));
    uj->tipus = toronytipusok+fajta;
    uj->csempe_pozicio = csp;
    uj->tukros = false;
    uj->animacio = 0.0;
    uj->ujratoltodes = 0.0;
    uj->disz = disz;
    hozzafuz(&tornyok, uj);
}

Toronyfajta torony_letorol(Vektor2e csp) {
    for (ListaElem *m = tornyok.elso; m != NULL; m = m->kovi) {
        Torony *t = m->adat;
        Vektor2e poz = t->csempe_pozicio;
        if (!t->disz && poz.x == csp.x && poz.y == csp.y) {
            Toronyfajta fajta = t->tipus->fajta;
            letorol(&tornyok, m);
            return fajta;
        }
    }
    return EGYIK_TORONY_SEM;
}

bool latotav_szabad_e(Vektor2e csp, Toronyfajta fajta) {
    for (ListaElem *m = tornyok.elso; m != NULL; m = m->kovi) {
        Torony *t = m->adat;
        double nagyobbik_latotav = t->tipus->latosugar > toronytipusok[fajta].latosugar
                                 ? t->tipus->latosugar : toronytipusok[fajta].latosugar;
        Vektor2d poz1 = csempe_kozepe(csp);
        Vektor2d poz2 = csempe_kozepe(t->csempe_pozicio);
        if (!t->disz && v2d_benne_van_e(poz1, poz2, nagyobbik_latotav))
            return false;
    }
    return true;
}

