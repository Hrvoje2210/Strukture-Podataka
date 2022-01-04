#include <stdio.h>
#include <malloc.h>

typedef struct Element {
    int Value;
   struct Element* LeftChild;
   struct Element* RightChild;
};

 typedef struct  Element* Node;

enum ExceptionTypes {
    InsufficientMemory,
    ElementDoesNotExist,
    PrintingEmptyTree,
    WrongCommand
};

int IsOutOfRange(Node);
int IsLeafElement(Node);
int ShouldBeLeft(int, int);
int ShouldBeRight(int, int);

Node CreateNode(int);
Node AddElementToTree(Node, Node);
Node GetElementOfMinimalValue(Node);
Node GetElementOfMaximalValue(Node);
Node GetElementByValue(Node, int);

Node DeleteElementByValue(Node, int);
void DeleteAllElements(Node);

void PrintElement(Node);
void PrintAllElements(Node);

void PrintUserMenuChoices();
void PrintUserMenu(Node);

void ExceptionHandler(int);

int main()
{
    Node _root = NULL;

    PrintUserMenu(_root);

    DeleteAllElements(_root);
    return 0;
}

int IsOutOfRange(Node _element)
{
    return _element == NULL;
}

int IsLeafElement(Node _element)
{
    return IsOutOfRange(_element->LeftChild) && IsOutOfRange(_element->RightChild);
}

int ShouldBeLeft(int first, int second)
{
    return first < second;
}

int ShouldBeRight(int first, int second)
{
    return first > second;
}

Node CreateNode(int value)
{
    Node _newNode = (Node)malloc(sizeof(char));
    if (IsOutOfRange(_newNode))
    {
        return NULL;
    }

    _newNode->Value = value;
    _newNode->LeftChild = NULL;
    _newNode->RightChild = NULL;

    return _newNode;
}

Node AddElementToTree(Node _root, Node _element)
{
    if (IsOutOfRange(_root)) return _element;

    if (ShouldBeLeft(_element->Value, _root->Value))
    {
        _root->LeftChild = AddElementToTree(_root->LeftChild, _element);
    }
    else
    {
        _root->RightChild = AddElementToTree(_root->RightChild, _element);
    }

    return _root;
}

Node GetElementOfMinimalValue(Node _root)
{
    if (IsOutOfRange(_root))
    {
        return NULL;
    }

    if (IsOutOfRange(_root->LeftChild)) return _root;

    return GetElementOfMinimalValue(_root->LeftChild);
}

Node GetElementOfMaximalValue(Node _root)
{
    if (IsOutOfRange(_root))
    {
        return NULL;
    }

    if (IsOutOfRange(_root->RightChild)) return _root;

    return GetElementOfMaximalValue(_root->RightChild);
}

Node GetElementByValue(Node _root, int value)
{
    if (IsOutOfRange(_root))
    {
        return NULL;
    }

    if (value < _root->Value)
    {
        return GetElementByValue(_root->LeftChild, value);
    }
    else if (value > _root->Value)
    {
        return GetElementByValue(_root->RightChild, value);
    }

    return _root;
}

Node DeleteElementByValue(Node _root, int value)
{
    if (IsOutOfRange(_root))
    {
        return _root;
    }

    if (ShouldBeLeft(value, _root->Value))
    {
        _root->LeftChild = DeleteElementByValue(_root->LeftChild, value);
    }
    else if (ShouldBeRight(value, _root->Value))
    {
        _root->RightChild = DeleteElementByValue(_root->RightChild, value);
    }
    else if (!IsLeafElement(_root))
    {
        _root->Value = GetElementOfMinimalValue(_root->RightChild)->Value;
        _root->RightChild = DeleteElementByValue(_root->RightChild, _root->Value);
    }
    else
    {
        Node _currentElement = _root;

        if (!IsOutOfRange(_root->LeftChild))
        {
            _root = _root->LeftChild;
        }
        else
        {
            _root = _root->RightChild;
        }

        free(_currentElement);
    }

    return _root;
}

void DeleteAllElements(Node _root)
{
    if (!IsOutOfRange(_root))
    {
        DeleteAllElements(_root->LeftChild);
        DeleteAllElements(_root->RightChild);
        free(_root);
    }
}

void PrintElement(Node _element)
{
    if (IsOutOfRange(_element)) return;

    printf("%d\n", _element->Value);
}

void PrintAllElements(Node _root)
{
    if (IsOutOfRange(_root)) return;

    PrintElement(_root);
    PrintAllElements(_root->LeftChild);
    PrintAllElements(_root->RightChild);
}

void PrintUserMenuChoices()
{
    int i;
    char* options[] = {
        "Unos elementa u stablo",
        "Pronalazak elementa po vrijednosti",
        "Brisanje elementa po vrijednosti",
        "Ispis svih elemenata stabla",
        "Izlaz" };
    for (i = 0; i < sizeof(options) / sizeof(char*); i++)
        printf("%d.\t%s\n", i + 1, options[i]);
}

void PrintUserMenu(Node _root)
{
    int userChoice, newValue, searchValue;

    printf("\tMENI\n");
    PrintUserMenuChoices();

    printf("Odabir:\t");
    scanf("%d", &userChoice);

    switch (userChoice)
    {
        case 1:
            printf("Unesite novu vrijednost za upisati u stablo: ");
            scanf("%d", &newValue);
            _root = AddElementToTree(_root, CreateNode(newValue));
            break;
        case 2:
            printf("Unesite vrijednost po kojoj ce se pretrazivati: ");
            scanf("%d", &searchValue);
            PrintElement(GetElementByValue(_root, searchValue));
            break;
        case 3:
            printf("Unesite vrijednost elementa kojeg zelite izbrisati: ");
            scanf("%d", &searchValue);
            _root = DeleteElementByValue(_root, searchValue);
            break;
        case 4:
            IsOutOfRange(_root)
                ? PrintingEmptyTree
                : PrintAllElements(_root);
            break;
        case 5:
            return;
            break;
        default:
            WrongCommand;
            break;
    }

    printf("\n");
    PrintUserMenu(_root);
}

void ExceptionHandler(int exceptionCode)
{
    printf("\nGreska:\n");

    switch (exceptionCode)
    {
        case InsufficientMemory:
            printf("Nije moguce alocirati potrebnu memoriju.");
            break;
        case ElementDoesNotExist:
            printf("Ne postoji taj element.");
            break;
        case PrintingEmptyTree:
            printf("Stablo je prazno.");
            break;
        case WrongCommand:
            printf("Neispravna naredba.");
            break;
    }

    printf("\n");
}
