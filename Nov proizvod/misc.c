#define _CRT_SECURE_NO_WARNINGS
#include "../GlavniDeoAplikacije/misc.h"
#include "../GlavniDeoAplikacije/datoteka.h"
#include "../GlavniDeoAplikacije/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void obradi_nov_proizvod(PROIZVOD* proizvodi, int br_proizvoda,
    TRANSAKCIJA* transakcije, int br_transakcija,
    const char* mat_nova_path, const char* promene_rpt)
{
    char datum[10];
    danasnji_datum(datum, sizeof(datum));

    char rpt_path[256], nov_pro_path[256], tran_sum_path[256];
    sprintf(rpt_path, ".\\ASD\\RPT\\prom_%s.rpt", datum);
    sprintf(nov_pro_path, ".\\ASD\\RPT\\nov_pro_%s.rpt", datum);
    sprintf(tran_sum_path, ".\\ASD\\DATA\\OLD\\tran_%s.dat", datum);

    TRANSAKCIJA sumirane[MAX_BR];
    int br_sum = 0;

    for (int i = 0; i < br_transakcija; i++) {
        int index = -1;
        for (int k = 0; k < br_sum; k++) {
            if (sumirane[k].Id == transakcije[i].Id) { index = k; break; }
        }
        if (index != -1) {
            int s = (sumirane[index].Promena == ULAZ) ? (int)sumirane[index].Kolicina : -(int)sumirane[index].Kolicina;
            int t = (transakcije[i].Promena == ULAZ) ? (int)transakcije[i].Kolicina : -(int)transakcije[i].Kolicina;
            int ukupno = s + t;
            sumirane[index].Promena = (ukupno >= 0) ? ULAZ : IZLAZ;
            sumirane[index].Kolicina = (unsigned)abs(ukupno);
        }
        else {
            sumirane[br_sum++] = transakcije[i];
        }
    }

    for (int i = 0; i < br_sum - 1; i++) {
        for (int j = i + 1; j < br_sum; j++) {
            if (sumirane[i].Id > sumirane[j].Id) {
                TRANSAKCIJA temp = sumirane[i];
                sumirane[i] = sumirane[j];
                sumirane[j] = temp;
            }
        }
    }
    FILE* fs = fopen(tran_sum_path, "wb");
    if (fs) { fwrite(sumirane, sizeof(TRANSAKCIJA), br_sum, fs); fclose(fs); }

    FILE* fr = fopen(rpt_path, "w");
    FILE* fn = fopen(nov_pro_path, "w");
    if (!fr || !fn) return;

    fprintf(fr, "IZVESTAJ O PROMENAMA (SLUCAJ 3)\n%-5s %-15s %-10s %-5s %-10s %-10s\n", "Id", "Naziv", "Staro", "Tip", "Prom", "Novo");
    fprintf(fn, "IZVESTAJ O NOVIM PROIZVODIMA\n%-5s %-15s %-10s\n", "Id", "Naziv", "Kolicina");

    PROIZVOD nova_maticna_niz[MAX_BR];
    int br_novih_ukupno = 0;

    for (int i = 0; i < br_sum; i++) {
        int nadjen_u_maticnoj = -1;
        for (int j = 0; j < br_proizvoda; j++) {
            if (proizvodi[j].Id == sumirane[i].Id) { nadjen_u_maticnoj = j; break; }
        }

        if (nadjen_u_maticnoj != -1) {
            unsigned staro = proizvodi[nadjen_u_maticnoj].Kolicina;
            int prom = (sumirane[i].Promena == ULAZ) ? (int)sumirane[i].Kolicina : -(int)sumirane[i].Kolicina;
            proizvodi[nadjen_u_maticnoj].Kolicina += prom;

            fprintf(fr, "%-5u %-15s %-10u %c %-10u %-10u\n",
                proizvodi[nadjen_u_maticnoj].Id, proizvodi[nadjen_u_maticnoj].Naziv, staro,
                (prom >= 0 ? '+' : '-'), abs(prom), proizvodi[nadjen_u_maticnoj].Kolicina);
        }
        else {
            if (sumirane[i].Promena == ULAZ) {
                PROIZVOD nov;
                nov.Id = sumirane[i].Id;
                sprintf(nov.Naziv, "Pro_%u", nov.Id);
                nov.Kolicina = sumirane[i].Kolicina;

                nova_maticna_niz[br_novih_ukupno++] = nov;

                fprintf(fn, "%-5u %-15s %-10u\n", nov.Id, nov.Naziv, nov.Kolicina);
            }
        }
    }

    PROIZVOD konacni[MAX_BR];
    int ukupno = 0;
    for (int i = 0; i < br_proizvoda; i++) konacni[ukupno++] = proizvodi[i];
    for (int i = 0; i < br_novih_ukupno; i++) konacni[ukupno++] = nova_maticna_niz[i];

    for (int i = 0; i < ukupno - 1; i++) {
        for (int j = i + 1; j < ukupno; j++) {
            if (konacni[i].Id > konacni[j].Id) {
                PROIZVOD temp = konacni[i];
                konacni[i] = konacni[j];
                konacni[j] = temp;
            }
        }
    }

    FILE* fm = fopen(mat_nova_path, "wb");
    if (fm) { fwrite(konacni, sizeof(PROIZVOD), ukupno, fm); fclose(fm); }

    fclose(fr); fclose(fn);
}