#ifndef SEM_FUN_H
#define SEM_FUN_H

sem_t* create(const char*);
int value(sem_t*);
void handle(sem_t*, char);
void end(sem_t*);
void delete(const char*);

#endif