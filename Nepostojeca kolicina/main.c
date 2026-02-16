#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "datoteka.h"
#include "misc.h"
#include "kki.h"

int main(void) {
    const char mat_dat[] = ".\\ASD\\DATA\\maticna.dat";
    const char tran_dat[] = ".\\ASD\\DATA\\transakciona.dat";
    const char mat_nova[] = ".\\ASD\\DATA\\maticna.dat";

    char datum[10];
    danasnji_datum(datum, sizeof(datum));

    // OBAVEZNO KOPIRANJE DEMO PODATAKA ZA SLUCAJ 2
    kopiraj_fajl(".\\ASD\\DEMO\\maticna.dat", mat_dat);
    kopiraj_fajl(".\\ASD\\DEMO\\SLUC_2\\transakciona.dat", tran_dat);

    PROIZVOD proizvodi[100];
    int br_proizvoda = ucitaj_maticnu(mat_dat, proizvodi, 100);

    TRANSAKCIJA transakcije[100];
    int br_transakcija = ucitaj_transakcije(tran_dat, transakcije, 100);

    // PRIKAZ PRE
    demo_naslov("STARA MATICNA");
    demo_ispisi_maticnu(mat_dat);
    demo_naslov("TRANSAKCIJE SLUCAJ 2");
    demo_ispisi_transakcije(tran_dat);

    // OBRADA
    obradi_nepostojecu_kolicinu(proizvodi, br_proizvoda, transakcije, br_transakcija, mat_nova, NULL);

    // PRIKAZ POSLE
    char tran_sum[256], prom_rpt[256], err_rpt[256];
    sprintf(tran_sum, ".\\ASD\\DATA\\OLD\\tran_%s.dat", datum);
    sprintf(prom_rpt, ".\\ASD\\RPT\\prom_%s.rpt", datum);
    sprintf(err_rpt, ".\\ASD\\ERR\\err_kol_%s.rpt", datum);

    demo_naslov("SUMARNA DATOTEKA (Agregirano)");
    demo_ispisi_transakcije(tran_sum);

    demo_naslov("NOVA MATICNA");
    demo_ispisi_maticnu(mat_nova);

    demo_naslov("IZVESTAJ O PROMENAMA");
    demo_stampaj_rpt(prom_rpt);

    demo_naslov("IZVESTAJ O GRESKAMA");
    demo_stampaj_rpt(err_rpt);

    return 0;
}