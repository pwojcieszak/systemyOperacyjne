#ifndef SEM_FUN_H
#define SEM_FUN_H

#include <semaphore.h>
sem_t* create(const char*, int);
sem_t* openS(const char*);
int value(sem_t*);
void handle(sem_t*, char);
void end(sem_t*);
void del(const char*);

#endif