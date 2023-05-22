#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <stdlib.h>
#include "funkcja.h"

/* Program dziala jak w (b), tylko wstawia funkcje sleep w takich 
    miejscach programu, aby procesy pojawialy sie, na ekranie grupowane 
    pokoleniami od najstarszego do najmlodszego, a proces macierzysty 
    kończyl sie, dopiero po procesach potomnych.*/

//funkcja wypisujaca, wszystkie rozne ID aktywnego procesu
void allID()
{
    printf("UID: %d GID: %d PID: %d PPID: %d PGID: %d\n", getuid(), getgid(), getpid(), getppid(), getpgrp());
}

int main()
{
    int pokolenie=0;
    printf("\nPROCES MACIERZYSTY\n(0)");
    allID();
    //utworzenie petli sluzacej do trzykrotnego wywolania funkcji fork
    printf("\nPROCESY POTOMNE\n");
    for (int i=1; i<4; i++)
    {
        switch(fork())
        {
            //obsluga bledow
            case -1:
                perror("blad fork");
                exit(EXIT_FAILURE);
                break;
            //polecenia dla procesu potomnego (wyswietla wszystkie ID i konczy swoja prace)
            case 0:
                pokolenie++;
                printf("(%d) ", pokolenie);
                allID();
                sleep(1);
                break;
        }
    }
    sleep(5);
        
}