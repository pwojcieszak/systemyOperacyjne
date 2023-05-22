#ifndef WATKI_H
#define WATKI_H

#include <pthread.h>

pthread_t t_create (void *(void *), void *);
void t_join (pthread_t);
void m_init (pthread_mutex_t *);
void m_close (pthread_mutex_t *mutex);
void m_open (pthread_mutex_t *mutex);
void m_destroy (pthread_mutex_t *mutex);

#endif