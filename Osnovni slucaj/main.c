#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include "util.h"
#include "datoteka.h"
#include "misc.h"
#include "kki.h"

int main(int argc, char** argv) {

    const char mat_dat[] = ".\\ASD\\DATA\\maticna.dat";
    const char tran_dat[] = ".\\ASD\\DATA\\transakciona.dat";
    const char mat_nova[] = ".\\ASD\\DATA\\maticna.dat";

    // 1. OBAVEZNO KOPIRANJE (ako DATA folder ne postoji ili je prazan)
    if (!fajl_postoji(mat_dat)) {
        printf("Kopiram demo maticnu...\n");
        kopiraj_fajl(".\\ASD\\DEMO\\maticna.dat", mat_dat);
    }

    if (!fajl_postoji(tran_dat)) {
        printf("Kopiram demo transakcije (SLUC_1)...\n");
        kopiraj_fajl(".\\ASD\\DEMO\\SLUC_1\\transakciona.dat", tran_dat);
    }

    // 2. Generisanje naziva datoteka sa datumom (ggmmdd) prema SRS
    char datum[10];
    danasnji_datum(datum, sizeof(datum));

    char mat_tek_path[256];   // mat_210429.dat
    char tran_sum_path[256];  // tran_210429.dat
    char prom_rpt_path[256];  // prom_210429.rpt

    sprintf(mat_tek_path, ".\\ASD\\DATA\\OLD\\mat_%s.dat", datum);
    sprintf(tran_sum_path, ".\\ASD\\DATA\\OLD\\tran_%s.dat", datum);
    sprintf(prom_rpt_path, ".\\ASD\\RPT\\prom_%s.rpt", datum);

    // 3. Logika za DEMO argumente (-d1, -d2...)
    if (argc > 1 && strncmp(argv[1], "-d", 2) == 0) {
        char slucaj = argv[1][2];
        char demo_tran[256];
        sprintf(demo_tran, ".\\ASD\\DEMO\\SLUC_%c\\transakciona.dat", slucaj);

        // Uvek kopiraj svezu maticnu i odgovarajucu transakcionu za demo
        kopiraj_fajl(".\\ASD\\DEMO\\maticna.dat", mat_dat);
        kopiraj_fajl(demo_tran, tran_dat);
    }

    // --- PRIKAZ PRE AZURIRANJA ---
    demo_naslov("STARA MATICNA DATOTEKA (maticna.dat)");
    demo_ispisi_maticnu(mat_dat);

    demo_naslov("TRANSAKCIONA DATOTEKA (transakciona.dat)");
    demo_ispisi_transakcije(tran_dat);

    // 4. Ucitavanje podataka u memoriju (nizovi)
    PROIZVOD proizvodi[100];
    int br_proizvoda = ucitaj_maticnu(mat_dat, proizvodi, 100);

    TRANSAKCIJA transakcije[100];
    int br_transakcija = ucitaj_transakcije(tran_dat, transakcije, 100);

    // 5. Kreiranje backup-a stare maticne (mat_ggmmdd.dat) pre azuriranja
    kopiraj_fajl(mat_dat, mat_tek_path);

    // 6. GLAVNA OPERACIJA (Azuriranje)
    if (br_proizvoda > 0) {
        obradi_osnovni_slucaj(proizvodi, br_proizvoda, transakcije, br_transakcija, mat_nova, prom_rpt_path);
    }

    // --- PRIKAZ NAKON AZURIRANJA ---

    // Prikaz sumarne transakcione (tran_ggmmdd.dat) - kreirane unutar obradi_osnovni_slucaj
    demo_naslov("SUMARNA TRANSAKCIONA DATOTEKA (tran_sum)");
    demo_ispisi_tran_sum(tran_sum_path);

    // Prikaz nove maticne
    demo_naslov("NOVA MATICNA DATOTEKA (maticna.dat)");
    demo_ispisi_maticnu(mat_nova);

    // Prikaz tekstualnog izvestaja
    demo_naslov("IZVESTAJ O PROMENAMA (rpt)");
    demo_stampaj_rpt(prom_rpt_path);

    printf("\nStatus: Azuriranje zavrseno.\n");
    printf("Lokacija izvestaja: %s\n", prom_rpt_path);

    return 0;
}