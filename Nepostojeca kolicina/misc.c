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