#include "util.h"
#include "datoteka.h"
#include <stdio.h>

void ispisi_proizvode(PROIZVOD* proizvodi, int br_proizvoda) {
    printf("Lista proizvoda:\n");
    for (int i = 0; i < br_proizvoda; i++) {
        printf("ID: %u, Naziv: %s, Kolicina: %u\n",
            proizvodi[i].Id, proizvodi[i].Naziv, proizvodi[i].Kolicina);
    }
}

int main() {
    PROIZVOD* proizvodi = NULL;
    TRANSAKCIJA* transakcije = NULL;

    int br_proizvoda = ucitaj_maticnu("ASDProjekatAndrijanaAndjela\\ASD\\DATA\\maticna.dat", &proizvodi);
    int br_transakcija = ucitaj_transakcije("ASDProjekatAndrijanaAndjela\\ASD\\DATA\\transakciona.dat", &transakcije);

    printf("=== Pre obrade ===\n");
    ispisi_listu(proizvodi, br_proizvoda);

    obradi_osnovni_slucaj(proizvodi, br_proizvoda, transakcije, br_transakcija);

    printf("=== Posle obrade ===\n");
    ispisi_listu(proizvodi, br_proizvoda);

    free(proizvodi);
    free(transakcije);

    ispisi_kraj();
    return 0;
}
