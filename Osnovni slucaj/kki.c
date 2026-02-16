#include "kki.h"
#include "datoteka.h"
#include <stdio.h>
#include <stdlib.h>

void demo_pauza(void) {
    printf("\nPritisni ENTER za nastavak...");
    getchar();
}

void demo_naslov(const char* tekst) {
    printf("\n=================================\n");
    printf("%s\n", tekst);
    printf("=================================\n");
}

void demo_ispisi_maticnu(const char* path) {
    PROIZVOD niz[100];
    int br = ucitaj_maticnu(path, niz, 100);
    demo_naslov("PREGLED MATICNE DATOTEKE");
    if (br <= 0) {
        printf("Datoteka je prazna ili ne postoji.\n");
    }
    else {
        printf("%-5s %-12s %8s\n", "Id", "Naziv", "Kolicina");
        for (int i = 0; i < br; i++) {
            printf("%-5u %-12s %8u\n", niz[i].Id, niz[i].Naziv, niz[i].Kolicina);
        }
    }
    demo_pauza();
}

void demo_ispisi_transakcije(const char* path) {
    TRANSAKCIJA niz[100];
    int br = ucitaj_transakcije(path, niz, 100);
    demo_naslov("PREGLED TRANSAKCIJA");
    if (br <= 0) {
        printf("Nema transakcija.\n");
    }
    else {
        printf("%-5s %-8s %8s\n", "Id", "Promena", "Kolicina");
        for (int i = 0; i < br; i++) {
            printf("%-5u %-8s %8u\n", niz[i].Id, (niz[i].Promena == ULAZ) ? "ULAZ" : "IZLAZ", niz[i].Kolicina);
        }
    }
    demo_pauza();
}

void demo_ispisi_tran_sum(const char* path) {
    demo_ispisi_transakcije(path);
}

void demo_stampaj_rpt(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) return;
    char buf[256];
    demo_naslov("IZVESTAJ O PROMENAMA");
    while (fgets(buf, sizeof(buf), f)) {
        fputs(buf, stdout);
    }
    fclose(f);
    demo_pauza();
}