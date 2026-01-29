#ifndef DATOTEKE_H
#define DATOTEKE_H

#include "util.h"

int ucitaj_maticnu(const char* path, PROIZVOD** proizvodi);
int ucitaj_transakcije(const char* path, TRANSAKCIJA** transakcije);

int file_exists(const char* path);
void copy_file(const char* src, const char* dest);


#endif
