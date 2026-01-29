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
    if (!err_kolicina) return; // fajl nije otvoren
    fprintf(err_kolicina,
        "Greska kolicina - ID:%u | %s | trazeno: %u, dostupno: %u\n",
        p->Id, p->Naziv, trazena_kolicina, p->Kolicina);
}

void ispisi_gresku_proizvod(unsigned id) {
    if (!err_proizvod) return; // fajl nije otvoren
    fprintf(err_proizvod,
        "Greska proizvod - ID:%u ne postoji u maticnoj datoteci\n",
        id);
}

