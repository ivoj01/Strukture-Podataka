#include "Declarations.h";
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv)
{
    double result = 0;

    if (CalculatePostfixFromFile(&result, argv[1]) == EXIT_SUCCESS)
        printf("Result is: %lf", result);

    else
        printf("Wrong input please check your file\n");

    return EXIT_SUCCESS;
}

Position CreateElement(double number)
{
    Position newElement = NULL;

    newElement = (Position)malloc(sizeof(StackElement));

    if (!newElement)
    {
        perror("Can't allocate!\n");
        return NULL;
    }

    newElement->number = number;
    newElement->next = NULL;
    return newElement;
}

int InsertAfter(Position position, Position newElement)
{
    newElement->next = position->next;
    position->next = newElement;

    return EXIT_SUCCESS;
}

int Push(Position head, double number)
{
    Position newElement = NULL;

    newElement = CreateElement(number);

    if (!newElement)
        return -1;

    InsertAfter(head, newElement);

    return EXIT_SUCCESS;
}

int DeleteAfter(Position position)
{
    Position temp = position->next;

    if (!temp)
        return EXIT_SUCCESS;

    position->next = temp->next;
    free(temp);

    return EXIT_SUCCESS;
}

int Pop(double* result, Position head)
{
    Position first = head->next;
    if (!first)
    {
        perror("Postfix not valid! Please check your file!\n");
        return -1;
    }

    *result = first->number;
    DeleteAfter(head);

    return EXIT_SUCCESS;
}

int PerformOperation(Position head, char operation)
{
    double operand1 = 0;
    double operand2 = 0;
    double result = 0;
    int status1 = 0;
    int status2 = 0;

    status2 = Pop(&operand2, head);
    if (status2 != EXIT_SUCCESS)
        return -1;

    status1 = Pop(&operand1, head);
    if (status1 != EXIT_SUCCESS)
        return -2;

    switch (operation)
    {
    case '+':
        result = operand1 + operand2;
        break;

    case '-':
        result = operand1 - operand2;
        break;

    case '*':
        result = operand1 * operand2;
        break;

    case '/':
        if (operand2 == 0)
        {
            printf("Cannot devide with zero!\n");
            return -3;
        }
        result = operand1 + operand2;
        break;
    default:
        printf("This operation is not supported yet!\n");
        return -4;
        break;
    }

    Push(head, result);

    return EXIT_SUCCESS;
}

int CalculatePostfixFromFile(double* result, char* fileName)
{
    FILE* file = NULL;
    int fileLength = 0;
    char* buffer = NULL;
    char* currrentBuffer = 0;
    char operation = 0;
    double number = 0;
    int numBytes = 0;
    int status = 0;
    StackElement head = { .number = 0, .next = NULL };

    file = fopen(fileName, "rb");

    if (!file)
    {
        perror("Can't open file!\n");
        return -1;
    }

    fseek(file, 0, SEEK_END);
    fileLength = ftell(file);

    buffer = (char*)calloc(fileLength + 1, sizeof(char));

    if (!buffer)
    {
        perror("Can't alloc!\n");
        return -1;
    }

    rewind(file);
    fread(buffer, sizeof(char), fileLength, file);
    printf("%s", buffer);
    fclose(file);

    currrentBuffer = buffer;
    while (strlen(currrentBuffer) > 0)
    {
        status = sscanf(currrentBuffer, " %lf %n", &number, &numBytes);

        if (status == 1)
        {
            Push(&head, number);
            currrentBuffer += numBytes;
        }

        else
        {
            sscanf(currrentBuffer, " %c %n", &operation, &numBytes);
            status = PerformOperation(&head, operation);

            if (status != EXIT_SUCCESS)
            {
                free(buffer);
                while (head.next)
                {
                    DeleteAfter(&head);
                }

                return -2;
            }

            currrentBuffer += numBytes;
        }
    }

    free(buffer);
    status = Pop(result, &head);

    if (status != EXIT_SUCCESS)
        return -3;
    if (head.next != NULL)
    {
        printf("Postfix not valid! Please check your file!\n");
        return -4;
    }
    return EXIT_SUCCESS;
}