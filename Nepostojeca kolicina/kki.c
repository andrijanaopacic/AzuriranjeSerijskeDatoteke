#include <stdio.h>
#include <stdlib.h>
#include "kki.h"
#include "datoteka.h"

void demo_pauza(void) {
    printf("\nPritisni ENTER za nastavak...");
    getchar();
}

void demo_naslov(const char* tekst) {
    //system("cls"); // Brise prethodni sadrzaj sa konzole
    printf("=================================\n");
    printf("%s\n", tekst);
    printf("=================================\n\n");
}

void demo_ispisi_maticnu(const char* path) {
    PROIZVOD privremeni_niz[100];
    int br = ucitaj_maticnu(path, privremeni_niz, 100);

    printf("\n--- MATICNA DATOTEKA ---\n");
    if (br <= 0) {
        printf("Prazna ili nepostojeca datoteka.\n");
    }
    else {
        printf("%-5s %-12s %8s\n", "Id", "Naziv", "Kolicina");
        for (int i = 0; i < br; ++i) {
            printf("%-5u %-12s %8u\n", privremeni_niz[i].Id, privremeni_niz[i].Naziv, privremeni_niz[i].Kolicina);
        }
    }
    demo_pauza();
}

void demo_ispisi_transakcije(const char* path) {
    TRANSAKCIJA privremeni_niz[100];
    int br = ucitaj_transakcije(path, privremeni_niz, 100);

    printf("\n--- TRANSAKCIONA DATOTEKA ---\n");
    if (br <= 0) {
        printf("Prazna ili nepostojeca datoteka.\n");
    }
    else {
        printf("%-5s %-8s %8s\n", "Id", "Tip", "Kolicina");
        for (int i = 0; i < br; ++i) {
            printf("%-5u %-8s %8u\n", privremeni_niz[i].Id, (privremeni_niz[i].Promena == ULAZ) ? "ULAZ" : "IZLAZ", privremeni_niz[i].Kolicina);
        }
    }
    demo_pauza();
}

void demo_stampaj_rpt(const char* putanja) {
    FILE* f = fopen(putanja, "r");
    if (f == NULL) return;
    char buf[256];
    printf("\n--- IZVESTAJ: %s ---\n", putanja);
    while (fgets(buf, sizeof(buf), f)) fputs(buf, stdout);
    fclose(f);
}