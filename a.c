#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 500

extern const char * const sys_siglist[];

void sighandler (int sygnal)
{
	printf ("Wlasna obsluga sygnalu o numerze %d i nazwie '%s'.\n", sygnal, sys_siglist[sygnal]);
}

void switchHandle(char *argv[])
{
    char obsluga = *argv[1];
    int sygnal = atoi(argv[2]);
    switch(obsluga)
    {
        case 'd':
            if ( signal (sygnal, SIG_DFL) == SIG_ERR )
            {
                perror ("Blad obslugi sygnalu (SIG_DFL)");
                exit (EXIT_FAILURE);
            }
            break;
        case 'i':
            if ( signal (sygnal, SIG_IGN) == SIG_ERR )
            {
                perror ("Blad obslugi sygnalu (SIG_IGN)");
                exit (EXIT_FAILURE);
            }
            break;
        case 'w':
            if ( signal (sygnal, sighandler) == SIG_ERR )
            {
                perror ("Blad obslugi sygnalu (sighandler)");
                exit (EXIT_FAILURE);
            }
            break;
        default:
            perror("Wprowadzono zly argument");
            exit(EXIT_FAILURE);
    }
    
}

int main(int argc, char *argv[])
{
    printf("\n");
    if(argc!=2 && argc!=3)
	{
		perror("Podano za malo argumentow. Skladnia '<nazwa_programu> <obsluga> <sygnal>'");
		exit(EXIT_FAILURE);
	}
    if(argc==2) 
    {
        if(pause()!=-1)
        {
            perror("Blad funkcji pause");
            exit(EXIT_FAILURE);
        }
    }
    else 
    {
        printf("PID  %d PGID %d\nOczekiwanie na sygnal...\n\n", getpid(), getpgid(0));        
        switchHandle(argv);
    }
    pause();
    return EXIT_SUCCESS;
}