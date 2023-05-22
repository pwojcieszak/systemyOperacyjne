#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sem_fun.h"

//tworzenie semafora
sem_t* create(const char *name, int value)
{
    sem_t* res= sem_open(name, O_CREAT | O_EXCL, 0777, value);
    if (res == SEM_FAILED)
    {
        perror ("Blad sem_open\n");
        _exit(EXIT_FAILURE); 
    }
    return res;
}

//otwarcie semafora 
sem_t *openS (const char *name) {
	sem_t *res = sem_open(name, 0);
	if (res == SEM_FAILED)
    {
        perror ("Blad sem_open\n");
        _exit(EXIT_FAILURE); 
    }
    return res;
}


//uzyskiwanie wartosci semafora
int value(sem_t *sem) 
{
    int sval;
    if(sem_getvalue(sem, &sval)==-1)
    {
        perror ("Blad sem_getvalue\n");
        _exit(EXIT_FAILURE); 
    }
    return sval;
}


//operowanie semaforem
void handle(sem_t* sem, char t)
{
    if(t=='1')
    {
        if(sem_post(sem) == -1)
        {
            perror ("Blad sem_post\n");
            _exit(EXIT_FAILURE); 
        }
    }
    else if(t=='0')
    {
        
        if(sem_wait(sem) == -1)
        {
            perror ("Blad sem_wait\n");
            _exit(EXIT_FAILURE); 
        }
    }
    else
    {
        perror("Zla opcja t\n");
        _exit(EXIT_FAILURE);
    }
}
//zamykanie semafora
void end(sem_t *sem) 
{
    if(sem_close(sem)==-1)
    {
        perror ("Blad sem_close\n");
        _exit(EXIT_FAILURE); 
    }
}

//usuwanie semafora
void del(const char *name) 
{
    if(sem_unlink(name)==-1)
    {
        perror ("Blad sem_unlink\n");
        _exit(EXIT_FAILURE); 
    }
}