#include <math.h>

#include "lovedekek.h"
#include "mallokacio.h"
#include "lancolt_lista.h"
#include "vektor2.h"
#include "penz.h"
#include "jatek_ff.h"
#include "keprajzolo.h"
#include "palya.h"
#include "ellensegek.h"
#include "ablak.h"

#define MATEK_PI 3.14159265358979323846
#define RADBOLFOK (180.0/MATEK_PI)
#define GRAVITACIO 98.1 // pixel / másodperc^2

typedef struct Lovedektipus {
    Lovedekfajta fajta;
    Kep kep;
    double kezdosebesseg; // pixel / másodpercben
} Lovedektipus;

// lovedektipusok[Lovedekfajta] adja meg az Lovedektipus-t
static Lovedektipus const lovedektipusok[3] = {
    {NYIL, WEAPON_ARROW, 160.0},
    {DARDA, WEAPON_SPEAR, 100.0},
    {VIZESUVEG, FLASK_BIG_BLUE, 90.0},
};

typedef struct Lovedek {
    Lovedektipus const *tipus;
    Vektor2d pozicio; // pixelben, double!
    Vektor2d sebesseg; // pixel / másodpercben
    bool disz;
} Lovedek;

static Lista lovedekek = {NULL, NULL};

// hozzáfűz egy újonnan mallokált lövedéket a listához
static void lovedek_letrehoz(Lovedekfajta fajta, Vektor2d pozicio, double szog, bool disz) {
    Lovedek *uj = mallokal(sizeof(Lovedek));
    uj->tipus = lovedektipusok+fajta;
    uj->pozicio = pozicio;
    uj->sebesseg = v2d_polar(uj->tipus->kezdosebesseg, szog);
    uj->disz = disz;
    hozzafuz(&lovedekek, uj);
}

void lovedek_disz_letrehoz(Lovedekfajta fajta, Vektor2e csp) {
    lovedek_letrehoz(fajta, csempe_kozepe(csp), 0, true);
}

// visszaadja a dobásszöget, ha v0 kezdősebességgel szeretnénk az origóból d-t eltalálni
// a rövidebbik utat választja; és ha lehetetlen, a lehető legközelebbi szöget adja vissza
static double dobasszog_poziciobol(double v0, Vektor2d d) {
    // ismert: kezdősebesség (v0), megteendő_vektor (d.x és d.y), GRAVITÁCIÓ (G)
    // keresett: idő (t), szög (α), ill. létezik-e
    // (1) d.x == t * v0*cos(α)
    // (2) d.y == t * v0*sin(α) + t² * G/2
    // (1)-ből: t == d.x / (v0*cos(α))
    // ez (2)-be: d.y = d.x / (v0*cos(α)) * v0*sin(α) + (d.x / (v0*cos(α)))² * G/2
    // rendezve: d.y = d.x * tg(α) + d.x² / (v0²*cos²(α)) * G/2
    // . . . hosszú varázslatok után, amit https://www.youtube.com/watch?v=bqYtNrhdDAY végzett el:
    // α = ( cos⁻¹( (G * d.x² / v0² - d.y) / sqrt(d.x² + d.y²) ) + tg⁻¹(d.x/d.y) ) / 2
    
    // ennek az egyenletnek több buktatója van:
    // egyrészt nem szabad elfelejteni, hogy d.y lefelé pozitív, mint ahogy a videóban
    // és azt sem, hogy α viszont óramutató járásával megegyezően pozitív, szemben a videóval
    double acos_belseje = (GRAVITACIO * d.x*d.x / (v0*v0) - d.y) / v2d_hossz(d);
    
    // azt, hogy van-e megoldás, az határozza meg, hogy az acos-ban érvényes szám van-e (-1<=x<=1)
    // egyenlőre nem foglalkozunk, ha hatótávolságon kívül van, csak a lehető legközelebb dobjuk
    if (acos_belseje > 1)
        acos_belseje = 1;
    else if (acos_belseje < -1)
        acos_belseje = -1;
    
    // ezenkívül minden lehetséges pontnak 2-féle megoldása van: egy rövidebb útú és egy hosszabb útú
    // ezt az egyenletben a cos visszakereséskor a nemegyértelműség határozza meg:
    // d.x>0 esetén +acos(...) esetén a hosszabb megoldás, -acos(...) esetén a rövidebb jön ki
    // d.x<0 esetén +acos(...) esetén a rövidebb megoldás, -acos(...) esetén a hosszabb jön ki
    // d.x=0 esetén +acos esetén (d.y-tól függetlenül) mindenképp felfelé dobja (hosszabb megoldás),
    // -acos esetén pedig a rövidebb irányba (felfelé ha d.y>0, lefelé egyébként)
    // mi mindig a rövidebbet akarjuk, ezért d.x>=0 esetén berakjuk az előjelet
    
    // első mínusz azért, mert óramutató járása a pozitív nálunk
    // a hármas operátor a 2-féle megoldás közül választja a rövidebbet minden esetben
    // azt is érdemes megjegyezni, hogy atan2 y/x atanját számolja ki, de y az első operandus, és x a második
    double szog = -( (d.x>=0?-1:1) * acos(acos_belseje) + atan2(d.x, d.y)) / 2;
    
    // erre jön rá még, hogy d.x<0 esetén hozzá kell adni / le kell vonni fél periódusnyi szöget (PI rad)
    // a végeredményből, különben pl balra fel helyett jobbra le fogja dobni a lövedéket
    // mellékesen a szögre és a pontra illesztett parabola mindkét esetben helyes,
    // csak nem mindegy, hogy a parabola mentén a pont irányába indul-e a lövedék vagy pont a másik irányba
    if (d.x < 0)
        szog += MATEK_PI;
    
    return szog;
}

// visszaadja, hogy szog szöggel, v0 kezdősebességgel az origóból d.x-hez mennyi idő alatt ér el a lövedék
// nem pontos, ha pontosan függőlegesen fel vagy le kell célozni, de ez remélhetőleg nem okoz túl nagy galibát
static double dobasido_szogbol(double v0, Vektor2d d, double szog) {
    // a dobasszog_poziciobol-t egészíti ki, és kiszámolja a landolásig eltelendő időt
    // (1) d.x == t * v0*cos(α)
    // t = d.x / (v0*cos(α))
    // biztos ami biztos, a 0/0-t el szeretném kerülni
    if (v0*cos(szog) == 0.0)
        return 1.0; // csak valami legalább nagyjából értelmes érték, 1 másodperc
    // nagyon picike szám esetén d.x is hasonlóan pici kell hogy legyen,
    // tehát egyéb esetben értelmes értéknek kell kijönnie
    return d.x / (v0*cos(szog));
}

bool lovedek_celozva(Lovedekfajta fajta, Vektor2d kiindulas, EllensegPredikcio ep) {
    Vektor2d d = v2d_kivon(ep.pozicio, kiindulas);
    double const v0 = lovedektipusok[fajta].kezdosebesseg;
    double szog = dobasszog_poziciobol(v0, d);
    // kiszámoljuk, hogy annyi idő múlva, amikor a lövedék odaérne, hol van az ellenség
    // ha pedig már célba ért akkorra, akkor is utánadobunk, a mostani pozíciójára (e_poz akkor nem változik)
    // még pontosabb célzásért lehet ismételgetni
    int const iteracio = 2; // a célzás pontosságát befolyásolja
    for (int i = 0; i < iteracio; ++i) {
        double ido = dobasido_szogbol(v0, d, szog);
        pozicio_megtett_utbol(ep.megtett_ut + ep.sebesseg * ido, &ep.pozicio);
        d = v2d_kivon(ep.pozicio, kiindulas);
        // majd oda célzunk
        szog = dobasszog_poziciobol(v0, d);
    }
    lovedek_letrehoz(fajta, kiindulas, szog, false);
    
    return d.x < 0; // balra lőtte: igaz, jobbra: hamis
}

void lovedekek_destrukcio(void) {
    kiurit(&lovedekek);
}

void lovedekek_frissites(double delta) {
    
    ListaElem *m = lovedekek.elso;
    while (m != NULL) {
        Lovedek *l = m->adat;
        
        if (!l->disz) {
            static Vektor2d const gravitacios_gyorsulas = {0, GRAVITACIO};
            l->sebesseg = v2d_osszead(l->sebesseg, v2d_nyujt(gravitacios_gyorsulas, delta));
            l->pozicio = v2d_osszead(l->pozicio, v2d_nyujt(l->sebesseg, delta));
        }
        
        // a képrajzolásnál a kép bal felső sarkát kell megadni, l->poz pedig a lövedék közepe
        Vektor2e const nagysag = szerez_kepinformacio(l->tipus->kep)->nagysag;
        Vektor2d const poz = v2d_kivon(l->pozicio, v2d_nyujt(v2d_v2ebol(nagysag), 0.5));
        double const szog = v2d_irany(l->sebesseg) * RADBOLFOK + 90; // +90° mert v2d_iranynál 0 jobbra mutat, a képrajzolónál pedig felfele
        kep_kirajzol_lebegopontos(l->tipus->kep, poz, 0, szog, false);
        
        bool talalt_a_loves = !l->disz && ellenseg_leloves(l->pozicio, l->tipus->fajta);
        if (talalt_a_loves) {
            penz_hozzaad(1);
            penz_animal();
        }
        
        // ha talált a lövés vagy kiment a képernyőről, akkor töröljük le (felfelé ne, mert visszaesik)
        ListaElem *uj_m = m->kovi;
        if (talalt_a_loves || l->pozicio.x < 0 || l->pozicio.x > ABLAK_X || l->pozicio.y > ABLAK_Y)
            letorol(&lovedekek, m);
        m = uj_m;
        
    }
}
