#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 50

struct _Person;
typedef struct _Person* Position;
typedef struct _Person {
	char name[MAX_SIZE];
	char surname[MAX_SIZE];
	int birthYear;
	Position next;
} Person;

int PersonsInput(Position head);
void MenuPrint();
Position CreatePerson();
int PrependList(Position head, Position newPerson);
int PrintAllPersons(Position firstPerson);
int AppendList(Position head, Position newPerson);
void PersonSurnameInput(char* surname);
void PrintSearchedPerson(Position person);
Position FindPersonBySurname(Position head, char* surname);
int RemovePerson(Position head, Position personToRemove);
int AddPersonAfterSelected(Position selectedPerson, Position newPerson);
int AddPersonBeforeSelected(Position head, Position selectedPerson);
int SortBySurname(Position head);
int PrintPersonsInFile(Position head, char* fileName);
int LoadPersonsFromFile(Position head, char* fileName);

int main(int agrc, char** argv)
{
	Person head = { .name = {0}, .surname = {0}, .birthYear = 0, .next = NULL };
	Position p = &head;
	int choice = 0;
	char surname[MAX_SIZE] = { 0 };
	int result = 0;

	PersonsInput(p);

	do
	{
		MenuPrint();

		scanf("%d", &choice);

		switch (choice)
		{
		case 0:
			printf("Lijep pozdrav\n");
			break;
		case 1:
			result = PrependList(p, CreatePerson());
			break;

		case 2:
			result = PrintAllPersons(p);
			break;

		case 3:
			result = AppendList(p, CreatePerson());
			break;

		case 4:
			PersonSurnameInput(surname);
			PrintSearchedPerson(FindPersonBySurname(p, surname));
			break;

		case 5:
			PersonSurnameInput(surname);
			result = RemovePerson(p, FindPersonBySurname(p, surname));
			if (result == 1)
				printf("Ne mozemo pronaci trazenu osobu.\n");
			break;

		case 6:
			PersonSurnameInput(surname);
			result = AddPersonAfterSelected(FindPersonBySurname(p, surname), CreatePerson());
			break;

		case 7:
			PersonSurnameInput(surname);
			result = AddPersonBeforeSelected(p, FindPersonBySurname(p, surname));
			break;

		case 8:
			result = SortBySurname(p);

			if (result)
				printf("Lista je prazna\n");
			break;

		case 9:
			result = PrintPersonsInFile(p, argv[1]);
			break;

		case 10:
			result = LoadPersonsFromFile(p, argv[1]);
			break;

		default:
			printf("Nepostojeca naredba.\n");
			break;
		}

		if (result == -1)
		{
			printf("Greska pri unosu");
			return -1;
		}

	} while (choice != 0 && result != -1);

	free(surname);
	return EXIT_SUCCESS;
}

int PersonsInput(Position head)
{

	int numberOfPersons = 0;
	int i = 0;
	Position personBefore = NULL;
	Position temp = NULL;

	do
	{
		printf("Unesi koliko osoba zelite unijeti: ");
		scanf("%d", &numberOfPersons);
	} while (numberOfPersons < 1);

	personBefore = CreatePerson();

	if (!personBefore || !head)
		return -1;

	head->next = personBefore;

	for (i = 1; i < numberOfPersons; i++)
	{
		temp = CreatePerson();

		if (!personBefore || !temp)
			return -1;

		personBefore->next = temp;
		personBefore = temp;
	}

	return EXIT_SUCCESS;
}

void MenuPrint()
{
	printf("\n"
		"1->Dinamicki dodaj novi element na pocetak liste\n"
		"2->Ispisi listu\n"
		"3->Dinamicki dodaj novi element na kraj liste\n"
		"4->Pronadi element u listi (po prezimenu)\n"
		"5->Brisi odredeni element iz liste\n"
		"6->Dinamicki dodaj novi element iza odredenog\n"
		"7->Dinamicki dodaj novi element ispred odredenog\n"
		"8->Sortiraj listu po prezimenima\n"
		"9->Upisi listu u datoteku\n"
		"10->Citaj listu iz datoteku\n"
		"0->Izlaz\n");
}

Position CreatePerson()
{
	Position newPerson = (Person*)malloc(sizeof(Person));

	if (!newPerson)
	{
		perror("Can't alloc mem");
		return NULL;
	}

	printf("\nUnesi ime: ");
	scanf(" %[^\n]", newPerson->name);
	printf("Unesi prezime: ");
	scanf(" %[^\n]", newPerson->surname);
	printf("Unesi godinu rodenja: ");
	scanf("%d", &(newPerson->birthYear));
	newPerson->next = NULL;
}

Position CreateLoadedPerson(char* name, char* surname, int birthYear)
{
	Position newPerson = (Person*)malloc(sizeof(Person));

	if (!newPerson)
	{
		perror("Can't alloc mem");
		return NULL;
	}

	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birthYear = birthYear;
	newPerson->next = NULL;
}

int PrependList(Position head, Position newPerson)
{
	if (!head || !newPerson)
		return -1;

	newPerson->next = head->next;
	head->next = newPerson;

	return EXIT_SUCCESS;
}

int PrintAllPersons(Position head)
{
	Position temp = NULL;

	if (!head->next)
	{
		printf("Lista je prazna.\n");
		return 1;
	}

	printf("Ispis svih osoba.\nIme, prezime i godina rodenja:\n");

	for (temp = head->next; temp; temp = temp->next)
		printf("%s %s %d\n", temp->name, temp->surname, temp->birthYear);

	return EXIT_SUCCESS;
}

int AppendList(Position head, Position newPerson)
{
	Position temp = NULL;

	if (!head || !newPerson)
		return -1;

	if (!head->next)
	{
		head->next = newPerson;
		return EXIT_SUCCESS;
	}

	for (temp = head->next; temp->next; temp = temp->next);

	temp->next = newPerson;
	return EXIT_SUCCESS;
}

void PersonSurnameInput(char* surname)
{
	printf("Unesi prezime osobe koju hoces oznacit: ");
	scanf(" %[^\n]", surname);
}

Position FindPersonBySurname(Position head, char* surname)
{
	Position temp = NULL;

	for (temp = head->next; temp; temp = temp->next)
		if (!strcmp(temp->surname, surname))
			return temp;

	return NULL;
}

void PrintSearchedPerson(Position person)
{
	if (person)
		printf("Pronasli ste osobu s trazenim prezimenom:\n%s %s %d\n", person->name, person->surname, person->birthYear);
	else
		printf("Osoba s tim prezimenom ne postoji.\n");
}

int RemovePerson(Position head, Position personToRemove)
{
	Position temp = NULL;

	if (!personToRemove)
		return 1;

	for (temp = head; temp->next; temp = temp->next)
		if (temp->next == personToRemove)
		{
			temp->next = personToRemove->next;
			free(personToRemove);
			printf("Osoba je uspjesno izbrisana.\n");
			return EXIT_SUCCESS;
		}

	return 1;
}

int AddPersonAfterSelected(Position selectedPerson, Position newPerson)
{
	if (!selectedPerson || !newPerson)
		return -1;

	newPerson->next = selectedPerson->next;
	selectedPerson->next = newPerson;

	printf("Uspjesno napravljena radnja.\n");

	return EXIT_SUCCESS;
}

int AddPersonBeforeSelected(Position head, Position selectedPerson)
{
	Position temp = NULL;
	Position personToAdd = NULL;

	if (!head || !selectedPerson)
		return -1;

	for (temp = head; temp; temp = temp->next)
		if (temp->next == selectedPerson)
		{
			personToAdd = CreatePerson();

			if (!personToAdd)
				return -1;

			return(AddPersonAfterSelected(selectedPerson, personToAdd));
		}

	return -1;
}

int SortBySurname(Position head)
{
	Position i = NULL;
	Position j = NULL;
	Position prevJ = NULL;
	Position temp = NULL;
	Position end = NULL;

	if (!head)
		return -1;

	if (!head->next)
		return 1;

	i = head;

	while (i->next != end)
	{
		prevJ = i;
		j = i->next;

		while (j->next != end)
		{
			if (strcmp(j->surname, j->next->surname) > 0)
			{
				temp = j->next;
				prevJ->next = temp;
				j->next = temp->next;
				temp->next = j;

				j = temp;
			}
			
			prevJ = j;
			j = j->next;
		}
		end = j;
	}

	printf("Lista je uspjesno sortirana!\n");

	return EXIT_SUCCESS;
}

int PrintPersonsInFile(Position head, char* fileName)
{
	FILE* file = NULL;
	Position temp = NULL;

	file = fopen(fileName, "w");

	if (!file || !head)
		return -1;

	fprintf(file, "Ime, prezime i godina rodenja :\n");

	for (temp = head->next; temp; temp = temp->next)
		fprintf(file, "%s %s %d\n", temp->name, temp->surname, temp->birthYear);

	fclose(file);

	printf("Podaci su uspjesno upisani u datoteku!\n");

	return EXIT_SUCCESS;
}

int LoadPersonsFromFile(Position head, char* fileName)
{
	FILE* file = NULL;
	Position newPerson = NULL;
	char buffer[MAX_SIZE] = { 0 };
	char name[MAX_SIZE] = { 0 };
	char surname[MAX_SIZE] = { 0 };
	int birthYear = 0;
	int i = 0;
	int numberOfValues;

	file = fopen(fileName, "r");

	if (!file || !head)
		return -1;

	while (!feof(file))
	{
		fgets(buffer, MAX_SIZE, file);
		numberOfValues = sscanf(buffer, " %s %s%d", name, surname, &birthYear);

		if (numberOfValues == 3)
		{
			newPerson = CreateLoadedPerson(name, surname, birthYear);
			AppendList(head, newPerson);
		}

		strcpy(buffer, "");
	}

	fclose(file);

	printf("Podaci su uspjesno ispisani iz datoteke i spremljeni u memoriju!\n");

	return EXIT_SUCCESS;
}