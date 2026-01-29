#include "kki.h"
#include <stdio.h>

void ispisi_promenu(PROIZVOD* p, unsigned stara_kolicina) {
    fprintf(promene,
        "ID:%u | %s | %u -> %u\n",
        p->Id, p->Naziv, stara_kolicina, p->Kolicina
    );
}

void ispisi_kraj(void) {
    printf("\n=== KRAJ RADA APLIKACIJE ===\n");
}

void ispisi_listu(PROIZVOD* proizvodi, int br_proizvoda) {
    printf("Lista proizvoda:\n");
    for (int i = 0; i < br_proizvoda; i++) {
        printf("ID: %u, Naziv: %s, Kolicina: %u\n",
            proizvodi[i].Id, proizvodi[i].Naziv, proizvodi[i].Kolicina);
    }
}

void ispisi_gresku_kolicina(PROIZVOD* p, unsigned trazena_kolicina) {
    if (!err_kolicina) return; 
    fprintf(err_kolicina,
        "Greska kolicina - ID:%u | %s | trazeno: %u, dostupno: %u\n",
        p->Id, p->Naziv, trazena_kolicina, p->Kolicina);
}

void ispisi_gresku_proizvod(unsigned id) {
    if (!err_proizvod) return; 
    fprintf(err_proizvod,
        "Greska proizvod - ID:%u ne postoji u maticnoj datoteci\n",
        id);
}

void demo_pauza(void) {
    printf("\n---------------------------------\n");
    printf("Pritisni ENTER za nastavak...");
    getchar();
}

void demo_naslov(const char* tekst) {
    system("cls");
    printf("=================================\n");
    printf("%s\n", tekst);
    printf("=================================\n\n");
}

void demo_tekst_osnovni_slucaj(void) {
    demo_naslov("OSNOVNI SLUCAJ - OBJASNJENJE");

    printf("Osnovni slucaj azuriranja serijske datoteke:\n\n");
    printf("- Primljena kolicina proizvoda se dodaje na trenutno raspolozivu kolicinu, a poruka o uspesnom azuriranju i nastaloj promeni se upisuje u odgovarajucu datoteku\n");
    printf("- Izdata kolicina proizvoda je manja od trenutno raspolozive kolicine, a poruka o uspesnom azuiranju i nastaloj promeni se upisuje u odgovarajucu datoteku\n");

    demo_pauza();
}

void demo_ispisi_maticnu(const char* path) {
    FILE* f = fopen(path, "r");   
    if (!f) {
        printf("Ne mogu da otvorim %s\n", path);
        demo_pauza();
        return;
    }

    PROIZVOD p;
    demo_naslov("MATICNA DATOTEKA");

    while (fscanf(f, "%u %s %u",
        &p.Id,
        p.Naziv,
        &p.Kolicina) == 3) {

        printf("%-5u %-10s %5u\n",
            p.Id,
            p.Naziv,
            p.Kolicina);
    }

    fclose(f);
    demo_pauza();
}


void demo_ispisi_transakcije(const char* path) {
    TRANSAKCIJA* t = NULL;
    int br = ucitaj_transakcije(path, &t);

    demo_naslov("TRANSAKCIONA DATOTEKA");

    if (br <= 0 || !t) {
        printf("Nema transakcija.\n");
        demo_pauza();
        return;
    }

    for (int i = 0; i < br; i++) {
        printf("%3u  %-5s  %5u\n",
            t[i].Id,
            (t[i].Promena == ULAZ) ? "ULAZ" : "IZLAZ",
            t[i].Kolicina);
    }

    free(t);
    demo_pauza();
}

