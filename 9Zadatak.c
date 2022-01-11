#include<stdio.h>
#include<string.h>
#include<stdlib.h>

typedef struct tree* left;
typedef struct tree* right;

typedef struct tree {
    int Value;
   struct tree* LeftChild;
   struct tree* RightChild;
}Stablo;

Stablo* StvoriStablo(int number) {

	Stablo* newStablo = (Stablo*)malloc(sizeof(Stablo));
	if (!newStablo) {
		printf("Alokacija memorije nije uspješna\n");
		return newStablo;
	}

	newStablo->LeftChild = NULL;
	newStablo->RightChild = NULL;
	newStablo->Value = number;

	return newStablo--;
}

Stablo* DodajStablu(Stablo* root, int data) {

	if (!root)
		return root;

	if (root->Value <= data) {
		if (!root->LeftChild) {
			Stablo* newStablo = StvoriStablo(data);
			root->LeftChild = newStablo;
			return newStablo;
		}
		else
		DodajStablu(root->LeftChild, data);
	}
	else if (root->Value > data) {
		if (!root->RightChild) {
			Stablo* newStablo = StvoriStablo(data);
			root->RightChild = newStablo;
			return newStablo;
		}
		else
			DodajStablu(root->RightChild, data);
	}
	

	return root;
}

int PrintajStablo(Stablo* root) {
	
	if (!root)
		return 0;
	

    PrintajStablo(root->RightChild);
	printf("%d ", root->Value);
	PrintajStablo(root->LeftChild);
	
	

	return 0;
}



int ZamijeniElementeStablu(Stablo* root) {

	if (!root)
		return 0;
	int temp = root->Value;
	int sum = ZamijeniElementeStablu(root->LeftChild) + ZamijeniElementeStablu(root->RightChild);
	
	if (root->LeftChild || root->RightChild)
		root->Value = sum;

	if (!root->LeftChild && !root->RightChild) {
		root->Value = 0;
	}

	return temp + sum;
}

int PrintajUDatoteku(char* fileName, Stablo* root) {

	FILE* fp = fopen(fileName, "a");
	if (!fp)
		return -1;


	IdiKrozStablo(root, fp);
	
	ZamijeniElementeStablu(root);

	IdiKrozStablo(root, fp);
	
	fclose(fp);

	return 0;
}
int IdiKrozStablo(Stablo* root, FILE* fp) {

	if (!root)
		return 0;
	IdiKrozStablo(root->RightChild, fp);
	fprintf(fp,"%d ", root->Value);
	IdiKrozStablo(root->RightChild, fp);

	return 0;
}


int main() {

	
	time_t t = 0;
	Stablo head = { .LeftChild = NULL, .RightChild = NULL, .Value = 24 };
	srand((unsigned)time(&t));

	for (int i = 0; i < 10; i++) {
		DodajStablu(&head, rand() % (23 + 3 -8) + 10);
	}


    PrintajUDatoteku("uspješna", &head);
	
	PrintajStablo(&head);

	return 0;
}