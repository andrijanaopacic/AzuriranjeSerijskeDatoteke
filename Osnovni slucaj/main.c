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

    char datum[10];
    danasnji_datum(datum, sizeof(datum));
    char rpt_p[256], sum_p[256];
    sprintf(rpt_p, ".\\ASD\\RPT\\prom_%s.rpt", datum);
    sprintf(sum_p, ".\\ASD\\DATA\\OLD\\tran_%s.dat", datum);

    // --- USLOV IZ SRS-a ---
    // 1. Ako nema maticne u DATA, kopiraj pocetnu iz DEMO
    if (!fajl_postoji(mat_dat)) {
        printf("INFO: Maticna ne postoji, inicijalizujem iz DEMO...\n");
        kopiraj_fajl(".\\ASD\\DEMO\\maticna.dat", mat_dat);
    }

    // 2. Ako nema transakcione u DATA, kopiraj iz DEMO\SLUC_1
    if (!fajl_postoji(tran_dat)) {
        printf("INFO: Transakciona ne postoji, uzimam DEMO SLUC_1...\n");
        kopiraj_fajl(".\\ASD\\DEMO\\SLUC_1\\transakciona.dat", tran_dat);
    }

    // PRIKAZ PRE
    demo_naslov("STARA MATICNA DATOTEKA");
    demo_ispisi_maticnu(mat_dat);
    demo_naslov("TRANSAKCIONA DATOTEKA (SLUC_1)");
    demo_ispisi_transakcije(tran_dat);

    // UCITAVANJE
    PROIZVOD p[MAX_BR];
    int br_p = ucitaj_maticnu(mat_dat, p, MAX_BR);
    TRANSAKCIJA t[MAX_BR];
    int br_t = ucitaj_transakcije(tran_dat, t, MAX_BR);

    // OBRADA
    if (br_p > 0) {
        obradi_osnovni_slucaj(p, br_p, t, br_t, mat_dat, rpt_p);
    }

    // PRIKAZ POSLE
    demo_naslov("SUMARNA TRANSAKCIONA (AGREGIRANO)");
    demo_ispisi_transakcije(sum_p);
    demo_naslov("NOVA MATICNA DATOTEKA");
    demo_ispisi_maticnu(mat_dat);
    demo_naslov("IZVESTAJ O PROMENAMA");
    demo_stampaj_rpt(rpt_p);

    remove(tran_dat);

    return 0;
}