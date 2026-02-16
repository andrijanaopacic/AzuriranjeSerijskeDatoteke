#include "util.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

FILE* maticna = NULL;
FILE* mat_tek = NULL;
FILE* mat_nova = NULL;

FILE* transakciona = NULL;
FILE* tran_tek = NULL;

FILE* promene = NULL;
FILE* err_kolicina = NULL;
FILE* nov_proizvod = NULL;
FILE* err_proizvod = NULL;

static void print_fopen_error(const char* action, const char* path);
int fajl_postoji(const char* path);
void kopiraj_fajl(const char* src, const char* dest);
void make_filename(char* out, size_t size, const char* folder, const char* prefix, const char* ext) {
    char datum[7];
    danasnji_datum(datum, sizeof(datum));
    snprintf(out, size, "%s\\%s_%s.%s", folder, prefix, datum, ext);
}
