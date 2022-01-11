﻿#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_SIZE (100)
#define MAX_NUMBER (89)
#define MIN_NUMBER (11)

struct _BinaryNode;
typedef struct _BinaryNode* Position;
typedef struct _BinaryNode {
	int number;
	Position left;
	Position right;
}BinaryNode;

struct _ListNode;
typedef struct _ListNode* ListPosition;
typedef struct _ListNode {
	int number;
	ListPosition next;
}ListNode;

Position Insert(Position current, Position newElement);
Position CreateNewElement(int value);
int Replace(Position current);
int UserEnteringInt();
ListPosition CreateNewListElement(int nodeNumber);
int InsertIntoList(ListPosition listHead, int newNumber);
int PrintIntoFile(ListPosition initialListHead, ListPosition replacedListHead);
int InsertTreeIntoList(Position root, ListPosition listHead);

int main()
{
	time_t t;
	Position root = NULL;
	ListPosition initialListHead = NULL;
	ListPosition replacedListHead = NULL;

	ListNode headListElement1 = { .number = 0, .next = NULL };
	initialListHead = &headListElement1;

	ListNode headListElement2 = { .number = 0, .next = NULL };
	replacedListHead = &headListElement2;

	int userInput = 0;
	int newNumber = 0;

	srand((unsigned)time(&t));

	do
	{
		printf("Odaberi akciju: \n"
			" 1 - Random broj od 10 do 90 \n 2 - Zamijenite svaku vrijednost čvora zbrojem njegovih potomaka"
			"i ispisati oba stabla u datoteku\n"
			" 0 - Exit \n");

		userInput = UserEnteringInt();
		switch (userInput)
		{
		case 1:
		{
			Position newElement = NULL;
			ListPosition newListElement = NULL;
			if (!root)
			{
				newNumber = rand() % (MAX_NUMBER - MIN_NUMBER + 1) + MIN_NUMBER;
				BinaryNode headElement = { .number = newNumber, .left = NULL, .right = NULL };
				root = &headElement;
			}
			else
			{
				newNumber = rand() % (MAX_NUMBER - MIN_NUMBER + 1) + MIN_NUMBER;
				newElement = CreateNewElement(newNumber);
				Insert(root, newElement);
			}
			printf("Number inserted: %d \n", newNumber);
			break;
		}
		case 2:
		{
			InsertTreeIntoList(root, initialListHead);
			Replace(root);
			InsertTreeIntoList(root, replacedListHead);
			PrintIntoFile(initialListHead, replacedListHead);
			break;
		}
		default:
		{
			if (userInput != 0)
			{
				printf("Wrong input, please try again \n \n");
				break;
			}
		}

		}

	} while (userInput != 0);

	return 0;
}

Position Insert(Position current, Position newElement)
{
	if (!current)
		return newElement;

	else if (newElement->number < current->number)
		current->right = Insert(current->right, newElement);

	else if (newElement->number >= current->number)
		current->left = Insert(current->left, newElement);

	return current;
}

Position CreateNewElement(int value)
{
	Position newElement = NULL;
	newElement = (Position)malloc(sizeof(BinaryNode));

	if (!newElement)
	{
		perror("Could not allocate memory!");
		return NULL;
	}

	newElement->number = value;
	newElement->right = NULL;
	newElement->left = NULL;

	return newElement;
}

int Replace(Position current)
{
	int temp = 0;

	if (!(current->left || current->right))
	{
		temp = current->number;
		current->number = 0;
		return temp;
	}

	else if (current->left && current->right)
	{
		temp = current->number;
		current->number = Replace(current->left) + Replace(current->right);
		temp += current->number;
		return temp;
	}

	else if (current->left && !current->right)
	{
		temp = current->number;
		current->number = Replace(current->left);
		temp += current->number;
		return temp;
	}

	else
	{
		temp = current->number;
		current->number = Replace(current->right);
		temp += current->number;
		return temp;
	}

	return current->number;
}

int UserEnteringInt()
{
	char userInput[MAX_SIZE] = { 0 };
	int userInputInt = 0;
	int inputStatus = 0;
	do {

		scanf("%s", userInput);
		for (int i = 0; i < strlen(userInput); i++)
		{
			inputStatus = 0;

			if (isdigit(userInput[i]) == 0)
			{
				inputStatus = 1;
				printf("Wrong input, please enter a number \n");
				break;
			}
		}

	} while (inputStatus == 1);

	userInputInt = atoi(userInput);

	return userInputInt;
}

ListPosition CreateNewListElement(int nodeNumber)
{
	ListPosition newElement = NULL;
	newElement = (ListPosition)malloc(sizeof(ListNode));

	if (!newElement)
	{
		perror("Could not allocate memory!");
		return NULL;
	}

	newElement->number = nodeNumber;
	newElement->next = NULL;

	return newElement;
}

int InsertIntoList(ListPosition listHead, int newNumber)
{
	ListPosition temp = listHead;
	ListPosition newElement = NULL;

	newElement = CreateNewListElement(newNumber);

	if (!newElement)
	{
		perror("Could not allocate memory");
		return EXIT_FAILURE;
	}

	while (temp->next)
		temp = temp->next;

	newElement->next = temp->next;
	temp->next = newElement;

	return EXIT_SUCCESS;
}

int PrintIntoFile(ListPosition initialListHead, ListPosition replacedListHead)
{
	FILE* file = NULL;
	ListPosition temp = initialListHead->next;
	ListPosition temp2 = replacedListHead->next;


	file = fopen("tree.txt", "w");
	if (!file)
	{
		perror("Could not open file");
		return EXIT_FAILURE;
	}

	fprintf(file, "Initial binary tree: \n");

	while (temp)
	{
		fprintf(file, "%d ", temp->number);
		temp = temp->next;
	}
	fprintf(file, "\n");

	fprintf(file, "Binary tree after replacing values: \n");
	while (temp2)
	{
		fprintf(file, "%d ", temp2->number);
		temp2 = temp2->next;
	}

	fclose(file);
	return EXIT_SUCCESS;
}

int InsertTreeIntoList(Position current, ListPosition listHead)
{
	Position temp = current;

	if (temp && temp->left)
		InsertTreeIntoList(temp->left, listHead);

	if (temp)
		InsertIntoList(listHead, temp->number);

	if (temp && temp->right)
		InsertTreeIntoList(temp->right, listHead);

	return EXIT_SUCCESS;
}