#include "datoteka.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>


int file_exists(const char* path) {
    FILE* f = fopen(path, "rb");
    if (f) { fclose(f); return 1; }
    return 0;
}

void copy_file(const char* src, const char* dest) {
    FILE* fs = fopen(src, "rb");
    FILE* fd = fopen(dest, "wb");
    char buf[1024];
    size_t n;

    while ((n = fread(buf, 1, sizeof(buf), fs)) > 0)
        fwrite(buf, 1, n, fd);

    fclose(fs);
    fclose(fd);
}

int ucitaj_maticnu(const char* path, PROIZVOD** proizvodi) {
    FILE* f = fopen(path, "rb");
    if (!f) return 0;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    int br = (size > 0) ? (int)(size / sizeof(PROIZVOD)) : 0;
    rewind(f);

    *proizvodi = (PROIZVOD*)malloc(br * sizeof(PROIZVOD));
    if (!*proizvodi && br > 0) { fclose(f); return 0; }

    if (br > 0) fread(*proizvodi, sizeof(PROIZVOD), br, f);
    fclose(f);
    return br;
}

int ucitaj_transakcije(const char* path, TRANSAKCIJA** transakcije) {
    FILE* f = fopen(path, "rb");
    if (!f) return 0;

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    int br = (size > 0) ? (int)(size / sizeof(TRANSAKCIJA)) : 0;
    rewind(f);

    *transakcije = (TRANSAKCIJA*)malloc(br * sizeof(TRANSAKCIJA));
    if (!*transakcije && br > 0) { fclose(f); return 0; }

    if (br > 0) fread(*transakcije, sizeof(TRANSAKCIJA), br, f);
    fclose(f);
    return br;
}

void get_today_string(char* buf, size_t size) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(buf, size, "%02d%02d%02d", tm.tm_year % 100, tm.tm_mon + 1, tm.tm_mday);
}

void save_old_file(const char* src_path, const char* old_folder, const char* prefix) {
    char datum[7]; get_today_string(datum, sizeof(datum));
    char dest[256];
    snprintf(dest, sizeof(dest), "%s\\%s_%s.dat", old_folder, prefix, datum);
    copy_file(src_path, dest);
}


