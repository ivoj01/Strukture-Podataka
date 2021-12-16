#include<stdio.h>
#include<stdlib.h>

struct _treeElement;
typedef struct _treeElement* Position;
typedef struct _treeElement
{
	int number;
	Position left;
	Position right;
} TreeElement;

void MenuPrint();
void InsertNumber(int* number);
Position InsertElement(Position root, int number);
Position CreateElement(int number);
int InOrderPrint(Position root);
int PostOrderPrint(Position root);
int PreOrderPrint(Position root);
Position DeleteElement(Position root, int number);
Position FindMin(Position root);

int main()
{
	Position root = NULL, searchedElement = NULL;
	int choice = 0, number = 0;

	do
	{
		MenuPrint();

		scanf("%d", &choice);

		switch (choice)
		{
		case 1:
			InsertNumber(&number);
			root = InsertElement(root, number);
			break;

		case 2:
			printf("Inorder ispis:\n");
			InOrderPrint(root);
			break;

		case 3:
			printf("Preorder ispis:\n");
			PreOrderPrint(root);
			break;

		case 4:
			printf("Postorder ispis:\n");
			PostOrderPrint(root);
			break;

		case 5:
			//levelorder
			break;

		case 6:
			InsertNumber(&number);
			root = DeleteElement(root, number);
			break;

		case 7:
			InsertNumber(&number);
			searchedElement = FindElement(root, number);
			if (searchedElement)
				printf("Postoji broj na adresi %p\n", searchedElement);
			break;

		case 8:
			printf("Kraj programa.");
			break;

		default:
			printf("Pogresan unos.\n");
			break;
		}
	} while (choice != 8);

	return 0;
}

void MenuPrint()
{
	printf("Odaberi radnju:\n"
		"1 - unos novog elementa\n"
		"2 - ispis inorder\n"
		"3 - ispis preorder\n"
		"4 - ispis postorder\n"
		"5 - ispis level order\n"
		"6 - brisanje elmenta\n"
		"7 - pronalazenje elmenta\n");
}

void InsertNumber(int* number)
{
	printf("Unesi broj koji zelis unijeti u binarno stablo: ");
	scanf("%d", number);
}

Position InsertElement(Position root, int number)
{
	if (!root)
		return CreateElement(number);
	else if (number < root->number)
		root->left = InsertElement(root->left, number);
	else if (number > root->number)
		root->right = InsertElement(root->right, number);

	return root;
}

Position CreateElement(int number)
{
	Position newElement = (Position)malloc(sizeof(TreeElement));

	if (!newElement)
	{
		perror("Tree element memory allocation");
		return NULL;
	}

	newElement->number = number;
	newElement->right = NULL;
	newElement->left = NULL;

	return newElement;
}

int InOrderPrint(Position root)
{
	if (root)
	{
		InOrderPrint(root->left);
		printf("%d\n", root->number);
		InOrderPrint(root->right);
	}
	
	return 0;
}

int PostOrderPrint(Position root)
{
	if (root)
	{
		PostOrderPrint(root->left);
		PostOrderPrint(root->right);
		printf("%d\n", root->number);
	}

	return 0;
}

int PreOrderPrint(Position root)
{
	if (root)
	{
		printf("%d\n", root->number);
		PreOrderPrint(root->left);
		PreOrderPrint(root->right);
	}

	return 0;
}

Position DeleteElement(Position root, int number)
{
	Position tempElement = NULL;

	if (!root)
		return root;
	else if (number < root->number)
		root->left = DeleteElement(root->left, number);
	else if (number > root->number)
		root->right = DeleteElement(root->right, number);
	else
	{
		if (root->left && root->right)
		{
			tempElement = FindMin(root->right);
			root->number = tempElement->number;
			root->right = DeleteElement(root->right, tempElement->number);
		}
		else
		{
			tempElement = root;
			if (!root->left)
				root = root->right;
			else if (!root->right)
				root = root->left;
			free(tempElement);
		}
	}
}

Position FindMin(Position root)
{
	while (root->left)
		root = root->left;
	return root;
}

Position FindElement(Position root, int number)
{
	if (!root)
		return root;
	else if (number < root->number)
		return FindElement(root->left, number);
	else if (number > root->number)
		return FindElement(root->right, number);
	return root;
}