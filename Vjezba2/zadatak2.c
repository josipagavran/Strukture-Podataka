#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>   /* strcpy, strcmp */

#define LINE (20)
#define ALLOC (-2)

/* Position = pokazivac na Person; mora biti prije strukture jer ga ona koristi za next */
typedef struct _Person* Position;

/* jedan cvor liste: podaci + adresa sljedeceg cvora */
typedef struct _Person {
    char name[LINE];
    char lastName[LINE];
    int year;
    Position next;
} Person;

Position createPerson(char* ime, char* prezime, int god);
int insertAfter(Position p, Position newP);
int insertBegin(Position head, char* ime, char* prezime, int god);
int insertEnd(Position head, char* ime, char* prezime, int god);
int printList(Position p);
Position findLastName(Position head, char* prezime);
int deletePerson(Position head, char* prezime);
int freeList(Position head);

int main() {
    /* Head je prazna pocetna struktura, nije osoba - prvi pravi element je Head.next.
       Lokalna je u main-u (nema globalnih varijabli), pa se funkcijama salje &Head. */
    Person Head = { .name = {0}, .lastName = {0}, .year = 0, .next = NULL };
    Position found = NULL;

    /* A) dodavanje na pocetak i C) dodavanje na kraj */
    insertBegin(&Head, "Ivan", "Horvat", 2004);
    insertBegin(&Head, "Ana", "Kovac", 2005);
    insertEnd(&Head, "Marko", "Babic", 2003);
    insertEnd(&Head, "Petra", "Juric", 2006);

    /* B) ispis - saljemo Head.next da se Head ne ispise */
    printf("Lista:\n");
    printList(Head.next);

    /* D) trazenje po prezimenu */
    found = findLastName(&Head, "Babic");
    if (found != NULL) {
        printf("\nPronadjen: %s %s %d\n", found->name, found->lastName, found->year);
    }
    else {
        printf("\nNema osobe s tim prezimenom.\n");
    }

    /* E) brisanje po prezimenu */
    deletePerson(&Head, "Kovac");
    printf("\nLista nakon brisanja:\n");
    printList(Head.next);

    /* oslobadjanje svih cvorova - svaki malloc ima svoj free */
    freeList(&Head);
    return 0;
}

/* Stvara novi cvor: malloc, provjera, punjenje podataka. Vraca adresu cvora ili NULL. */
Position createPerson(char* ime, char* prezime, int god) {
    Position newPerson = (Position)malloc(sizeof(Person));
    if (newPerson == NULL) {
        printf("Greska: neuspjela alokacija memorije.\n");
        return NULL;
    }

    /* stringovi se kopiraju sa strcpy, ne sa = ; -> jer je newPerson pokazivac */
    strcpy(newPerson->name, ime);
    strcpy(newPerson->lastName, prezime);
    newPerson->year = god;
    newPerson->next = NULL;

    return newPerson;
}

/* Ubacuje cvor newP iza cvora p. Redoslijed dva retka je bitan:
   prvo novi preuzme vezu na ostatak liste, tek onda p pokazuje na novog. */
int insertAfter(Position p, Position newP) {
    newP->next = p->next;
    p->next = newP;
    return 0;
}

/* A) dodavanje na pocetak = ubacivanje odmah iza Head-a */
int insertBegin(Position head, char* ime, char* prezime, int god) {
    Position newPerson = createPerson(ime, prezime, god);
    if (newPerson == NULL) {
        return ALLOC;
    }

    return insertAfter(head, newPerson);
}

/* C) dodavanje na kraj = prosetaj do zadnjeg cvora (onog ciji je next NULL), pa ubaci iza njega */
int insertEnd(Position head, char* ime, char* prezime, int god) {
    Position p = head;
    Position newPerson = createPerson(ime, prezime, god);
    if (newPerson == NULL) {
        return ALLOC;
    }

    while (p->next != NULL) {
        p = p->next;
    }

    return insertAfter(p, newPerson);
}

/* B) ispis: idi od cvora do cvora preko next dok ne dodjes do NULL */
int printList(Position p) {
    if (p == NULL) {
        printf("Lista je prazna.\n");
        return 0;
    }

    while (p != NULL) {
        printf("%s %s %d\n", p->name, p->lastName, p->year);
        p = p->next;
    }

    return 0;
}

/* D) trazenje: krece od head->next (preskace Head), usporedjuje prezimena sa strcmp.
   Vraca adresu pronadjenog cvora ili NULL ako ga nema. */
Position findLastName(Position head, char* prezime) {
    Position p = NULL;

    for (p = head->next; p != NULL; p = p->next) {
        if (strcmp(p->lastName, prezime) == 0) {
            return p;
        }
    }

    return NULL;
}

/* E) brisanje: nadji cvor (curr), nadji njegovog prethodnika (prev),
   premosti ga (prev->next = curr->next) i tek onda free. */
int deletePerson(Position head, char* prezime) {
    Position prev = head;
    Position curr = findLastName(head, prezime);

    if (curr == NULL) {
        printf("Nema osobe s prezimenom %s.\n", prezime);
        return -1;
    }

    while (prev->next != curr) {
        prev = prev->next;
    }

    prev->next = curr->next;
    free(curr);

    return 0;
}

/* Brise cijelu listu: uvijek skida prvi cvor iza Head-a dok lista ne ostane prazna */
int freeList(Position head) {
    Position temp = NULL;

    while (head->next != NULL) {
        temp = head->next;
        head->next = temp->next;
        free(temp);
    }

    return 0;
}
