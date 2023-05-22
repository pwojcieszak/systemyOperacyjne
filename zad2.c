#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include <stdlib.h>
#include <sys/wait.h>

/* Zmodyfikować program z ćwiczenia 1b, tak aby komunikaty procesów potomnych by ly
wypisywane przez program uruchamiany przez funkcje , execlp. Nazwe , programu do uru-
chomienia przekazywać przez argumenty programu procesu macierzystego.
! Ile procesów powstanie przy n-krotnym wywo laniu funkcji fork-exec jak wyżej i dlaczego? */

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
            //proces potomny wywoluje program wypisujacy jego id
            case 0:
                if(execlp("./a.x","a.x", NULL)==-1)
                {
                    perror("execlp error");
                    _exit(2);
                }
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