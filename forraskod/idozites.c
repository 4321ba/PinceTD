#include <stdio.h>
#include <SDL2/SDL.h>

#include "idozites.h"
#include "ablak.h"

static double szimulacio_sebesseg = NORMAL_SEBESSEG;

void szimulacio_sebesseg_allitas(double sebesseg) {
    szimulacio_sebesseg = sebesseg;
}

double szerez_szimulacio_sebesseg(void) {
    return szimulacio_sebesseg;
}

Uint32 szerez_eltelt_ido(void) {
    static Uint32 elozo_idopillanat = 0; // ezredmásodpercben
    
    Uint32 mostani = SDL_GetTicks();
    Uint32 eltelt_ido = mostani - elozo_idopillanat;
    elozo_idopillanat = mostani;
    return eltelt_ido;
}

// kps = kép per másodperc = fps
void kps_egy_kep_letrejott(Uint32 eltelt_ido) {
    static Uint32 eltelt_ido_kpshez = 0;
    static int kirajzolt_kepszam_kpshez = 0;
    
    eltelt_ido_kpshez += eltelt_ido;
    ++kirajzolt_kepszam_kpshez;
    if (eltelt_ido_kpshez >= 1000) {
        eltelt_ido_kpshez -= 1000;
        char kps_sztr[20];
        sprintf(kps_sztr, "%d kps", kirajzolt_kepszam_kpshez);
        kirajzolt_kepszam_kpshez = 0;
        beallit_ablakcim(kps_sztr);
    }
}
