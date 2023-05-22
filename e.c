#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <stdlib.h>
#include <sys/wait.h>
#include "funkcja.h"

/* Program dziala jak w (b), tylko przy użyciu fukcji setpgid sprawia
by każdy proces potomny stawal sie, liderem swojej wlasnej grupy procesów. */

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
            //polecenia dla procesu potomnego (zostaje mu przypisana nowa grupa procesow, wyswietla wszystkie swoje ID i konczy prace)
            case 0:
                if(setpgid(0,0)==-1)
                {
                    perror("blad setpgid");
                    exit(EXIT_FAILURE);
                }
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