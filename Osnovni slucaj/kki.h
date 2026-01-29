#ifndef KKI_H
#define KKI_H

#include "util.h"

void ispisi_promenu(PROIZVOD* p, unsigned stara_kolicina);
void ispisi_kraj(void);
void ispisi_listu(PROIZVOD* proizvodi, int br_proizvoda);
void ispisi_gresku_kolicina(PROIZVOD* p, unsigned trazena_kolicina);
void ispisi_gresku_proizvod(unsigned id);

void demo_pauza(void);
void demo_naslov(const char* tekst);
void demo_tekst_osnovni_slucaj(void);

void demo_ispisi_transakcije(const char* path);
void demo_ispisi_maticnu(const char* path);

#endif
