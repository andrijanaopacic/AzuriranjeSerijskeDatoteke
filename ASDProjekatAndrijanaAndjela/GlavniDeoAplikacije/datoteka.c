#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "datoteka.h"

int fajl_postoji(const char* path) {
    FILE* f = fopen(path, "rb");
    if (f) { fclose(f); return 1; }
    return 0;
}

void kopiraj_fajl(const char* src, const char* dest) {
    FILE* fs = fopen(src, "rb");
    FILE* fd = fopen(dest, "wb");
    if (!fs || !fd) { if (fs) fclose(fs); if (fd) fclose(fd); return; }
    char buf[1024];
    size_t n;
    while ((n = fread(buf, 1, sizeof(buf), fs)) > 0) fwrite(buf, 1, n, fd);
    fclose(fs); fclose(fd);
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


void demo_ispisi_maticnu(const char* path) {
    PROIZVOD niz[MAX_BR];
    int br = ucitaj_maticnu(path, niz, MAX_BR);
    if (br <= 0) printf("Datoteka prazna ili ne postoji.\n");
    else {
        printf("%-5s %-15s %-10s\n", "ID", "Naziv", "Kolicina");
        for (int i = 0; i < br; i++)
            printf("%-5u %-15s %-10u\n", niz[i].Id, niz[i].Naziv, niz[i].Kolicina);
    }
    demo_pauza();
}

void demo_ispisi_transakcije(const char* path) {
    TRANSAKCIJA niz[MAX_BR];
    int br = ucitaj_transakcije(path, niz, MAX_BR);
    if (br <= 0) printf("Nema transakcija.\n");
    else {
        printf("%-5s %-10s %-10s\n", "ID", "Tip", "Kol");
        for (int i = 0; i < br; i++)
            printf("%-5u %-10s %-10u\n", niz[i].Id, (niz[i].Promena == ULAZ ? "ULAZ" : "IZLAZ"), niz[i].Kolicina);
    }
    demo_pauza();
}

void demo_stampaj_rpt(const char* path) {
    FILE* f = fopen(path, "r");
    if (!f) { printf("Izvestaj %s ne postoji.\n", path); return; }
    char linija[256];
    while (fgets(linija, sizeof(linija), f)) printf("%s", linija);
    fclose(f);
    demo_pauza();
}

void demo_kreiraj_transakcionu(const char* path) {
    if (path == NULL) return;

    FILE* f = fopen(path, "rb");
    if (f) {
        fclose(f);
        printf("U folderu sa podacima vec postoji transakciona datoteka.\n");
        printf("Da li zelite da je obrisete i kreirate novu transakcionu datoteku? (D/N): ");
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}

        char izbor[8];
        if (fgets(izbor, sizeof(izbor), stdin) == NULL) {
            printf("Kreiranje otkazano.\n");
            return;
        }
        if (!(izbor[0] == 'D' || izbor[0] == 'd')) {
            printf("Kreiranje otkazano.\n");
            return;
        }
    }

    FILE* f2 = fopen(path, "wb");
    if (!f2) {
        printf("Greska: Ne mogu da kreiram %s\n", path);
        return;
    }
    fclose(f2);
    printf("Kreirana je nova transakciona datoteka: %s\n", path);
}

void demo_drop_transakcionu(const char* path) {
    if (remove(path) == 0)
        printf("INFO: Тransakciona datoteka je obrisana.\n");
    else
        printf("ERROR: Тransakciona datoteka nije obrisana.\n");
}

void demo_insert_transakciona(const char* path) {
    TRANSAKCIJA t;
    FILE* f = fopen(path, "ab");
    if (!f) {
        printf("Greska: Ne mogu da otvorim datoteku: %s\n", path);
        return;
    }
    unsigned id1;
	PROMENA p1;
	unsigned kolicina1;
    printf("Unesite ID proizvoda: ");
    scanf("%u", &id1);
    printf("Unesite tip transakcije (1 za ULAZ, -1 za IZLAZ): ");
    scanf("%d", (int*)&p1);
    if (p1 != ULAZ && p1 != IZLAZ) {
        printf("Greska: Nepostojeci tip transakcije.\n");
        fclose(f);
        return;
    }
    printf("Unesite kolicinu: ");
    scanf("%u", &kolicina1);
    t.Id = id1;
    t.Promena = p1;
	t.Kolicina = kolicina1;
    fwrite(&t, sizeof(TRANSAKCIJA), 1, f);
    fclose(f);
    printf("Dodata nova transakcija: ID=%u, Tip=%s, Kolicina=%u\n", t.Id, (t.Promena == ULAZ ? "ULAZ" : "IZLAZ"), t.Kolicina);
}

void demo_selectId_transakciona(char* path) {

    TRANSAKCIJA niz[MAX_BR];
    int br = ucitaj_transakcije(path, niz, MAX_BR);
    if (br <= 0) {
        printf("Nema transakcija.\n");
        return;
    }
    int found = 0;
    unsigned id1;
    printf("Unesite ID proizvoda: ");
    scanf("%u", &id1);
    printf("%-5s %-10s %-10s\n", "ID", "Tip", "Kol");
    for (int i = 0; i < br; i++) {
        if (niz[i].Id == id1) {
            printf("%-5u %-10s %-10u\n", niz[i].Id, (niz[i].Promena == ULAZ ? "ULAZ" : "IZLAZ"), niz[i].Kolicina);
            found = 1;
        }
    }
    if (!found) printf("Ne postoje transakcije za proizvod sa ID=%d.\n", id1);
    demo_pauza();

}

void demo_kreiranje_maticne(const char* path) {
    if (path == NULL) return;

    FILE* f = fopen(path, "rb");
    if (f) {
        fclose(f);
        printf("U folderu sa podacima vec postoji maticna datoteka.\n");
        printf("Da li zelite da je obrisete i kreirate novu maticnu datoteku? (D/N): ");

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {}

        char izbor[8];
        if (fgets(izbor, sizeof(izbor), stdin) == NULL) {
            printf("Kreiranje otkazano.\n");
            return;
        }
        if (!(izbor[0] == 'D' || izbor[0] == 'd')) {
            printf("Kreiranje otkazano.\n");
            return;
        }
    }

    FILE* f2 = fopen(path, "wb");
    if (!f2) {
        printf("Greska: Ne mogu da kreiram %s\n", path);
        return;
    }
    fclose(f2);
    printf("Kreirana je nova maticna datoteka: %s\n", path);
}

void demo_drop_maticnu(char* path) {
    if (remove(path) == 0)
        printf("INFO: Maticna datoteka je obrisana.\n");
    else
        printf("ERROR: Maticna datoteka nije obrisana.\n");

}

void demo_insert_maticna(const char* path) {
    PROIZVOD niz[MAX_BR];
    PROIZVOD p;
    unsigned id;
    char Naziv[15];
    unsigned Kolicina;
	printf("Unesite ID proizvoda: ");
	scanf("%u", &id);
	printf("Unesite naziv proizvoda (max 14 karaktera): ");
	scanf("%14s", Naziv);
	printf("Unesite kolicinu proizvoda: ");
	scanf("%u", &Kolicina);
	p.Id = id;
	strcpy(p.Naziv, Naziv);
	p.Kolicina = Kolicina;

    int br = ucitaj_maticnu(path, niz, MAX_BR);
    if (br < 0) br = 0;


    for (int i = 0; i < br; ++i) {
        if (niz[i].Id == p.Id) {
            printf("Greska: Proizvod sa ID=%u vec postoji.\n", p.Id);
            return;
        }
    }

    if (br >= MAX_BR) {
        printf("Greska: Dostignut maksimalan broj proizvoda (%d).\n", MAX_BR);
        return;
    }

    int id1 = 0;
    while (id1 < br && niz[id1].Id < p.Id) ++id1;


    for (int j = br; j > id1; --j) {
        niz[j] = niz[j - 1];
    }
    niz[id1] = p;
    ++br;


    char privremenPath[512];
    if (snprintf(privremenPath, sizeof(privremenPath), "%s.tmp", path) < 0) {
        printf("Greska: neuspesno formiranje privremenog imena datoteke.\n");
        return;
    }

    FILE* ftmp = fopen(privremenPath, "wb");
    if (!ftmp) {
        printf("Greska: Ne mogu da kreiram privremenu datoteku: %s\n", privremenPath);
        return;
    }

    if (fwrite(niz, sizeof(PROIZVOD), br, ftmp) != (size_t)br) {
        printf("Greska pri pisanju u privremenu datoteku.\n");
        fclose(ftmp);
        remove(privremenPath);
        return;
    }
    fclose(ftmp);


    remove(path);
    if (rename(privremenPath, path) != 0) {
        printf("Greska: Ne mogu da preimenujem privremenu datoteku u %s\n", path);
        remove(privremenPath);
        return;
    }

    printf("Dodat novi proizvod: ID=%u, Naziv=%s, Kolicina=%u\n", p.Id, p.Naziv, p.Kolicina);

}

void demo_deleteProizvod_maticna(const char* path) {
    PROIZVOD niz[MAX_BR];


	printf("Unesite ID proizvoda koji zelite da obrisete: ");
	unsigned id;
	scanf("%u", &id);

    int br = ucitaj_maticnu(path, niz, MAX_BR);
    if (br <= 0) {
        printf("Nema proizvoda.\n");
        return;
    }

    int idx = -1;
    for (int i = 0; i < br; ++i) {
        if ((int)niz[i].Id == id) { idx = i; break; }
    }

    if (idx == -1) {
        printf("Proizvod sa ID=%d nije pronadjen.\n", id);
        return;
    }

    printf("Pronadjen proizvod:\n");
    printf("%-5s %-15s %-10s\n", "ID", "Naziv", "Kolicina");
    printf("%-5u %-15s %-10u\n", niz[idx].Id, niz[idx].Naziv, niz[idx].Kolicina);
    printf("Da li ste sigurni da zelite da obrisete navedeni proizvod? D(а)/N(e): ");

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}

    char izbor[8];
    if (fgets(izbor, sizeof(izbor), stdin) == NULL) {
        printf("Brisanje otkazano (greska pri unosu).\n");
        return;
    }
    if (!(izbor[0] == 'D' || izbor[0] == 'd')) {
        printf("Brisanje otkazano.\n");
        return;
    }

    char privremenPath[512];
    if (snprintf(privremenPath, sizeof(privremenPath), "%s.tmp", path) < 0) {
        printf("Greska: neuspesno formiranje privremenog imena datoteke.\n");
        return;
    }

    FILE* ftmp = fopen(privremenPath, "wb");
    if (!ftmp) {
        printf("Greska: Ne mogu da otvorim privremenu datoteku: %s\n", privremenPath);
        return;
    }

    for (int i = 0; i < br; ++i) {
        if (i == idx) continue;
        if (fwrite(&niz[i], sizeof(PROIZVOD), 1, ftmp) != 1) {
            printf("Greska pri pisanju u privremenu datoteku.\n");
            fclose(ftmp);
            remove(privremenPath);
            return;
        }
    }
    fclose(ftmp);

    if (remove(path) != 0) {
        printf("ERROR: Ne mogu da obrisem staru datoteku: %s\n", path);
        remove(privremenPath);
        return;
    }
    if (rename(privremenPath, path) != 0) {
        printf("ERROR: Ne mogu da preimenujem privremenu datoteku.\n");
        remove(privremenPath);
        return;
    }

    printf("Proizvod sa ID=%u je obrisan.\n", (unsigned)id);
}

void demo_updateId_maticna(const char* path) {
    if (path == NULL) {
        printf("Greska: neispravna putanja.\n");
        return;
    }

	printf("Unesite ID proizvoda koji zelite da azurirate: ");
	unsigned id;
	scanf("%u", &id);

    PROIZVOD niz[MAX_BR];
    int br = ucitaj_maticnu(path, niz, MAX_BR);
    if (br <= 0) {
        printf("Nema proizvoda.\n");
        return;
    }

    int idx = -1;
    for (int i = 0; i < br; ++i) {
        if ((int)niz[i].Id == id) { idx = i; break; }
    }

    if (idx == -1) {
        printf("Proizvod sa ID=%d nije pronadjen.\n", id);
        return;
    }

    printf("Pronadjen proizvod:\n");
    printf("%-5s %-15s %-10s\n", "ID", "Naziv", "Kolicina");
    printf("%-5u %-15s %-10u\n", niz[idx].Id, niz[idx].Naziv, niz[idx].Kolicina);

    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {}

    char unosNaziv[64];
    printf("Unesite novi naziv (maks %zu znaka): ", sizeof(niz[idx].Naziv) - 1);
    if (fgets(unosNaziv, sizeof(unosNaziv), stdin) == NULL) {
        printf("Azuriranje otkazano (greska pri unosu naziva).\n");
        return;
    }
    size_t ln = strlen(unosNaziv);
    if (ln > 0 && unosNaziv[ln - 1] == '\n') unosNaziv[ln - 1] = '\0';

    char unosLinija[64];
    printf("Unesite novu kolicinu: ");
    if (fgets(unosLinija, sizeof(unosLinija), stdin) == NULL) {
        printf("Azuriranje otkazano (greska pri unosu kolicine).\n");
        return;
    }
    char* endptr = NULL;
    unsigned long parsed = strtoul(unosLinija, &endptr, 10);
    if (endptr == unosLinija || parsed > UINT_MAX) {
        printf("Neispravan unos kolicine. Azuriranje otkazano.\n");
        return;
    }
    unsigned novaKolicina = (unsigned)parsed;

    strncpy(niz[idx].Naziv, unosNaziv, sizeof(niz[idx].Naziv) - 1);
    niz[idx].Naziv[sizeof(niz[idx].Naziv) - 1] = '\0';
    niz[idx].Kolicina = novaKolicina;

    char privremenPath[512];
    if (snprintf(privremenPath, sizeof(privremenPath), "%s.tmp", path) < 0) {
        printf("Greska: neuspesno formiranje privremenog imena datoteke.\n");
        return;
    }

    FILE* ftmp = fopen(privremenPath, "wb");
    if (!ftmp) {
        printf("Greska: Ne mogu da otvorim privremenu datoteku: %s\n", privremenPath);
        return;
    }

    if (fwrite(niz, sizeof(PROIZVOD), br, ftmp) != (size_t)br) {
        printf("Greska pri pisanju u privremenu datoteku.\n");
        fclose(ftmp);
        remove(privremenPath);
        return;
    }
    fclose(ftmp);

    if (remove(path) != 0) {
        printf("ERROR: Ne mogu da obrisem staru datoteku: %s\n", path);
        remove(privremenPath);
        return;
    }
    if (rename(privremenPath, path) != 0) {
        printf("ERROR: Ne mogu da preimenujem privremenu datoteku.\n");
        remove(privremenPath);
        return;
    }

    printf("Proizvod sa ID=%u je azuriran. Novi podaci: Naziv=%s, Kolicina=%u\n",
        niz[idx].Id, niz[idx].Naziv, niz[idx].Kolicina);
}

void demo_selectId_maticna(const char* path) {
    PROIZVOD niz[MAX_BR];
    int br = ucitaj_maticnu(path, niz, MAX_BR);
    if (br <= 0) {
        printf("Datoteka prazna ili ne postoji.\n");
        demo_pauza();
        return;
    }

    unsigned id;
	printf("Unesite ID proizvoda koji zelite da prikazete: ");
	scanf("%u", &id);

    int pronadjen = 0;
    for (int i = 0; i < br; ++i) {
        if (niz[i].Id == (unsigned)id) {
            printf("%-5s %-15s %-10s\n", "ID", "Naziv", "Kolicina");
            printf("%-5u %-15s %-10u\n", niz[i].Id, niz[i].Naziv, niz[i].Kolicina);
            pronadjen = 1;
            break;
        }
    }

    if (!pronadjen) {
        printf("Proizvod sa ID=%d nije pronadjen.\n", id);
    }

    demo_pauza();
}


void o_nama() {
    printf("Softverski inzenjer: Аndrijana Opacic, ao20220136@student.fon.bg.ac.rs, Student cetvrte godine Fakulteta organizacionih nauka. .\n");
    printf("Softverski inzenjer: Аndjela Nikolic, an20220117@student.fon.bg.ac.rs, Student cetvrte godine Fakulteta organizacionih nauka. \n");
    printf("Domenski inzenjer: Sasa D. Lazarevic, sasa.lazarevic@fon.bg.rs, Redovni profesor na Katedri za softversko inzenjerstvo.\n");
}

void o_azuriranju_serijske_datoteke() {
    printf("O azuriranju serijske datoteke\n");
    printf("Azuriranje serijske datoteke vrsi se uporednim citanjem zapisa iz maticne datoteke i transakcione datoteke.\n U zavisnosti od mogucih dogadjaja, razlikujemo pet slucajeva:\n\n");
    printf("1. Ako proizvod postoji u maticnoj datoteci i evidentirana je promena (ulaz ili izlaz),\n primljena kolicina se dodaje na trenutno raspolozivu kolicinu i upisuje se poruka o uspesnom azuriranju.\n\n");
    printf("2. Ako proizvod postoji u maticnoj datoteci i evidentirano je izdavanje, \n a izdata kolicina je manja od raspolozive, kolicina se umanjuje i upisuje se poruka o uspesnom azuriranju.\n\n");
    printf("3. Ako je izdata kolicina veca ili jednaka raspolozivoj, \n operacija se odbija i upisuje se poruka o neuspesnom azuriranju sa navodjenjem greske.\n\n");
    printf("4. Ako je evidentiran prijem proizvoda koji ne postoji u maticnoj datoteci, \n dodaje se novi proizvod sa primljenom kolicinom i upisuje se poruka o uspesnom azuriranju.\n\n");
    printf("5. Ako je evidentirano izdavanje proizvoda koji ne postoji u maticnoj datoteci, \n operacija se odbija i upisuje se poruka o neuspesnom azuriranju.\n\n");

}





void o_osnovnom_slucaju() {

    printf("Osnovni slucaj\n");
    printf("Azurira se proizvod koji postoji u maticnoj datoteci, a kolicine su uvek odgovarajuce.\n");
    printf("Verbalni opis scenarija:\n");
    printf("\t(a) primljena kolicina proizvoda uvecava trenutno raspolozivu kolicinu,\n\t a poruka o uspesno izvrsenom azuriranju i evidentiranoj promeni se upisuje u odgovarajucu datoteku.\n");
    printf("\t(b) ukoliko je izdata kolicina manja od raspolozive, stanje zaliha se umanjuje,\n\t a poruka o uspesno izvrsenom azuriranju i evidentiranoj promeni se upisuje u odgovarajucu datoteku.\n");
    printf("\n\n");
    printf("Stara maticna datoteka:\n");
    printf("Id   Naziv     Kolicina\n");
    printf("--------------------------------\n");
    printf("20   Pro_20    100\n");
    printf("30   Pro_30    150\n");
    printf("40   Pro_40    200\n");
    printf("50   Pro_50    250\n");
    printf("60   Pro_60    300\n");
    printf("70   Pro_70    350\n");
    printf("80   Pro_80    400\n");
    printf("90   Pro_90    450\n\n");
    printf("Transakciona datoteka: \n");
    printf("Id   Promena   Kolicina\n");
    printf("--------------------------------\n");
    printf("60   ULAZ      150\n");
    printf("40   ULAZ      80\n");
    printf("60   ULAZ      50\n");
    printf("60   IZLAZ     100\n");
    printf("40   IZLAZ     50\n");
    printf("60   IZLAZ     150\n");
    printf("60   ULAZ      200\n");
    printf("40   IZLAZ     80\n");
    printf("60   ULAZ      20\n");
    printf("60   IZLAZ     120\n");
    printf("40   ULAZ      100\n");
    printf("60   IZLAZ     100\n");
    printf("70   ULAZ      50\n");
    printf("70   IZLAZ     50\n\n");
    printf("Sumarna i sortirana transakciona datoteka:\n");
    printf("Id   Promena   Kolicina\n");
    printf("--------------------------------\n");
    printf("40   ULAZ      50\n");
    printf("60   IZLAZ     50\n");
    printf("70   ULAZ      0\n\n");
    printf("Nova maticna datoteka: \n");
    printf("Id   Naziv     Kolicina\n");
    printf("--------------------------------\n");
    printf("20   Pro_20    100\n");
    printf("30   Pro_30    150\n");
    printf("40   Pro_40    250\n");
    printf("50   Pro_50    250\n");
    printf("60   Pro_60    250\n");
    printf("70   Pro_70    350\n");
    printf("80   Pro_80    400\n");
    printf("90   Pro_90    450\n\n");
    printf("Izvestaj o promenama:\n");
    printf("Id   StaraKol  Naziv     Tip   Promena  NovaKol\n");
    printf("-----------------------------------------------------\n");
    printf("40   200       Pro_40    +     50       250\n");
    printf("60   300       Pro_60    -     50       250\n");
    printf("70   350       Pro_70    +     0        350\n\n");

}

void o_nepostojecoj_kolicini() {

    printf("Nepostojeca kolicina\n");
    printf("Azurira se proizvod koji postoji u maticnoj datoteci, ali se uzima u obzir da se moze desiti izdavanje nepostojece kolicine.\n");
    printf("Verbalni opis scenarija:\n");
    printf("\t(a) primljena kolicina proizvoda uvecava trenutno raspolozivu kolicinu,\n\t a poruka o uspesno izvrsenom azuriranju i evidentiranoj promeni se upisuje u odgovarajucu datoteku.\n");
    printf("\t(b) ukoliko je izdata kolicina manja od raspolozive, stanje zaliha se umanjuje,\n\t a poruka o uspesno izvrsenom azuriranju i evidentiranoj promeni se upisuje u odgovarajucu datoteku.\n");
    printf("\t(v) ako izdata kolicina nije manja od raspolozive (odnosno ako je veca ili jednaka),\n\t operacija se smatra greskom i odbija se, a poruka o neuspesnom azuriranju\n\t sa navodjenjem razloga greske se upisuje u odgovarajucu datoteku.\n");
    printf("\n\n");
    printf("\n");
    printf("Stara maticna datoteka: \n");
    printf("Id   Naziv     Kolicina\n");
    printf("--------------------------------\n");
    printf("20   Pro_20    100\n");
    printf("30   Pro_30    150\n");
    printf("40   Pro_40    200\n");
    printf("50   Pro_50    250\n");
    printf("60   Pro_60    300\n");
    printf("70   Pro_70    350\n");
    printf("80   Pro_80    400\n");
    printf("90   Pro_90    450\n\n");
    printf("Transakciona datoteka: \n");
    printf("Id   Promena   Kolicina\n");
    printf("--------------------------------\n");
    printf("80   IZLAZ     50\n");
    printf("70   IZLAZ     400\n");
    printf("80   ULAZ      100\n");
    printf("90   IZLAZ     800\n");
    printf("60   IZLAZ     500\n");
    printf("20   IZLAZ     500\n");
    printf("30   ULAZ      100\n");
    printf("20   IZLAZ     300\n");
    printf("60   IZLAZ     300\n");
    printf("40   ULAZ      150\n");
    printf("50   IZLAZ     200\n");
    printf("40   IZLAZ     100\n");
    printf("50   IZLAZ     300\n");
    printf("50   IZLAZ     300\n");
    printf("30   IZLAZ     50\n");
    printf("90   ULAZ      800\n");
    printf("70   ULAZ      200\n\n");
    printf("Sumarna i sortirana transakciona datoteka: \n");
    printf("Id   Promena   Kolicina\n");
    printf("--------------------------------\n");
    printf("20   IZLAZ     800\n");
    printf("30   ULAZ      50\n");
    printf("40   ULAZ      50\n");
    printf("50   IZLAZ     800\n");
    printf("60   IZLAZ     800\n");
    printf("70   IZLAZ     200\n");
    printf("80   ULAZ      50\n");
    printf("90   ULAZ      0\n\n");
    printf("Nova maticna datoteka: \n");
    printf("Id   Naziv     Kolicina\n");
    printf("--------------------------------\n");
    printf("20   Pro_20    100\n");
    printf("30   Pro_30    200\n");
    printf("40   Pro_40    250\n");
    printf("50   Pro_50    250\n");
    printf("60   Pro_60    300\n");
    printf("70   Pro_70    150\n");
    printf("80   Pro_80    450\n");
    printf("90   Pro_90    450\n\n");
    printf("Izvestaj o promenama: \n");
    printf("Id   StaraKol  Naziv     Tip   Promena  NovaKol\n");
    printf("-----------------------------------------------------\n");
    printf("30   150       Pro_30    +     50       200\n");
    printf("40   200       Pro_40    +     50       250\n");
    printf("70   350       Pro_70    -     200      150\n");
    printf("80   400       Pro_80    +     50       450\n");
    printf("90   450       Pro_90    +     0        450\n\n");
    printf("Izvestaj o greskama: \n");
    printf("Id   Kolicina  Naziv     Tip   Promena  Greska\n");
    printf("---------------------------------------------------------------\n");
    printf("20   100       Pro_20    -     800      Nepostojeca kolicina proizvoda\n");
    printf("50   250       Pro_50    -     800      Nepostojeca kolicina proizvoda\n");
    printf("60   300       Pro_60    -     800      Nepostojeca kolicina proizvoda\n\n");
}

void o_novom_proizvodu() {
    printf("Nov proizvod\n");
    printf("Azurira se proizvod koji postoji u maticnoj datoteci, ali se uzima u obzir da se moze desiti primanje novog proizvoda.\n");
    printf("Verbalni opis scenarija:\n");
    printf("\t(a) primljena kolicina proizvoda uvecava trenutno raspolozivu kolicinu,\n\t a poruka o uspesno izvrsenom azuriranju i evidentiranoj promeni se upisuje u odgovarajucu datoteku.\n");
    printf("\t(b) ukoliko je izdata kolicina manja od raspolozive, stanje zaliha se umanjuje,\n\t a poruka o uspesno izvrsenom azuriranju i evidentiranoj promeni se upisuje u odgovarajucu datoteku.\n");
    printf("\t(v) ako je primljen proizvod koji ne postoji u maticnoj datoteci,\n\t u maticnu datoteku se dodaje novi zapis sa primljenom kolicinom,\n\t a poruka o uspesno izvrsenom azuriranju se upisuje u odgovarajucu datoteku.\n");
    printf("\n\n");
    printf("\n");
    printf("Stara maticna datoteka:\n");
    printf("Id   Naziv     Kolicina\n");
    printf("--------------------------------\n");
    printf("20   Pro_20    100\n");
    printf("30   Pro_30    150\n");
    printf("40   Pro_40    200\n");
    printf("50   Pro_50    250\n");
    printf("60   Pro_60    300\n");
    printf("70   Pro_70    350\n");
    printf("80   Pro_80    400\n");
    printf("90   Pro_90    450\n\n");
    printf("Transakciona datoteka:\n");
    printf("Id   Promena   Kolicina\n");
    printf("--------------------------------\n");
    printf("55   ULAZ      205\n");
    printf("95   ULAZ      295\n");
    printf("55   ULAZ      50\n");
    printf("45   ULAZ      245\n");
    printf("95   IZLAZ     15\n");
    printf("15   ULAZ      230\n");
    printf("10   ULAZ      190\n");
    printf("20   IZLAZ     100\n");
    printf("30   IZLAZ     50\n");
    printf("20   ULAZ      50\n");
    printf("35   ULAZ      285\n");
    printf("36   ULAZ      236\n");
    printf("35   IZLAZ     50\n");
    printf("37   ULAZ      240\n");
    printf("37   IZLAZ     3\n");
    printf("70   ULAZ      100\n");
    printf("10   IZLAZ     20\n");
    printf("10   ULAZ      40\n");
    printf("15   IZLAZ     15\n");
    printf("95   ULAZ      15\n\n");
    printf("Sumarna i sortirana transakciona datoteka: \n");
    printf("Id   Promena   Kolicina\n");
    printf("--------------------------------\n");
    printf("10   ULAZ      210\n");
    printf("15   ULAZ      215\n");
    printf("20   IZLAZ     100\n");
    printf("30   IZLAZ     50\n");
    printf("35   ULAZ      235\n");
    printf("36   ULAZ      236\n");
    printf("37   ULAZ      237\n");
    printf("45   ULAZ      245\n");
    printf("55   ULAZ      255\n");
    printf("70   ULAZ      100\n");
    printf("90   ULAZ      295\n\n");
    printf("Nova maticna datoteka: \n");
    printf("Id   Naziv     Kolicina\n");
    printf("--------------------------------\n");
    printf("10   Pro_10    210\n");
    printf("15   Pro_15    215\n");
    printf("20   Pro_20    50\n");
    printf("30   Pro_30    100\n");
    printf("35   Pro_35    235\n");
    printf("36   Pro_36    236\n");
    printf("37   Pro_37    237\n");
    printf("40   Pro_40    200\n");
    printf("45   Pro_45    245\n");
    printf("50   Pro_50    250\n");
    printf("55   Pro_55    255\n");
    printf("60   Pro_60    300\n");
    printf("70   Pro_70    450\n");
    printf("80   Pro_80    400\n");
    printf("90   Pro_90    450\n");
    printf("95   Pro_95    295\n\n");
    printf("Izvestaj o promenama:\n");
    printf("Id   StaraKol  Naziv     Tip   Promena  NovaKol\n");
    printf("-----------------------------------------------------\n");
    printf("20   100       Pro_20    -     50       50\n");
    printf("30   150       Pro_30    -     50       100\n");
    printf("70   350       Pro_70    +     100      450\n\n");
    printf("Izvestaj o novim proizvodima: \n");
    printf("Id   Naziv     Kolicina\n");
    printf("--------------------------------\n");
    printf("10   Pro_10    210\n");
    printf("15   Pro_15    215\n");
    printf("35   Pro_35    235\n");
    printf("36   Pro_36    236\n");
    printf("37   Pro_37    237\n");
    printf("45   Pro_45    245\n");
    printf("55   Pro_55    255\n");
    printf("95   Pro_95    295\n\n");

}

void o_nepostojecem_proizvodu() {
    printf("Nepostojeci proizvod\n");
    printf("Azurira se proizvod koji postoji u maticnoj datoteci,\n\t ali se uzima u obzir da se moze desiti izdavanje kolicine nepostojeceg proizvoda.\n");
    printf("Verbalni opis scenarija:\n");
    printf("\t(a) primljena kolicina se dodaje na trenutno raspolozivu kolicinu proizvoda,\n\t a poruka o uspesno izvrsenom azuriranju i evidentiranoj promeni se upisuje u odgovarajucu datoteku.\n");
    printf("\t(b) ako je izdata kolicina manja od raspolozive, stanje zaliha se umanjuje,\n\t a poruka o uspesno izvrsenom azuriranju i evidentiranoj promeni se upisuje u odgovarajucu datoteku.\n");
    printf("\t(v) ukoliko se pokusa izdavanje proizvoda koji ne postoji u maticnoj datoteci,\n\t operacija se odbija, a poruka o neuspesnom azuriranju sa navodjenjem razloga greske\n\t se upisuje u odgovarajucu datoteku.\n");
    printf("\n\n");
    printf("\n");
    printf("Stara maticna datoteka: \n");
    printf("Id   Naziv     Kolicina\n");
    printf("--------------------------------\n");
    printf("20   Pro_20    100\n");
    printf("30   Pro_30    150\n");
    printf("40   Pro_40    200\n");
    printf("50   Pro_50    250\n");
    printf("60   Pro_60    300\n");
    printf("70   Pro_70    350\n");
    printf("80   Pro_80    400\n");
    printf("90   Pro_90    450\n\n");
    printf("Transakciona datoteka: \n");
    printf("Id   Promena   Kolicina\n");
    printf("--------------------------------\n");
    printf("30   ULAZ      100\n");
    printf("92   IZLAZ     40\n");
    printf("30   IZLAZ     20\n");
    printf("23   IZLAZ     40\n");
    printf("50   ULAZ      20\n");
    printf("12   IZLAZ     40\n");
    printf("22   IZLAZ     20\n");
    printf("22   IZLAZ     20\n\n");
    printf("Sumarna i sortirana transakciona datoteka: \n");
    printf("Id   Promena   Kolicina\n");
    printf("--------------------------------\n");
    printf("12   IZLAZ     40\n");
    printf("22   IZLAZ     40\n");
    printf("23   IZLAZ     40\n");
    printf("30   ULAZ      80\n");
    printf("50   ULAZ      20\n");
    printf("92   IZLAZ     40\n\n");
    printf("Nova maticna datoteka: \n");
    printf("Id   Naziv     Kolicina\n");
    printf("--------------------------------\n");
    printf("20   Pro_20    100\n");
    printf("30   Pro_30    230\n");
    printf("40   Pro_40    200\n");
    printf("50   Pro_50    270\n");
    printf("60   Pro_60    300\n");
    printf("70   Pro_70    350\n");
    printf("80   Pro_80    400\n");
    printf("90   Pro_90    450\n\n");
    printf("Izvestaj o promenama:\n");
    printf("Id   StaraKol  Naziv     Tip   Promena  NovaKol\n");
    printf("-----------------------------------------------------\n");
    printf("30   150       Pro_30    +     80       230\n");
    printf("50   250       Pro_50    +     20       270\n\n");
    printf("Izvestaj o greskama:\n");
    printf("Id   StaraKol  Naziv     Tip   Promena  NovaKol\n");
    printf("-----------------------------------------------------\n");
    printf("12   X         X         -     40       Nepostojeci proizvod\n");
    printf("22   X         X         -     40       Nepostojeci proizvod\n");
    printf("23   X         X         -     40       Nepostojeci proizvod\n");
    printf("92   X         X         -     40       Nepostojeci proizvod\n\n");


   
}

void o_sveouhvatnom() {
    printf("Sveobuhvatni slucaj\n");
    printf("Azurira se proizvod u maticnoj datoteci, ali se uzimaju u obzir svi moguci (prethodno razmatrani) slucajevi.\n");
    printf("Verbalni opis scenarija:\n");
    printf("\t(a) primljena kolicina proizvoda se dodaje na trenutno raspolozivu kolicinu, \n\ta poruka o uspesnom azuriranju i nastaloj promeni se upisuje u odgovarajucu datoteku,\n");
    printf("\t(b) ukoliko je izdata kolicina manja od trenutno raspolozive, stanje zaliha se umanjuje,\n\ta poruka o uspesno izvrsenom azuriranju se upisuje u odgovarajucu datoteku.\n");
    printf("\t(v) izdata kolicina proizvoda nije manja od trenutno raspolozive kolicine \n\t(ili: izdata kolicina proizvoda je veca od trenutno raspolozive kolicine), sto je greska; zato se operacija odbija, \n\t a poruka o neuspesnom azuriranju i razlogu nastale greske se upisuje u odgovarajucu datoteku,\n");
    printf("\t(g) ukoliko je primljen proizvod koji ne postoji u maticnoj datoteci,\n\tkreira se novi zapis sa primljenom kolicinom, a zatim se upisuje poruka o uspesnom azuriranju.\n");
    printf("\t(d) izdaje se proizvod koji se ne nalazi u maticnoj datoteci, sto je greska; zato se operacija odbija, \n\ta poruka o neuspesnom azuriranju i razlogu nastale greske se upisuje u odgovarajucu datoteku.\n");
    printf("\n\n");
    printf("\n");
    printf("Stara maticna datoteka:\n");
    printf("Id   Naziv     Kolicina\n");
    printf("--------------------------------\n");
    printf("20   Pro_20    100\n");
    printf("30   Pro_30    150\n");
    printf("40   Pro_40    200\n");
    printf("50   Pro_50    250\n");
    printf("60   Pro_60    300\n");
    printf("70   Pro_70    350\n");
    printf("80   Pro_80    400\n");
    printf("90   Pro_90    450\n\n");
    printf("Transakciona datoteka: \n");
    printf("Id   Promena   Kolicina\n");
    printf("--------------------------------\n");
    printf("20   ULAZ      50\n");
    printf("70   IZLAZ     50  (Osnovni slucaj)\n");
    printf("90   IZLAZ     50\n");
    printf("50   IZLAZ     800  (Nepostojeca kolicina proizvoda)\n");
    printf("60   IZLAZ     800\n");
    printf("35   ULAZ      150  (Nov proizvod)\n");
    printf("15   ULAZ      150\n");
    printf("22   IZLAZ     175  (Nepostojeci proizvod)\n");
    printf("20   ULAZ      50\n");
    printf("70   IZLAZ     50  (Osnovni slucaj)\n");
    printf("90   ULAZ      50\n\n");
    printf("Sumarna i sortirana transakciona datoteka: \n");
    printf("Id   Promena   Kolicina\n");
    printf("--------------------------------\n");
    printf("15   ULAZ      150\n");
    printf("20   ULAZ      100\n");
    printf("22   IZLAZ     175\n");
    printf("35   ULAZ      150\n");
    printf("50   IZLAZ     800\n");
    printf("60   IZLAZ     800\n");
    printf("70   IZLAZ     100\n");
    printf("90   ULAZ      0\n");
    printf("92   IZLAZ     175\n\n");
    printf("Nova maticna datoteka: \n");
    printf("Id   Naziv     Kolicina\n");
    printf("--------------------------------\n");
    printf("15   Pro_15    150\n");
    printf("20   Pro_20    200\n");
    printf("30   Pro_30    150\n");
    printf("35   Pro_35    150\n");
    printf("40   Pro_40    200\n");
    printf("50   Pro_50    250\n");
    printf("60   Pro_60    300\n");
    printf("70   Pro_70    250\n");
    printf("80   Pro_80    400\n");
    printf("90   Pro_90    450\n\n");
    printf("Izvestaj o promenama: \n");
    printf("Id   StaraKol  Naziv     Tip   Promena  NovaKol\n");
    printf("-----------------------------------------------------\n");
    printf("20   100       Pro_20    +     100      200\n");
    printf("70   350       Pro_70    +     100      250\n");
    printf("90   450       Pro_90    -     0        450\n\n");
    printf("Izvestaj o novim proizvodima: \n");
    printf("Id   Proizvod   Kolicina\n");
    printf("--------------------------------\n");
    printf("15   Pro_15    150\n");
    printf("35   Pro_35    150\n\n");
    printf("Izvestaj o greskama:\n");
    printf("Id   StaraKol  Naziv     Tip   Promena  NovaKol\n");
    printf("-----------------------------------------------------\n");
    printf("22   X         X         -     175      Nepostojeci proizvod\n");
    printf("92   X         X         -     175      Nepostojeci proizvod\n\n");
    printf("Izvestaj o greskama: \n");
    printf("Id   StaraKol  Naziv     Tip   Promena  NovaKol\n");
    printf("-----------------------------------------------------\n");
    printf("50   250       Pro_50    -     800      Nepostojeca kolicina proizvoda\n");
    printf("60   300       Pro_60    -     800      Nepostojeca kolicina proizvoda\n\n");

}


