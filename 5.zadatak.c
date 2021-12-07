#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define BUFFER_MAX_SIZE 4096
#define FILE_NAME "postfix.txt"


typedef struct Element {
	int Value;
struct Element* Next;
};

typedef struct Element* Node;

int IsListEmpty(Node);
int IsLastElement(Node);
int IsOutOfRange(Node);

int IsCharacterNumber(char);
int IsValidOperation(char);
int ConvertCharNumberToInt(char);

Node CreateHeadNode();
Node CreateNode(int);
Node GetNextElement(Node);
Node GetLastElement(Node);
int* GetValuesAndDeleteFirstNElements(Node, int);

void InsertElement(Node, Node);
void AddElementToListStart(Node, Node);
void DeleteFirstElementOfList(Node);
void DeleteAllElements(Node);

void Push(Node, int);
void Pop(Node);

void ExecuteOperationOnStack(Node, char);
void CalculateSinglePostfixTerm(Node, char*);
void CalculatePostfixTerm(Node, char*);

char* ReadFirstLineInFile(char*);

void PrintListElement(Node);
void PrintAllListElements(Node);

int main()
{
	Node _head = CreateHeadNode();

	char* postfixFormattedInput = ReadFirstLineInFile(FILE_NAME);
	if (postfixFormattedInput != NULL) 
	{
		CalculatePostfixTerm(_head, postfixFormattedInput);

		PrintAllListElements(_head);
		DeleteAllElements(_head);
	}
	else
		printf("Datoteka %s ne postoji.\n", FILE_NAME);

	free(_head);

	return 0;
}

int IsListEmpty(Node _head)
{
	return _head->Next == NULL;
}

int IsLastElement(Node _element)
{
	return _element->Next == NULL;
}

int IsOutOfRange(Node _element)
{
	return _element == NULL;
}

int IsCharacterNumber(char character)
{
	return	character >= '0' &&
			character <= '9';
}

int IsValidOperation(char operation)
{
	return	operation == '+' ||
			operation == '-' ||
			operation == '*' ||
			operation == '/';
}

int ConvertCharNumberToInt(char* character)
{
	return atoi(character);
}

Node CreateHeadNode()
{
	Node _newHead = (Node)malloc(sizeof(char));
	if (IsOutOfRange(_newHead)) return NULL;

	_newHead->Next = NULL;
	return _newHead;
}

Node CreateNode(int value)
{
	Node _newNode = (Node)malloc(sizeof(char));
	if (IsOutOfRange(_newNode)) return NULL;

	_newNode->Value = value;
	_newNode->Next = NULL;

	return _newNode;
}

Node GetNextElement(Node _element)
{
	return _element->Next;
}

Node GetLastElement(Node _head)
{
	Node _lastElement = _head;

	while (!IsLastElement(_lastElement))
		_lastElement = GetNextElement(_lastElement);

	return _lastElement;
}

int* GetValuesAndDeleteFirstNElements(Node _head, int numberOfElements)
{
	int i;
	int* firstNElements = (int*)malloc(sizeof(int) * numberOfElements);

	for (i = 0; i < numberOfElements; i++)
	{
		firstNElements[i] = GetNextElement(_head)->Value;
		Pop(_head);
	}

	return firstNElements;
}

void InsertElement(Node _previous, Node _next)
{
	_next->Next = GetNextElement(_previous);
	_previous->Next = _next;
}

void AddElementToListStart(Node _head, Node _element)
{
	InsertElement(_head, _element);
}

void DeleteFirstElementOfList(Node _head)
{
	Node _firstElement = GetNextElement(_head);

	_head->Next = GetNextElement(_firstElement);

	free(_firstElement);
}

void DeleteAllElements(Node _head)
{
	while (!IsListEmpty(_head))
		DeleteFirstElementOfList(_head);
}

void Push(Node _head, int value)
{
	AddElementToListStart(_head, CreateNode(value));
}

void Pop(Node _head)
{
	if (!IsListEmpty(_head))
		DeleteFirstElementOfList(_head);
}

void ExecuteOperationOnStack(Node _head, char operation)
{
	int* operationElements = GetValuesAndDeleteFirstNElements(_head, 2);
	switch (operation)
	{
		case '+':
			Push(_head, operationElements[1] + operationElements[0]);
			break;
		case '-':
			Push(_head, operationElements[1] - operationElements[0]);
			break;
		case '*':
			Push(_head, operationElements[1] * operationElements[0]);
			break;
		case '/':
			Push(_head, operationElements[1] / operationElements[0]);
			break;
	}
}

void CalculateSinglePostfixTerm(Node _head, char* term)
{
	if (IsCharacterNumber(*term))
		Push(_head, ConvertCharNumberToInt(term));
	else if (IsValidOperation(*term))
		ExecuteOperationOnStack(_head, *term);
}

void CalculatePostfixTerm(Node _head, char* term)
{
	char* currentTerm;
	currentTerm = strtok(term, " ");
	while (currentTerm != NULL)
	{
		CalculateSinglePostfixTerm(_head, currentTerm);
		currentTerm = strtok(NULL, " ");
	}
}

char* ReadFirstLineInFile(char* fileName)
{
	FILE* listLog;
	char buffer[BUFFER_MAX_SIZE];

	listLog = fopen(fileName, "r");
	if (listLog == NULL) return NULL;

	fgets(buffer, BUFFER_MAX_SIZE, listLog);

	return 0;
}

void PrintListElement(Node _element)
{
	printf("%d\n", _element->Value);
}

void PrintAllListElements(Node _head)
{
	if (IsListEmpty(_head))
	{
		printf("Lista je prazna.\n");
		return;
	}

	Node _currentElement = GetNextElement(_head);
	while (!IsOutOfRange(_currentElement))
	{
		PrintListElement(_currentElement);
		_currentElement = GetNextElement(_currentElement);
	}
}
