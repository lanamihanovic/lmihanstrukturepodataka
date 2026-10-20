#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//a)
typedef struct _person *position;

typedef struct _person {
	char name[50];
	char lastname[50];
	int birthyear;
	position next;
} Person;

int insertAfter(position p, position newperson) {
	newperson->next = p->next;//sve se vrti oko ove dvi linije na teoriji pazi! + graf prikazi ovo
	p->next = newperson;
	return 0;
}

int insertBegin(char *ime, char *prezime, int godina, position p) {
	position newperson = NULL;

	newperson = (position)malloc(sizeof(Person));
	newperson->birthyear = godina; 
	strcpy(newperson->name, ime);
	strcpy(newperson->lastname, prezime);
	newperson->next = NULL;
	insertAfter(p, newperson);

	return 0;
}


//b)
int ispis(position p) {
	for (position q = p->next; q != NULL; q = q->next) {
		printf("%s %s %d\n", q->name, q->lastname, q->birthyear);
	}
	return 0;
};
//c)
int insertEnd(char* ime, char* prezime, int godina, position p) {
	position newperson = NULL;
	newperson = (position)malloc(sizeof(Person));
	newperson->birthyear = godina;
	strcpy(newperson->name, ime);
	strcpy(newperson->lastname, prezime);
	newperson->next = NULL;
	while (p->next != NULL) {
		p = p->next;
	}
	insertAfter(p, newperson);
	return 0;
}

//d)
int findLastname(char* lastname, position p) {
	position q = NULL;
	for (q = p->next; q != NULL; q = q->next) {
		if (strcmp(q->lastname, lastname) == 0) {
			//printf("%s %s %d\n", q->name, q->lastname, q->birthyear);
			return 1;
		}
	}
	return 0;
}

//e)
int deleteperson(char* lastname, position p) {

	position curr = NULL;
	position prev = p;
	curr = prev->next;
	if (strcmp(curr->lastname, lastname) == 0) {
		prev->next = curr->next;
		free(curr);
		return 1;
	}
	else {
		prev = curr;
		curr = prev->next;
	}
	return 0;
}
int main() {

	Person head = { .name = "", .lastname = "", .birthyear = 0, .next = NULL };
	position p = &head;

	insertBegin("Ivan", "Ivic", 2000, p);
	insertBegin("Ana", "Anic", 2001, p);

	insertEnd("Marko", "Maric", 1999, p);

	printf("Ispis liste:\n");
	ispis(p);

	if (findLastname("Ivic", p))
		printf("Osoba je pronadena!\n");
	else
		printf("Osoba nije pronadena!\n");

	deleteperson("Anic", p);

	printf("\nLista nakon brisanja:\n");
	ispis(p);
	free();
	return 0;
}
