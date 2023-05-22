#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
	{
		perror("Podano za malo argumentow. Skladnia '<nazwa_programu> <obsluga> <sygnal>'");
		exit (EXIT_FAILURE);
	}
    int filedes[2];
    if(pipe(filedes)==-1)
    {
        perror("Blad funkcji pipe\n");
        exit(EXIT_FAILURE);
    }
    char *dane = argv[1];
    char *wynik = argv[2];
    switch(fork())
    {
        case -1:
            perror ("Blad fork.");
			exit(EXIT_FAILURE);
			break;
            ///////////////////////////////potomek filedes[1] zapis, filedes[0] odczyt
        case 0:
            if(close(filedes[1])==-1)
            {
                perror ("6Blad close.\n");
                exit (EXIT_FAILURE);
                break;
            }
            int wy = open(wynik, O_WRONLY | O_CREAT, 0777);
            char buffor_out[100];
            if(wy==-1)
            {
                perror ("7Blad open.\n");
                exit (EXIT_FAILURE);
                break;
            }
            int bajty_kons = read(filedes[0], buffor_out, 50); 
            if(bajty_kons==-1)
            {
                perror ("8Blad read.\n");
                exit (EXIT_FAILURE);
                break;
            } 
            while(bajty_kons>0)
            {
                printf("Zapisane dane \n");
                if(write(1, buffor_out, bajty_kons)==-1)
                {
                    perror("Blad write\n");
                    exit(EXIT_FAILURE);
                    break;
                }
                printf("\n"); 
                if(write(wy, buffor_out, bajty_kons)==-1)
                {
                    perror ("9Blad read.\n");
                    exit (EXIT_FAILURE);
                    break;
                }
                sleep(rand()%5+1);
                bajty_kons = read(filedes[0], buffor_out, 50);
            }
            if(close(filedes[0])==-1)
            {
                perror ("10Blad close.\n");
                exit (EXIT_FAILURE);
                break;
            }
            if(close(wy)==-1)
            {
                perror ("5Blad close.\n");
                exit (EXIT_FAILURE);
                break;
            }
            break;
            _exit(EXIT_SUCCESS);
            ////////////////////////////////rodzic
        default:
            if(close(filedes[0])==-1)
            {
                perror ("1Blad close.\n");
                exit (EXIT_FAILURE);
                break;
            }
            int we = open(dane, O_RDONLY);
            char buffor_in[100];
            if(we==-1)
            {
                perror ("2Blad open.\n");
                exit(EXIT_FAILURE);
                break;
            }
            int bajty_prod = read(we, buffor_in, 50);
            if(bajty_prod==-1)
            {
                perror ("3Blad read.\n");
                exit (EXIT_FAILURE);
                break;
            } 
            while(bajty_prod>0)
            {
                printf("\nOdebrane dane \n");
                if(write(1, buffor_in, bajty_prod)==-1)
                {
                    perror("Blad write\n");
                    exit(EXIT_FAILURE);
                    break;
                }
                printf("\n"); 
                if(write(filedes[1], buffor_in, bajty_prod)==-1)
                {
                    perror ("4Blad read.\n");
                    exit (EXIT_FAILURE);
                    break;
                }
                sleep(rand()%5+1);
                bajty_prod = read(we, buffor_in, 50);
            }
            if(close(filedes[1])==-1)
            {
                perror ("5Blad close.\n");
                exit (EXIT_FAILURE);
                break;
            }
            if(close(we)==-1)
            {
                perror ("5Blad close.\n");
                exit (EXIT_FAILURE);
                break;
            }
            wait(NULL);         
    }
}