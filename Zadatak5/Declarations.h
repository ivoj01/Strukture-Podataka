#ifndef  DECLARATIONS_H
#define DECLARATIONS_H

struct _StackElement;
typedef struct _StackElement* Position;
typedef struct _StackElement {
    double number;
    Position next;
} StackElement;

Position CreateElement(double number);
int InsertAfter(Position position, Position newElement);
int Push(Position head, double number);
int DeleteAfter(Position position);
int Pop(double* result, Position head);
int PerformOperation(Position head, char operation);
int CalculatePostfixFromFile(double* result, char* fileName);
#endif // ! DECLARATIONS_h