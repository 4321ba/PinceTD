#include <stdbool.h>

#include "ellensegek.h"
#include "mallokacio.h"
#include "lancolt_lista.h"
#include "vektor2.h"
#include "keprajzolo.h"
#include "palya.h"
#include "elet.h"
#include "lovedekek.h"
#include "hullamok.h"

typedef struct Ellensegtipus {
    Ellensegfajta fajta;
    Ellensegfajta gyereke;
    Kep kep; // sétáló animáció
    Kep disz_kep; // álló animáció
    double sebesseg; // csempe / másodpercben
    double animacio_sebesseg; // animációképkocka / másodpercben
} Ellensegtipus;

// ellensegtipusok[Ellensegfajta] adja meg az Ellensegtipus-t
static Ellensegtipus const ellensegtipusok[5] = {
    {LELKECSKE, EGYIK_ELLENSEG_SEM, SKELET_RUN_ANIM, SKELET_IDLE_ANIM, 2.4, 7.0},
    {KIS_OGRE, LELKECSKE, ORC_WARRIOR_RUN_ANIM, ORC_WARRIOR_IDLE_ANIM, 2.0, 6.5},
    {KIS_DEMON, LELKECSKE, CHORT_RUN_ANIM, CHORT_IDLE_ANIM, 3.0, 8.0},
    {NAGY_OGRE, KIS_DEMON, OGRE_RUN_ANIM, OGRE_IDLE_ANIM, 2.2, 6.0},
    {NAGY_DEMON, KIS_OGRE, BIG_DEMON_RUN_ANIM, BIG_DEMON_IDLE_ANIM, 2.8, 6.8},
};

typedef struct Ellenseg {
    Ellensegtipus const *tipus;
    
    // 1 egy csempének felel meg, a palya.c számolja ki belőle a koordinátát
    double megtett_ut;
    
    // minden frissítésnél felülíródik
    // vízszintesen a közepét, függőlegesen az alsó negyedelőpontját jelöli az ellenségnek
    // ez pixel, nem csempe koordináta, ráadásul double
    // azért eltároljuk, hogy ne kelljen mindig kiszámolni
    Vektor2d pozicio;
    
    // melyik irányba néz, tükrös: balra, nem tükrös: jobbra
    bool tukros;
    
    // dupla érték, mert nem minden képkockánál lesz új animációkocka
    // mindig kisebb kell legyen, mint a kép ismétlésszáma
    double animacio;
    
    // ha dísz, akkor nem mozog, csak animálva van
    bool disz;
} Ellenseg;

static Lista ellensegek = {NULL, NULL};

void ellensegek_destrukcio(void) {
    kiurit(&ellensegek);
}

void ellensegek_frissites(double delta) {
    // az ellenségeket fordítva szeretnénk bejárni, hátulról előre, mert az újonnan idézett ellenségek a lista végére
    // lesznek hozzáfűzve, és őket szeretnénk a legkorábban kirajzolni, mivel a pálya kezdete lefelé halad
    // nem cél komplex módon megoldani, hogy mindig a magasabb y koordinátájú entitások legyenek később kirajzolva,
    // de ezzel legalább valamennyivel szebb lesz, és legalább a lista duplán láncoltságát is kihasználja
    ListaElem *m = ellensegek.utolso;
    while (m != NULL) {
        Ellenseg *e = m->adat;
        
        // animáció
        e->animacio += e->tipus->animacio_sebesseg * delta;
        KepInformacio const *kepinfo = szerez_kepinformacio(e->tipus->kep);
        if (e->animacio >= kepinfo->ismetlesszam)
            e->animacio -= kepinfo->ismetlesszam;
        
        // pozíció frissítés
        bool celt_ert = false;
        if (!e->disz) {
            double utkulonbseg = e->tipus->sebesseg * szerez_gyorsitas() * delta;
            e->megtett_ut += utkulonbseg;
            Vektor2d elozo_pozicio = e->pozicio;
            // ha elérte a végét, le kell törölni (ekkor a pozíció nem frissül)
            celt_ert = !pozicio_megtett_utbol(e->megtett_ut, &e->pozicio);
        
            // melyik irányba néz
            double hibahatar = utkulonbseg / 10.0;
            if (elozo_pozicio.x < e->pozicio.x - hibahatar)
                e->tukros = false;
            else if (elozo_pozicio.x > e->pozicio.x + hibahatar)
                e->tukros = true;
        }
        
        // a képrajzolásnál a kép bal felső sarkát kell megadni, e->poz pedig a csempe közepe
        // azt szeretnénk, ha az aljuk egy vonalban lenne, majdnem a csempe legalján
        Vektor2d poz = v2d_kivon(e->pozicio, v2d(kepinfo->nagysag.x / 2.0, kepinfo->nagysag.y - CSEMPEMERET * 7/16.0));
        Kep k = e->disz ? e->tipus->disz_kep : e->tipus->kep;
        kep_kirajzol_lebegopontos(k, poz, e->animacio, 0, e->tukros); //animációnál double->int
        
        // ha elérte a végét, le kell vennünk az életet
        // annak függvényében, hogy hány gyereke van még hányat kellett volna belelőni
        if (celt_ert) {
            Ellensegfajta fajta = e->tipus->fajta;
            while (fajta != EGYIK_ELLENSEG_SEM) {
                elet_levon(1);
                fajta = ellensegtipusok[fajta].gyereke;
            }
        }
        
        ListaElem *uj_m = m->elozo; // fordítva járjuk be!
        if (celt_ert)
            letorol(&ellensegek, m);
        m = uj_m;
    }
}

// hozzáfűz egy újonnan makkokált ellenséget a listához
static void ellenseg_letrehoz(Ellensegfajta fajta, Vektor2d poz, bool disz) {
    Ellenseg *uj = mallokal(sizeof(Ellenseg));
    uj->tipus = ellensegtipusok+fajta;
    uj->megtett_ut = 0.0;
    uj->pozicio = poz;
    uj->tukros = false;
    uj->animacio = 0.0;
    uj->disz = disz;
    hozzafuz(&ellensegek, uj);
    
}

void ellenseg_idezes(Ellensegfajta fajta) {
    // pozíció bármi lehet, a következő kirajzolás előtt úgyis meghívja
    // a pozicio_megtett_utbol függvényt, ami felülírja
    ellenseg_letrehoz(fajta, v2d(0.0, 0.0), false);
}

void ellenseg_disz_idezes(Ellensegfajta fajta, Vektor2e csp) {
    ellenseg_letrehoz(fajta, csempe_kozepe(csp), true);
}

// melyik lövedék melyik ellenséget tudja lelőni
static bool lelovi_e(Lovedekfajta lovedek, Ellensegfajta ellenseg) {
    enum {MAXLELOHETOSEG = 3};
    // ezt a tömböt Lovedekfajta-val kell megindexelni (3-féle lövedék van)
    // és mindegyik lövedékfajta max 3-féle ellenséget tudhat lelőni (MAXLELOHETOSEG)
    static Ellensegfajta const lelohetoseg[3][MAXLELOHETOSEG] = {
        {LELKECSKE, EGYIK_ELLENSEG_SEM, EGYIK_ELLENSEG_SEM}, // NYIL
        {LELKECSKE, KIS_OGRE, NAGY_OGRE}, // DARDA
        {KIS_DEMON, NAGY_DEMON, EGYIK_ELLENSEG_SEM}, // VIZESUVEG
    };
    
    for (int i = 0; i < MAXLELOHETOSEG; ++i)
        if (lelohetoseg[lovedek][i] == ellenseg)
            return true;
    return false;
}

bool ellenseg_celzando_keres(Vektor2d kozeppont, double sugar, Lovedekfajta lovedek, EllensegPredikcio *ep) {
    // szeretnénk célozni olyan ellenséget is, akit nem tud a lövedékfajta lelőni,
    // de csak akkor, ha nincs egyáltalán más a környezetben
    // ettől eltekintve a célhoz legközelebb elhelyezkedőt célozzuk
    Ellenseg *talalat = NULL;
    for (ListaElem *m = ellensegek.elso; m != NULL; m = m->kovi) {
        Ellenseg *e = m->adat;
        // benne van ÉS (eddig nem találtunk semmit VAGY ez az első lelőhető amit találtunk VAGY találtunk egy célhoz közelebbi lelőhetőt
        // VAGY találtunk egy nem lelőhetőnél a célhoz közelebbit)
        if (
            !e->disz && v2d_benne_van_e(e->pozicio, kozeppont, sugar) && (
                (talalat == NULL) ||
                (lelovi_e(lovedek, e->tipus->fajta) && !lelovi_e(lovedek, talalat->tipus->fajta)) ||
                (e->megtett_ut > talalat->megtett_ut && lelovi_e(lovedek, e->tipus->fajta)) ||
                (e->megtett_ut > talalat->megtett_ut && !lelovi_e(lovedek, talalat->tipus->fajta))
            )
        )
            talalat = e;
    }
    if (talalat == NULL)
        return false;
    ep->pozicio = talalat->pozicio;
    ep->megtett_ut = talalat->megtett_ut;
    ep->sebesseg = talalat->tipus->sebesseg * szerez_gyorsitas();
    return true;
}


bool ellenseg_leloves(Vektor2d lovedek_poz, Lovedekfajta lovedek) {
    for (ListaElem *m = ellensegek.elso; m != NULL; m = m->kovi) {
        Ellenseg *e = m->adat;
        double sugar = szerez_kepinformacio(e->tipus->kep)->nagysag.x / 2.0;
        if (!e->disz && v2d_benne_van_e(e->pozicio, lovedek_poz, sugar) && lelovi_e(lovedek, e->tipus->fajta)) {
            if (e->tipus->gyereke == EGYIK_ELLENSEG_SEM)
                // nem baj, hogy elveszítjük a listát, mert úgyis visszatérünk
                letorol(&ellensegek, m);
            else
                e->tipus = ellensegtipusok+e->tipus->gyereke;
            return true;
        }
    }
    return false;
}

