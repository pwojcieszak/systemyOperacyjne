#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <semaphore.h>
#include "dzielona.h"
#include "sem_fun.h"

#define NELE 20 // Rozmiar elementu bufora (jednostki towaru) w bajtach
#define NBUF 5 // Liczba elementow bufora
#define SEMAFOR_PRODUCENT "/sem_producent"
#define SEMAFOR_KONSUMENT "/sem_konsument"
#define NAZWA_PD "/prod_kons_pd"
#define PLIK_WE "dane.txt"
#define PLIK_WY "wynik.txt"
int PD_desk;

// Segment pamieci dzielonej
typedef struct {
	char bufor[NBUF][NELE]; // Wspolny bufor danych
	int wstaw, wyjmij;
} SegmentPD;

sem_t *semafor;

void funkcja() 
{
	end (semafor);
	del (SEMAFOR_PRODUCENT);
	del (SEMAFOR_KONSUMENT);
	del_PD (NAZWA_PD);
}

// [0] nazwa_programu [1] program_producent [2] program_konsument
int main (int argc, char *argv[]) 
{
	if (atexit (funkcja) != 0) 
    {
		perror("atexit error");
		_exit(EXIT_FAILURE);
	}
	if (signal (SIGINT, funkcja) == SIG_ERR) 
    {
		perror("Blad signal");
		exit(EXIT_FAILURE);
	}
	if (argc != 3) 
    {
		perror ("Blad: niepoprawna liczba argumentow!\n");
		exit (EXIT_FAILURE);
	}
    semafor = create (SEMAFOR_PRODUCENT, NBUF);
    printf ("Utworzono semafor producenta %s (%p) o wartosci poczatkowej %d\n", SEMAFOR_PRODUCENT, (void *)semafor, value(semafor));
    semafor = create (SEMAFOR_KONSUMENT, 0);
    printf ("Utworzono semafor producenta %s (%p) o wartosci poczatkowej %d\n", SEMAFOR_KONSUMENT, (void *)semafor, value(semafor));
    
    PD_desk = create_PD (NAZWA_PD);
	size_PD (PD_desk, sizeof(SegmentPD));
	printf ("Utworzono pamiec dzielona %s o deskryptorze %d i rozmiarze %ld\n\n\n", NAZWA_PD, PD_desk, sizeof(SegmentPD));

 
	switch (fork ()) 
	{
		case -1:
			perror("fork error");
			exit(EXIT_FAILURE);
		case 0:
			if(execlp ("./producent.x", "producent.x", SEMAFOR_PRODUCENT, SEMAFOR_KONSUMENT, NAZWA_PD, PLIK_WE, NULL)==-1) 
			{
				perror ("blad execlp");
				exit (EXIT_FAILURE);
			}
			break;
		default:
			switch (fork ()) 
			{
				case -1:
					perror("fork error");
					exit(EXIT_FAILURE);
				case 0:
					if(execlp ("./konsument.x", "konsument.x", SEMAFOR_PRODUCENT, SEMAFOR_KONSUMENT, NAZWA_PD, PLIK_WY, NULL)==-1)
					{
						perror ("blad execlp");
						exit (EXIT_FAILURE);
					}
					break;
			}
			break;
	}
	
	if (wait (NULL) == -1) 
    {
		perror ("Blad wait");
		_exit (EXIT_FAILURE);
	}
	
	if (wait (NULL) == -1) 
    {
		perror ("Blad wait");
		_exit (EXIT_FAILURE);
	}
	
	close_PD (PD_desk);	
	printf ("\n\nKONIEC GLOWNEGO\n");
	
	return 0;
}
