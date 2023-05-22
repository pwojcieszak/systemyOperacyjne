#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>
#include "dzielona.h"
#include "sem_fun.h"

#define NELE 20 // Rozmiar elementu bufora (jednostki towaru) w bajtach
#define NBUF 5 // Liczba elementow bufora

// Segment pamieci dzielonej
typedef struct {
    char bufor[NBUF][NELE]; // Wspolny bufor danych
    int wstaw, wyjmij; // Pozycje wstawiania i wyjmowania z bufora
} SegmentPD;

// [0] nazwa_programu [1] SEMAFOR_PRODUCENT [2] SEMAFOR_KONSUMENT [3] NAZWA_PD [4] PLIK_WY
int main(int argc, char *argv[])
{
    if (argc != 5)
	{
		printf ("Wywołaj program wpisując: %s semafor_producent semafor_konsument nazwa_pd plik_we\n", argv[0]);
		exit (EXIT_FAILURE);
	}

	int we = open(argv[4], O_RDONLY | O_CREAT, 0777);
    if(we==-1)
    {
        perror ("Blad open.\n");
        exit(EXIT_FAILURE);
    }

	sem_t* semafor_producent = openS (argv[1]);
	printf ("Producent: otwarto semafor producenta %s deskryptor %p\n", argv[1], (void *)semafor_producent);
	sem_t* semafor_konsument = openS (argv[2]);
	printf ("Producent: otwarto semafor konsumenta %s deskryptor %p\n", argv[2], (void *)semafor_konsument);
    
    int PD_desk = open_PD (argv[3]);
	printf ("Producent: pamięć dzielona %s deskryptor %d\n\n", argv[3], PD_desk);
	SegmentPD* PD = (SegmentPD *) map_PD (PD_desk, sizeof(SegmentPD));
	PD->wstaw = 0;
    
	int odczyt;
	while (1) 
	{		
		sleep(1);
		printf ("\nProducent: semafor producenta: %d\n", value(semafor_producent));
		handle (semafor_producent, '0');
		
        odczyt = read(we, PD->bufor[PD->wstaw], NELE);
		if (odczyt == -1) 
		{
			printf("Błąd odczytu z pliku\n");
			exit(EXIT_FAILURE);
		}

		if (odczyt != NELE) 
		{
			PD->bufor[PD->wstaw][odczyt] = '\0';
			if(write (1, "Produkuje reszte => ", 20) == -1) 
			{
				perror("Błąd write na ekran");
				exit(EXIT_FAILURE);
			}
			if(write (1, PD->bufor[PD->wstaw], odczyt) == -1) 
			{
				perror("Błąd write na ekran");
				exit(EXIT_FAILURE);
			}
			printf ("\nProducent: koniec pliku\n");
			
			handle (semafor_konsument, '1');
			break;
		} 
		else 
		{
			if(write (1, "Produkuje => (20) ", 19) == -1) 
			{
				perror("Błąd write na ekran");
				exit(EXIT_FAILURE);
			}
			if(write (1, PD->bufor[PD->wstaw], NELE) == -1) 
			{
				perror("Błąd write na ekran");
				exit(EXIT_FAILURE);
			}
			printf ("\nIndeks bufora zapis: %d\n", PD->wstaw);
			PD->wstaw = (PD->wstaw + 1) % NBUF;
		}
		
		handle (semafor_konsument, '1');
	}

    if (close (we) == -1) 
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