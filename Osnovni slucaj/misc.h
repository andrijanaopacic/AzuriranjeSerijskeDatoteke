#ifndef MISC_H
#define MISC_H

#include "util.h"

void obradi_osnovni_slucaj(
    PROIZVOD* proizvodi, int br_proizvoda,
    TRANSAKCIJA* transakcije, int br_transakcija,
    const char* mat_nova_path,
    const char* promene_rpt
);

#endif
