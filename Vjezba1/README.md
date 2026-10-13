# Vježba 1

## Zadatak 1

Napisati program koji prvo pročita koliko redaka ima datoteka, tj. koliko ima studenata
zapisanih u datoteci. Nakon toga potrebno je dinamički alocirati prostor za niz struktura
studenata (ime, prezime, bodovi) i učitati iz datoteke sve zapise. Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.

Napomena: svaki redak datoteke sadrži ime i prezime studenta te broj bodova na kolokviju.

```
relativni broj bodova = broj bodova / max broj bodova * 100
```

## Datoteke

| Datoteka | Opis |
|----------|------|
| `zadatak1.c` | rješenje zadatka |
| `popis.txt` | ulazna datoteka (ime prezime bodovi) |

## Pokretanje

Datoteka `popis.txt` mora biti u istoj mapi iz koje se program pokreće
(u Visual Studiju: u mapi projekta, pored `.c` datoteke).

```
gcc zadatak1.c -o zadatak1
./zadatak1
```
