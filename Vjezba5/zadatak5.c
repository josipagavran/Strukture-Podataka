#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>   /* strchr */

#define MAX_TOKEN (32)
#define FILE_NAME "izraz.txt"

/* jedan element stoga: broj i adresa elementa ispod njega */
typedef struct _Stack* Position;
typedef struct _Stack {
    int broj;
    Position next;
} Stack;

int Push(Position head, int broj);
int Pop(Position head, int* broj);
int EvaluateExpression(FILE* fp, Position head, int* rezultat);
void DeleteStack(Position head);

int main(void) {
    /* head je pomocni cvor (zaglavlje), vrh stoga je uvijek head.next */
    Stack head = { .broj = 0, .next = NULL };
    int rezultat = 0;
    FILE* fp = fopen(FILE_NAME, "r");

    if (fp == NULL) {
        printf("Greska: datoteka %s se ne moze otvoriti.\n", FILE_NAME);
        return -1;
    }

    if (EvaluateExpression(fp, &head, &rezultat) == 0) {
        printf("Rezultat izraza: %d\n", rezultat);
    }
    else {
        printf("Izraz nije ispravan, obrada prekinuta.\n");
    }

    /* u oba slucaja: zatvori datoteku i oslobodi sto je ostalo na stogu */
    fclose(fp);
    DeleteStack(&head);
    return 0;
}

/* PUSH - stavlja broj na vrh stoga = umetanje na pocetak liste (LIFO) */
int Push(Position head, int broj) {
    Position newP = (Position)malloc(sizeof(Stack));
    if (newP == NULL) {
        printf("Greska: neuspjela alokacija memorije.\n");
        return -1;
    }

    newP->broj = broj;
    newP->next = head->next;
    head->next = newP;

    return 0;
}

/* POP - skida broj s vrha stoga i vraca ga preko pokazivaca; -1 ako je stog prazan */
int Pop(Position head, int* broj) {
    Position p = head->next;
    if (p == NULL) {
        return -1;
    }

    *broj = p->broj;
    head->next = p->next;
    free(p);

    return 0;
}

/* OBRADA IZRAZA - cita token po token: broj ide na stog, a kod operatora se skinu
   dva operanda (prvo desni, pa lijevi), izracuna se lijevi OP desni i rezultat
   vrati na stog. Na kraju na stogu mora ostati tocno jedan broj - rezultat.
   Vraca 0 ako je izraz ispravan, -1 kod bilo koje greske. */
int EvaluateExpression(FILE* fp, Position head, int* rezultat) {
    char token[MAX_TOKEN] = { 0 };
    int broj = 0;
    int lijevi = 0;
    int desni = 0;

    while (fscanf(fp, "%s", token) == 1) {
        /* token je broj (radi i za negativne, npr. -3) */
        if (sscanf(token, "%d", &broj) == 1) {
            if (Push(head, broj) != 0) {
                return -1;
            }
        }
        /* token je jedan od operatora + - * / */
        else if (token[1] == '\0' && strchr("+-*/", token[0]) != NULL) {
            if (Pop(head, &desni) != 0 || Pop(head, &lijevi) != 0) {
                printf("Greska: nedostaje operand za '%c'.\n", token[0]);
                return -1;
            }

            switch (token[0]) {
            case '+': broj = lijevi + desni; break;
            case '-': broj = lijevi - desni; break;
            case '*': broj = lijevi * desni; break;
            case '/':
                if (desni == 0) {
                    printf("Greska: dijeljenje s nulom.\n");
                    return -1;
                }
                broj = lijevi / desni;
                break;
            }

            if (Push(head, broj) != 0) {
                return -1;
            }
        }
        else {
            printf("Greska: nepoznat znak '%s'.\n", token);
            return -1;
        }
    }

    /* ispravan izraz: na stogu je ostao tocno jedan broj */
    if (Pop(head, rezultat) != 0 || head->next != NULL) {
        printf("Greska: krivi broj operanada u izrazu.\n");
        return -1;
    }

    return 0;
}

/* oslobadja sve preostale elemente stoga */
void DeleteStack(Position head) {
    Position temp = NULL;

    while (head->next != NULL) {
        temp = head->next;
        head->next = temp->next;
        free(temp);
    }
}
