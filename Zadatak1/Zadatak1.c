#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE 1024
#define MAX_NAME 128
#define MAX_BODOVI 100

typedef struct {
	char ime[MAX_NAME];
	char prezime[MAX_NAME];
	double bodovi;
} student;

int izracunajBrojRedakaDatoteke(char* imeDatoteke);
student* alocirajIUcitajStudenteIzDatoteke(char* imeDatoteke, int brojStudenata);
double nadiMaksimalanBrojBodovaStudenata(student* studenti, int brojStudenata);
void ispisStudenata(student* studenti, int brojStudenata);

int main(int argc, char* argv[])
{
	int brojStudenata = 0;
	char* imeDatoteke = NULL;
	student* studenti = NULL;

	if (argc < 2)
	{
		printf("Ime datoteke datoteke se ne nalazi unutar command line argumenata.\n");
		return -1;
	}

	imeDatoteke = argv[1];

	brojStudenata = izracunajBrojRedakaDatoteke(imeDatoteke);

	if (brojStudenata <= 0)
	{
		printf("Datoteka je prazna te se ne mogu ucitati podaci.\n");
		return -1;
	}

	studenti = alocirajIUcitajStudenteIzDatoteke(imeDatoteke, brojStudenata);

	if (!studenti)
		return -1;

	ispisStudenata(studenti, brojStudenata);

	free(studenti);
	return 0;
}

int izracunajBrojRedakaDatoteke(char* imeDatoteke)
{
	int brojac = 0;
	FILE* datoteka = NULL;
	char buffer[MAX_LINE] = { 0 };

	datoteka = fopen(imeDatoteke, "r");

	if (!datoteka)
	{
		printf("Greska pri otvaranju datoteke!\n");
		return -1;
	}

	while (!feof(datoteka))
	{
		fgets(buffer, MAX_LINE, datoteka);
		brojac++;
	}

	fclose(datoteka);
	return brojac;
}

student* alocirajIUcitajStudenteIzDatoteke(char* imeDatoteke, int brojStudenata)
{
	int i = 0;
	FILE* datoteka = NULL;
	student* studenti = NULL;

	datoteka = fopen(imeDatoteke, "r");

	studenti = (student*)malloc(brojStudenata * sizeof(student));

	if (!datoteka || !studenti)
	{
		printf("Greska pri otvaranju datoteke ili alociranju studenata!\n");
		return NULL;
	}

	for (i = 0; i < brojStudenata; i++)
		fscanf(datoteka, " %s %s %lf", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi);

	fclose(datoteka);
	return studenti;
}

double nadiMaksimalanBrojBodovaStudenata(student* studenti, int brojStudenata)
{
	int i = 0;
	double najveciBrojBodova = 0;

	najveciBrojBodova = studenti[0].bodovi;

	for (i = 0; i < brojStudenata; i++)
		if (studenti[i].bodovi > najveciBrojBodova)
			najveciBrojBodova = studenti[i].bodovi;

	return najveciBrojBodova;
}

void ispisStudenata(student* studenti, int brojStudenata)
{
	int i = 0;
	double maksimalanBrojBodovaStudenata = 0;

	maksimalanBrojBodovaStudenata = nadiMaksimalanBrojBodovaStudenata(studenti, brojStudenata);

	printf("%-25s%-25s%-25s%-25s\n", "IME", "PREZIME", "APSOLUTAN BROJ BODOVA", "RELATIVAN BROJ BODOVA");

	for (i = 0; i < brojStudenata; i++)
		printf("%-25s%-25s%-25.2lf%-25.2lf\n",
			studenti[i].ime,
			studenti[i].prezime,
			studenti[i].bodovi / MAX_BODOVI * 100,
			studenti[i].bodovi / maksimalanBrojBodovaStudenata * 100);
}