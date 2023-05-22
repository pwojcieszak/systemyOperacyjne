#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <mqueue.h>
#include "kolejki.h"


mqd_t serwer;
mqd_t klient;
char kolejka_klient_nazwa[10];

void end(void) 
{
    q_close(serwer);
    q_close (klient);
    q_unlink (kolejka_klient_nazwa);
	printf ("\n\nKlient: koniec procesu\n");
}

void sig_end(int signal) 
{
	printf ("\nObsluga sygnalu, zamykanie");
	exit(EXIT_SUCCESS);
}

int main()
{
    if (atexit (end) != 0) {
		perror("atexit error");
		_exit(EXIT_FAILURE);
	}
	
	if (signal (SIGINT, sig_end) == SIG_ERR) {
		perror("Błąd signal");
		exit(EXIT_FAILURE);
	}

    sleep (2);    

    sprintf (kolejka_klient_nazwa, "/%d", getpid());
    klient = q_create (kolejka_klient_nazwa, O_RDONLY);
    struct mq_attr atrybuty = q_getattr (klient);
    printf ("Nazwa kolejki %s, deskryptor %d, atrybuty:\n", kolejka_klient_nazwa, klient);
    printf ("mq_flags: = %ld\n", atrybuty.mq_flags);
    printf ("mq_maxmsg: = %ld\n", atrybuty.mq_maxmsg);
    printf ("mq_msgsize: = %ld\n", atrybuty.mq_msgsize);
    printf ("mq_curmsgs: = %ld\n\n", atrybuty.mq_curmsgs);

    serwer = q_open (KOLEJKA_NAZWA, O_WRONLY);
    char odczyt[DLUGOSC];
	char kom[DLUGOSC+10];
    while (1) 
    {	
		printf ("Wpisz działanie\n");
		if (scanf ("%s", odczyt) == EOF) 
        {	
			printf ("\nKoniec odczytu");
			exit(EXIT_SUCCESS);
		}
		sprintf(kom, "%d %s", getpid(), odczyt);
		
		q_send(serwer, kom);
		printf ("\nWysłano zapytanie do serwera: %s\n", kom);
		printf ("Oczekiwanie na odpowiedź z serwera\n");
        sleep(rand()%5+1);
		q_receive (klient, kom);
		printf ("Odpowiedź z serwera: %s\n\n", kom);
		

		
	}
    
	return 0;
}