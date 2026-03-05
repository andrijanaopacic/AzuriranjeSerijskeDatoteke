#include "util.h"
#include <time.h>
#include <stdio.h>

void danasnji_datum(char* buf, size_t size) {
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    strftime(buf, size, "%y%m%d", tm_info);
}