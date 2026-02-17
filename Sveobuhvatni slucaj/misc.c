#define _CRT_SECURE_NO_WARNINGS
#include "../GlavniDeoAplikacije/misc.h"
#include "../GlavniDeoAplikacije/datoteka.h"
#include "../GlavniDeoAplikacije/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void obradi_sveobuhvatni_slucaj(PROIZVOD* proizvodi, int br_proizvoda,
    TRANSAKCIJA* transakcije, int br_transakcija,
    const char* mat_nova_path)
{
    char datum[10];
    danasnji_datum(datum, sizeof(datum));

    char rpt_prom[256], rpt_nov[256], rpt_err_kol[256], rpt_err_pro[256], tran_sum_path[256];
    sprintf(rpt_prom, ".\\ASD\\RPT\\prom_%s.rpt", datum);
    sprintf(rpt_nov, ".\\ASD\\RPT\\nov_pro_%s.rpt", datum);
    sprintf(rpt_err_kol, ".\\ASD\\ERR\\err_kol_%s.rpt", datum);
    sprintf(rpt_err_pro, ".\\ASD\\ERR\\err_pro_%s.rpt", datum);
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
                TRANSAKCIJA tmp = sumirane[i]; sumirane[i] = sumirane[j]; sumirane[j] = tmp;
            }
        }
    }
    FILE* fs = fopen(tran_sum_path, "wb");
    if (fs) { fwrite(sumirane, sizeof(TRANSAKCIJA), br_sum, fs); fclose(fs); }

    FILE* fp = fopen(rpt_prom, "w"), * fn = fopen(rpt_nov, "w"), * fk = fopen(rpt_err_kol, "w"), * fe = fopen(rpt_err_pro, "w");
    if (!fp || !fn || !fk || !fe) return;

    fprintf(fp, "IZVESTAJ O PROMENAMA\n%-5s %-15s %-10s %-5s %-10s %-10s\n", "Id", "Naziv", "Staro", "Tip", "Prom", "Novo");
    fprintf(fn, "IZVESTAJ O NOVIM PROIZVODIMA\n%-5s %-15s %-10s\n", "Id", "Naziv", "Kolicina");
    fprintf(fk, "IZVESTAJ - NEPOSTOJECA KOLICINA\n%-5s %-15s %-10s %-10s\n", "Id", "Naziv", "Stanje", "Trazena");
    fprintf(fe, "IZVESTAJ - NEPOSTOJECI PROIZVOD\n%-5s %-15s %-10s\n", "Id", "Tip", "Kolicina");

    PROIZVOD konacni_niz[MAX_BR];
    int br_konacni = 0;

    for (int j = 0; j < br_proizvoda; j++) {
        int index_t = -1;
        for (int i = 0; i < br_sum; i++) {
            if (sumirane[i].Id == proizvodi[j].Id) { index_t = i; break; }
        }

        if (index_t != -1) {
            int neto = (sumirane[index_t].Promena == ULAZ) ? (int)sumirane[index_t].Kolicina : -(int)sumirane[index_t].Kolicina;

            if (neto < 0 && (unsigned)abs(neto) > proizvodi[j].Kolicina) {
               
                fprintf(fk, "%-5u %-15s %-10u %-10u\n", proizvodi[j].Id, proizvodi[j].Naziv, proizvodi[j].Kolicina, (unsigned)abs(neto));
                konacni_niz[br_konacni++] = proizvodi[j];
            }
            else {
               
                unsigned staro = proizvodi[j].Kolicina;
                proizvodi[j].Kolicina += neto;
                fprintf(fp, "%-5u %-15s %-10u %c %-10u %-10u\n", proizvodi[j].Id, proizvodi[j].Naziv, staro, (neto >= 0 ? '+' : '-'), abs(neto), proizvodi[j].Kolicina);
                konacni_niz[br_konacni++] = proizvodi[j];
            }
        }
        else {
            konacni_niz[br_konacni++] = proizvodi[j];
        }
    }

    for (int i = 0; i < br_sum; i++) {
        int nadjen = 0;
        for (int j = 0; j < br_proizvoda; j++) {
            if (proizvodi[j].Id == sumirane[i].Id) { nadjen = 1; break; }
        }

        if (!nadjen) {
            if (sumirane[i].Promena == ULAZ) {
                PROIZVOD nov; nov.Id = sumirane[i].Id; sprintf(nov.Naziv, "Pro_%u", nov.Id); nov.Kolicina = sumirane[i].Kolicina;
                konacni_niz[br_konacni++] = nov;
                fprintf(fn, "%-5u %-15s %-10u\n", nov.Id, nov.Naziv, nov.Kolicina);
            }
            else {
                fprintf(fe, "%-5u %-15s %-10u\n", sumirane[i].Id, "-", sumirane[i].Kolicina);
            }
        }
    }

    for (int i = 0; i < br_konacni - 1; i++) {
        for (int j = i + 1; j < br_konacni; j++) {
            if (konacni_niz[i].Id > konacni_niz[j].Id) {
                PROIZVOD tmp = konacni_niz[i]; konacni_niz[i] = konacni_niz[j]; konacni_niz[j] = tmp;
            }
        }
    }

    FILE* fm = fopen(mat_nova_path, "wb");
    if (fm) { fwrite(konacni_niz, sizeof(PROIZVOD), br_konacni, fm); fclose(fm); }

    fclose(fp); fclose(fn); fclose(fk); fclose(fe);
}