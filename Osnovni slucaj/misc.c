#define _CRT_SECURE_NO_WARNINGS
#include "../GlavniDeoAplikacije/misc.h"
#include "../GlavniDeoAplikacije/datoteka.h"
#include "../GlavniDeoAplikacije/util.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

void obradi_osnovni_slucaj(PROIZVOD* proizvodi, int br_proizvoda,
    TRANSAKCIJA* transakcije, int br_transakcija,
    const char* mat_nova_path, const char* promene_rpt)
{
    char datum[10];
    danasnji_datum(datum, sizeof(datum));
    char tran_sum_path[256];
    sprintf(tran_sum_path, ".\\ASD\\DATA\\OLD\\tran_%s.dat", datum);

    FILE* fr = fopen(promene_rpt, "w");
    if (!fr) return;

    fprintf(fr, "Id    Naziv           Staro      Tip   Prom       Novo\n------------------------------------------------------------\n");

    TRANSAKCIJA sumirane[MAX_BR];
    int br_s = 0;

    for (int j = 0; j < br_proizvoda; j++) {
        int neto = 0;
        int nadjena = 0;

        for (int i = 0; i < br_transakcija; i++) {
            if (transakcije[i].Id == proizvodi[j].Id) {
                nadjena = 1;
                if (transakcije[i].Promena == ULAZ) neto += (int)transakcije[i].Kolicina;
                else neto -= (int)transakcije[i].Kolicina;
            }
        }

        if (nadjena) {
            unsigned staro = proizvodi[j].Kolicina;

            if (neto < 0 && (unsigned)abs(neto) > staro) {
                printf("GRESKA: Id %u bi otisao u minus! Postavljam na 0.\n", proizvodi[j].Id);
                proizvodi[j].Kolicina = 0;
            }
            else {
                proizvodi[j].Kolicina += neto;
            }

            sumirane[br_s].Id = proizvodi[j].Id;
            sumirane[br_s].Promena = (neto >= 0) ? ULAZ : IZLAZ;
            sumirane[br_s].Kolicina = (unsigned)abs(neto);
            br_s++;

            fprintf(fr, "%-5u %-15s %-10u %c %-10u %-10u\n",
                proizvodi[j].Id, proizvodi[j].Naziv, staro,
                (neto >= 0 ? '+' : '-'), abs(neto), proizvodi[j].Kolicina);
        }
    }

    FILE* fs = fopen(tran_sum_path, "wb");
    if (fs) {
        fwrite(sumirane, sizeof(TRANSAKCIJA), br_s, fs); 
        fclose(fs);
    }

    FILE* fm = fopen(mat_nova_path, "wb");
    if (fm) { 
        fwrite(proizvodi, sizeof(PROIZVOD), br_proizvoda, fm);
        fclose(fm); 
    }

    fclose(fr);
}