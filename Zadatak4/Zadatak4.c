#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

struct _Polynomial;

typedef struct _Polynomial* Position;

typedef struct _Polynomial {
	int Coefficient;
	int Exponent;
	Position Next;
} Polynomial;

int LoadPolynomialsFromFile(char* fileName, Position firstHead, Position secondHead);
int CreatePolynomialsList(Position head, char* buffer);
int InsertSorted(Position head, Position newPolynomial);
Polynomial* CreatePolynomial(int exponent, int coefficient);
int AddPolynomialToList(Position head, Position newPolynomial);
int SumSameCoefficientPolynomials(Position polynomialInList, Polynomial* newPolynomial);
int MergePolynomial(Position selectedPolynomial, Polynomial* newPolynomial);
int SumPolynomials(Position sumHead, Position firstHead, Position secondHead);
void FormatedPolynomialsPrint(Position head);
int MultiplyPolynomials(Position multiplyHead, Position firstHead, Position secondHead);

int main(int argc, char** argv)
{
	Polynomial firstHead = { .Coefficient = 0, .Exponent = 0, .Next = NULL };
	Polynomial secondHead = { .Coefficient = 0, .Exponent = 0, .Next = NULL };
	Polynomial sumHead = { .Coefficient = 0, .Exponent = 0, .Next = NULL };
	Polynomial multiplyHead = { .Coefficient = 0, .Exponent = 0, .Next = NULL };

	Position temp = NULL;

	Position firstP = &firstHead;
	Position secondP = &secondHead;
	Position sumP = &sumHead;
	Position multiplyP = &multiplyHead;

	LoadPolynomialsFromFile(argv[1], firstP, secondP);
	printf("Prvi\n");
	FormatedPolynomialsPrint(firstP);
	printf("\nDrugi\n");
	FormatedPolynomialsPrint(secondP);

	SumPolynomials(sumP, firstP, secondP);

	printf("\nTreci\n");
	FormatedPolynomialsPrint(sumP);

	MultiplyPolynomials(multiplyP, firstP, secondP);

	printf("\nMnozenje\n");
	FormatedPolynomialsPrint(multiplyP);
}

int LoadPolynomialsFromFile(char* fileName, Position firstHead, Position secondHead)
{
	FILE* file = NULL;
	char buffer[MAX_LINE] = { 0 };

	file = fopen(fileName, "r");

	if (!file || !firstHead || !secondHead)
		return -1;

	fgets(buffer, MAX_LINE, file);
	CreatePolynomialsList(firstHead, buffer);

	fgets(buffer, MAX_LINE, file);
	CreatePolynomialsList(secondHead, buffer);

	fclose(file);
	return 0;
}

int CreatePolynomialsList(Position head, char* buffer)
{
	char* pBuffer = buffer;
	int coefficient = 0;
	int exponent = 0;
	int bytesNumber = 0;
	int numberOfReturnedValues = 0;

	Polynomial* newPolynomial = NULL;

	while (strlen(pBuffer) > 0)
	{
		numberOfReturnedValues = sscanf(pBuffer, " %d %d %n", &coefficient, &exponent, &bytesNumber);

		if (numberOfReturnedValues == 2)
		{
			newPolynomial = CreatePolynomial(exponent, coefficient);

			if (!newPolynomial)
				return -1;

			if (InsertSorted(head, newPolynomial))
				return -1;
		}
			

		pBuffer += bytesNumber;
	}
	
	return 0;
}

int InsertSorted(Position head, Position newPolynomial)
{
	Position temp = NULL;

	if (!head || !newPolynomial)
		return -1;

	if (AddPolynomialToList(head, newPolynomial))
		return -1;

	return 0;
}

Polynomial* CreatePolynomial(int exponent, int coefficient)
{
	Polynomial* newPolynomial = NULL;
	
	newPolynomial = (Polynomial*)malloc(sizeof(Polynomial));

	if (!newPolynomial)
		return NULL;

	newPolynomial->Exponent = exponent;
	newPolynomial->Coefficient = coefficient;
	newPolynomial->Next = NULL;

	return newPolynomial;
}

int AddPolynomialToList(Position head, Position newPolynomial)
{
	Position temp = NULL;

	for (temp = head; temp->Next; temp = temp->Next)
	{
		if (newPolynomial->Exponent == temp->Next->Exponent)
			if (!SumSameCoefficientPolynomials(temp, newPolynomial))
				return 0;
			else
				return -1;

		if (newPolynomial->Exponent > temp->Next->Exponent)
			if (!MergePolynomial(temp, newPolynomial))
				return 0;
			else
				return -1;
	}

	temp->Next = newPolynomial;
	return 0;
}

int SumSameCoefficientPolynomials(Position polynomialInList, Polynomial* newPolynomial)
{
	int sum = 0;
	sum = polynomialInList->Next->Coefficient + newPolynomial->Coefficient;

	free(newPolynomial);

	if (!sum)
	{
		polynomialInList->Next = polynomialInList->Next->Next;
		free(polynomialInList->Next);
	}
	else
		polynomialInList->Next->Coefficient = sum;

	return 0;
}

int MergePolynomial(Position selectedPolynomial, Polynomial* newPolynomial)
{
	newPolynomial->Next = selectedPolynomial->Next;
	selectedPolynomial->Next = newPolynomial;

	return 0;
}

int SumPolynomials(Position sumHead, Position firstHead, Position secondHead)
{
	Position firstTemp = NULL;
	Position secondTemp = NULL;
	Position sumTemp = NULL;
	Position newPolynomial = NULL;
	int sum = 0;

	if (!firstHead || !secondHead)
		return -1;

	firstTemp = firstHead;
	secondTemp = secondHead;
	sumTemp = sumHead;

	while (firstTemp->Next != NULL && secondTemp->Next != NULL)
	{
		if (firstTemp->Next->Exponent == secondTemp->Next->Exponent)
		{
			int sum = 0;
			sum = firstTemp->Next->Coefficient + secondTemp->Next->Coefficient;

			if (sum)
			{
				newPolynomial = CreatePolynomial(firstTemp->Next->Exponent, sum);
				sumTemp->Next = newPolynomial;
				sumTemp = sumTemp->Next;
			}
			firstTemp = firstTemp->Next;
			secondTemp = secondTemp->Next;
		}

		else if (firstTemp->Next->Exponent > secondTemp->Next->Exponent)
		{
			newPolynomial = CreatePolynomial(firstTemp->Next->Exponent, firstTemp->Next->Coefficient);
			sumTemp->Next = newPolynomial;
			sumTemp = sumTemp->Next;

			firstTemp = firstTemp->Next;
		}
		else
		{
			newPolynomial = CreatePolynomial(secondTemp->Next->Exponent, secondTemp->Next->Coefficient);
			sumTemp->Next = newPolynomial;
			sumTemp = sumTemp->Next;

			secondTemp = secondTemp->Next;
		}
	}

	firstTemp = firstTemp->Next;
	secondTemp = secondTemp->Next;

	if (!secondTemp)
		for (; firstTemp; firstTemp = firstTemp->Next)
		{
			newPolynomial = CreatePolynomial(firstTemp->Exponent, firstTemp->Coefficient);
			sumTemp->Next = newPolynomial;
			sumTemp = sumTemp->Next;
		}

	if (!firstTemp)
	{
		for (; secondTemp; secondTemp = secondTemp->Next)
		{
			newPolynomial = CreatePolynomial(secondTemp->Exponent, secondTemp->Coefficient);
			sumTemp->Next = newPolynomial;
			sumTemp = sumTemp->Next;
		}
	}
	return 0;
}

void FormatedPolynomialsPrint(Position head)
{
	Position temp = NULL;
	int i = 0;
	
	for (temp = head->Next; temp; temp = temp->Next, i++)
	{
		if (temp->Coefficient < 0)
			printf("- %dx^(%d) ", temp->Coefficient, temp->Exponent);
		else if (temp->Coefficient > 0)
			printf("+ %dx^(%d) ", temp->Coefficient, temp->Exponent);
	}
}

int MultiplyPolynomials(Position multiplyHead, Position firstHead, Position secondHead)
{
	Position firstTemp = NULL;
	Position secondTemp = NULL;
	Position multiplyTemp = NULL;
	Position newPolynomial = NULL;

	int newExponent = 0;
	int newCoefficient = 0;

	if (!firstHead || !secondHead)
		return -1;

	multiplyTemp = multiplyHead;

	for(firstTemp = firstHead->Next; firstTemp; firstTemp = firstTemp->Next)
		for (secondTemp = secondHead->Next; secondTemp; secondTemp = secondTemp->Next)
		{
			newExponent = firstTemp->Exponent + secondTemp->Exponent;
			newCoefficient = firstTemp->Coefficient * secondTemp->Coefficient;
			newPolynomial = CreatePolynomial(newExponent, newCoefficient);
			multiplyTemp->Next = newPolynomial;
			multiplyTemp = multiplyTemp->Next;
		}

	return 0;
}