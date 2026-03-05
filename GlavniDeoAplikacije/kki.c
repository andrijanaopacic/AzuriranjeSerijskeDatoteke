#define _CRT_SECURE_NO_WARNINGS
#include "kki.h"
#include "datoteka.h" 
#include <stdio.h>
#include <stdlib.h>

static AkcijaAzuriranja globalna_akcija = NULL;

void postavi_akciju_azuriranja(AkcijaAzuriranja akcija) {
    globalna_akcija = akcija;
}

void pokreni_glavni_meni() {
    meni_glavni();
}



void meni_glavni() {
    int izbor;
    do {
        system("cls"); 
        printf("************************************************************************\n");
        printf("* Aplikacija: ASD\n");
        printf("* Opis      : Azuriranje serijske (sortirane redne) datoteke\n");
        printf("* Verzija   : 1.0\n");
        printf("* Upotreba : asd.exe [-d[1|2|3|4|5]]\n");
        printf("* 1 - demo za prvi slucaj azuriranja\n");
        printf("* 2 - demo za drugi slucaj azuriranja\n");
        printf("* 2 - demo za treci slucaj azuriranja \n");
        printf("* 2 - demo za cetvrti slucaj azuriranja \n");
        printf("* 2 - demo za peti slucaj azuriranja \n");
        printf("* Datum : 23.02.2026.\n");
        printf("* Autor : Andrijana Opacic, ao20220136@student.fon.bg.ac.rs\n");
        printf("* Autor : Andjela Nikolic, an20220117@student.fon.bg.ac.rs\n");
        printf("* Mentor : Sasa D. Lazarevic, slazar@fon.rs\n");
        printf("************************************************************************\n");
        printf("==================================\n");
        printf("GLAVNI MENI\n");
        printf("==================================\n");
        printf("0. Kraj rada\n");
        printf("1. Rad sa transakcionom datotekom\n");
        printf("2. Rad sa maticnom datotekom\n");
        printf("3. Pomoc\n");
        printf("Izbor: ");
        scanf("%d", &izbor);

        switch (izbor) {
        case 1: meni_transakciona(); break;
        case 2: meni_maticna(); break;
        case 3: meni_pomoc(); break;
        case 0: printf("Kraj rada...\n"); break;
        default: printf("Pogresan izbor!\n"); demo_pauza();
        }
    } while (izbor != 0);
}

void meni_transakciona() {
    int izbor;
    do {
        printf("\n====================================================================\n");
        printf("MENI Transakciona datoteka\n");
        printf("====================================================================\n");
        printf("0. Povratak\n");
        printf("1. Create ::= Kreiranje nove transakcione datoteke\n");
		printf("2. Drop ::= Unistavanje postojece transakcione datoteke\n");
		printf("3. Insert ::= Dodavanje nove transakcije\n");
        printf("4. Select All ::= Prikazivanje svih transakcija\n");
		printf("5. Select Id ::= Prikazivanje svih transakcija jednog proizvoda\n");
        printf("Izbor: ");
        scanf("%d", &izbor);

        switch (izbor) {
		case 1: demo_kreiraj_transakcionu(".\\ASD\\DATA\\transakciona.dat"); break;
		case 2: demo_drop_transakcionu(".\\ASD\\DATA\\transakciona.dat"); break;
		case 3: demo_insert_transakciona(".\\ASD\\DATA\\transakciona.dat"); break;
        case 4: demo_ispisi_transakcije(".\\ASD\\DATA\\transakciona.dat"); break;
		case 5: demo_selectId_transakciona(".\\ASD\\DATA\\transakciona.dat"); break;
        case 0: break;
        default: printf("Opcija u razvoju...\n"); break;
        }
    } while (izbor != 0);
}

void meni_maticna() {
    int izbor;
    do {
        printf("\n================================================================================\n");
        printf("MENI Maticna datoteka\n");
        printf("================================================================================\n");
        printf("0. Povratak\n");
        printf("1. Create ::= Kreiranje nove maticne datoteke\n");
        printf("2. Drop ::= Unistavanje postojece maticne datoteke\n");
        printf("3. Insert ::= Dodavanje novog proizvoda\n");
		printf("4. Delete ::= Brisanje postojeceg proizvoda\n");
        printf("5. Update All ::= Azuriranje maticne datoteke\n");
		printf("6. Update Id ::= Azuriranje jednog proizvoda\n");
        printf("7. Select All ::= Prikazivanje svih proizvoda\n");
		printf("8. Select Id ::= Prikazivanje jednog proizvoda\n");
        printf("Izbor: ");
        scanf("%d", &izbor);

        switch (izbor) {
		case 1: demo_kreiranje_maticne(".\\ASD\\DATA\\maticna.dat"); break;
		case 2: demo_drop_maticnu(".\\ASD\\DATA\\maticna.dat"); break;
		case 3: demo_insert_maticna(".\\ASD\\DATA\\maticna.dat"); break;
		case 4: demo_deleteProizvod_maticna(".\\ASD\\DATA\\maticna.dat"); break;
        case 5:
            if (globalna_akcija != NULL) globalna_akcija();
            else printf("ERROR: Akcija azuriranja nije postavljena!\n");
            //demo_pauza();
            break;
		case 6: demo_updateId_maticna(".\\ASD\\DATA\\maticna.dat"); break;
        case 7:
            demo_ispisi_maticnu(".\\ASD\\DATA\\maticna.dat");
            break;
		case 8: demo_selectId_maticna(".\\ASD\\DATA\\maticna.dat"); break;
        case 0: break;
        }
    } while (izbor != 0);
}

void meni_pomoc() {
    int izbor;
    do {
        printf("\n==================================\n");
        printf("MENI Pomoc\n");
        printf("==================================\n");
        printf("0. Povratak\n");  
        printf("1. O azuriranju serijske datoteke\n");
        printf("2. Demo\n");
        printf("3. O nama\n");
        printf("Izbor: ");
        scanf("%d", &izbor);
        switch (izbor)
        {
        case 0: break;
		case 1: o_azuriranju_serijske_datoteke(); break;
		case 2: meni_demo(); break;
		case 3: o_nama(); break;
		default: printf("Slucaj ne postoji..\n"); break;

        }
       
    } while (izbor != 0);
}

void meni_demo() {

     int izbor;
    do {
        printf("\n==================================\n");
        printf("MENI Demo\n");
        printf("==================================\n");
        printf("0. Povratak\n\n");
        printf("1. Osnovni slucaj\n");
        printf("2. Nepostojeca kolicina\n");
		printf("3. Nov proizvod\n");
		printf("4. Nepostojeci proizvod\n");
		printf("5. Sveobuhvatni slucaj\n");
        printf("Izbor: ");
        scanf("%d", &izbor);
        switch (izbor) {
        case 0: break;
        case 1: o_osnovnom_slucaju(); break;
		case 2: o_nepostojecoj_kolicini(); break;
		case 3: o_novom_proizvodu(); break;
		case 4: o_nepostojecem_proizvodu(); break;
		case 5: o_sveouhvatnom(); break;
        default: printf("Slucaj ne postoji..\n"); break;
		
        }
       
    } while (izbor != 0);
}


void demo_naslov(const char* tekst) {
    printf("\n--- %s ---\n", tekst);
}

void demo_pauza(void) {
    printf("\nPritisnite ENTER za nastavak...");
    getchar(); getchar();
}

