#include <stdio.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include "sem_fun.h"

sem_t* semafor;

void sekcja_krytyczna()
{
    handle(semafor, '0');
    int w = open("./numer.txt", O_RDONLY);
    char buff[1];
    if(read(w, buff, 1)==-1)
    {
        perror ("Blad read.\n");
        _exit(EXIT_FAILURE);
    }
    if(close(w)==-1)
    {
        perror ("Blad close.\n");
        _exit(EXIT_FAILURE);
    }
    int ibuff = (int)buff[0] - '0';
    printf("Wartosc semafora %d\n", value(semafor));
    printf("Wartosc wyjmijana z pliku: %d\n", ibuff);
    ibuff++;
    sleep(rand()%5+1);
    buff[0] = ibuff + '0';
    w = open("./numer.txt", O_WRONLY);
    if(write(w, buff, 1)==-1)
    {
        perror ("Blad write.\n");
        _exit(EXIT_FAILURE);
    }
    if(close(w)==-1)
    {
        perror ("Blad close.\n");
        _exit(EXIT_FAILURE);
    } 
    handle(semafor, '1');
}
int main (int argc, char *argv[])
{
    
    printf("PID PROCESU POTOMNEGO: %d\n", getpid());
    char* sem_path = argv[2];
    semafor = sem_open(sem_path, O_CREAT);

    int ile = *argv[1]-'0';
    for(int i=0; i<ile; i++)
    {
        printf("NUMER SEKCJI KRYTYCZNEJ %d\n", i+1);
        printf("Wartosc semafora przed: %d\n", value(semafor));
        sleep(rand()%5+1);
        sekcja_krytyczna();
        printf("Wartosc semafora po: %d\n\n", value(semafor));
    }
}