#ifndef KKI_H
#define KKI_H

#include "defs.h"


typedef void (*AkcijaAzuriranja)();

void postavi_akciju_azuriranja(AkcijaAzuriranja akcija);
void pokreni_glavni_meni();

void demo_naslov(const char* tekst);
void demo_pauza(void);
void demo_ispisi_maticnu(const char* path);
void demo_ispisi_transakcije(const char* path);
void demo_stampaj_rpt(const char* path);
void o_nama();
void o_azuriranju_serijske_datoteke();


void o_osnovnom_slucaju();
void o_nepostojecoj_kolicini();
void o_novom_proizvodu();
void o_nepostojecem_proizvodu();
void o_sveouhvatnom();



void demo_kreiraj_transakcionu(const char* path);
void demo_drop_transakcionu(const char* path);
void demo_insert_transakciona(const char* path);
void demo_selectId_transakciona(const char* path);

void demo_kreiranje_maticne(const char* path);
void demo_drop_maticnu(const char* path);
void demo_insert_maticna(const char* path);
void demo_deleteProizvod_maticna(const char* path);
void demo_updateId_maticna(const char* path);
void demo_selectId_maticna(const char* path);


void meni_glavni();
void meni_transakciona();
void meni_maticna();
void meni_pomoc();
void meni_demo();


void scenario_osnovni_slucaj();
void scenario_sveobuhvatno();
void scenario_nov_proizvod();
void scenario_nepostojeci_proizvod();
void scenario_nepostojeca_kolicina();

#endif