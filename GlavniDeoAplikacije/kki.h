#ifndef KKI_H
#define KKI_H
#include "defs.h"

void demo_naslov(const char* tekst);
void demo_pauza(void);
void demo_ispisi_maticnu(const char* path);
void demo_ispisi_transakcije(const char* path);
void demo_stampaj_rpt(const char* path);

void postavi_akciju_azuriranja(AkcijaAzuriranja akcija);
void pokreni_glavni_meni();

int meni_glavni();
int meni_transakciona();
int meni_maticna();
int meni_pomoc();
int meni_demo();

#endif