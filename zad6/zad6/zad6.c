
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "zad6.h"

typedef struct article {
    char name[100];
    double quantity;
    double price;
	struct article* next;//pokazivac na sljedeci artikl u listi
} article;

typedef struct bill {
    char date[11];
    article articlehead; //pocetak liste artikala tog racuna
    struct bill* next;//sljedeci racun u listi
} bill;

void trim(char* s) {
	//koliko je znakova u stringu
    size_t n = strlen(s);
	//dok na kraju imamo razmak, tab ili novi redak, smanji string
    while (n > 0 && isspace((unsigned char)s[n - 1])) {
        //obrisi zadnji znak i skrati tekst
        s[--n] = '\0';
    }
}

//usporedi nazive bez obzira na velika i mala slova
int samename(char* a, char* b) {
    while (*a != '\0' && *b != '\0') {
        if (tolower((unsigned char)*a) !=
            tolower((unsigned char)*b))
            return 0;

        a++;
        b++;
    }
    return *a == *b;
}

//a) sortirano dodavanje artikala
int insertarticle(bill* bill, char* name, double quantity, double price) {
	//napravi novi artikl i zauzmi memoriju za njega
    article* new = malloc(sizeof(article));

    if (new == NULL) return -1;
    //upisi nove podatke u artikl
    strcpy(new->name, name);
    new->quantity = quantity;
    new->price = price;

	//od pomocnog heada artikala tog racuna, trazi mjesto za novi artikl
    article* p = &bill->articlehead;

    //// idi dalje dok je sljedeci naziv abecedno prije novog, tj trazis misto po abecedi
    while (p->next != NULL && strcmp(p->next->name, name) < 0) {
        p = p->next;
    }

    //dodaj novi artikl iza p
	//novi povezi s ostatkom liste, a stari povezi s novim
    new->next = p->next;
    p->next = new;

    return 0;
}

//b) sortirano dodavanje racuna
void insertbill(bill* head, bill* new) {
	//od pomocnog heada liste racuna
    bill* p = head;
    //trazi misto za novi racun po datumu
    while (p->next != NULL &&
        strcmp(p->next->date, new->date) <= 0) {
        p = p->next;
    }

    new->next = p->next;
    p->next = new;
}

//brisanje svih artikala iz racuna
void deleteallart(article* head) {
    // kreni od prvog stvarnog artikla
    article* p = head->next;

    while (p != NULL) {
		//sacuvaj pokazivac na sljedeci prije nego sto oslobodis trenutni
        article* next = p->next;
		//oslobodi trenutni artikl
        free(p);
		//idi na sljedeci
        p = next;
    }
	// na kraju lista je prazna, pa head pokazuje na NULL
    head->next = NULL;
}

//brisanje racuna
void deleteallbills(bill* head) {
	// kreni od prvog stvarnog racuna
    bill* p = head->next;

    while (p != NULL) {
		//sacuvaj pokazivac na sljedeci prije nego sto oslobodis trenutni
        bill* next = p->next;

        //obrisi artikle sa racuna
        deleteallart(&p->articlehead);

        //obrisi racun
        free(p);
		//idi na sljedeci
        p = next;
    }

    head->next = NULL;
}

int readbill(char* file, bill* head) {

    FILE* fp = fopen(file, "r");
    if (fp == NULL) return -1;

    //zauzmi memoriju za CIJELI racun, a ne pokazivac
    bill* bill = malloc(sizeof(*bill));

    if (bill == NULL) {
        fclose(fp);
        return -1;
    }
    // novi racun zasad nema artikala ni sljedeceg racuna
    bill->articlehead.next = NULL;
    bill->next = NULL;
    // ovdje spremamo jedan procitani redak
    char line[250];

    //prvi redak je datum
    if (fgets(line, sizeof(line), fp) == NULL) {
        fclose(fp);
        free(bill);
        return -1;
    }
    // makni novi red s kraja datuma
    trim(line);
    // datum mora imati 10 znakova
    if (strlen(line) != 10) {
        fclose(fp);
        free(bill);
        return -1;
    }

    //spremi datum u novi racun
    strcpy(bill->date, line);

    // citaj ostale retke, svaki je jedan artikl
    while (fgets(line, sizeof(line), fp) != NULL) {

        char name[100];
        double quantity, price;

        //procitaj naziv kolicinu i cijenu
        if (sscanf(line, " %99[^,], %lf, %lf",
            name, &quantity, &price) != 3) {

            //ako nisu ocitani, ocisti memoriju
            deleteallart(&bill->articlehead);
            fclose(fp);
            free(bill);
            return -1;
        }

        trim(name);
        //procitani artikl dodaj u sortiranu listu
        if (insertarticle(bill, name, quantity, price) != 0) {
            deleteallart(&bill->articlehead);
            fclose(fp);
            free(bill);
            return -1;
        }
    }
    //akoe doslo do greske u citanju
    if (ferror(fp)) {
        deleteallart(&bill->articlehead);
        fclose(fp);
        free(bill);
        return -1;
    }

    fclose(fp);

    //dodas cili racun u sortiranu listu racuna
    insertbill(head, bill);
    return 0;
}

//d)citanje svih racuna
int readall(bill* head) {
    FILE* fp = fopen("datoteke.txt", "r");
    if (fp == NULL) return -1;

    char filename[200];

    //citaj nazive datoteka iz datoteke.txt
    while (fgets(filename, sizeof(filename), fp) != NULL) {
        trim(filename);

        // ako je redak prazan, preskoci ga
        if (filename[0] == '\0') continue;

        // otvori i procitaj taj racun
        if (readbill(filename, head) != 0) {
            fclose(fp);
            return -1;
        }
    }
    //akoe greska
    if (ferror(fp)) {
        fclose(fp);
        return -1;
    }

    fclose(fp);
    return 0;
}

//e)prikazivanje i racunanje
void showcount(bill* head, char* name, char* from, char* to) {
    double totalquantity = 0;
    double totalspent = 0;

    //kroz sve racune
    for (bill* b = head->next; b != NULL; b = b->next) {

        //preskoci racune prije pocetnog datuma
        if (strcmp(b->date, from) < 0) continue;

        //nakon zavrsnog datuma prekini
        if (strcmp(b->date, to) > 0) break;

        //kroz artikle tog racuna
        for (article* a = b->articlehead.next;
            a != NULL; a = a->next) {

            //ako je to trazeni artikl
            if (samename(a->name, name)) {
                totalquantity += a->quantity;
                totalspent += a->quantity * a->price;
            }
        }
    }

    printf("ukupna kolicina: %.2lf\n", totalquantity);
    printf("ukupno potroseno: %.2lf\n", totalspent);
}

int main() {
    bill head = { 0 };

    //procitaj sve racune iz datoteka
    if (readall(&head) != 0) {
        printf("greska pri citanju racuna\n");
        deleteallbills(&head);
        return -1;
    }

    char name[100];
    char from[11], to[11];

    //koji artikl?
    printf("unesi naziv artikla: ");

    if (fgets(name, sizeof(name), stdin) == NULL) {
        printf("greska pri citanju naziva artikla\n");
        deleteallbills(&head);
        return -1;
    }

    trim(name);

    //pocetni datum
    printf("unesi pocetni datum (YYYY-MM-DD): ");

    if (scanf("%10s", from) != 1) {
        deleteallbills(&head);
        return -1;
    }

    //zavrsni datum
    printf("Datum do (YYYY-MM-DD): ");

    if (scanf("%10s", to) != 1) {
        deleteallbills(&head);
        return -1;
    }

    //ako korisnik upise / umjesto -, pretvori u -
    for (int i = 0; from[i] != '\0'; i++) {
        if (from[i] == '/') from[i] = '-';
    }

    for (int i = 0; to[i] != '\0'; i++) {
        if (to[i] == '/') to[i] = '-';
    }

    //izracunaj ukupnu kolicinu i potroseno
    showcount(&head, name, from, to);

    //ocisti memoriju
    deleteallbills(&head);

    return 0;
}
