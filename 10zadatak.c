#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
int InputCountriesFromFile(countryNode *, char *);

cityNode *CreateNewCityNode(char *, int );
cityNode *InsertToBinTree(cityNode *, cityNode *);
int FreeBinTree(cityNode *);
int PrintTreeInOrder(cityNode *);
int InputCitiesFromFile(countryNode *, char *, char *);

int main() {
    countryNode countries = {"", "", NULL, NULL};

    InputCountriesFromFile(&countries, DRZAVE_FILE);
    PrintAll(&countries);

    FreeList(countries.next);
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

int PrintCountries(countryNode *listHead) {
	countryNode *tmp = NULL;

	if (!listHead){
        return -1;}

	tmp = listHead->next;
	while (tmp) {
		printf("\n%s %s", tmp->countryName, tmp->cityFile);
		printf("\n");
		tmp = tmp->next;
	}

	return 0;
}

int PrintAll(countryNode *listHead) {
	countryNode *tmp = NULL;

	if (!listHead) 
        return -1;

	tmp = listHead->next;
	while (tmp) {
		printf("\n%s", tmp->countryName);
		PrintTreeInOrder(tmp->root);
		printf("\n");
		tmp = tmp->next;
	}

	return 0;
}

int InputCountriesFromFile(countryNode *listHead, char *fileName) {
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
		SortedInsertCountry(listHead, CreateNewCountryNode(name, cityFile));

        InputCitiesFromFile(listHead, name, cityFile);
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
		argTaken = sscanf(buffer, "%s %d", name, &cityPopulation);
		if (argTaken != 2) {
			printf("\nFollowing line was not read successfully: \"%s\"", buffer);
			continue;
		}
		tmp->root = InsertToBinTree(tmp->root, CreateNewCityNode(name, cityPopulation));
	}

	fclose(fp);
	return 0;
}