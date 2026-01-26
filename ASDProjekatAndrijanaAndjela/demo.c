#include <stdio.h>
#include <stdlib.h>
#include <direct.h> 
#include <string.h>

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

void createFolder(const char* folder) {
	if (_mkdir(folder) == 0) {
		printf("Folder kreiran: %s\n", folder);
	}
}

void createDemoMaticna(const char* path) {
	FILE* f = fopen(path, "wb");
	if (!f) {
		printf("Greska prilikom kreiranja maticna.dat\n");
		return;
	}

	PROIZVOD proizvodi[] = {
		{20, "Pro_20", 100},
		{30, "Pro_30", 150},
		{40, "Pro_40", 200},
		{50, "Pro_50", 250},
		{60, "Pro_60", 300},
		{70, "Pro_70", 350},
		{80, "Pro_80", 400},
		{90, "Pro_90", 450}
	};

	fwrite(proizvodi, sizeof(PROIZVOD), 8, f);
	fclose(f);
	printf("Demo maticna.dat kreirana: %s\n", path);
}

void createDemoTransakcione(const char* path, TRANSAKCIJA* niz, size_t n, const char* msg) {
	FILE* f = fopen(path, "wb");
	if (!f) {
		printf("Greska prilikom kreiranja transakciona.dat za %s\n", msg);
		return;
	}
	fwrite(niz, sizeof(TRANSAKCIJA), n, f);
	fclose(f);
	printf("Demo transakciona.dat za %s kreirana: %s\n", msg, path);
}

int main() {
	createFolder(".\\ASD");
	createFolder(".\\ASD\\DATA");
	createFolder(".\\ASD\\DATA\\OLD");
	createFolder(".\\ASD\\RPT");
	createFolder(".\\ASD\\ERR");
	createFolder(".\\ASD\\DEMO");
	createFolder(".\\ASD\\DEMO\\SLUC_1");
	createFolder(".\\ASD\\DEMO\\SLUC_2");
	createFolder(".\\ASD\\DEMO\\SLUC_3");
	createFolder(".\\ASD\\DEMO\\SLUC_4");
	createFolder(".\\ASD\\DEMO\\SLUC_5");

	createDemoMaticna(".\\ASD\\DATA\\maticna.dat");

	createDemoMaticna(".\\ASD\\DEMO\\maticna.dat");

	TRANSAKCIJA s1[] = {
		{60, ULAZ, 150},
		{40, ULAZ, 80},
		{60, ULAZ, 50},
		{60, IZLAZ, 100},
		{40, IZLAZ, 50},
		{60, IZLAZ, 150},
		{60, ULAZ, 200},
		{40, IZLAZ, 80},
		{60, ULAZ, 20},
		{60, IZLAZ, 120},
		{40, ULAZ, 100},
		{60, IZLAZ, 100},
		{70, ULAZ, 50},
		{70, IZLAZ, 50}
	};
	createDemoTransakcione(".\\ASD\\DEMO\\SLUC_1\\transakciona.dat", s1, 14, "SLUC_1");

	TRANSAKCIJA s2[] = {
		{80, IZLAZ, 50},
		{70, IZLAZ, 400},
		{80, ULAZ, 100},
		{90, IZLAZ, 800},
		{60, IZLAZ, 500},
		{20, IZLAZ, 500},
		{30, ULAZ, 100},
		{20, IZLAZ, 300},
		{60, IZLAZ, 300},
		{40, ULAZ, 150},
		{50, IZLAZ, 200},
		{40, IZLAZ, 100},
		{50, IZLAZ, 300},
		{50, IZLAZ, 300},
		{30, IZLAZ, 50},
		{90, ULAZ, 800},
		{70, ULAZ, 200}
	};
	createDemoTransakcione(".\\ASD\\DEMO\\SLUC_2\\transakciona.dat", s2, 17, "SLUC_2");

	TRANSAKCIJA s3[] = {
		{55, ULAZ, 205},
		{95, ULAZ, 295},
		{55, ULAZ, 50},
		{45, ULAZ, 245},
		{95, IZLAZ, 15},
		{15, ULAZ, 230},
		{10, ULAZ, 190},
		{20, IZLAZ, 100},
		{30, IZLAZ, 50},
		{20, ULAZ, 50},
		{35, ULAZ, 285},
		{36, ULAZ, 236},
		{35, IZLAZ, 50},
		{37, ULAZ, 240},
		{37, IZLAZ, 3},
		{70, ULAZ, 100},
		{10, IZLAZ, 20},
		{10, ULAZ, 40},
		{15, IZLAZ, 15},
		{95, ULAZ, 15}
	};
	createDemoTransakcione(".\\ASD\\DEMO\\SLUC_3\\transakciona.dat", s3, 20, "SLUC_3");

	TRANSAKCIJA s4[] = {
		{30, ULAZ, 100},
		{92, IZLAZ, 40},
		{30, IZLAZ, 20},
		{23, IZLAZ, 40},
		{50, ULAZ, 20},
		{12, IZLAZ, 40},
		{22, IZLAZ, 20},
		{22, IZLAZ, 20}
	};
	createDemoTransakcione(".\\ASD\\DEMO\\SLUC_4\\transakciona.dat", s4, 8, "SLUC_4");

	TRANSAKCIJA s5[] = {
		{20, ULAZ, 50},
		{70, IZLAZ, 50},
		{90, IZLAZ, 50},
		{50, IZLAZ, 800},
		{60, IZLAZ, 800},
		{35, ULAZ, 150},
		{15, ULAZ, 150},
		{22, IZLAZ, 175},
		{92, IZLAZ, 175},
		{20, ULAZ, 50},
		{70, IZLAZ, 50},
		{90, ULAZ, 50}
	};
	createDemoTransakcione(".\\ASD\\DEMO\\SLUC_5\\transakciona.dat", s5, 12, "SLUC_5");

	printf("Svi folderi i demo fajlovi kreirani.\n");
	return 0;
}