#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <signal.h>
#include "sem_fun.h"

char* sem_path;
sem_t* semafor;

void funkcja()
{   
    end(semafor);
    delete(sem_path);
}

int main (int argc, char *argv[])
{
    sem_path = argv[1];
    int ile = *argv[3]-'0';
    semafor = create(sem_path);
    printf("\nWartosc semafora %d\n\n", value(semafor));
    //rejestracja at_exit
    if(atexit(funkcja) != 0)
    {
        perror("Blad atexit \n");
        exit(EXIT_FAILURE);
    }
    //osbluga sygnalu SIGINT
    if ( signal (2, funkcja) == SIG_ERR )
    {
        perror ("Blad obslugi sygnalu (sighandler)");
        exit (EXIT_FAILURE);
    }
    //
    int w = open("./numer.txt", O_WRONLY | O_CREAT, 0777);
    if(write(w, "0", 1)==-1)
    {
        perror ("Blad write.\n");
        _exit(EXIT_FAILURE);
    }
    if(close(w)==-1)
    {
        perror ("Blad close.\n");
        _exit(EXIT_FAILURE);
    }
    for(int i=0; i<ile; i++)
    {
        switch(fork())
            {
            case -1:
                perror ("Blad fork.");
                exit(EXIT_FAILURE);
            case 0:
                if (execlp(argv[2], argv[2], argv[4], argv[1], NULL)==-1)
                {
                    perror ("Blad exec.\n");
                    _exit(EXIT_FAILURE);
                }
            }
    }
    for(int j=0; j<ile; j++)
    {
        if(wait(NULL) == -1)
        {
            perror("Blad wait \n");
            _exit(EXIT_FAILURE);
        }
    }
    w = open("./numer.txt", O_RDONLY);
    char buff[1];
    if(read(w, buff, 1)==-1)
    {
        perror ("Blad write.\n");
        _exit(EXIT_FAILURE);
    }
    if(close(w)==-1)
    {
        perror ("Blad close.\n");
        _exit(EXIT_FAILURE);
    }
    printf("WARTOSC KONCOWA ZAPISANA W PLIKU: %c\n", buff[0]);
    int procesy = *argv[4]-'0';
    int oczekiwana = ile * procesy;
    printf("OCZEKIWANA WARTOSC KONCOWA: %d\n", oczekiwana);
}