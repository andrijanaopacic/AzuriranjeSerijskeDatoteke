#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_BR_PRO 10
#define TRUE 1
#define FALSE 0

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

extern FILE* maticna, * mat_tek, * mat_nova;
extern FILE* transakciona, * tran_tek;
extern FILE* promene, * err_kolicina, * nov_proizvod, * err_proizvod;
