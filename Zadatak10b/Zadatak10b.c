#include<stdio.h>
#include<stdlib.h>

#define MAX_LINE 1024

struct _CityElement;
typedef struct _CityElement* CityPosition;

struct _CountryElement;
typedef struct _CountryElement* CountryPosition;
typedef struct _CountryElement
{
	char name[MAX_LINE];
	CityPosition cityHead;
	CountryPosition left;
	CountryPosition right;

}CountryElement;

typedef struct _CityElement
{
	char name[MAX_LINE];
	int population;
	CityPosition next;
}CityElement;

CountryPosition CreateNewCountryElement(char name[]);
CityPosition CreateNewCityElement(char name[], int population);
int ReadFromFirstFile(CountryPosition head, char fileName[MAX_LINE], char* countrieNamesDestination, char* fileNamesDestination);
int ReadFromNewFiles(CountryPosition head, char countryName[MAX_LINE], char fileName[MAX_LINE]);
int InsertCountry(CountryPosition head, CountryPosition new);
CountryPosition Insert(CountryPosition head, CountryPosition new);
int InsertCity(CityPosition first, CityPosition new);
int Print(CountryPosition head);
int PrintCities(CityPosition head);
int FindBigger(CountryPosition current, int number);
CountryPosition Find(CountryPosition head, char countryName[MAX_LINE]);

int main()
{
	char countries[MAX_LINE] = { 0 }, files[MAX_LINE] = { 0 };
	char* buffer = malloc(MAX_LINE * sizeof(char));
	char* buffer2 = malloc(MAX_LINE * sizeof(char));

	char countryName[MAX_LINE] = { 0 };
	char file[MAX_LINE] = { 0 };
	int numBytes = 0;
	int numBytes2 = 0;

	CountryElement head = { .cityHead = CreateNewCityElement(" ", 0), .left = NULL, .right = NULL, .name = {0} };
	CountryPosition countryHead = &head;

	if (ReadFromFirstFile(countryHead, "drzave.txt", &countries, &files)) 
		return EXIT_FAILURE;

	buffer = countries;

	if (sscanf(buffer, "%s %n", countryName, &numBytes) == 1)
	{
		strcpy(countryHead->name, countryName);
		buffer += numBytes;
	}

	while (buffer)
	{
		if (sscanf(buffer, "%s %n", countryName, &numBytes) == 1)
		{
			CountryPosition new = CreateNewCountryElement(countryName);
			InsertCountry(countryHead, new);
			buffer += numBytes;
		}

		else break;
	}

	buffer = files;
	buffer2 = countries;

	while (sscanf(buffer, "%s %n", file, &numBytes) == 1)
	{
		if (sscanf(buffer2, "%s %n", countryName, &numBytes2) != 1)break;
		ReadFromNewFiles(countryHead, countryName, file);
		buffer += numBytes;
		buffer2 += numBytes2;
	}

	Print(countryHead);

	return EXIT_SUCCESS;
}

CountryPosition CreateNewCountryElement(char name[])
{
	CountryPosition new = NULL;
	new = (CountryPosition)malloc(sizeof(CountryElement));

	if (!new)
	{
		perror("Couldn't allocate memory!\n");
		return NULL;
	}

	new->cityHead = CreateNewCityElement(" ", 0);
	new->left = NULL;
	new->right = NULL;
	strcpy(new->name, name);

	return new;
}

CityPosition CreateNewCityElement(char name[], int population)
{
	CityPosition new = NULL;
	new = (CityPosition)malloc(sizeof(CityElement));

	if (!new)
	{
		perror("Couldn't allocate memory!\n");
		return NULL;
	}

	strcpy(new->name, name);
	new->next = NULL;
	new->population = population;

	return new;
}

int ReadFromFirstFile(CountryPosition head, char fileName[MAX_LINE], char* countrieNamesDestination, char* fileNamesDestination)
{
	char* buffer = (char*)malloc(MAX_LINE * sizeof(char));

	char countryName[MAX_LINE] = { 0 }, fileNameNew[MAX_LINE] = { 0 };

	FILE* fp = fopen(fileName, "r");

	if (!fp)return EXIT_FAILURE;

	while (!feof(fp))
	{
		fgets(buffer, MAX_LINE, fp);

		if (sscanf(buffer, "%s %s", countryName, fileNameNew) != 2)break;

		strncat(countrieNamesDestination, countryName);
		strncat(countrieNamesDestination, " ");

		strncat(fileNamesDestination, fileNameNew);
		strncat(fileNamesDestination, " ");

	}

	fclose(fp);

	return EXIT_SUCCESS;
}

int InsertCountry(CountryPosition head, CountryPosition new)
{
	if (head && new)
	{
		if (strcmp(head->name, new->name) < 0)
		{
			head->right = Insert(head->right, new);
		}

		else head->left = Insert(head->left, new);
	}

	return EXIT_SUCCESS;
}

CountryPosition Insert(CountryPosition head, CountryPosition new)
{
	if (!head)
		return new;

	else
	{
		if (strcmp(head->name, new->name) > 0)head->left = Insert(head->left, new);

		else head->right = Insert(head->right, new);

		return head;
	}
}

int ReadFromNewFiles(CountryPosition head, char countryName[MAX_LINE], char fileName[MAX_LINE])
{
	FILE* fp = fopen(fileName, "r");
	char* buffer = (char*)malloc(MAX_LINE * sizeof(char));


	int cityPopulation = 0;
	char cityName[MAX_LINE] = { 0 };

	CountryPosition found = Find(head, countryName);

	while (!feof(fp))
	{
		fgets(buffer, MAX_LINE, fp);

		sscanf(buffer, "%s %d", cityName, &cityPopulation);

		CityPosition new = CreateNewCityElement(cityName, cityPopulation);

		InsertCity(found->cityHead, new);
	}

	fclose(fp);

	return EXIT_SUCCESS;
}

int InsertCity(CityPosition first, CityPosition new)
{
	CityPosition temp = first->next;
	CityPosition prev = first;

	if (!temp)
	{
		first->next = new;
		return EXIT_SUCCESS;
	}
	while (temp != NULL && new->population < temp->population)
	{
		prev = temp;
		temp = temp->next;
	}

	if (!temp)
	{
		prev->next = new;
		return EXIT_SUCCESS;
	}

	if (new->population == temp->population)
	{
		if (strcmp(new->name, temp->name) > 0)
		{
			new->next = temp->next;
			temp->next = new;
		}
	}
	new->next = temp;
	prev->next = new;

	return EXIT_SUCCESS;
}

int Print(CountryPosition head)
{
	if (head)
	{
		printf("%s ", head->name);
		PrintCities(head->cityHead);
		printf("\n");
		Print(head->left);
		Print(head->right);
	}

	return EXIT_SUCCESS;
}

int PrintCities(CityPosition head)
{
	CityPosition temp = head->next;
	while (temp)
	{
		printf("%s ", temp->name);
		temp = temp->next;
	}

	printf("\n");

	return EXIT_SUCCESS;
}

int FindBigger(CountryPosition current, int number)
{
	CityPosition temp = current->cityHead->next;

	while (temp != NULL && temp->population > number)
	{
		printf("%s ", temp->name);
		temp = temp->next;
	}

	return EXIT_SUCCESS;
}


CountryPosition Find(CountryPosition head, char countryName[MAX_LINE])
{
	CountryPosition temp = NULL;

	if (!head)
		return NULL;

	if (strcmp(head->name, countryName) < 0)
		temp = Find(head->right, countryName);
	else if (strcmp(head->name, countryName) > 0)
		temp = Find(head->left, countryName);
	else 
		temp = head;

	return temp;
}