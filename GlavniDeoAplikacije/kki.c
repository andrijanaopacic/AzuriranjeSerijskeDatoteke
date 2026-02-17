#include "kki.h"
#include "datoteka.h"

static AkcijaAzuriranja _trenutna_akcija = NULL;

void postavi_akciju_azuriranja(AkcijaAzuriranja akcija) {
    _trenutna_akcija = akcija;
}

void demo_naslov(const char* tekst) {
    printf("\n=================================\n");
    printf("%s\n", tekst);
    printf("=================================\n");
}

void demo_pauza(void) {
    printf("\nPritisni ENTER za nastavak...");
    getchar();
}

void demo_ispisi_maticnu(const char* path) {
    PROIZVOD niz[MAX_BR];
    int br = ucitaj_maticnu(path, niz, MAX_BR);
    demo_naslov("PREGLED MATICNE DATOTEKE");
    if (br <= 0) printf("Prazna datoteka.\n");
    else {
        printf("%-5s %-12s %8s\n", "Id", "Naziv", "Kolicina");
        for (int i = 0; i < br; i++)
            printf("%-5u %-12s %8u\n", niz[i].Id, niz[i].Naziv, niz[i].Kolicina);
    }
    demo_pauza();
}

void demo_ispisi_transakcije(const char* path) {
    TRANSAKCIJA niz[MAX_BR];
    int br = ucitaj_transakcije(path, niz, MAX_BR);
    demo_naslov("PREGLED TRANSAKCIJA");
    if (br <= 0) printf("Nema transakcija.\n");
    else {
        printf("%-5s %-8s %8s\n", "Id", "Tip", "Kolicina");
        for (int i = 0; i < br; i++)
            printf("%-5u %-8s %8u\n", niz[i].Id, (niz[i].Promena == ULAZ) ? "ULAZ" : "IZLAZ", niz[i].Kolicina);
    }
    demo_pauza();
}

void demo_stampaj_rpt(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) return;
    char buf[256];
    while (fgets(buf, sizeof(buf), f)) fputs(buf, stdout);
    fclose(f);
}

int meni_glavni() {
    int izbor;
    demo_naslov("GLAVNI MENI");
    printf("0. Kraj rada\n1. Rad sa transakcionom datotekom\n2. Rad sa maticnom datotekom\n3. Pomoc\n\nIzbor: ");
    scanf("%d", &izbor); getchar();
    return izbor;
}

int meni_transakciona() {
    int izbor;
    demo_naslov("MENI Transakciona datoteka");
    printf("0. Povratak\n1. Create (Nova datoteka)\n2. Drop (Obrisi datoteku)\n3. Insert (Nova transakcija)\n4. Select All (Prikazi sve)\n5. Select Id\n\nIzbor: ");
    scanf("%d", &izbor); getchar();
    return izbor;
}

int meni_maticna() {
    int izbor;
    demo_naslov("MENI Maticna datoteka");
    printf("0. Povratak\n1. Create\n2. Drop\n3. Insert\n4. Delete\n5. Update All (Azuriranje)\n6. Update Id\n7. Select All\n8. Select Id\n\nIzbor: ");
    scanf("%d", &izbor); getchar();
    return izbor;
}

int meni_demo() {
    int izbor;
    demo_naslov("MENI Demo");
    printf("0. Povratak\n1. Osnovni slucaj\n2. Nepostojeca kolicina\n3. Nov proizvod\n4. Nepostojeci proizvod\n5. Sveobuhvatni slucaj\n\nIzbor: ");
    scanf("%d", &izbor); getchar();
    return izbor;
}