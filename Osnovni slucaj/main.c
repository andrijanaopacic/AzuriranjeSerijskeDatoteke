#include "util.h"
#include "datoteka.h"
#include "misc.h"
#include "kki.h"
#include <stdio.h>

int main() {

    PROIZVOD* proizvodi = NULL;
    TRANSAKCIJA* transakcije = NULL;

    demo_tekst_osnovni_slucaj();

    demo_ispisi_maticnu("ASDProjekatAndrijanaAndjela\\ASD\\DATA\\maticna.dat");

    demo_ispisi_transakcije("ASDProjekatAndrijanaAndjela\\ASD\\DATA\\transakciona.dat");

    int br_proizvoda = ucitaj_maticnu(
        "ASDProjekatAndrijanaAndjela\\ASD\\DATA\\maticna.dat", &proizvodi);

    int br_transakcija = ucitaj_transakcije(
        "ASDProjekatAndrijanaAndjela\\ASD\\DATA\\transakciona.dat", &transakcije);

    obradi_osnovni_slucaj(proizvodi, br_proizvoda, transakcije, br_transakcija);

    demo_ispisi_maticnu("ASDProjekatAndrijanaAndjela\\ASD\\DATA\\maticna.dat");

    free(proizvodi);
    free(transakcije);

    ispisi_kraj();
    return 0;
}
