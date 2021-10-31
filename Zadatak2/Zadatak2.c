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

int PersonsEntry(Position head);
void MenuPrint();
Position CreatePerson();
int PrependList(Position head, Position newPerson);
int PrintAllPersons(Position firstPerson);
int AppendList(Position head, Position newPerson);
void PersonSurnameInput(char* surname);
void PrintSearchedPerson(Position person);
Position FindPersonBySurname(Position head, char* surname);
int RemovePerson(Position head, Position personToRemove);

int main()
{
	Person head = { .name = {0}, .surname = {0}, .birthYear = 0, .next = NULL };

	Position p = &head;
	int choice = 0;
	char surname[MAX_SIZE] = { 0 };
	int result = 0;

	PersonsEntry(p);

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
		default:
			printf("Nepostojeca naredba.\n");
			break;
		}

		if (result == -1)
		{
			printf("Greska u kodu");
			return -1;
		}

	} while (choice != 0 && result != -1);

	free(surname);
	return EXIT_SUCCESS;
}

int PersonsEntry(Position head)
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
	printf("\n1->Dinamicki dodaj novi element na pocetak liste"
		"\n2->Ispisi listu\n3->Dinamicki dodaj novi element na kraj liste"
		"\n4->Pronadi element u listi (po prezimenu)\n5->Brisi odredeni element iz liste\n0->izlaz\n");
}

Position CreatePerson()
{
	Person* newPerson = (Person*)malloc(sizeof(Person));

	if (!newPerson)
	{
		perror("Can't alloc mem");
		return NULL;
	}

	printf("\nUnesi ime: ");
	scanf(" %[^\n]", newPerson->name);
	printf("Unesi prezime: ");
	scanf(" %[^\n]", newPerson->surname);
	printf("Unesi godinu rodenja:");
	scanf("%d", &(newPerson->birthYear));
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

	for(temp = head->next; temp; temp = temp->next)
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

void PersonSurnameInput(char * surname)
{
	printf("Unesi prezime osobe: ");
	scanf(" %[^\n]", surname);
}

Position FindPersonBySurname(Position head, char * surname)
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