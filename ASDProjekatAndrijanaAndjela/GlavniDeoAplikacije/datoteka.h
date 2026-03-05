#ifndef DATOTEKA_H
#define DATOTEKA_H
#include "defs.h"

int ucitaj_maticnu(const char* path, PROIZVOD niz[], int max_br);
int ucitaj_transakcije(const char* path, TRANSAKCIJA niz[], int max_br);
int fajl_postoji(const char* path);
void kopiraj_fajl(const char* src, const char* dest);


void o_azuriranju_serijske_datoteke();
void o_nama();


void o_osnovnom_slucaju();
void o_nepostojecoj_kolicini();
void o_novom_proizvodu();
void o_nepostojecem_proizvodu();
void o_sveouhvatnom();



void demo_kreiranje_maticne(const char* path);
void demo_drop_maticnu(const char* path);
void demo_insert_maticna(const char* path);
void demo_deleteProizvod_maticna(const char* path);
void demo_updateId_maticna(const char* path);
void demo_selectId_maticna(const char* path);

void demo_kreiraj_transakcionu(const char* path);
void demo_drop_transakcionu(const char* path);
void demo_insert_transakciona(const char* path);
void demo_selectId_transakciona(const char* path);
void demo_ispisi_transakcije(const char* path);

#endif