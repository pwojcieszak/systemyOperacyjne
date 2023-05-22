#include <stdio.h>
#include <unistd.h>
#include <sys/types.h> 
#include "funkcja.h"

/* Program wypisujacy identyfikatory UID, GID, PID, PPID i PGID dla danego procesu */

//funkcja wypisujaca, wszystkie rozne ID aktywnego procesu
void allID()
{
    printf("UID: %d GID: %d PID: %d PPID: %d PGID: %d\n", getuid(), getgid(), getpid(), getppid(), getpgrp());
}

int main()
{
    allID();
}