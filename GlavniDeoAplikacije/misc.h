#ifndef MISC_H
#define MISC_H
#include "defs.h"

void obradi_osnovni_slucaj(PROIZVOD* proizvodi, int br_proizvoda, TRANSAKCIJA* transakcije, int br_transakcija, const char* mat_nova_path, const char* promene_rpt);
void obradi_nepostojecu_kolicinu(PROIZVOD* proizvodi, int br_proizvoda, TRANSAKCIJA* transakcije, int br_transakcija, const char* mat_nova_path, const char* promene_rpt);
void obradi_nov_proizvod(PROIZVOD* proizvodi, int br_proizvoda, TRANSAKCIJA* transakcije, int br_transakcija, const char* mat_nova_path, const char* promene_rpt);
void obradi_nepostojeci_proizvod(PROIZVOD* proizvodi, int br_proizvoda, TRANSAKCIJA* transakcije, int br_transakcija, const char* mat_nova_path, const char* promene_rpt);
void obradi_sveobuhvatni_slucaj(PROIZVOD* proizvodi, int br_proizvoda, TRANSAKCIJA* transakcije, int br_transakcija, const char* mat_nova_path);

#endif