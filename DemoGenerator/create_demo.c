#include <stdio.h>
#include <stdlib.h>
#include <direct.h> 
#include <io.h>

#define MKDIR(dir) _mkdir(dir)

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



int main(void) {
    MKDIR("ASD");
    MKDIR("ASD\\DEMO");
    MKDIR("ASD\\DEMO\\SLUC_1");
    MKDIR("ASD\\DEMO\\SLUC_2");
    MKDIR("ASD\\DEMO\\SLUC_3");
    MKDIR("ASD\\DEMO\\SLUC_4");
    MKDIR("ASD\\DEMO\\SLUC_5");
    MKDIR("ASD\\DATA");

    return 0;
}