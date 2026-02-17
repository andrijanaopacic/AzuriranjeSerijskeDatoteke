#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../GlavniDeoAplikacije/defs.h"
#include "../GlavniDeoAplikacije/datoteka.h"
#include "../GlavniDeoAplikacije/misc.h"
#include "../GlavniDeoAplikacije/kki.h"
#include "../GlavniDeoAplikacije/util.h"

int main(void) {
    const char mat_dat[] = ".\\ASD\\DATA\\maticna.dat";
    const char tran_dat[] = ".\\ASD\\DATA\\transakciona.dat";
    const char mat_nova[] = ".\\ASD\\DATA\\maticna.dat";

    char datum[10];
    danasnji_datum(datum, sizeof(datum));

    if (!fajl_postoji(mat_dat)) {
        kopiraj_fajl(".\\ASD\\DEMO\\maticna.dat", mat_dat);
    }
    if (!fajl_postoji(tran_dat)) {
        kopiraj_fajl(".\\ASD\\DEMO\\SLUC_3\\transakciona.dat", tran_dat);
    }

    demo_naslov("STARA MATICNA DATOTEKA");
    demo_ispisi_maticnu(mat_dat);
    demo_naslov("TRANSAKCIONA DATOTEKA (SLUCAJ 3 - Novi proizvodi)");
    demo_ispisi_transakcije(tran_dat);

    PROIZVOD p[MAX_BR];
    int br_p = ucitaj_maticnu(mat_dat, p, MAX_BR);
    TRANSAKCIJA t[MAX_BR];
    int br_t = ucitaj_transakcije(tran_dat, t, MAX_BR);

    obradi_nov_proizvod(p, br_p, t, br_t, mat_nova, NULL);

    char sum_p[256], prom_p[256], nov_p[256];
    sprintf(sum_p, ".\\ASD\\DATA\\OLD\\tran_%s.dat", datum);
    sprintf(prom_p, ".\\ASD\\RPT\\prom_%s.rpt", datum);
    sprintf(nov_p, ".\\ASD\\RPT\\nov_pro_%s.rpt", datum);

    demo_naslov("SUMARNA TRANSAKCIONA (Agregirano)");
    demo_ispisi_transakcije(sum_p);

    demo_naslov("NOVA MATICNA DATOTEKA (Sadrzi i nove proizvode)");
    demo_ispisi_maticnu(mat_nova);

    demo_naslov("IZVESTAJ O PROMENAMA");
    demo_stampaj_rpt(prom_p);

    demo_naslov("IZVESTAJ O NOVIM PROIZVODIMA");
    demo_stampaj_rpt(nov_p);

    remove(tran_dat);
    printf("\nStatus: Slucaj 3 zavrsen.\n");
    return 0;
}