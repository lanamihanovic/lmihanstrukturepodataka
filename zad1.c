#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
typedef struct _Student {
	char name[30];
	char lastname[30];
	int points;
} Student;

int main() {

	FILE* fp = fopen("data.txt", "r");
	if (!fp) {
		printf("Error opening file\n");
		return 1;
	}
	
	
	char buffer[50] = { 0 };
	int br = 0;

	
	//iz nekog razloga neradi, nean pojma 
	//while (!feof(fp))
	//{
	//	fgetc(buffer, 50, fp); //---> file get char(di upisujemo u koji niz, koliko charova da se procita, iz kog fajla); 
	//	br++;
	//}
	


	while (fgets(buffer, 50, fp) != NULL) {
		br++;
	}
	printf("broj studenata: %d\n", br);

	Student* students = NULL; //pazi na ovo cudo za ispit!!!
	students = (Student*)malloc(br * sizeof(Student)); //alokacija memorije za 100 studenata

	if (!fp) {
		printf ("Error opening file\n");
		free(students);
		return -1; 
	
	}
	rewind(fp); //vraca pokazivac na pocetak datoteke, jer smo prethodno procitali sve linije i pokazivac je na kraju datoteke

	//ovi nacin neradi???????
	//while (!feof(fp)) {
	//	fscanf(fp, "%29s %29s %d", students[br].name, students[br].lastname, &students[br].points);
	//	br++;
	//}
	
	//upis u datoteku, tj. u niz struktura
	for (int i = 0; i < br; i++) {
		fscanf(fp, "%29s %29s %d",
			students[i].name,
			students[i].lastname,
			&students[i].points);
	}
	

	int maxPoints = students[0].points; 
	//postavi da je max bodova na prvom studentu pa provjerava jel iduci ima vise, ako ima on postane maximum 
	for (int i = 1; i < br; i++) {
		if (students[i].points > maxPoints) {
			maxPoints = students[i].points;
		}
	}


	for (int i = 0; i < br; i++) {
		float relativepoints = (float)students[i].points / maxPoints * 100;//formula racunanje relativnih bodova

		printf("%s %s - %d, %.2f%% \n", students[i].name, students[i].lastname, students[i].points, relativepoints); //ispis podataka i 2 put postotak je samo ispis postotka... 
	}

	free(students);//ocisti memoriju alociranu za studente 
	fclose(fp);

}