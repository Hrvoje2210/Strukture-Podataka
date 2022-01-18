#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_ARRAY_SIZE 11
#define BUFFER_LENGTH 1024
#define DRZAVE_FILE "drzave.txt"

typedef struct _cityNode {
    char cityName[BUFFER_LENGTH];
    int cityPopulation;

    struct _cityNode *left;
    struct _cityNode *right;
} cityNode;

typedef struct _countryNode {
    char countryName[BUFFER_LENGTH];
    char cityFile[BUFFER_LENGTH];
    cityNode *root;

    struct _countryNode *next;
} countryNode;

countryNode *CreateNewCountryNode(char *, char *);
int SortedInsertCountry(countryNode *, countryNode *);
int PrintCountries(countryNode *);
int FreeList(countryNode *);
int PrintAll(countryNode *);
int InputCountriesFromFile(char *, countryNode *);

cityNode *CreateNewCityNode(char *, int );
cityNode *InsertToBinTree(cityNode *, cityNode *);
int FreeBinTree(cityNode *);
int PrintTreeInOrder(cityNode *);
int InputCitiesFromFile(countryNode *, char *, char *);

int GetHashKey(char *);
void InsertToHashArray(char *, char *, countryNode *);
int FreeAll(countryNode *);

int main() {
    countryNode hashCountries[HASH_ARRAY_SIZE] = { {"", "", NULL, NULL} };

    InputCountriesFromFile(DRZAVE_FILE, hashCountries);
    PrintAll(hashCountries);

    FreeAll(hashCountries);
    return 0;
}

countryNode *CreateNewCountryNode(char *name, char *cityFile) {
	countryNode *newNode = NULL;

	if (!name || strlen(name) <= 0) 
        return NULL;

	newNode = (countryNode *)malloc(sizeof(countryNode));
	if(!newNode) 
        return NULL;

	strncpy(newNode->countryName, name, BUFFER_LENGTH);
	strncpy(newNode->cityFile, cityFile, BUFFER_LENGTH);

	newNode->root = NULL;
	newNode->next = NULL;

	return newNode;
}

int SortedInsertCountry(countryNode *listHead, countryNode *toInsert) {
	countryNode *tmp = NULL;

	if (!listHead || !toInsert)
        return -1;

	tmp = listHead;
	while (tmp->next && strcmp(tmp->next->countryName, toInsert->countryName) < 0)
		tmp = tmp->next;

	toInsert->next = tmp->next;
	tmp->next = toInsert;

	return 0;
}

int FreeList(countryNode *element) {
	if (element == NULL) return 0;

	FreeList(element->next);
	FreeBinTree(element->root);
	free(element);

	return 0;
}

int PrintCountry(countryNode *listHead) {
	countryNode *tmp = NULL;

	if (!listHead){
        return -1;}

	tmp = listHead->next;
	while (tmp) {
		printf("\n%s", tmp->countryName);
		PrintTreeInOrder(tmp->root);
		printf("\n");
		tmp = tmp->next;
	}

	return 0;		
}

int PrintAll(countryNode *hashCountries) {
	if (!hashCountries) 
        return -1;

	for(int i=0; i<HASH_ARRAY_SIZE; i++){
		printf("\nHash Key: %d\n", i);
		PrintCountry(&hashCountries[i]);
	}
	return 0;
}

int InputCountriesFromFile(char *fileName, countryNode *hashCountries) {
	FILE *fp = NULL;
	char buffer[BUFFER_LENGTH] = {'\0'};
	char name[BUFFER_LENGTH] = {'\0'};
    char cityFile[BUFFER_LENGTH] = {'\0'};
    int argTaken = 0;

	if (!fileName || strlen(fileName) <= 0)
        return -1;

	fp = fopen(fileName, "r");
	if (!fp)
        return -1;

	while (!feof(fp)) {
		fgets(buffer, BUFFER_LENGTH, fp);
		argTaken = sscanf(buffer, "%s %s", name, cityFile);
		if (argTaken != 2) {
			printf("\nFollowing line was not read successfully: \"%s\"", buffer);
			continue;
		}
		InsertToHashArray(name, cityFile, hashCountries);
	}

	fclose(fp);
	return 0;
}

cityNode *CreateNewCityNode(char *cityName, int cityPopulation) {
	cityNode *newNode = NULL;

	if (!cityName || strlen(cityName) <= 0)
        return NULL;

	newNode = (cityNode *)malloc(sizeof(cityNode));
	if(!newNode)
        return NULL;

	strncpy(newNode->cityName, cityName, BUFFER_LENGTH);
	newNode->cityPopulation = cityPopulation;

	newNode->left = NULL;
	newNode->right = NULL;

	return newNode;
}

cityNode *InsertToBinTree(cityNode *current, cityNode *toInsert) {
	if (current == NULL) {
		return toInsert;
	
	} else if (current->cityPopulation < toInsert->cityPopulation) {
		current->left = InsertToBinTree(current->left, toInsert);
	} else if (current->cityPopulation > toInsert->cityPopulation) {
		current->right = InsertToBinTree(current->right, toInsert);
	} else if (strcmp(current->cityName, toInsert->cityName) < 0) {
		current->right = InsertToBinTree(current->right, toInsert);
	} else if (strcmp(current->cityName, toInsert->cityName) > 0) {
		current->right = InsertToBinTree(current->right, toInsert);
	} else {
		printf("City with the same name already exists!");
		free(toInsert);
	}
	return current;
}

int FreeBinTree(cityNode *current) {
	if (current == NULL) return 0;

	FreeBinTree(current->left);
	FreeBinTree(current->right);
	free(current);

	return 0;
}

int PrintTreeInOrder(cityNode *current) {
	if (current == NULL) return 0;

	PrintTreeInOrder(current->left);
	printf("\n\t%s", current->cityName);
	PrintTreeInOrder(current->right);

	return 0;
}

int InputCitiesFromFile(countryNode *listHead, char *countryName, char *fileName) {
	FILE *fp = NULL;
	char buffer[BUFFER_LENGTH] = {'\0'};
	char name[BUFFER_LENGTH] = {'\0'};
	int cityPopulation = 0;
	int argTaken = 0;
    countryNode *tmp = NULL;

	if (!fileName || strlen(fileName) <= 0)
        return -1;

    tmp = listHead->next;
    while (tmp) {
        if (!(strcmp(tmp->countryName, countryName)))
            break;
        tmp = tmp->next;
    }

	fp = fopen(fileName, "r");
	if (!fp)
        return -1;

	while (!feof(fp)) {
		fgets(buffer, BUFFER_LENGTH, fp);
		strcpy(name, strtok(buffer, ","));
		cityPopulation = (int) strtok(NULL, "\n");
		tmp->root = InsertToBinTree(tmp->root, CreateNewCityNode(name, cityPopulation));
	}

	fclose(fp);
	return 0;
}

int GetHashKey(char *countryName){
	int letterCounter = 0;
	int sum = 0;
	int nameLength = strlen(countryName);

	while(letterCounter < nameLength && letterCounter < 5){
		sum += (int) countryName[letterCounter];
		letterCounter += 1;
	}
	return sum % HASH_ARRAY_SIZE;
}

void InsertToHashArray(char *name, char *cityFile, countryNode *hashCountries){
	int hashKey = GetHashKey(name);
	SortedInsertCountry(&hashCountries[hashKey], CreateNewCountryNode(name, cityFile));
	InputCitiesFromFile(&hashCountries[hashKey], name, cityFile);
}

int FreeAll(countryNode *hashCountries) {
	if (hashCountries == NULL) return 0;

	for(int i=0; i<HASH_ARRAY_SIZE; i++){
		FreeList(hashCountries[i].next);
	}

	return 0;
}
