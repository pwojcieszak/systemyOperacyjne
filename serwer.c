#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <mqueue.h>
#include "kolejki.h"

mqd_t q_desk;

void end(void) 
{
    q_close (q_desk);
    q_unlink (KOLEJKA_NAZWA);
	printf ("\n\nSerwer: koniec procesu\n");
}

void sig_end(int signal) 
{
	printf ("\nObsluga sygnalu, zamykanie\n");
	exit(EXIT_SUCCESS);
}

int main()
{
    if (atexit (end) != 0) 
    {
		perror("atexit error");
		_exit(EXIT_FAILURE);
	}
	
	if (signal (SIGINT, sig_end) == SIG_ERR) 
    {
		perror("Błąd signal");
		exit(EXIT_FAILURE);
	}
    q_desk = q_create (KOLEJKA_NAZWA, O_RDONLY);
    struct mq_attr atrybuty = q_getattr (q_desk);
    printf ("Nazwa kolejki %s, deskryptor %d, atrybuty:\n", KOLEJKA_NAZWA, q_desk);
    printf ("mq_flags: = %ld\n", atrybuty.mq_flags);
    printf ("mq_maxmsg: = %ld\n", atrybuty.mq_maxmsg);
    printf ("mq_msgsize: = %ld\n", atrybuty.mq_msgsize);
    printf ("mq_curmsgs: = %ld\n\n", atrybuty.mq_curmsgs);

    char kom[DLUGOSC];
    int wynik, x ,y;
    char operator;
    char wynik_s[DLUGOSC];
    int PID_klienta;
    mqd_t klient;
    while (1) 
    {	
		q_receive (q_desk, kom);
		sscanf(kom, "%d %d%c%d", &PID_klienta, &x, &operator, &y);
		printf ("Odebrano zapytanie od %d - działanie %d %c %d\n", PID_klienta, x, operator, y);
		
		if (operator == '+') 
        {
			wynik = x + y;
			sprintf (wynik_s, "%d", wynik);
		} 
        else if (operator == '-') 
        {
			wynik = x - y;
			sprintf (wynik_s, "%d", wynik);
		} 
        else if (operator == '*') 
        {
			wynik = x * y;
			sprintf (wynik_s, "%d", wynik);
		} 
        else if (operator == '/') 
        {
			wynik = x / y;
			sprintf (wynik_s, "%d", wynik);
		} 
        else 
        {
			sprintf (wynik_s, "Błąd: nieznany operator");
		}
		
        sleep(rand()%5+1);
		// odtwórz kolejkę do nadania odpowiedzi
		sprintf (kom, "/%d", PID_klienta);
		klient = q_open (kom, O_WRONLY);
		
		// odpowiedz
		printf ("Wysyłanie odpowiedzi %s do procesu %d\n", wynik_s, PID_klienta);
		q_send (klient, wynik_s);
		q_close (klient);
	}
}