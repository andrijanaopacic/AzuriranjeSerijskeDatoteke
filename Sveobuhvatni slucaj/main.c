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

    char datum[10]; danasnji_datum(datum, sizeof(datum));
    char rpt_prom[256], rpt_nov[256], rpt_err_kol[256], rpt_err_pro[256], tran_sum[256];

    sprintf(rpt_prom, ".\\ASD\\RPT\\prom_%s.rpt", datum);
    sprintf(rpt_nov, ".\\ASD\\RPT\\nov_pro_%s.rpt", datum);
    sprintf(rpt_err_kol, ".\\ASD\\ERR\\err_kol_%s.rpt", datum);
    sprintf(rpt_err_pro, ".\\ASD\\ERR\\err_pro_%s.rpt", datum);
    sprintf(tran_sum, ".\\ASD\\DATA\\OLD\\tran_%s.dat", datum);

    if (!fajl_postoji(mat_dat)) {
        kopiraj_fajl(".\\ASD\\DEMO\\maticna.dat", mat_dat);
    }
    if (!fajl_postoji(tran_dat)) {
        kopiraj_fajl(".\\ASD\\DEMO\\SLUC_5\\transakciona.dat", tran_dat);
    }

    demo_naslov("STARA MATICNA DATOTEKA");
    demo_ispisi_maticnu(mat_dat);
    demo_naslov("TRANSAKCIONA DATOTEKA (SLUCAJ 5)");
    demo_ispisi_transakcije(tran_dat);

    PROIZVOD p[MAX_BR];
    int br_p = ucitaj_maticnu(mat_dat, p, MAX_BR);
    TRANSAKCIJA t[MAX_BR];
    int br_t = ucitaj_transakcije(tran_dat, t, MAX_BR);

    obradi_sveobuhvatni_slucaj(p, br_p, t, br_t, mat_nova);

    demo_naslov("SUMARNA TRANSAKCIONA (Agregirano)");
    demo_ispisi_transakcije(tran_sum);

    demo_naslov("NOVA MATICNA DATOTEKA");
    demo_ispisi_maticnu(mat_nova);

    demo_naslov("IZVESTAJ O PROMENAMA");
    demo_stampaj_rpt(rpt_prom);

    demo_naslov("IZVESTAJ O NOVIM PROIZVODIMA");
    demo_stampaj_rpt(rpt_nov);

    demo_naslov("GRESKE - KOLICINA");
    demo_stampaj_rpt(rpt_err_kol);

    demo_naslov("GRESKE - NEPOSTOJECI PROIZVOD");
    demo_stampaj_rpt(rpt_err_pro);

    remove(tran_dat);
    printf("\nStatus: Sveobuhvatni slucaj zavrsen.\n");
    return 0;
}