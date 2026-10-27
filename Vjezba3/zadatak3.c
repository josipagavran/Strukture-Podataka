#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>   /* strcpy, strcmp */

#define LINE (20)
#define ALLOC (-2)
#define FILE_NAME "osobe.txt"

/* Position = pokazivac na Person; mora biti prije strukture jer ga ona koristi za next */
typedef struct _Person* Position;

/* jedan cvor liste: podaci + adresa sljedeceg cvora */
typedef struct _Person {
    char name[LINE];
    char lastName[LINE];
    int year;
    Position next;
} Person;

/* funkcije iz zadatka 2 */
Position createPerson(char* ime, char* prezime, int god);
int insertAfter(Position p, Position newP);
int insertBegin(Position head, char* ime, char* prezime, int god);
int insertEnd(Position head, char* ime, char* prezime, int god);
int printList(Position p);
Position findLastName(Position head, char* prezime);
int deletePerson(Position head, char* prezime);
int freeList(Position head);

/* nove funkcije - zadatak 3 */
Position findPrevious(Position head, char* prezime);
int insertAfterLastName(Position head, char* trazeno, char* ime, char* prezime, int god);
int insertBeforeLastName(Position head, char* trazeno, char* ime, char* prezime, int god);
int sortList(Position head);
int writeListToFile(Position p, char* imeDatoteke);
int readListFromFile(Position head, char* imeDatoteke);

int main() {
    /* Head je prazna pocetna struktura, nije osoba - prvi pravi element je Head.next.
       Lokalna je u main-u (nema globalnih varijabli), pa se funkcijama salje &Head. */
    Person Head = { .name = {0}, .lastName = {0}, .year = 0, .next = NULL };

    insertBegin(&Head, "Ivan", "Horvat", 2004);
    insertBegin(&Head, "Ana", "Kovac", 2005);
    insertEnd(&Head, "Marko", "Babic", 2003);
    insertEnd(&Head, "Petra", "Juric", 2006);
    printf("Pocetna lista:\n");
    printList(Head.next);

    /* A) umetanje iza osobe s prezimenom Horvat, B) ispred osobe s prezimenom Babic */
    insertAfterLastName(&Head, "Horvat", "Luka", "Maric", 2002);
    insertBeforeLastName(&Head, "Babic", "Iva", "Anic", 2001);
    printf("\nNakon umetanja iza Horvat i ispred Babic:\n");
    printList(Head.next);

    /* C) sortiranje po prezimenu */
    sortList(&Head);
    printf("\nSortirano po prezimenu:\n");
    printList(Head.next);

    /* D) zapis liste u datoteku */
    writeListToFile(Head.next, FILE_NAME);
    printf("\nLista je zapisana u datoteku %s.\n", FILE_NAME);

    /* lista se isprazni, pa se E) ucita natrag iz datoteke */
    freeList(&Head);
    readListFromFile(&Head, FILE_NAME);
    printf("\nLista ucitana iz datoteke:\n");
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

/* dodavanje na pocetak = ubacivanje odmah iza Head-a */
int insertBegin(Position head, char* ime, char* prezime, int god) {
    Position newPerson = createPerson(ime, prezime, god);
    if (newPerson == NULL) {
        return ALLOC;
    }

    return insertAfter(head, newPerson);
}

/* dodavanje na kraj = prosetaj do zadnjeg cvora (onog ciji je next NULL), pa ubaci iza njega */
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

/* ispis: idi od cvora do cvora preko next dok ne dodjes do NULL */
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

/* trazenje: krece od head->next (preskace Head), usporedjuje prezimena sa strcmp.
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

/* brisanje: nadji cvor (curr), nadji njegovog prethodnika (prev),
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

/* Vraca PRETHODNIKA cvora s trazenim prezimenom (cvor ciji next ima to prezime),
   ili NULL ako takve osobe nema. Ako je trazena osoba prva, prethodnik je Head. */
Position findPrevious(Position head, char* prezime) {
    Position p = head;

    while (p->next != NULL) {
        if (strcmp(p->next->lastName, prezime) == 0) {
            return p;
        }
        p = p->next;
    }

    return NULL;
}

/* A) umetanje iza osobe s trazenim prezimenom: nadji je, pa insertAfter iza nje */
int insertAfterLastName(Position head, char* trazeno, char* ime, char* prezime, int god) {
    Position p = findLastName(head, trazeno);
    Position newPerson = NULL;

    if (p == NULL) {
        printf("Nema osobe s prezimenom %s.\n", trazeno);
        return -1;
    }

    newPerson = createPerson(ime, prezime, god);
    if (newPerson == NULL) {
        return ALLOC;
    }

    return insertAfter(p, newPerson);
}

/* B) umetanje ISPRED osobe = umetanje iza njenog prethodnika, pa opet insertAfter */
int insertBeforeLastName(Position head, char* trazeno, char* ime, char* prezime, int god) {
    Position prev = findPrevious(head, trazeno);
    Position newPerson = NULL;

    if (prev == NULL) {
        printf("Nema osobe s prezimenom %s.\n", trazeno);
        return -1;
    }

    newPerson = createPerson(ime, prezime, god);
    if (newPerson == NULL) {
        return ALLOC;
    }

    return insertAfter(prev, newPerson);
}

/* C) bubble sort po prezimenu. Zamjenjuju se VEZE cvorova, ne podaci u njima.
   U svakom prolazu susjedi u krivom redoslijedu se zamijene, pa najvece prezime
   "ispliva" na kraj nesortiranog dijela; 'sorted' pamti gdje sortirani dio pocinje. */
int sortList(Position head) {
    Position sorted = NULL;
    Position prev = NULL;
    Position p = NULL;
    Position q = NULL;

    while (head->next != sorted) {
        prev = head;
        p = head->next;

        while (p->next != sorted) {
            q = p->next;
            if (strcmp(p->lastName, q->lastName) > 0) {
                /* prev -> p -> q  postaje  prev -> q -> p */
                prev->next = q;
                p->next = q->next;
                q->next = p;
                prev = q;
            }
            else {
                prev = p;
                p = p->next;
            }
        }

        sorted = p;
    }

    return 0;
}

/* D) zapis liste u datoteku: "w" = pisanje (brise stari sadrzaj), fprintf radi kao printf ali u datoteku */
int writeListToFile(Position p, char* imeDatoteke) {
    FILE* fp = fopen(imeDatoteke, "w");
    if (fp == NULL) {
        printf("Greska: datoteka %s se ne moze otvoriti.\n", imeDatoteke);
        return -1;
    }

    while (p != NULL) {
        fprintf(fp, "%s %s %d\n", p->name, p->lastName, p->year);
        p = p->next;
    }

    fclose(fp);
    return 0;
}

/* E) citanje iz datoteke: fscanf cita redak po redak dok vraca 3 (ime, prezime, godina),
   svaka procitana osoba ide na kraj liste da se zadrzi redoslijed iz datoteke */
int readListFromFile(Position head, char* imeDatoteke) {
    FILE* fp = fopen(imeDatoteke, "r");
    char ime[LINE] = { 0 };
    char prezime[LINE] = { 0 };
    int god = 0;

    if (fp == NULL) {
        printf("Greska: datoteka %s se ne moze otvoriti.\n", imeDatoteke);
        return -1;
    }

    while (fscanf(fp, "%s %s %d", ime, prezime, &god) == 3) {
        insertEnd(head, ime, prezime, god);
    }

    fclose(fp);
    return 0;
}
