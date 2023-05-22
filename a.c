#define _REENTRANT
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "watki.h"

unsigned int liczba_watkow;
unsigned int liczba_sekcji;
pthread_mutex_t mutex;
int liczba;
pthread_t watki[10];

void gotoxy(unsigned x, unsigned y)
{
  printf("\033[%d;%dH\033[2K", y, x);
}

void *funkcja (void *arg) 
{
	int licznik = 0;
	int numer_watku = *(int *)arg;
	printf ("Wątek nr: %d, ID: %d \n", numer_watku + 1, watki[numer_watku]);
	sleep(1);
	
	for (int i=0; i<liczba_sekcji; i++) 
    {
		gotoxy (0, 4 + liczba_watkow + numer_watku);
		printf("Wątek nr: %d, numer sekcji P: %d, liczba: %d\n", numer_watku + 1, i + 1, liczba);
		sleep(rand()%3+1);

        m_close(&mutex);
		gotoxy (25, 4 + liczba_watkow + numer_watku);
		printf("\033[1;4mWątek nr: %d, nr sekcji K: %d, liczba: %d \033[0m\n", numer_watku + 1, i + 1, liczba);
		licznik = liczba + 1;
        sleep(rand()%3+1);
		liczba = licznik;
		m_open (&mutex);

		gotoxy (0, 4 + liczba_watkow + numer_watku);
		printf("Wątek nr: %d, numer sekcji P: %d, liczba: %d \n", numer_watku + 1, i + 1, liczba);
    }
    return 0;
}

int main (int argc, char *argv[]) 
{
	
	if (argc != 3) 
    {
		printf ("Błąd: niepoprawna liczba argumentów!\n");
		exit (EXIT_FAILURE);
	}
 	liczba_watkow = atoi(argv[1]);
    liczba_sekcji = atoi(argv[2]);
    m_init (&mutex);
	printf("Zainicjalizowano mutex o adresie %p\n", (void *)&mutex);

    liczba = 0;
	for (int i=0; i<liczba_watkow; i++) 
    {	
		int *pamiec = malloc (sizeof (int));
		*pamiec = i;
        sleep((double)i/liczba_watkow);
		watki[i] = t_create (funkcja, (void *)pamiec);
	}
    for (int i=0; i<liczba_watkow; i++) 
    {
		t_join(watki[i]);
	}
    gotoxy(0, 4+liczba_watkow*2);

    if (liczba != liczba_watkow*liczba_sekcji) 
    {
		printf ("Niepoprawna wartosc licznika.\nOczekiwano: %d\nOtrzymano: %d\n", (liczba_watkow * liczba_sekcji), liczba);
	} 
    else 
    {
		printf ("Poprawna wartosc licznika.\n");
	}
	m_destroy (&mutex);
	return 0;
}