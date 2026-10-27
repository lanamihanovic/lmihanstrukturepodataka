#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
//nekoristi bubble sort nikad jer ces past wooho :( 
typedef struct poly* position;  
typedef struct poly {
	int coeff;
	int pow; 
	position next;
} polynom;

int insertbegin( position q, position newpoly) {
	newpoly->next = q->next;
	q->next = newpoly;
	return 0;
}
//sortirani unos
int sort(int coeff, int pow, position head) {
	position q = head;

	if (coeff == 0) return 0;

	position newpoly = (position)malloc(sizeof(polynom));

	if (newpoly == NULL) return -1;

	newpoly->coeff = coeff;
	newpoly->pow = pow; 
	newpoly->next = NULL;
	//trazis misto po eksponentu
	while (q->next != NULL && q->next->pow > newpoly->pow) {
		q = q->next;
	}
	//ako nades isti
	if (q -> next != NULL && q->next->pow == newpoly->pow) {
		//zbroji koef
		q->next->coeff += newpoly->coeff;

		if (q->next->coeff == 0) {
			position temp = q->next;
			q->next = temp->next;
			free(temp);
		}
		free(newpoly);
		return 0;
	}
	//inace umetnemo novi el iza q
	insertbegin(q, newpoly);
	return 0;
}

//citanje iz datoteke 

int readfile(FILE* fp, position head) {
	int n, coeff, pow;
	if (fscanf(fp, "%d", &n) != 1 || n < 0) return -1;

	for (int i = 0; i < n;i++) {
		//citamo koef i eksp
		if (fscanf(fp, "%d %d", &coeff, &pow) != 2) return -1;
		//odma umetnemo sortirani clan
		if (sort(coeff, pow, head) != 0) return -1;


	}

	return 0;
}
int ispispoly(position head) {
	position q = head->next;
	if (q == NULL) {
		printf("0\n");
		return 0;
	}
	while (q != NULL) {
		printf("(%d)x^%d", q->coeff, q->pow);
		if (q->next != NULL) printf(" + "); 

		q = q->next;
	}

	printf("\n");
	return 0;
}
int zbrajanjepoly(position first, position second, position result) {
	position q = first->next;
	//sve clanove prvog dodamo u rezultat
	while (q != NULL) {
		if (sort(q->coeff, q->pow, result) != 0) return -1;

		q = q -> next;

	}

	q = second->next; 
	while (q != NULL) {
		if (sort(q->coeff, q->pow, result) != 0) return -1;

		q = q->next;

	}
	return 0;
}
int mnozenjepoly(position first, position second, position result) {
	position p = first->next;
	while (p != NULL) {
		position q = second->next;

		while (q != NULL) {
			// koef mnozimo
			int coeff = p->coeff * q->coeff;

			// eksp zbrajamo
			int pow = p->pow + q->pow;

			if (sort(coeff, pow, result) != 0)
				return -1;

			q = q->next;
		}

		p = p->next;
	}
	return 0;

}

int deleteall(position head) {
	position q = head->next;
	while (q != NULL) {
		position temp = q;
		q = q->next;
		free(temp);
	}
	head->next = NULL;
	return 0;
}

int main() {

	polynom head = {0,0,NULL};
	polynom headFirst = head;
	polynom headSecond = head; 
	position q = &headFirst;
	
	polynom headsum = { 0, 0 ,NULL };
	polynom headproduct = { 0,0,NULL };

	FILE* fp = fopen("data.txt", "r");

	if (fp == NULL) {
		printf("greska pri otvaranju datoteke");
		return 1;
	}

	if (readfile(fp, &headFirst) != 0 || readfile(fp, &headSecond) != 0) {
		printf("greska pri ucitavanju");
		fclose(fp);
		deleteall(&headFirst);
		deleteall(&headSecond);
		return 1;
	}
	fclose(fp);

	printf("prvi polinom: ");
	ispispoly(&headFirst);

	printf("prvi polinom: ");
	ispispoly(&headSecond);

	if (zbrajanjepoly(&headFirst, &headSecond, &headsum) == 0) {
		printf("\nzbroj: ");
		ispispoly(&headsum);

	}
	else printf("greska kod zbrajanja");

	if (mnozenjepoly(&headFirst, &headSecond, &headproduct) == 0) {
		printf("\n umnozak: ");
		ispispoly(&headproduct);
	}
	else printf("geska u mnozenju");

	deleteall(&headFirst);
	deleteall(&headSecond);
	deleteall(&headsum);
	deleteall(&headproduct);

	


	return 0;

}