#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>

#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 500


int main(int argc, char *argv[])
{
    printf("\n\n");
	if (argc != 3)
	{
		perror ("Podano za malo argumentow. Skladnia '<nazwa_programu> <obsluga> <sygnal>'");
		exit (EXIT_FAILURE);
	}
	char *obsluga = argv[1];
	int sygnal = atoi (argv[2]);
	int pid;
	switch (pid = fork ())
	{
		case -1:
			perror ("Blad fork.");
			exit (EXIT_FAILURE);
			break;
		case 0:
			if(execv ("./a.x", argv)==-1)
            {
                perror("blad execlp");
                _exit(2);
            };
			break;
		default:
            sleep(2);
			printf ("Sygnal %d do procesu o id %d\n", sygnal, pid);
            if(kill (pid, 0)==-1)
            {
                perror("blad kill(proces nie istnieje)");
                exit(EXIT_FAILURE);
            }
            else
            {
                if(kill (pid, sygnal)==-1)
                {
                    perror("blad kill");
                    exit(EXIT_FAILURE);
                }
            }
			break;
	}
    sleep(1);
    printf("Koniec\n\n");
	return EXIT_SUCCESS;
}