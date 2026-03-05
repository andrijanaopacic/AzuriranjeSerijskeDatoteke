#define _CRT_SECURE_NO_WARNINGS
#include "../GlavniDeoAplikacije/misc.h"
#include "../GlavniDeoAplikacije/datoteka.h"
#include "../GlavniDeoAplikacije/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

void obradi_nepostojecu_kolicinu(PROIZVOD* proizvodi, int br_proizvoda, TRANSAKCIJA* transakcije, int br_transakcija, const char* mat_nova_path, const char* promene_rpt) {
    char datum[10]; danasnji_datum(datum, sizeof(datum));
    char tran_sum_path[256], rpt_p[256], err_p[256];
    sprintf(tran_sum_path, ".\\ASD\\DATA\\OLD\\tran_%s.dat", datum);
    sprintf(err_p, ".\\ASD\\ERR\\err_kol_%s.rpt", datum);
    if (promene_rpt == NULL) sprintf(rpt_p, ".\\ASD\\RPT\\prom_%s.rpt", datum);
    else strcpy(rpt_p, promene_rpt);

    FILE* fr = fopen(rpt_p, "w"); FILE* fe = fopen(err_p, "w");
    fprintf(fr, "IZVESTAJ O PROMENAMA\n"); fprintf(fe, "IZVESTAJ O GRESKAMA\n");

    TRANSAKCIJA sumirane[MAX_BR];
    int br_s = 0;

    for (int j = 0; j < br_proizvoda; j++) {
        int neto = 0; int nadjena = 0;
        for (int i = 0; i < br_transakcija; i++) {
            if (transakcije[i].Id == proizvodi[j].Id) {
                nadjena = 1;
                neto += (transakcije[i].Promena == ULAZ) ? (int)transakcije[i].Kolicina : -((int)transakcije[i].Kolicina);
            }
        }
        if (nadjena) {
            if (neto < 0 && (unsigned)abs(neto) > proizvodi[j].Kolicina) {
                fprintf(fe, "%-5u %-10u %-15s %c %-10u Nepostojeca kolicina.\n", proizvodi[j].Id, proizvodi[j].Kolicina, proizvodi[j].Naziv, '-', abs(neto));
            }
            else {
                unsigned staro = proizvodi[j].Kolicina;
                proizvodi[j].Kolicina += neto;
                sumirane[br_s].Id = proizvodi[j].Id;
                sumirane[br_s].Promena = (neto >= 0) ? ULAZ : IZLAZ;
                sumirane[br_s].Kolicina = (unsigned)abs(neto);
                br_s++;
                fprintf(fr, "%-5u %-15s %-10u %c %-10u %-10u\n", proizvodi[j].Id, proizvodi[j].Naziv, staro, (neto >= 0 ? '+' : '-'), abs(neto), proizvodi[j].Kolicina);
            }
        }
    }
    FILE* fs = fopen(tran_sum_path, "wb");
    if (fs) { fwrite(sumirane, sizeof(TRANSAKCIJA), br_s, fs); fclose(fs); }
    FILE* fm = fopen(mat_nova_path, "wb");
    if (fm) { fwrite(proizvodi, sizeof(PROIZVOD), br_proizvoda, fm); fclose(fm); }
    fclose(fr); fclose(fe);
}

void scenario_nepostojeca_kolicina() {
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
}