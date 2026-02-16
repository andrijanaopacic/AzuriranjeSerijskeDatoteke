#ifndef UTIL_H
#define UTIL_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

typedef struct proizvod {
    unsigned Id;
    char Naziv[15];
    unsigned Kolicina;
} PROIZVOD;

typedef enum promena {
    IZLAZ = -1,
    ULAZ = 1
} PROMENA;

typedef struct transakcija {
    unsigned Id;
    PROMENA Promena;
    unsigned Kolicina;
} TRANSAKCIJA;

#endif