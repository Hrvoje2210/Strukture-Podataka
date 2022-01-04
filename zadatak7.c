#include <stdio.h>
#include <string.h>
#include <malloc.h>

typedef struct Directory {
    char Name[256];
struct  Directory* FirstChild;
struct  Directory* NextBrother;
};

typedef struct Directory* Node;

enum ExceptionTypes {
    InsufficientMemory,
    DuplicatedFileName,
    WrongCommand
};

int IsOutOfRange(Node);
int IsInDirectory(Node, Node);
int IsLastInDirectory(Node);
int IsDirectoryEmpty(Node);

Node CreateRootDirectory();
Node CreateDirectory(char*);
Node GetParentDirectory(Node, Node);
Node GetLastInDirectory(Node);
Node GetFromDirectoryByName(Node, char*);

void AddToDirectory(Node, Node);
void PrintDirectoryDetails(Node);
void PrintDirectoryContent(Node);

void PrintUserMenuChoices();
void PrintUserMenu(Node, Node);

int   Command_IsMd(char*);
int   Command_IsCdOut(char*);
int   Command_IsCd(char*);
int   Command_IsDir(char*);
int   Command_IsExit(char*);
char* Command_GetValue(char*);
Node  Command_Execute(Node, Node, char*);

void ExceptionHandler(int);

int main()
{
    Node _root = CreateRootDirectory();
    Node _currentDirectory = _root;

    PrintUserMenu(_root, _currentDirectory);

    free(_root);
    return 0;
}

int IsOutOfRange(Node _directory)
{
    return _directory == NULL;
}

int IsNameIdentical(char* firstName, char* secondName)
{
    return strcmp(firstName, secondName) == 0;
}

int IsInDirectory(Node _source, Node _questioned)
{
    Node _currentDirectory = _source->FirstChild;
    while (!IsOutOfRange(_currentDirectory))
    {
        if (_currentDirectory == _questioned)
            return 1;
        _currentDirectory = _currentDirectory->NextBrother;
    }
    return 0;
}

int IsLastInDirectory(Node _directory)
{
    return _directory->NextBrother == NULL;
}

int IsDirectoryEmpty(Node _directory)
{
    return _directory->FirstChild == NULL;
}

Node CreateRootDirectory()
{
    return CreateDirectory("Root");
}

Node CreateDirectory(char* name)
{
    Node _newDirectory = (Node)malloc(sizeof(char));
    if (_newDirectory == NULL)
    {
        return NULL;
    }

    strcpy(_newDirectory->Name, name);
    _newDirectory->FirstChild = NULL;
    _newDirectory->NextBrother = NULL;

    return _newDirectory;
}

Node GetParentDirectory(Node _directory, Node _root)
{
    if (_directory == _root) return _root;

    Node _currentDirectory = _root;
    while (!IsOutOfRange(_currentDirectory))
    {
        if (!IsDirectoryEmpty(_currentDirectory))
        {
            if (IsInDirectory(_currentDirectory, _directory))
            {
                return _currentDirectory;
            }

            if (!IsDirectoryEmpty(_currentDirectory->FirstChild))
                _currentDirectory = GetParentDirectory(_directory, _currentDirectory->FirstChild);
        }

        if (!IsInDirectory(_currentDirectory, _directory))
            _currentDirectory = _currentDirectory->NextBrother;
    }
    return NULL;
}

Node GetLastInDirectory(Node _directory)
{
    if (IsDirectoryEmpty(_directory)) return NULL;

    Node _currentDirectory = _directory->FirstChild;
    while (!IsLastInDirectory(_currentDirectory))
    {
        _currentDirectory = _currentDirectory->NextBrother;
    }

    return _currentDirectory;
}

Node GetFromDirectoryByName(Node _directory, char* name)
{
    if (IsDirectoryEmpty(_directory)) return NULL;
    if (IsNameIdentical(_directory->Name, name)) return _directory;

    Node _currentDirectory = _directory->FirstChild;
    while (!IsOutOfRange(_currentDirectory))
    {
        if (IsNameIdentical(_currentDirectory->Name, name))
            return _currentDirectory;
        _currentDirectory = _currentDirectory->NextBrother;
    }

    return NULL;
}

void AddToDirectory(Node _directory, Node _parent)
{
    if (IsDirectoryEmpty(_parent))
    {
        _parent->FirstChild = _directory;
        return;
    }
    if (IsOutOfRange(GetFromDirectoryByName(_parent, _directory->Name)))
    {
        GetLastInDirectory(_parent)->NextBrother = _directory;
        return;
    }
}

void PrintDirectoryDetails(Node _directory)
{
    printf("-- %s\n", _directory->Name);
}

void PrintDirectoryContent(Node _directory)
{
    printf("\n%s:\n", _directory->Name);

    if (IsDirectoryEmpty(_directory))
    {
        printf("Direktorij je prazan.\n");
        return;
    }

    Node _currentDirectory = _directory->FirstChild;
    while (!IsOutOfRange(_currentDirectory))
    {
        PrintDirectoryDetails(_currentDirectory);
        _currentDirectory = _currentDirectory->NextBrother;
    }
}

void PrintUserMenuChoices()
{
    int i;
    printf("--MENI--\n[Naredba] <-> [Objasnjenje]\n");

    char* options[] = {
        "md <dir>   -  kreiraj novi direktorij u trenutnom",
        "cd..       -  izadi iz trenutnog direktorija u roditeljski",
        "cd <dir>   -  pozicioniraj se u odredeni direktorij",
        "dir        -  ispisi sadrzaj trenutnog direktorija",
        "exit       -  izlaz"
    };
    for (i = 0; i < sizeof(options) / sizeof(char*); i++)
        printf("%s\n", options[i]);
}

void PrintUserMenu(Node _root, Node _currentDirectory)
{
    char userChoice[64];

    PrintUserMenuChoices();

    fgets(userChoice, sizeof userChoice, stdin);
    strtok(userChoice, "\n");

    Node _commandResult = Command_Execute(_currentDirectory, _root, userChoice);
    if (!IsOutOfRange(_commandResult))
    {
        _currentDirectory = _commandResult;
    }

    if (!Command_IsExit(userChoice))
    {
        printf("\n");
        PrintUserMenu(_root, _currentDirectory);
    }
}

int Command_IsMd(char* command)
{
    return strcmp(command, "md") == 0;
}

int Command_IsCdOut(char* command)
{
    return strcmp(command, "cd..") == 0;
}

int Command_IsCd(char* command)
{
    return strcmp(command, "cd") == 0;
}

int Command_IsDir(char* command)
{
    return strcmp(command, "dir") == 0;
}

int Command_IsExit(char* command)
{
    return strcmp(command, "exit") == 0;
}

char* Command_GetValue(char* expression)
{
    char* value;

    value = strtok(expression, " ");
    value = strtok(NULL, " ");

    return value;
}

Node Command_Execute(Node _currentDirectory, Node _root, char* rawCommand)
{
    char* value = Command_GetValue(rawCommand);
    char* command = strtok(rawCommand, " ");

    if (Command_IsMd(command))
    {
        if (value == NULL) return NULL;

        AddToDirectory(
            CreateDirectory(value),
            _currentDirectory
        );  
    }
    else if (Command_IsCdOut(command))
    {
        _currentDirectory = GetParentDirectory(_currentDirectory, _root);
    }
    else if (Command_IsCd(command))
    {
        char* singleDirectoryName;
        _currentDirectory = _root;

        singleDirectoryName = strtok(value, "\\/");
        while (singleDirectoryName != NULL)
        {
            _currentDirectory = GetFromDirectoryByName(_currentDirectory, singleDirectoryName);
            singleDirectoryName = strtok(NULL, "\\/");
        }
    }
    else if (Command_IsDir(command))
    {
        PrintDirectoryContent(_currentDirectory);
    }
    else if (!Command_IsExit(command))
    {
        return NULL;
    }

    return _currentDirectory;
}

void ExceptionHandler(int exceptionCode)
{
    printf("\nGreska:\n");

    switch (exceptionCode)
    {
        case InsufficientMemory:
            printf("Nije moguce alocirati potrebnu memoriju.");
            break;
        case DuplicatedFileName:
            printf("Vec postoji datoteka s tim imenom u direktoriju.");
            break;
        case WrongCommand:
            printf("Neispravna naredba.");
            break;
    }

    printf("\n");
}

