#define _CRT_SECURE_NO_WARNINGS
#include "misc.h"
#include "datoteka.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void obradi_nepostojecu_kolicinu(PROIZVOD* proizvodi, int br_proizvoda,
    TRANSAKCIJA* transakcije, int br_transakcija,
    const char* mat_nova_path, const char* promene_rpt)
{
    char datum[10];
    danasnji_datum(datum, sizeof(datum));

    char rpt_path[256], err_path[256], tran_sum_path[256];
    sprintf(rpt_path, ".\\ASD\\RPT\\prom_%s.rpt", datum);
    sprintf(err_path, ".\\ASD\\ERR\\err_kol_%s.rpt", datum);
    sprintf(tran_sum_path, ".\\ASD\\DATA\\OLD\\tran_%s.dat", datum);

    // OBRISI STARE FAJLOVE da ne bi gledala stare rezultate
    remove(rpt_path);
    remove(err_path);
    remove(tran_sum_path);

    TRANSAKCIJA sumirane[100];
    int br_sum = 0;

    // 1. AGREGACIJA (Sumiramo transakcije prolaskom kroz sortirane proizvode)
    for (int j = 0; j < br_proizvoda; j++) {
        int neto = 0;
        int nadjena = 0;
        for (int i = 0; i < br_transakcija; i++) {
            if (transakcije[i].Id == proizvodi[j].Id) {
                nadjena = 1;
                neto += (transakcije[i].Promena == ULAZ) ? transakcije[i].Kolicina : -(int)transakcije[i].Kolicina;
            }
        }
        if (nadjena) {
            sumirane[br_sum].Id = proizvodi[j].Id;
            sumirane[br_sum].Promena = (neto >= 0) ? ULAZ : IZLAZ;
            sumirane[br_sum].Kolicina = abs(neto);
            br_sum++;
        }
    }

    // Snimi sumarnu (tran_ggmmdd.dat) - OVO JE SADA TACNO SORTIRANO
    FILE* fs = fopen(tran_sum_path, "wb");
    if (fs) { fwrite(sumirane, sizeof(TRANSAKCIJA), br_sum, fs); fclose(fs); }

    // 2. OBRADA I IZRADA IZVESTAJA
    FILE* fr = fopen(rpt_path, "w");
    FILE* fe = fopen(err_path, "w");
    if (!fr || !fe) return;

    fprintf(fr, "IZVESTAJ O PROMENAMA\n--------------------\n");
    fprintf(fr, "%-5s %-10s %-15s %-5s %-10s %-10s\n", "Id", "Kol", "Naziv", "Tip", "Prom", "Nova");

    fprintf(fe, "IZVESTAJ O GRESKAMA\n-------------------\n");
    fprintf(fe, "%-5s %-10s %-15s %-5s %-10s %-20s\n", "Id", "Kol", "Naziv", "Tip", "Prom", "Greska");

    for (int i = 0; i < br_sum; i++) {
        for (int j = 0; j < br_proizvoda; j++) {
            if (sumirane[i].Id == proizvodi[j].Id) {
                if (sumirane[i].Promena == IZLAZ && sumirane[i].Kolicina > proizvodi[j].Kolicina) {
                    // SCENARIO (v) - GREŠKA
                    fprintf(fe, "%-5u %-10u %-15s %-5s %-10u %-20s\n",
                        proizvodi[j].Id, proizvodi[j].Kolicina, proizvodi[j].Naziv, "-",
                        sumirane[i].Kolicina, "Nepostojeca kolicina proizvoda.");
                }
                else {
                    // SCENARIO (a, b) - USPEŠNO
                    unsigned staro = proizvodi[j].Kolicina;
                    if (sumirane[i].Promena == ULAZ) proizvodi[j].Kolicina += sumirane[i].Kolicina;
                    else proizvodi[j].Kolicina -= sumirane[i].Kolicina;

                    fprintf(fr, "%-5u %-10u %-15s %-5s %-10u %-10u\n",
                        proizvodi[j].Id, staro, proizvodi[j].Naziv,
                        (sumirane[i].Promena == ULAZ ? "+" : "-"),
                        sumirane[i].Kolicina, proizvodi[j].Kolicina);
                }
                break;
            }
        }
    }

    FILE* fm = fopen(mat_nova_path, "wb");
    if (fm) { fwrite(proizvodi, sizeof(PROIZVOD), br_proizvoda, fm); fclose(fm); }

    fclose(fr);
    fclose(fe);
}