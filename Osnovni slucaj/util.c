#include "util.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>

// globalni fajl objekti
FILE* maticna = NULL;
FILE* mat_tek = NULL;
FILE* mat_nova = NULL;

FILE* transakciona = NULL;
FILE* tran_tek = NULL;

FILE* promene = NULL;
FILE* err_kolicina = NULL;
FILE* nov_proizvod = NULL;
FILE* err_proizvod = NULL;

// helper funkcije
static void print_fopen_error(const char* action, const char* path);
int file_exists(const char* path);
void copy_file(const char* src, const char* dest);
void make_filename(char* out, size_t size, const char* folder, const char* prefix, const char* ext) {
    char datum[7];
    get_today_string(datum, sizeof(datum));
    snprintf(out, size, "%s\\%s_%s.%s", folder, prefix, datum, ext);
}
