#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define _POSIX_C_SOURCE 200112L
#define _XOPEN_SOURCE 500

void potomny (char* argv[])
{
	switch (fork())
	{
		case -1:
			perror ("blad fork.");
			exit (EXIT_FAILURE);
			break;
		case 0:
			if(execv("a.x", argv)==-1)
            {
                perror("blad execlp");
                _exit(2);
            }
            printf("DZIALA\n\n");
			break;
        
	}
}

int main(int argc, char *argv[])
{
    char *obsluga = argv[1];
	int sygnal = atoi (argv[2]);
    if ( signal (sygnal, SIG_IGN) == SIG_ERR )
        {
            perror ("Blad obslugi sygnalu (SIG_IGN)");
            exit (EXIT_FAILURE);
        }
        if(setpgid(0,0)==-1)
        {
            perror("blad setpgid");
            exit(EXIT_FAILURE);
        }
        printf("POTOMNY1 PID %d PGID %d\n", getpid(), getpgid());
        potomny(argv);
        potomny(argv);
        potomny(argv);
        for(int i=0; i<3; i++)
        {
            if(wait(NULL)==-1)
            {
                perror("blad wait");
                exit(EXIT_FAILURE);
            }
        }
        return EXIT_SUCCESS;
}