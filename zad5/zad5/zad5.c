#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct stack* position;
typedef struct stack {
	int number;
	position next;
} stack;

//dodavanje broja na vrh stoga
int push(int number, position head) {
	position new = (position)malloc(sizeof(stack));
	//ako nema memorije onda je greska 
	if (new == NULL) return -1;

	//broj u novi element
	new->number = number;

	//doda na pocetak liste
	new->next = head->next;
	head->next = new;

	return 0;
}

//micanje broja sa pocetka stoga
//poanta je da broj izbrises iz memorije al ga neizgubis jer ga spremis preko *number
int pop(position head, int* number) {

	//ako nema ni jednog broja na stogu- greska 
	if (head->next == NULL) return -1;

	//postavi pokazivac na prvi el stoga
	position temp = head->next;

	//spremis broj prije brisanja
	*number = temp->number;

	//stavis da je prvi element sad zapravo novi koji si nasa, tj pokazuje na iduci
	head->next = temp->next;
	//obrisi stari prvi el iz memorije
	free(temp);

	return 0;
}

int calc(char* file, position head, int* result) {

	FILE* fp = fopen(file, "r");

	if (fp == NULL) return -1;
	char c;
	int a, b, r;

	//citaj znak jedan po jedan iz datoteke 
	while(fscanf(fp, " %c", &c) == 1) {
		// ako je znak broj
		if (isdigit((unsigned char) c)) {
			//pretvori ga u broj i stavi na stog
			if (push(c - '0', head) == -1) {
				//ovo oduzimanje sa 0 radi za znamenke od 0 do 9,
				// jer je ASCII kod za '0' 48, a za '1' 49 itd. 
				// oduzimanjem '0' od znaka dobivamo njegovu num. vrij. 
				fclose(fp);
				return -1;
			}
		}//ako je znak operator
		else if(c=='+'||c=='-'||c=='*'||c=='/'){
			//uzmi dva broja sa stoga, ako je b zadnji element, a je ispod njega
			if (pop(head, &b) == -1 || pop(head, &a) == -1) {
				fclose(fp);
				return -1;
			}

			switch (c) {
			case '+':
				r = a + b;
				break;
			case '-':
				r = a - b;
				break;
			case '*':
				r = a * b;
				break;
			case '/':

				if (b == 0) {
					fclose(fp);
					return -1;
				}
				r = a / b;
				break;
			}
		//rezultat vrati na vrh stoga

			if (push(r, head) == -1) {
				fclose(fp);
				return -1;
			}
		
		}
	}
	//zadnji broj sa stoga je rezultat
	pop(head, result);
	fclose(fp);
	return 0;
}

void deleteall(position head) {
	int broj;
	while (head->next != NULL) {

		//skini element sa stoga i obrisi ga iz memorije
		pop(head, &broj);
	}

}

int main() {

	stack head = { .number= 0, .next= NULL };

	int result;

	if (calc("data.txt", &head, &result) == 0) {
		printf("Result: %d\n", result);
	}
	else printf("greska");


	deleteall(&head);

	return 0;
}