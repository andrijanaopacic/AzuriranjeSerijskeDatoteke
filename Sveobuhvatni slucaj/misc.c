#define _CRT_SECURE_NO_WARNINGS
#include "../GlavniDeoAplikacije/misc.h"
#include "../GlavniDeoAplikacije/datoteka.h"
#include "../GlavniDeoAplikacije/util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

    FILE* fp = fopen(rpt_prom, "w"), * fn = fopen(rpt_nov, "w"), * fk = fopen(rpt_err_kol, "w"), * fe = fopen(rpt_err_pro, "w");
    if (!fp || !fn || !fk || !fe) return;

    // Zaglavlja prema specifikaciji
    fprintf(fp, "IZVESTAJ O PROMENAMA ZA DAN %s\n\n", datum);
    fprintf(fp, "Proizvod           Promena    Nova\n");
    fprintf(fp, "%-5s %-10s %-15s %-3s %-10s %-10s\n", "Id", "Kolicina", "Naziv", "Tip", "Kol", "kolicina");
    fprintf(fp, "------------------------------------------------------------\n");

    fprintf(fn, "IZVESTAJ O NOVIM PROIZVODIMA ZA DAN %s\n\n", datum);
    fprintf(fn, "%-5s %-15s %-10s\n", "Id", "Proizvod", "Kolicina");
    fprintf(fn, "------------------------------------------\n");

    fprintf(fk, "IZVESTAJ O GRESKAMA (KOLICINA) ZA DAN %s\n\n", datum);
    fprintf(fk, "Proizvod                       Promena    Greska\n");
    fprintf(fk, "%-5s %-10s %-15s %-3s %-10s %-20s\n", "Id", "Kolicina", "Naziv", "Tip", "Kol", "");
    fprintf(fk, "----------------------------------------------------------------------\n");

    fprintf(fe, "IZVESTAJ O GRESKAMA (NEPOSTOJECI) ZA DAN %s\n\n", datum);
    fprintf(fe, "Proizvod                       Promena    Greska\n");
    fprintf(fe, "%-5s %-10s %-15s %-3s %-10s %-20s\n", "Id", "Kolicina", "Naziv", "Tip", "Kol", "");
    fprintf(fe, "----------------------------------------------------------------------\n");

    PROIZVOD konacni_niz[MAX_BR];
    int br_konacni = 0;
    TRANSAKCIJA sumirane[MAX_BR];
    int br_sum = 0;

    unsigned svi_id[MAX_BR];
    int br_id = 0;
    for (int i = 0; i < br_transakcija; i++) {
        int nadjen = 0;
        for (int j = 0; j < br_id; j++) if (svi_id[j] == transakcije[i].Id) nadjen = 1;
        if (!nadjen) svi_id[br_id++] = transakcije[i].Id;
    }

    for (int i = 0; i < br_id; i++) {
        unsigned curr_id = svi_id[i];
        unsigned u_ukupno = 0, i_ukupno = 0;
        for (int t = 0; t < br_transakcija; t++) {
            if (transakcije[t].Id == curr_id) {
                if (transakcije[t].Promena == ULAZ) u_ukupno += transakcije[t].Kolicina;
                else i_ukupno += transakcije[t].Kolicina;
            }
        }

        int index_m = -1;
        for (int j = 0; j < br_proizvoda; j++) if (proizvodi[j].Id == curr_id) index_m = j;

        if (index_m != -1) {
            if ((int)proizvodi[index_m].Kolicina + (int)u_ukupno - (int)i_ukupno < 0) {
                fprintf(fk, "%-5u %-10u %-15s %-3s %-10u %s\n", proizvodi[index_m].Id, proizvodi[index_m].Kolicina, proizvodi[index_m].Naziv, "-", i_ukupno, "Nepostojeca kolicina proizvoda.");
                if (u_ukupno > 0) {
                    unsigned staro = proizvodi[index_m].Kolicina;
                    proizvodi[index_m].Kolicina += u_ukupno;
                    fprintf(fp, "%-5u %-10u %-15s %c   %-10u %-10u\n", proizvodi[index_m].Id, staro, proizvodi[index_m].Naziv, '+', u_ukupno, proizvodi[index_m].Kolicina);
                    sumirane[br_sum].Id = curr_id; sumirane[br_sum].Promena = ULAZ; sumirane[br_sum++].Kolicina = u_ukupno;
                }
                konacni_niz[br_konacni++] = proizvodi[index_m];
            }
            else {
                unsigned staro = proizvodi[index_m].Kolicina;
                int neto = (int)u_ukupno - (int)i_ukupno;
                proizvodi[index_m].Kolicina += neto;
                if (neto != 0 || (u_ukupno == 0 && i_ukupno == 0)) {
                    fprintf(fp, "%-5u %-10u %-15s %c   %-10u %-10u\n", proizvodi[index_m].Id, staro, proizvodi[index_m].Naziv, (neto >= 0 ? '+' : '-'), abs(neto), proizvodi[index_m].Kolicina);
                    sumirane[br_sum].Id = curr_id; sumirane[br_sum].Promena = (neto >= 0 ? ULAZ : IZLAZ); sumirane[br_sum++].Kolicina = abs(neto);
                }
                konacni_niz[br_konacni++] = proizvodi[index_m];
            }
        }
        else {
            if (i_ukupno > 0) {
                fprintf(fe, "%-5u %-10s %-15s %-3s %-10u %s\n", curr_id, "X", "X", "-", i_ukupno, "Nepostojeci proizvod.");
            }
            if (u_ukupno > 0) {
                PROIZVOD nov; nov.Id = curr_id; sprintf(nov.Naziv, "Pro_%u", nov.Id); nov.Kolicina = u_ukupno;
                fprintf(fn, "%-5u %-15s %-10u\n", nov.Id, nov.Naziv, nov.Kolicina);
                konacni_niz[br_konacni++] = nov;
                sumirane[br_sum].Id = curr_id; sumirane[br_sum].Promena = ULAZ; sumirane[br_sum++].Kolicina = u_ukupno;
            }
        }
    }

    for (int j = 0; j < br_proizvoda; j++) {
        int obradjen = 0;
        for (int i = 0; i < br_id; i++) if (svi_id[i] == proizvodi[j].Id) obradjen = 1;
        if (!obradjen) konacni_niz[br_konacni++] = proizvodi[j];
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

    for (int i = 0; i < br_sum - 1; i++) {
        for (int j = i + 1; j < br_sum; j++) {
            if (sumirane[i].Id > sumirane[j].Id) {
                TRANSAKCIJA tmp = sumirane[i]; sumirane[i] = sumirane[j]; sumirane[j] = tmp;
            }
        }
    }
    FILE* fs = fopen(tran_sum_path, "wb");
    if (fs) { fwrite(sumirane, sizeof(TRANSAKCIJA), br_sum, fs); fclose(fs); }

    fclose(fp); fclose(fn); fclose(fk); fclose(fe);
}


void scenario_sveobuhvatno() {
    const char mat_dat[] = ".\\ASD\\DATA\\maticna.dat";
    const char tran_dat[] = ".\\ASD\\DATA\\transakciona.dat";
    const char mat_nova[] = ".\\ASD\\DATA\\maticna.dat";

    char datum[10]; danasnji_datum(datum, sizeof(datum));
    char rpt_prom[256], rpt_nov[256], rpt_err_kol[256], rpt_err_pro[256], tran_sum[256];

    sprintf(rpt_prom, ".\\ASD\\RPT\\prom_%s.rpt", datum);
    sprintf(rpt_nov, ".\\ASD\\RPT\\nov_pro_%s.rpt", datum);
    sprintf(rpt_err_kol, ".\\ASD\\ERR\\err_kol_%s.rpt", datum);
    sprintf(rpt_err_pro, ".\\ASD\\ERR\\err_pro_%s.rpt", datum);
    sprintf(tran_sum, ".\\ASD\\DATA\\OLD\\tran_%s.dat", datum);

    if (!fajl_postoji(mat_dat)) {
        kopiraj_fajl(".\\ASD\\DEMO\\maticna.dat", mat_dat);
    }
    if (!fajl_postoji(tran_dat)) {
        kopiraj_fajl(".\\ASD\\DEMO\\SLUC_5\\transakciona.dat", tran_dat);
    }

    demo_naslov("STARA MATICNA DATOTEKA");
    demo_ispisi_maticnu(mat_dat);
    demo_naslov("TRANSAKCIONA DATOTEKA (SLUCAJ 5)");
    demo_ispisi_transakcije(tran_dat);

    PROIZVOD p[MAX_BR];
    int br_p = ucitaj_maticnu(mat_dat, p, MAX_BR);
    TRANSAKCIJA t[MAX_BR];
    int br_t = ucitaj_transakcije(tran_dat, t, MAX_BR);

    obradi_sveobuhvatni_slucaj(p, br_p, t, br_t, mat_nova);

    demo_naslov("SUMARNA TRANSAKCIONA (Agregirano)");
    demo_ispisi_transakcije(tran_sum);

    demo_naslov("NOVA MATICNA DATOTEKA");
    demo_ispisi_maticnu(mat_nova);

    demo_naslov("IZVESTAJ O PROMENAMA");
    demo_stampaj_rpt(rpt_prom);

    demo_naslov("IZVESTAJ O NOVIM PROIZVODIMA");
    demo_stampaj_rpt(rpt_nov);

    demo_naslov("GRESKE - KOLICINA");
    demo_stampaj_rpt(rpt_err_kol);

    demo_naslov("GRESKE - NEPOSTOJECI PROIZVOD");
    demo_stampaj_rpt(rpt_err_pro);

    printf("\nStatus: Sveobuhvatni slucaj zavrsen.\n");
    
}