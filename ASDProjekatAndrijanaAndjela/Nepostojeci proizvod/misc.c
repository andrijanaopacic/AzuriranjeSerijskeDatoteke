#define _CRT_SECURE_NO_WARNINGS
#include "../GlavniDeoAplikacije/datoteka.h"
#include "../GlavniDeoAplikacije/misc.h"
#include "../GlavniDeoAplikacije/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void obradi_nepostojeci_proizvod(PROIZVOD* proizvodi, int br_proizvoda,
    TRANSAKCIJA* transakcije, int br_transakcija,
    const char* mat_nova_path, const char* promene_rpt)
{
    char datum[10];
    danasnji_datum(datum, sizeof(datum));

    char rpt_path[256], err_pro_path[256], tran_sum_path[256];
    sprintf(rpt_path, ".\\ASD\\RPT\\prom_%s.rpt", datum);
    sprintf(err_pro_path, ".\\ASD\\ERR\\err_pro_%s.rpt", datum);
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
    FILE* fe = fopen(err_pro_path, "w");
    if (!fr || !fe) return;

    fprintf(fr, "IZVESTAJ O PROMENAMA (SLUCAJ 4)\n%-5s %-15s %-10s %-5s %-10s %-10s\n", "Id", "Naziv", "Staro", "Tip", "Prom", "Novo");
    fprintf(fe, "IZVESTAJ O GRESKAMA - NEPOSTOJECI PROIZVOD\n%-5s %-10s %-15s %-5s %-10s %-20s\n", "Id", "Kol", "Naziv", "Tip", "Prom", "Greska");

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
            char gen_name[16];
            snprintf(gen_name, sizeof(gen_name), "Pro_%u", sumirane[i].Id);

            const char* tip = (sumirane[i].Promena == ULAZ) ? "ULAZ" : "IZLAZ";

            fprintf(fe, "%-5u %-10u %-15s %-7s %-10u %-20s\n",
                sumirane[i].Id,
                sumirane[i].Kolicina,
                gen_name,
                tip,
                sumirane[i].Kolicina,
                "Nepostojeci proizvod.");
        }
    }

    FILE* fm = fopen(mat_nova_path, "wb");
    if (fm) { fwrite(proizvodi, sizeof(PROIZVOD), br_proizvoda, fm); fclose(fm); }

    fclose(fr); fclose(fe);
}

void scenario_nepostojeci_proizvod() {
    const char mat_dat[] = ".\\ASD\\DATA\\maticna.dat";
    const char tran_dat[] = ".\\ASD\\DATA\\transakciona.dat";
    const char mat_nova[] = ".\\ASD\\DATA\\maticna.dat";

    char datum[10];
    danasnji_datum(datum, sizeof(datum));

    if (!fajl_postoji(mat_dat)) {
        kopiraj_fajl(".\\ASD\\DEMO\\maticna.dat", mat_dat);
    }
    if (!fajl_postoji(tran_dat)) {
        kopiraj_fajl(".\\ASD\\DEMO\\SLUC_4\\transakciona.dat", tran_dat);
    }

    demo_naslov("STARA MATICNA DATOTEKA");
    demo_ispisi_maticnu(mat_dat);
    demo_naslov("TRANSAKCIONA DATOTEKA (SLUCAJ 4)");
    demo_ispisi_transakcije(tran_dat);

    PROIZVOD p[MAX_BR];
    int br_p = ucitaj_maticnu(mat_dat, p, MAX_BR);
    TRANSAKCIJA t[MAX_BR];
    int br_t = ucitaj_transakcije(tran_dat, t, MAX_BR);


    obradi_nepostojeci_proizvod(p, br_p, t, br_t, mat_nova, NULL);


    char sum_p[256], prom_p[256], err_p[256];
    sprintf(sum_p, ".\\ASD\\DATA\\OLD\\tran_%s.dat", datum);
    sprintf(prom_p, ".\\ASD\\RPT\\prom_%s.rpt", datum);
    sprintf(err_p, ".\\ASD\\ERR\\err_pro_%s.rpt", datum);

    demo_naslov("SUMARNA TRANSAKCIONA (Agregirano)");
    demo_ispisi_transakcije(sum_p);

    demo_naslov("NOVA MATICNA DATOTEKA");
    demo_ispisi_maticnu(mat_nova);

    demo_naslov("IZVESTAJ O PROMENAMA");
    demo_stampaj_rpt(prom_p);

    demo_naslov("IZVESTAJ O GRESKAMA (Nepostojeci proizvod)");
    demo_stampaj_rpt(err_p);

    printf("\nStatus: Slucaj 4 zavrsen.\n");
}