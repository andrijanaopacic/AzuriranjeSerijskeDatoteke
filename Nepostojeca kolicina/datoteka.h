#ifndef DATOTEKA_H
#define DATOTEKA_H

#include "util.h"

int ucitaj_maticnu(const char* path, PROIZVOD niz[], int max_br);
int ucitaj_transakcije(const char* path, TRANSAKCIJA niz[], int max_br);
int fajl_postoji(const char* path);
void kopiraj_fajl(const char* src, const char* dest);
void danasnji_datum(char* buf, size_t size);

#endif