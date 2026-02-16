#include "datoteka.h"
#include <stdio.h>
#include <time.h>

int fajl_postoji(const char* path) {
    FILE* f = fopen(path, "rb");
    if (f) { fclose(f); return 1; }
    return 0;
}

void kopiraj_fajl(const char* src, const char* dest) {
    FILE* fs = fopen(src, "rb");
    FILE* fd = fopen(dest, "wb");
    if (!fs || !fd) {
        if (fs) fclose(fs);
        if (fd) fclose(fd);
        return;
    }
    char buf[1024];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), fs)) > 0) {
        fwrite(buf, 1, n, fd);
    }
    fclose(fs);
    fclose(fd);
}

int ucitaj_maticnu(const char* path, PROIZVOD niz[], int max_br) {
    FILE* f = fopen(path, "rb");
    if (!f) return 0;
    int procitano = (int)fread(niz, sizeof(PROIZVOD), max_br, f);
    fclose(f);
    return procitano;
}

int ucitaj_transakcije(const char* path, TRANSAKCIJA niz[], int max_br) {
    FILE* f = fopen(path, "rb");
    if (!f) return 0;
    int procitano = (int)fread(niz, sizeof(TRANSAKCIJA), max_br, f);
    fclose(f);
    return procitano;
}

void danasnji_datum(char* buf, size_t size) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(buf, size, "%y%m%d", tm_info);
}