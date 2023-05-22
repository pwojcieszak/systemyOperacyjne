#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include "funkcja.h"

/* Program dziala jak w (b), tylko przy użyciu funkcji sleep sprawia
by procesy potomne byly adoptowane przez proces systemd. */

//funkcja wypisujaca, wszystkie rozne ID aktywnego procesu
void allID()
{
    printf("UID: %d GID: %d PID: %d PPID: %d PGID: %d\n", getuid(), getgid(), getpid(), getppid(), getpgrp());
}

int main()
{
    printf("\nPROCES MACIERZYSTY\n");
    allID();
    //utworzenie petli sluzacej do trzykrotnego wywolania funkcji fork
    printf("\nPROCESY POTOMNE\n");
    for (int i=0; i<3; i++)
    {
        switch(fork())
        {
            //obsluga bledow
            case -1:
                perror("blad fork");
                exit(EXIT_FAILURE);
                break;
            //polecenia dla procesu potomnego (zasypia na 3 sekundy i wyswietla wszystkie ID)
            case 0:
                sleep(6);
                allID();
                break;
            //polecenia dla procesu macierzystego (konczy swoja prace)
            default:
                sleep(1);  
        }
    }
}

//procesy sa adoptowane przez proces systemd(1)