#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _person* position;

typedef struct _person {
    char name[100];
    char surname[100];
    int year;
	position next;
} person;

int insertafter(position new, position p) {
	new->next = p->next;
    p->next = new;
    return 0;
}

int insertend(char* name, char* surname, int year, position head) {
    position new = NULL;
    new = (position)malloc(sizeof(person));

	if (new == NULL) {
		perror("Can't allocate memory!\n");
		return -1;
	}

    //upis podataka
    strcpy(new->name, name);
    strcpy(new->surname, surname);
    new->year = year;
    new->next = NULL;


	while (head->next != NULL) {
		head = head->next;
	}
    
    insertafter(new, head); 
    return 0;
}

int ispis(position p) {

    while (p != NULL) {
        printf("%s %s %d\n", p->name, p->surname, p->year);
        p = p->next;

    }

    //ili ovako:

    /*for (position q = p->next; q != NULL; q = q->next) {
        printf("%s %s %d\n", q->name, q->surname, q->year);

    }*/
    return 0;  
}

//a) dodavanje nakon neke osobe 

int insertafterlastname(char* trazeno, char* name, char* surname, int year, position p) {

    position q = p->next;

    while (q != NULL) {
        if (strcmp(q->surname, trazeno) == 0) {
			position new = (position)malloc(sizeof(person));
            if (new == NULL) return -1; 
        
            strcpy(new->name, name);
            strcpy(new->surname, surname);
            new->year = year;
            
            //dodavanje nove nakon pronadene 

            insertafter(new, p);

            return 0;
        }
        q=q->next;
    }
    return 1;

}

//b)dodavanje prije neke osobe
int insertbeforelastname(char* trazeno, char* name, char* surname, int year, position p) {
    //p krene od heada
    //trazimo el ciji sljebednik ima trazeno prezime 
    while (p->next != NULL) {
        if(strcmp(p->next->surname, trazeno)==0){
            position new = (position)malloc(sizeof(person));

			if (new == NULL) return -1;
        
            strcpy(new->name, name);
            strcpy(new->surname, surname);
            new->year = year;

            insertafter(new, p);

            return 0;
        }
        p = p->next;
    }
    return 1;

}
//c) sortiranje (znan, neocekivano)
int sort(position p) {
    position i = NULL;
    position q = NULL; 

    for (i = p->next; i != NULL; i = i->next) {
        for (q = p->next; q->next != NULL; q = q->next) {
            //q prezime dolazi nakon p prezimena
            if (strcmp(q->surname, q->next->surname) > 0) {
            
                person temp = *q;

				strcpy(q->name, q->next->name);
                strcpy(q->surname, q->next->surname);
				q->year = q->next->year;

                strcpy(q->next->name, temp.name);
				strcpy(q->next->surname, temp.surname);
                q->next->year = temp.year;
            }
        }
    }
    return 0;
}

//d) upis u datoteku

int writefile(char* file, position p) {

	FILE* fp = fopen(file, "w");

	if (fp == NULL) {
		perror("Can't open file!\n");
		return -1;
	}

    while (p != NULL) {
        printf("%s %s %d\n", p->name, p->surname, p->year);
        p = p->next;

    }

	fclose(fp);
	return 0;
}

//e) citanje iz datoteke
int read(char* file, position p) {
    
	FILE* fp = fopen(file, "r");
    if (fp == NULL) return -1;

    char name[50];
    char surname[50];
    int year;
    while (fscanf(fp, "%49s %49s %d", name, surname, &year) == 3) {

        if (insertend(name, surname, year, p) != 0) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}


int deleteall(position p) {
    position curr = p->next;
    while (curr != NULL) {
        position next = curr->next;
        free(curr); 
        curr = next; 
    }
    p->next = NULL;
    return 0;
}

int main() {

    person head = { .name = "", .surname = "", .year = 0, .next = NULL };
	position p = &head;

	insertend("Ivan", "Ivic", 1990, p);
	insertend("Marko", "Markovic", 1985, p);
	insertend("Ana", "Anic", 1995, p);

	printf("pocetna lista:\n");
    ispis(p);

    //a) dodavanje iza ivana
    insertafterlastname("Ivic", "Petra", "Petric", 2002, p);
    
    //b)dodavanje ispred ane
    insertbeforelastname("Anic", "Luka", "Lukic", 2000, p);

    printf("nakon dodavanja: \n");
    ispis(p);


    //c)sortiranje po prezimenu
    sort(p);


    printf("sortirano: \n");
    ispis(p);

    //spremanje u datoteku

    if (writefile("dat.txt", p) == 0) {
        printf("\n lista spremljena u datoteku.\n");
        ispis(p);
    }

    //oslobodi memoriju

    deleteall(p);
    

    
}