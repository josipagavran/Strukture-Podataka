#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE (128)
#define ALLOC (-2)
#define FILE_NAME "polinomi.txt"

/* jedan clan polinoma: koeficijent, potencija i adresa sljedeceg clana */
typedef struct _Poly* Position;
typedef struct _Poly {
    int coeff;
    int power;
    Position next;
} Polynom;

int insertSorted(Position head, int coeff, int power);
int readPolynom(FILE* fp, Position head);
int printPolynom(Position p);
int addPolynoms(Position a, Position b, Position result);
int multiplyPolynoms(Position a, Position b, Position result);
int freeList(Position head);

int main() {
    /* cetiri liste, svaka sa svojim praznim Head-om: dva ucitana polinoma, zbroj i umnozak */
    Polynom headF = { 0, 0, NULL };
    Polynom headS = { 0, 0, NULL };
    Polynom sum = { 0, 0, NULL };
    Polynom product = { 0, 0, NULL };
    FILE* fp = fopen(FILE_NAME, "r");

    if (fp == NULL) {
        printf("Greska: datoteka %s se ne moze otvoriti.\n", FILE_NAME);
        return -1;
    }

    /* svaki redak datoteke je jedan polinom */
    readPolynom(fp, &headF);
    readPolynom(fp, &headS);
    fclose(fp);

    printf("P1 = "); printPolynom(headF.next);
    printf("P2 = "); printPolynom(headS.next);

    addPolynoms(&headF, &headS, &sum);
    printf("P1 + P2 = "); printPolynom(sum.next);

    multiplyPolynoms(&headF, &headS, &product);
    printf("P1 * P2 = "); printPolynom(product.next);

    freeList(&headF);
    freeList(&headS);
    freeList(&sum);
    freeList(&product);
    return 0;
}

/* SORTIRANI UNOS - srce zadatka. Clan se ubacuje tako da lista ostane sortirana
   po potenciji od najvece prema najmanjoj. Ako ta potencija vec postoji, koeficijenti
   se zbroje (zato zbrajanje i mnozenje mogu koristiti istu funkciju). Clan s
   koeficijentom 0 ne ostaje u listi. */
int insertSorted(Position head, int coeff, int power) {
    Position q = head;
    Position newPoly = NULL;

    if (coeff == 0) {
        return 0;
    }

    /* trazenje mjesta: stani ispred prvog clana cija potencija nije veca od nove */
    while (q->next != NULL && q->next->power > power) {
        q = q->next;
    }

    /* ista potencija vec postoji: samo zbroji koeficijente, a ako se ponistilo - izbaci clan */
    if (q->next != NULL && q->next->power == power) {
        q->next->coeff += coeff;
        if (q->next->coeff == 0) {
            newPoly = q->next;
            q->next = newPoly->next;
            free(newPoly);
        }
        return 0;
    }

    /* nova potencija: alociraj clan i uvezi ga iza q */
    newPoly = (Position)malloc(sizeof(Polynom));
    if (newPoly == NULL) {
        printf("Greska: neuspjela alokacija memorije.\n");
        return ALLOC;
    }
    newPoly->coeff = coeff;
    newPoly->power = power;
    newPoly->next = q->next;
    q->next = newPoly;

    return 0;
}

/* CITANJE: jedan redak datoteke ide u buffer, pa se iz buffera cita par po par
   (koeficijent, potencija). %n javlja koliko je znakova sscanf potrosio, pa se
   pokazivac p pomice za toliko i citanje nastavlja od sljedeceg para. */
int readPolynom(FILE* fp, Position head) {
    char buffer[MAX_LINE] = { 0 };
    char* p = buffer;
    int coeff = 0;
    int power = 0;
    int n = 0;

    if (fgets(buffer, MAX_LINE, fp) == NULL) {
        return -1;
    }

    while (sscanf(p, "%d %d%n", &coeff, &power, &n) == 2) {
        insertSorted(head, coeff, power);
        p += n;
    }

    return 0;
}

/* ISPIS: %+d uvijek ispisuje predznak, pa clanovi izgledaju kao +3x^2 -2x^0 */
int printPolynom(Position p) {
    if (p == NULL) {
        printf("0");
    }

    for (; p != NULL; p = p->next) {
        printf("%+dx^%d ", p->coeff, p->power);
    }
    printf("\n");

    return 0;
}

/* ZBRAJANJE: idemo kroz oba polinoma istovremeno i usporedjujemo potencije.
   Veca potencija ide u rezultat i taj pokazivac se pomakne; jednake potencije
   se zbroje i oba se pomaknu. Kad jedan polinom zavrsi, ostatak drugog se prepise. */
int addPolynoms(Position a, Position b, Position result) {
    Position p = a->next;
    Position q = b->next;

    while (p != NULL && q != NULL) {
        if (p->power > q->power) {
            insertSorted(result, p->coeff, p->power);
            p = p->next;
        }
        else if (p->power < q->power) {
            insertSorted(result, q->coeff, q->power);
            q = q->next;
        }
        else {
            insertSorted(result, p->coeff + q->coeff, p->power);
            p = p->next;
            q = q->next;
        }
    }

    for (; p != NULL; p = p->next) {
        insertSorted(result, p->coeff, p->power);
    }
    for (; q != NULL; q = q->next) {
        insertSorted(result, q->coeff, q->power);
    }

    return 0;
}

/* MNOZENJE: svaki clan prvog sa svakim clanom drugog - koeficijenti se mnoze,
   potencije zbrajaju. insertSorted sam spoji clanove koji dobiju istu potenciju. */
int multiplyPolynoms(Position a, Position b, Position result) {
    Position p = NULL;
    Position q = NULL;

    for (p = a->next; p != NULL; p = p->next) {
        for (q = b->next; q != NULL; q = q->next) {
            insertSorted(result, p->coeff * q->coeff, p->power + q->power);
        }
    }

    return 0;
}

/* oslobadjanje liste: skidaj prvi clan iza Head-a dok lista ne ostane prazna */
int freeList(Position head) {
    Position temp = NULL;

    while (head->next != NULL) {
        temp = head->next;
        head->next = temp->next;
        free(temp);
    }

    return 0;
}
