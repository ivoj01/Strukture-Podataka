#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGTH 256

struct _element;
struct _stackElement;
typedef struct _element* Position;
typedef struct _stackElement* StackPosition;

typedef struct _element {
	char name[MAX_LENGTH];
	Position sibling;
	Position child;
} Element;

typedef struct _stackElement {
	Position element;
	StackPosition next;
} StackElement;

int StackPush(Position currentElement, StackPosition stackHead);
Position PopStack(StackPosition stackHead);
StackPosition CreateNewStackElement(Position currentElement, StackPosition lastStackElement);
void ElementPrint(Position currentElement);
Position ChangeStackElement(Position currentElement, StackPosition stackHead, char* inputedName);
Position CreateNewElement(char* inputedName);
Position MakeNewElement(Position currentElement, char* inputedName);
void PathPrint(Position currentElement, StackPosition stackHead);
void UserCmdInput(char* command, char* elementName);
int CmdMenu(Position currentPosition, StackPosition stackHead, char* command, char* elementName);

int main()
{
	Element root = { .name = {'C', ':'}, .sibling = NULL, .child = NULL};
	StackElement stackHead = { .element = NULL, .next = NULL };
	
	int status = 0;
	char inputedCommand[10] = { 0 }, elementName[MAX_LENGTH] = { 0 };

	if (StackPush(&root, &stackHead))
		return 1;

	PathPrint(&root, stackHead.next);

	do {
		UserCmdInput(inputedCommand, elementName);

		status = CmdMenu(&root, &stackHead, inputedCommand, elementName);

	} while (!status);
}

int StackPush(Position currentElement, StackPosition stackHead)
{
	StackPosition newStackElement = NULL, tempStackElement = NULL;

	if (!stackHead || !currentElement)
		return -1;

	for (tempStackElement = stackHead; tempStackElement->next; tempStackElement = tempStackElement->next);

	if (!CreateNewStackElement(currentElement, tempStackElement))
		return -1;

	return 0;
}

Position PopStack(StackPosition stackHead)
{
	StackPosition tempStackElement = NULL, prevStackElement = NULL;
	Position selectedDirectory;

	if (!stackHead || !stackHead->next)
		return NULL;

	for (tempStackElement = stackHead; tempStackElement->next; prevStackElement = tempStackElement, tempStackElement = tempStackElement->next);

	selectedDirectory = tempStackElement->element;
	prevStackElement->next = tempStackElement->next;
	free(tempStackElement);
	return selectedDirectory;
}

StackPosition CreateNewStackElement(Position currentElement, StackPosition lastStackElement)
{
	StackPosition newStackElement = NULL;

	if (!lastStackElement || !currentElement)
		return NULL;

	newStackElement = (StackPosition)malloc(sizeof(StackElement));

	if (!newStackElement)
	{
		perror("Can't allocate memory for new stack element!\n");
		return NULL;
	}

	newStackElement->next = lastStackElement->next;
	lastStackElement->next = newStackElement;
	newStackElement->element = currentElement;

	return newStackElement;
}

void ElementPrint(Position currentElement)
{
	Position tempSibling = NULL;
	if (!currentElement || !currentElement->child)
	{
		printf("Directory is empty.\n");
		return;
	}

	for (tempSibling = currentElement->child; tempSibling; tempSibling = currentElement->sibling)
		printf("\t%s\n", tempSibling->name);
}

Position ChangeStackElement(Position currentElement, StackPosition stackHead, char* inputedName)
{
	Position parentElement = NULL, searchedElement = NULL;

	if (!currentElement->child)
		return currentElement;

	for(searchedElement = currentElement->child; searchedElement; searchedElement = searchedElement->sibling)
		if (!strcmp(searchedElement->name, inputedName))
		{
			StackPush(searchedElement, stackHead);
			return searchedElement;
		}
	
	return currentElement;
}

Position CreateNewElement(char* inputedName)
{
	Position newElement = NULL;

	newElement = (Position)malloc(sizeof(Element));

	if (!newElement)
	{
		perror("Can't allocate memory for new element!\n");
		return NULL;
	}

	strcpy(newElement->name, inputedName);
	newElement->sibling = NULL;
	newElement->child = NULL;

	return newElement;
}

Position MakeNewElement(Position currentElement, char* inputedName)
{
	Position newElement = NULL, tempChild = NULL;

	if (!currentElement)
		return NULL;

	newElement = CreateNewElement(inputedName);

	if (!currentElement->child)
		currentElement->child = newElement;
	else
	{
		for (tempChild = currentElement->child; tempChild->sibling; tempChild = tempChild->sibling);
		currentElement->sibling = newElement;
	}

	return currentElement;
}

void PathPrint(Position currentElement, StackPosition stackHead)
{
	StackPosition stackTemp = NULL;

	if (!currentElement || !stackHead)
		return;

	for (stackTemp = stackHead->next; stackTemp->element != currentElement; stackTemp = stackTemp->next)
		printf("%s", stackTemp->element->name);

	printf("> ");
}

void UserCmdInput(char* command, char* elementName)
{
	char userInput[MAX_LENGTH] = { 0 };
	
	fgets(userInput, MAX_LENGTH, stdin);
	sscanf(userInput, "%s %s", command, elementName);
}

int CmdMenu(Position currentPosition, StackPosition stackHead, char* command, char* elementName)
{
	if (!strcmp(command, "md"))
		currentPosition = MakeNewElement(currentPosition, elementName);
	else if (!strcmp(command, "cd"))
		currentPosition = ChangeStackElement(currentPosition, stackHead, elementName);
	else if (!strcmp(command, "cd.."))
	{
		currentPosition = PopStack(stackHead);
		if (currentPosition == NULL)
			currentPosition = &currentPosition;
	}
	else if (!strcmp(command, "dir"))
		ElementPrint(currentPosition);
	else if (!strcmp(command, "exit"))
		return 1;

	PathPrint(currentPosition, stackHead);

	return 0;
}