#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <stdlib.h>
#include <sys/wait.h>
#include "funkcja.h"

/* Program Wywolujacy funkcje fork trzy razy (W petli for) i wypisuje 
    powyższe identyfikatory dla procesu macierzystego oraz wszystkich 
    procesów  potomnych. Przy pomocy funkcji wait sprawia, aby proces 
    macierzysty zaczekal na zakończenie wszystkich procesów potomnych. */

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
            //polecenia dla procesu potomnego (wyswietla wszystkie ID)
            case 0:
                allID();
                break;
            //polecenia dla procesu macierzystego (czeka na koniec dzialania procesu potomnego)
            default:
                if(wait(NULL)==-1)
                {
                    perror("blad wait");
                    exit(EXIT_FAILURE);
                }
            
        }
    }
}