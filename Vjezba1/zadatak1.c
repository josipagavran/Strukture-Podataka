#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define LINE (20)
#define BUFFER (50)
#define ALLOC (-2)

typedef struct _Student {
    char name[LINE];
    char lastName[LINE];
    int bodovi;
} stud;

int main() {
    int i = 0;
    int brojac = 0;
    int max = 0;
    char buffer[BUFFER] = { 0 };
    stud* Student = NULL;
    FILE* fp = NULL;

    fp = fopen("popis.txt", "r");
    if (!fp) {
        printf("Greska: datoteka se ne moze otvoriti.\n");
        return -1;
    }

    while (fgets(buffer, BUFFER, fp) != NULL) {
        brojac++;
    }

    Student = (stud*)malloc(brojac * sizeof(stud));
    if (!Student) {
        printf("Greska: neuspjela alokacija memorije.\n");
        fclose(fp);
        return ALLOC;
    }

    rewind(fp);

    for (i = 0; i < brojac; i++) {
        fscanf(fp, "%s %s %d", Student[i].name, Student[i].lastName, &Student[i].bodovi);
    }

    fclose(fp);

    max = Student[0].bodovi;
    for (i = 1; i < brojac; i++) {
        if (Student[i].bodovi > max) {
            max = Student[i].bodovi;
        }
    }

    printf("%-10s %-10s %6s %10s\n", "Ime", "Prezime", "Bodovi", "Relativno");
    for (i = 0; i < brojac; i++) {
        printf("%-10s %-10s %6d %9.2f%%\n", Student[i].name, Student[i].lastName,
            Student[i].bodovi, (double)Student[i].bodovi / max * 100);
    }

    free(Student);
    return 0;
}
