#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include "dzielona.h"
#include "sem_fun.h"

#define NELE 20 // Rozmiar elementu bufora (jednostki towaru) w bajtach
#define NBUF 5 // Liczba elementow bufora

// Segment pamieci dzielonej
typedef struct {
    char bufor[NBUF][NELE]; // Wspolny bufor danych
    int wstaw, wyjmij; // Pozycje wstawiania i wyjmowania z bufora
} SegmentPD;

int koniec (SegmentPD *PD) 
{
	int i;
	for (i = 0; i < NELE; i++) 
	{
		if (PD->bufor[PD->wyjmij][i] == '\0') 
		{
			return 1;
		}
	}
	return 0;
}
// [0] nazwa_programu [1] SEMAFOR_PRODUCENT [2] SEMAFOR_KONSUMENT [3] NAZWA_PD [4] PLIK_WY
int main(int argc, char *argv[])
{
    if (argc != 5)
	{
		printf ("Wywołaj program wpisując: %s semafor_producent semafor_konsument nazwa_pd plik_wy\n", argv[0]);
		exit (EXIT_FAILURE);
	}
    
	int wy = open(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0777);
    if(wy==-1)
    {
        perror ("Blad open.\n");
        exit(EXIT_FAILURE);
    }

	sem_t* semafor_producent = openS (argv[1]);
	printf ("Konsument: otwarto semafor producenta %s deskryptor %p\n", argv[1], (void *)semafor_producent);
	sem_t* semafor_konsument = openS (argv[2]);
	printf ("Konsument: otwarto semafor konsumenta %s deskryptor %p\n", argv[2], (void *)semafor_konsument);
    
    int PD_desk = open_PD (argv[3]);
	printf ("Producent: pamięć dzielona %s deskryptor %d\n\n", argv[3], PD_desk);
	SegmentPD* PD = (SegmentPD *) map_PD (PD_desk, sizeof(SegmentPD));
	PD->wyjmij = 0;
    
        
	while (1) 
	{
		handle (semafor_konsument, '0');	
		printf ("\n\t\tKonsument: semafor konsumenta po obnizeniu: %d\n", value(semafor_konsument));
		
		if (!koniec (PD)) 
		{
			if (write (wy, PD->bufor[PD->wyjmij], NELE) == -1) 
			{
				perror("Błąd write do potoku");
				exit(EXIT_FAILURE);
			}
			if(write (1, "\t\tKonsumuje => (20) ", 20) == -1) 
			{
				perror("Błąd write na ekran");
				exit(EXIT_FAILURE);
			}
			if(write (1, PD->bufor[PD->wyjmij], NELE) == -1) 
			{
				perror("Błąd write na ekran");
				exit(EXIT_FAILURE);
			}
			printf ("\n\t\tIndeks bufora wyjmij: %d\n", PD->wyjmij);
			PD->wyjmij = (PD->wyjmij + 1) % NBUF;
		} 
		else 
		{
			if (write (wy, PD->bufor[PD->wyjmij], strlen (PD->bufor[PD->wyjmij])) == -1) 
			{
				perror("Błąd write do potoku");
				exit(EXIT_FAILURE);
			}
			if(write (1, "\t\tKonsumuje reszte => ", 22) == -1) 
			{
				perror("Błąd write na ekran");
				exit(EXIT_FAILURE);
			}			
			if(write (1, PD->bufor[PD->wyjmij], strlen (PD->bufor[PD->wyjmij])) == -1) 
			{
				perror("Błąd write na ekran");
				exit(EXIT_FAILURE);
			}
			printf ("\n\t\tKonsument: Sygnał końca odczytu\n");
			
			
			break;
		}
		handle (semafor_producent, '1');
	}

    if (close (wy) == -1) 
    {
		perror ("Blad close");
		exit (EXIT_FAILURE);
	}
	
	del_map_PD(PD, sizeof(SegmentPD));
	close_PD (PD_desk);
	end(semafor_konsument);
	end(semafor_producent);
	return 0;
}