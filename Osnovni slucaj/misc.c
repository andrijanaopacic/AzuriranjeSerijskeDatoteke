#include "misc.h"
#include "kki.h"
#include "datoteka.h"
#include <stdio.h>
#include <stdlib.h>

void obradi_osnovni_slucaj(PROIZVOD* proizvodi, int br_proizvoda,
    TRANSAKCIJA* transakcije, int br_transakcija) {

    char datum[16];
    get_today_string(datum, sizeof(datum));

    // Fajlovi
    char prom_rpt[256], err_kol_rpt[256], err_pro_rpt[256], tran_sum[256], stara_mat[256];
    snprintf(prom_rpt, sizeof(prom_rpt), ".\\ASDProjekatAndrijanaAndjela\\ASD\\RPT\\prom_ % s.rpt", datum);
    snprintf(err_kol_rpt, sizeof(err_kol_rpt), ".\\ASDProjekatAndrijanaAndjela\\ASD\\ERR\\err_kol_%s.rpt", datum);
    snprintf(err_pro_rpt, sizeof(err_pro_rpt),".\\ASDProjekatAndrijanaAndjela\\ASD\\ERR\\err_pro_%s.rpt", datum);
    snprintf(tran_sum, sizeof(tran_sum),".\\ASDProjekatAndrijanaAndjela\\ASD\\DATA\\OLD\\tran_%s.dat", datum);
    snprintf(stara_mat, sizeof(stara_mat), ".\\ASDProjekatAndrijanaAndjela\\ASD\\DATA\\OLD\\mat_%s.dat", datum);

    // Otvaranje fajlova
    promene = fopen(prom_rpt, "w");
    err_kolicina = fopen(err_kol_rpt, "w");
    err_proizvod = fopen(err_pro_rpt, "w");

    if (!promene || !err_kolicina || !err_proizvod) {
        printf("Greska pri otvaranju izvestaja.\n");
        return;
    }

    copy_file(".\\ASDProjekatAndrijanaAndjela\\ASD\\DATA\\maticna.dat", stara_mat);

    // Sumiranje promena po proizvodu
    TRANSAKCIJA* sum_trans = (TRANSAKCIJA*)calloc(br_proizvoda, sizeof(TRANSAKCIJA));
    if (!sum_trans) return;

    for (int i = 0; i < br_proizvoda; i++) {
        sum_trans[i].Id = proizvodi[i].Id;
        sum_trans[i].Kolicina = 0;
        sum_trans[i].Promena = 0;
    }

    for (int i = 0; i < br_transakcija; i++) {
        int found = 0;
        for (int j = 0; j < br_proizvoda; j++) {
            if (transakcije[i].Id == proizvodi[j].Id) {
                found = 1;
                unsigned stara = proizvodi[j].Kolicina;

                if (transakcije[i].Promena == ULAZ) {
                    proizvodi[j].Kolicina += transakcije[i].Kolicina;
                    sum_trans[j].Kolicina += transakcije[i].Kolicina;
                    sum_trans[j].Promena = ULAZ;
                }
                else { // IZLAZ
                    if (transakcije[i].Kolicina <= proizvodi[j].Kolicina) {
                        proizvodi[j].Kolicina -= transakcije[i].Kolicina;
                        sum_trans[j].Kolicina += transakcije[i].Kolicina;
                        sum_trans[j].Promena = IZLAZ;
                    }
                    else {
                        ispisi_gresku_kolicina(&proizvodi[j], transakcije[i].Kolicina);
                    }
                }

                ispisi_promenu(&proizvodi[j], stara);
                break;
            }
        }
        if (!found) {
            ispisi_gresku_proizvod(transakcije[i].Id);
        }
    }

    // Snimi sumarnu transakcionu datoteku
    FILE* f_tran = fopen(tran_sum, "wb");
    if (f_tran) {
        for (int i = 0; i < br_proizvoda; i++) {
            if (sum_trans[i].Promena != 0)
                fwrite(&sum_trans[i], sizeof(TRANSAKCIJA), 1, f_tran);
        }
        fclose(f_tran);
    }

    free(sum_trans);

    FILE* f_mat = fopen(".\\ASDProjekatAndrijanaAndjela\\ASD\\DATA\\maticna.dat", "wb");
    if (f_mat) {
        fwrite(proizvodi, sizeof(PROIZVOD), br_proizvoda, f_mat);
        fclose(f_mat);
    }
    else {
        printf("Greska pri upisu maticne.dat\n");
    }

    fclose(promene);
    fclose(err_kolicina);
    fclose(err_proizvod);
}
