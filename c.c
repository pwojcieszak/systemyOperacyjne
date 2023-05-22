#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 500



int main(int argc, char *argv[])
{
    printf("\n\n");
    //printf("glowny0 pid %d pgid %d\n", getpid(), getpgid());
	if (argc != 3)
	{
		perror ("Nieprawidlowa liczba argumentow.");
		exit (EXIT_FAILURE);
	}
	char *obsluga = argv[1];
	int sygnal = atoi (argv[2]);
	printf ("Uruchamiam program obsluga.x z parametrem \"%s\"...\n", obsluga);
    int pgid;
    switch(pgid = fork())
    {
        case -1:
            perror ("blad fork.");
			exit (EXIT_FAILURE);
			break;
        case 0:
            if(execv ("./dodatkowy.x", argv)==-1)
            {
                perror("blad execv");
                _exit(2);
            };
            break;
        default:
            sleep(4);
            //printf("\nDEFAULT C PID %d PGID %d\n", getpid(), getpgid());
            printf("Grupa %d\n", pgid);
            if(kill(-pgid, 0)==-1)
            {
                perror("blad kill(grupa procesow nie istnieje)");
                exit(EXIT_FAILURE);
            }
            else
            {
                if(kill(-pgid, sygnal)==-1)
                {
                    perror("blad kill");
                    exit(EXIT_FAILURE);
                }
            }
            if(wait(NULL)==-1)
            {
                perror("blad wait");
                exit(EXIT_FAILURE);
            }
            sleep(1);
            printf("KONIEC MACIERZYSTEGO \n\n");
    }
	return EXIT_SUCCESS;
}