#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../GlavniDeoAplikacije/util.h"
#include "../GlavniDeoAplikacije/datoteka.h"
#include "../GlavniDeoAplikacije/misc.h"
#include "../GlavniDeoAplikacije/kki.h"
#include "../GlavniDeoAplikacije/defs.h"

int main(void) {
    postavi_akciju_azuriranja(scenario_nepostojeca_kolicina);
    pokreni_glavni_meni();
}