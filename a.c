#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

static char *potok = NULL;

void funkcja()
{   
    if(unlink(potok) == -1)
    {
        perror("Blad unlink \n\n");
        exit(EXIT_FAILURE);
    }
}



int main(int argc, char *argv[])
{
    if (argc != 6)
	{
		perror("Podano za malo argumentow. Skladnia '<nazwa_programu> <PLIK1> <PLIK2> <POTOK> <DANE> <WYNIK>'");
		exit (EXIT_FAILURE);
	}
    char *p_path = argv[1];
    char *k_path = argv[2];
    potok = argv[3];
    char *dane = argv[4];
    char *wynik = argv[5];
    if (mkfifo(potok, 0777)==-1)
    {
        perror ("Blad mkfifo.\n");
        exit(EXIT_FAILURE);
    }
    //rejestracja at_exit
    if(atexit(funkcja) != 0)
    {
        perror("Blad atexit \n");
        exit(EXIT_FAILURE);
    }
    //osbluga sygnalu SIGINT
    if ( signal (2, funkcja) == SIG_ERR )
    {
        perror ("Blad obslugi sygnalu (sighandler)");
        exit (EXIT_FAILURE);
    }
    //uruchomienie oroducenta i konsumenta
    switch(fork())
    {
        case -1:
            perror ("Blad fork.");
			exit(EXIT_FAILURE);
        case 0:
            if (execlp(k_path, k_path, potok, wynik, NULL)==-1)
            {
                perror ("Blad exec.\n");
                _exit(EXIT_FAILURE);
            }
            _exit(EXIT_SUCCESS);
        default:
            switch(fork())
            {
                case -1:
                perror ("Blad fork.");
                exit(EXIT_FAILURE);
            case 0:
                if (execlp(p_path, k_path, potok, dane, NULL)==-1)
                {
                    perror ("Blad exec.\n");
                    _exit(EXIT_FAILURE);
                }
                _exit(EXIT_SUCCESS);
            default:
                if(wait(NULL) == -1)
                {
                    perror("Blad wait \n");
                    _exit(EXIT_FAILURE);
                }
            }
        if(wait(NULL) == -1)
        {
            perror("Blad wait \n");
            _exit(EXIT_FAILURE);
        }
    }
}