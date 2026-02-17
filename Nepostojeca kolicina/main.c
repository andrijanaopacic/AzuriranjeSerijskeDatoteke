#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../GlavniDeoAplikacije/util.h"
#include "../GlavniDeoAplikacije/datoteka.h"
#include "../GlavniDeoAplikacije/misc.h"
#include "../GlavniDeoAplikacije/kki.h"
#include "../GlavniDeoAplikacije/defs.h"

int main(void) {

    const char mat_dat[] = ".\\ASD\\DATA\\maticna.dat";
    const char tran_dat[] = ".\\ASD\\DATA\\transakciona.dat";
    const char mat_nova[] = ".\\ASD\\DATA\\maticna.dat";

    if (!fajl_postoji(mat_dat)) {
        printf("Prvo pokretanje: Kopiram pocetnu maticnu iz DEMO foldera...\n");
        kopiraj_fajl(".\\ASD\\DEMO\\maticna.dat", mat_dat);
    }

    if (!fajl_postoji(tran_dat)) {
        printf("Nema transakcija: Uzimam demo transakcije za SLUC_2...\n");
        kopiraj_fajl(".\\ASD\\DEMO\\SLUC_2\\transakciona.dat", tran_dat);
    }

    char datum[10];
    danasnji_datum(datum, sizeof(datum));

    demo_naslov("STARA MATICNA DATOTEKA");
    demo_ispisi_maticnu(mat_dat);
    demo_naslov("TRANSAKCIONA DATOTEKA (SLUCAJ 2)");
    demo_ispisi_transakcije(tran_dat);

    PROIZVOD proizvodi[MAX_BR];
    int br_p = ucitaj_maticnu(mat_dat, proizvodi, MAX_BR);

    TRANSAKCIJA transakcije[MAX_BR];
    int br_t = ucitaj_transakcije(tran_dat, transakcije, MAX_BR);

    obradi_nepostojecu_kolicinu(proizvodi, br_p, transakcije, br_t, mat_nova, NULL);

    char tran_sum[256], prom_rpt[256], err_rpt[256];
    sprintf(tran_sum, ".\\ASD\\DATA\\OLD\\tran_%s.dat", datum);
    sprintf(prom_rpt, ".\\ASD\\RPT\\prom_%s.rpt", datum);
    sprintf(err_rpt, ".\\ASD\\ERR\\err_kol_%s.rpt", datum);

    demo_naslov("SUMARNA TRANSAKCIONA DATOTEKA (Agregirano)");
    demo_ispisi_transakcije(tran_sum);

    demo_naslov("NOVA MATICNA DATOTEKA");
    demo_ispisi_maticnu(mat_nova);

    demo_naslov("IZVESTAJ O PROMENAMA");
    demo_stampaj_rpt(prom_rpt);

    demo_naslov("IZVESTAJ O GRESKAMA (Nepostojeca kolicina)");
    demo_stampaj_rpt(err_rpt);

    printf("\nStatus: Slucaj 2 uspesno zavrsen.\n");
    return 0;
}