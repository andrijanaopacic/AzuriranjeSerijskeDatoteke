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

void scenario_osnovni_slucaj() {
    const char mat_dat[] = ".\\ASD\\DATA\\maticna.dat";
    const char tran_dat[] = ".\\ASD\\DATA\\transakciona.dat";

    char datum[10];
    danasnji_datum(datum, sizeof(datum));
    char rpt_p[256], sum_p[256];
    sprintf(rpt_p, ".\\ASD\\RPT\\prom_%s.rpt", datum);
    sprintf(sum_p, ".\\ASD\\DATA\\OLD\\tran_%s.dat", datum);

    if (!fajl_postoji(mat_dat)) {
        printf("INFO: Maticna ne postoji, inicijalizujem iz DEMO...\n");
        kopiraj_fajl(".\\ASD\\DEMO\\maticna.dat", mat_dat);
    }

    if (!fajl_postoji(tran_dat)) {
        printf("INFO: Transakciona ne postoji, uzimam DEMO SLUC_1...\n");
        kopiraj_fajl(".\\ASD\\DEMO\\SLUC_1\\transakciona.dat", tran_dat);
    }

    demo_naslov("STARA MATICNA DATOTEKA");
    demo_ispisi_maticnu(mat_dat);
    demo_naslov("TRANSAKCIONA DATOTEKA (SLUC_1)");
    demo_ispisi_transakcije(tran_dat);

    PROIZVOD p[MAX_BR];
    int br_p = ucitaj_maticnu(mat_dat, p, MAX_BR);
    TRANSAKCIJA t[MAX_BR];
    int br_t = ucitaj_transakcije(tran_dat, t, MAX_BR);

    if (br_p > 0) {
        obradi_osnovni_slucaj(p, br_p, t, br_t, mat_dat, rpt_p);
    }

  
    demo_naslov("SUMARNA TRANSAKCIONA (AGREGIRANO)");
    demo_ispisi_transakcije(sum_p);
    demo_naslov("NOVA MATICNA DATOTEKA");
    demo_ispisi_maticnu(mat_dat);
    demo_naslov("IZVESTAJ O PROMENAMA");
    demo_stampaj_rpt(rpt_p);


}
