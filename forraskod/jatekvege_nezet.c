#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL2_gfxPrimitives.h>

#include "jatekvege_nezet.h"
#include "nezetek.h"
#include "ablak.h"
#include "keprajzolo.h"
#include "hullamok.h"
#include "dicsoseglista.h"

// ha felkerül a listára az illető, akkor egy szövegdobozba beírhatja a nevét,
// ha nem, akkor pedig kiírjuk neki a dicsőséglistát, és alá az ő eredményét
// felkerul csak inicializációkor változzon
static bool felkerul;
static char nev[20 + 1];
// kurzor animációhoz:
static double eltelt_ido;

void jatekvege_nezet_inicializacio(void) {
    felkerul = dicsoseglista_felkerul_e(szerez_hullamszam());
    nev[0] = '\0';
    eltelt_ido = 0.0;
    if (felkerul)
        SDL_StartTextInput();
}

void jatekvege_nezet_destrukcio(void) {
    if (felkerul)
        SDL_StopTextInput();
}

void jatekvege_nezet_frissites(double delta) {
    
    static SDL_Color const szurke = {211, 191, 169, 255};
    Vektor2e poz = v2e(KARAKTER / 2, KARAKTER /2);
    szoveg_kirajzol("J\xA0t\x82k v\x82ge!", poz, &szurke, false);
    poz.y += KARAKTER * 5/4;
    szoveg_kirajzol("Az ellens\x82gek el\x82rt\x82k a pinc\x82t,", poz, &szurke, false);
    poz.y += KARAKTER * 5/4;
    szoveg_kirajzol("\x82s megitt\xa0k az \x94sszes m\xa0lnasz\x94rp\x94t...", poz, &szurke, false);
    poz.y += 5 * KARAKTER * 5/4;
    
    if (felkerul) {
        szoveg_kirajzol("Gratula, felker\x81lt\x82l a dics\x93s\x82glist\xa0ra!", poz, &szurke, false);
        poz.y += 2 * KARAKTER * 5/4;
        szoveg_kirajzol("Neved:", poz, &szurke, false);
        poz.x += KARAKTER * 7;
        rectangleRGBA(szerez_megjelenito(), poz.x - 4, poz.y - 4, poz.x + KARAKTER * 21 + 4, poz.y + KARAKTER + 4, szurke.r, szurke.g, szurke.b, szurke.a);
        szoveg_kirajzol(nev, poz, &szurke, false);
        
        // kurzor villogtatás:
        eltelt_ido += delta;
        bool kurzor_be = eltelt_ido < 0.5;
        if (eltelt_ido >= 1.0)
            eltelt_ido -= 1.0;
        if (kurzor_be) {
            poz.x += strlen(nev) * KARAKTER;
            szoveg_kirajzol("_", poz, &szurke, false);
        }
        
    } else {
        dicsoseglista_kirajzol(poz, &szurke);
        poz.y += 8 * KARAKTER * 5/4;
        char kiirando[64];
        sprintf(kiirando, " Te:%7d. hull\xa0mig jutott\xa0l", szerez_hullamszam());
        szoveg_kirajzol(kiirando, poz, &szurke, false);
    }
    szoveg_kirajzol("Enterrel l\x82phetsz tov\xa0\x62\x62", v2e(KARAKTER / 2, ABLAK_Y - KARAKTER * 3/2), &szurke, false);
}

// visszaadja egy több bájtos utf8 karakter modcp437 megfelelőjét, illetve, hogy hány bájtos
static unsigned char egy_kar_tobbbajtos_utf8bol(unsigned char const *be, int *hanybajtos) {
    if (be[0] >> 5 == 0x6) { // 0x6 = 110 bin, azaz 2 bájtos karakter
        *hanybajtos = 2;
        uint16_t unikod = (be[0] & 0x1f) << 6 | (be[1] & 0x3f);
        switch (unikod) {
            // unikód kódok innét: https://unicode-table.com/en/
            case 0x00e1: // á
                return 0xa0;
            case 0x00ed: // í
                return 0xa1;
            case 0x0171: // ű
                return 0x96;
            case 0x0151: // ő
                return 0x93;
            case 0x00fc: // ü
                return 0x81;
            case 0x00f6: // ö
                return 0x94;
            case 0x00fa: // ú
                return 0xa3;
            case 0x00f3: // ó
                return 0xa2;
            case 0x00e9: // é
                return 0x82;
                
            case 0x00c1: // Á
                return 0x8e;
            case 0x00cd: // Í
                return 0x8b;
            case 0x0170: // Ű
                return 0x98;
            case 0x0150: // Ő
                return 0x97;
            case 0x00dc: // Ü
                return 0x9a;
            case 0x00d6: // Ö
                return 0x99;
            case 0x00da: // Ú
                return 0x8c;
            case 0x00d3: // Ó
                return 0x8d;
            case 0x00c9: // É
                return 0x90;
        }
        return 0xa8; // '¿', lefele kérdőjel
    }
    if (be[0] >> 4 == 0xe) { // 0xe = 1110 bin, azaz 3 bájtos karakter
        *hanybajtos = 3;
        return 0xa8; // '¿', lefele kérdőjel
    }
    if (be[0] >> 3 != 0x1e) // 0x1e = 11110 bin, azaz 4 bájtos karakter
        // mást nem tudunk csinálni, mint jelezni, és megpróbálni továbbmenni, mintha mi sem történt volna
        fprintf(stderr, "Hiba a karakterkonverziónál, érvénytelen UTF-8 formátum!\n");
    *hanybajtos = 4;
    return 0xa8; // '¿', lefele kérdőjel
}

// a megadott sztringet módosítja úgy, hogy a saját, módosított szövegtípus (modositott_szovegtipus.h)
// által értelmezett szöveget készítsen belőle
// a 0x00-0x7F-ig meghagyja a karaktereket,
// az egyéb utf8 kódok közül a magyar ékezetes karaktereket átalakítja, a maradék helyére ¿-et rak be
// így maximum ugyanakkora lesz a kimenet, de lehet, hogy rövidebb
static void modcp437_utf8bol(unsigned char *szoveg) {
    int ki_idx = 0;
    for (int be_idx = 0; szoveg[be_idx] != '\0'; ++be_idx) {
        if (szoveg[be_idx] < 0x80) // ha ASCII a karakter
            szoveg[ki_idx++] = szoveg[be_idx];
        else {
            int hanybajtos;
            szoveg[ki_idx++] = egy_kar_tobbbajtos_utf8bol(szoveg+be_idx, &hanybajtos);
            be_idx += hanybajtos - 1; // -1, mert a ciklusfejlécben lép még egyet
        }
    }
    szoveg[ki_idx] = '\0';
}

void jatekvege_nezet_bemenet(SDL_Event esemeny) {
    if (esemeny.type == SDL_TEXTINPUT && felkerul) {
        modcp437_utf8bol((unsigned char *)esemeny.text.text);
        if (strlen(nev) + strlen(esemeny.text.text) <= 20)
            strcat(nev, esemeny.text.text);
    }
    else if (esemeny.type == SDL_KEYDOWN) { // különleges karakter
        if (esemeny.key.keysym.sym == SDLK_BACKSPACE && strlen(nev) > 0)
            nev[strlen(nev) - 1] = '\0';
        else if (esemeny.key.keysym.sym == SDLK_RETURN) {
            if (felkerul) {
                dicsoseglista_feltesz(szerez_hullamszam(), nev);
                nezetvaltas(DICSOSEGLISTA_NEZET);
            } else
                nezetvaltas(FOMENU_NEZET);
        }
    }
}
