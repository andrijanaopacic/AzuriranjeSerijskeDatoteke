#ifndef MISC_H
#define MISC_H

#include "util.h"

void obradi_osnovni_slucaj(
    PROIZVOD* proizvodi, int br_proizvoda,
    TRANSAKCIJA* transakcije, int br_transakcija
);

void demo_ispisi_maticnu(const char* path);
void demo_ispisi_transakcije(const char* path);


#endif
