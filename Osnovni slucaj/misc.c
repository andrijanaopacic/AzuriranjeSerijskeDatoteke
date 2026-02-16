#define _CRT_SECURE_NO_WARNINGS
#include "misc.h"
#include "datoteka.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void obradi_osnovni_slucaj(PROIZVOD* proizvodi, int br_proizvoda,
    TRANSAKCIJA* transakcije, int br_transakcija,
    const char* mat_nova_path, const char* promene_rpt)
{
    char rpt_putanja[256];
    char datum[10];
    danasnji_datum(datum, sizeof(datum));

    if (promene_rpt == NULL) {
        sprintf(rpt_putanja, ".\\ASD\\RPT\\prom_%s.rpt", datum);
    }
    else {
        strcpy(rpt_putanja, promene_rpt);
    }

    char tran_sum_path[256];
    sprintf(tran_sum_path, ".\\ASD\\DATA\\OLD\\tran_%s.dat", datum);

    // 1. OTVARANJE IZVESTAJA
    FILE* f_prom = fopen(rpt_putanja, "w");
    if (!f_prom) return;

    fprintf(f_prom, "Izvestaj o promenama za dan %s\n", datum);
    fprintf(f_prom, "%-5s %-15s %-10s %-5s %-10s %-10s\n", "Id", "Naziv", "Staro", "Tip", "Prom", "Novo");
    fprintf(f_prom, "------------------------------------------------------------\n");

    // 2. AGREGACIJA I AZURIRANJE (Sortirano po Id-u)
    TRANSAKCIJA sumirane[100];
    int br_sumiranih = 0;

    // Idemo redom kroz PROIZVODE (jer su oni vec sortirani po Id-u)
    for (int j = 0; j < br_proizvoda; j++) {
        int ukupna_suma = 0;
        int nadjena_transakcija = 0;

        // Za trenutni proizvod trazimo sve njegove transakcije
        for (int i = 0; i < br_transakcija; i++) {
            if (transakcije[i].Id == proizvodi[j].Id) {
                nadjena_transakcija = 1;
                if (transakcije[i].Promena == ULAZ) {
                    ukupna_suma += (int)transakcije[i].Kolicina;
                }
                else {
                    ukupna_suma -= (int)transakcije[i].Kolicina;
                }
            }
        }

        // Ako smo nasli bar jednu transakciju za ovaj proizvod
        if (nadjena_transakcija) {
            unsigned staro_stanje = proizvodi[j].Kolicina;

            // Azuriramo stanje u memoriji
            proizvodi[j].Kolicina += ukupna_suma;

            // Dodajemo u sumarnu listu (koja ce sada biti sortirana)
            sumirane[br_sumiranih].Id = proizvodi[j].Id;
            sumirane[br_sumiranih].Promena = (ukupna_suma >= 0) ? ULAZ : IZLAZ;
            sumirane[br_sumiranih].Kolicina = (unsigned)abs(ukupna_suma);
            br_sumiranih++;

            // Upisujemo u tekstualni izvestaj (rpt)
            fprintf(f_prom, "%-5u %-15s %-10u %-5s %-10u %-10u\n",
                proizvodi[j].Id,
                proizvodi[j].Naziv,
                staro_stanje,
                (ukupna_suma >= 0 ? "+" : "-"),
                abs(ukupna_suma),
                proizvodi[j].Kolicina);
        }
    }

    // 3. SNIMANJE BINARNIH DATOTEKA
    // Snimi sumarnu transakcionu (tran_ggmmdd.dat)
    FILE* f_tran_sum = fopen(tran_sum_path, "wb");
    if (f_tran_sum) {
        fwrite(sumirane, sizeof(TRANSAKCIJA), br_sumiranih, f_tran_sum);
        fclose(f_tran_sum);
    }

    // Snimi novu maticnu (maticna.dat)
    FILE* f_mat = fopen(mat_nova_path, "wb");
    if (f_mat) {
        fwrite(proizvodi, sizeof(PROIZVOD), br_proizvoda, f_mat);
        fclose(f_mat);
    }

    fclose(f_prom);
}