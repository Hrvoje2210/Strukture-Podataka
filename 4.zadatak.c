#include <stdio.h>
#include <malloc.h>

#define FILE_NAME_FIRST "prvi_polinom.txt"
#define FILE_NAME_SECOND "drugi_polinom.txt"

typedef struct Polinom {
	int Coefficient;
	int Degree;
	struct Polinom* Next;
};

typedef struct Polinom* Node;

int IsListEmpty(Node);
int IsLastElement(Node);
int IsOutOfRange(Node);
int IsDegreePresent(Node, int);

Node CreateHeadNode();
Node CreateNode(int, int);
Node GetNextElement(Node);
Node GetLastElement(Node);
Node GetElementByDegree(Node, int);

void InsertElement(Node, Node);
void AddElementToFitOrder(Node, Node);
void AddElementToListStart(Node, Node);
void AddElementToListEnd(Node, Node);
void CopyAllElements(Node, Node);
void DeleteAllElements(Node);

Node SumTwoPolynomials(Node, Node);
Node MultiplyTwoPolynomials(Node, Node);

void PrintListElement(Node);
void PrintAllListElements(Node);
void ReadListElementsFromFile(Node, char*);

void PrintUserMenu(Node, Node);

int main()
{
	Node _headOfFirst = CreateHeadNode();
	Node _headOfSecond = CreateHeadNode();

	ReadListElementsFromFile(_headOfFirst, FILE_NAME_FIRST);
	ReadListElementsFromFile(_headOfSecond, FILE_NAME_SECOND);

	PrintUserMenu(_headOfFirst, _headOfSecond);

	DeleteAllElements(_headOfFirst);
	DeleteAllElements(_headOfSecond);
	free(_headOfFirst);
	free(_headOfSecond);

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

int IsHigherDegree(int first, int second)
{
	return first > second;
}

int IsDegreePresent(Node _head, int degree)
{
	Node _currentElement = GetNextElement(_head);

	while (!IsOutOfRange(_currentElement))
	{
		if (_currentElement->Degree == degree)
			return 1;
		_currentElement = GetNextElement(_currentElement);
	}

	return 0;
}

Node CreateHeadNode()
{
	Node _newHead = (Node)malloc(sizeof(char));
	_newHead->Next = NULL;
	return _newHead;
}

Node CreateNode(int coefficient, int degree)
{
	Node _newNode = (Node)malloc(sizeof(char));

	_newNode->Coefficient = coefficient;
	_newNode->Degree = degree;
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

Node GetElementByDegree(Node _head, int degree)
{
	Node _currentElement = GetNextElement(_head);

	while (!IsOutOfRange(_currentElement))
	{
		if (_currentElement->Degree == degree)
			return _currentElement;
		_currentElement = GetNextElement(_currentElement);
	}

	return NULL;
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

void AddElementToListEnd(Node _head, Node _element)
{
	InsertElement(GetLastElement(_head), _element);
}

void AddElementToFitOrder(Node _head, Node _element)
{
	if (IsListEmpty(_head))
	{
		AddElementToListStart(_head, _element);
		return;
	}

	Node _currentElement = _head;
	while (!IsLastElement(_currentElement))
	{
		if (IsHigherDegree(GetNextElement(_currentElement)->Degree, _element->Degree))
		{
			InsertElement(_currentElement, _element);
			return;
		}
		_currentElement = GetNextElement(_currentElement);
	}

	AddElementToListEnd(_head, _element);
}

void CopyAllElements(Node _sourceHead, Node _destinationHead)
{
	Node _currentElement = GetNextElement(_sourceHead);

	while (!IsOutOfRange(_currentElement))
	{
		AddElementToListEnd(_destinationHead, CreateNode(_currentElement->Coefficient, _currentElement->Degree));
		_currentElement = GetNextElement(_currentElement);
	}
}

void DeleteAllElements(Node _head)
{
	Node _currentElement = GetNextElement(_head);
	Node _nextElement;

	while (!IsOutOfRange(_currentElement))
	{
		_nextElement = GetNextElement(_currentElement);
		free(_currentElement);
		_currentElement = _nextElement;
	}

	_head->Next = NULL;
}

Node SumTwoPolynomials(Node _headOfFirst, Node _headOfSecond)
{
	Node _sumHead = CreateHeadNode();

	CopyAllElements(_headOfFirst, _sumHead);
	
	Node _currentElement = GetNextElement(_headOfSecond);
	while (!IsOutOfRange(_currentElement))
	{
		if (!IsDegreePresent(_sumHead, _currentElement->Degree))
			AddElementToFitOrder(_sumHead, CreateNode(_currentElement->Coefficient, _currentElement->Degree));
		else
			GetElementByDegree(_sumHead, _currentElement->Degree)->Coefficient += _currentElement->Coefficient;

		_currentElement = GetNextElement(_currentElement);
	}

	return _sumHead;
}

Node MultiplyTwoPolynomials(Node _headOfFirst, Node _headOfSecond)
{
	Node _multiplicationHead = CreateHeadNode();

	Node _firstFactor = GetNextElement(_headOfFirst);
	Node _secondFactor = GetNextElement(_headOfSecond);

	while (!IsOutOfRange(_firstFactor))
	{
		_secondFactor = GetNextElement(_headOfSecond);
		while (!IsOutOfRange(_secondFactor))
		{
			if (!IsDegreePresent(_multiplicationHead, _firstFactor->Degree + _secondFactor->Degree))
				AddElementToFitOrder(_multiplicationHead, CreateNode(_firstFactor->Coefficient * _secondFactor->Coefficient, _firstFactor->Degree + _secondFactor->Degree));
			else
				GetElementByDegree(_multiplicationHead, _firstFactor->Degree + _secondFactor->Degree)->Coefficient += _firstFactor->Coefficient * _secondFactor->Coefficient;
			_secondFactor = GetNextElement(_secondFactor);
		}
		_firstFactor = GetNextElement(_firstFactor);
	}

	return _multiplicationHead;
}

void PrintListElement(Node _element)
{
	printf("%d * x^%d\n", _element->Coefficient, _element->Degree);
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

void ReadListElementsFromFile(Node _head, char* fileName)
{
	FILE* polynomialLog;
	int coefficient, degree;

	polynomialLog = fopen(fileName, "r");
	if (polynomialLog == NULL) return;

	while (!feof(polynomialLog))
	{
		fscanf(polynomialLog, "%d %d", &coefficient, &degree);
		AddElementToFitOrder(_head, CreateNode(coefficient, degree));
	}
}

void PrintUserMenu(Node _headOfFirst, Node _headOfSecond)
{
	Node _resultHead;
	int userChoice;

	printf("\tMENI\n1.\tZbrajanje polinoma\n2.\tMnozenje polinoma\nOdabir:\t");
	scanf("%d", &userChoice);

	if (userChoice == 1)
		_resultHead = SumTwoPolynomials(_headOfFirst, _headOfSecond);
	else if (userChoice == 2)
		_resultHead = MultiplyTwoPolynomials(_headOfFirst, _headOfSecond);
	else
		return;

	printf("Rezultat operacije:\n");
	PrintAllListElements(_resultHead);

	DeleteAllElements(_resultHead);
	free(_resultHead);
}
