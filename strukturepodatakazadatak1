#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define MAX_BUFFER 50
#define FILE_NAME "rezultati.txt"

typedef struct Student {
	char FirstName[50];
	char LastName[50];
	int Score;
} Student;

typedef struct Student* StudentPtr;

int CountRowsInFile(char*);
int EnlistStudentsFromFile(char*, StudentPtr, int);
void PrintStudentsData(StudentPtr, int, int);

int main(void)
{
	int numberOfRows, maxScore;

	StudentPtr _students;

	numberOfRows = CountRowsInFile(FILE_NAME);
	if (numberOfRows == -1)
	{
		printf("Problem kod citanja datoteke.\n");
		return 0;
	}
	_students = (StudentPtr)malloc(sizeof(Student) * numberOfRows);

	maxScore = EnlistStudentsFromFile(FILE_NAME, _students, numberOfRows);
	if (maxScore == -1)
	{
		printf("Problem kod citanja datoteke.\n");
		return 0;
	}
	else if (maxScore == 0)
	{
		printf("Ne mozemo dijeliti s nula, stoga ne vrijedi relativno ocijenjivanje.\n");
		return 0;
	}

	PrintStudentsData(_students, numberOfRows, maxScore);

	free(_students);
	return 0;
}

int CountRowsInFile(char* fileName)
{
	FILE *file;
	char* buffer;
	int numberOfRows = 0;

	file = fopen(fileName, "r");
	if (file == NULL) return -1;

	while (!feof(file))
	{
		buffer = (char*)malloc(sizeof(char) * MAX_BUFFER);

		fgets(buffer, MAX_BUFFER, file);
		if (strlen(buffer) > 0)
			numberOfRows++;

		free(buffer);
	}

	fclose(file);
	return numberOfRows;
}

int EnlistStudentsFromFile(char* fileName, StudentPtr _students, int numberOfStudents)
{
	FILE *file;
	int counter = 0;
	int maxScore = 0;

	file = fopen(fileName, "r");
	if (file == NULL) return -1;

	while (!feof(file))
	{
		fscanf(file, "%s %s %d", _students[counter].FirstName, _students[counter].LastName, &_students[counter].Score);

		if (_students[counter].Score > maxScore)
			maxScore = _students[counter].Score;

		counter++;
	}

	fclose(file);
	return maxScore;
}

void PrintStudentsData(StudentPtr _students, int numberOfStudents, int maxScore)
{
	int i;
	for (i = 0; i < numberOfStudents; i++)
		printf("%s \t %s \t %d \t %.2f\n", _students[i].FirstName, _students[i].LastName, _students[i].Score, (float)_students[i].Score / maxScore * 100.0);
}
