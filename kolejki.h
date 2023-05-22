#ifndef KOLEJKI_H
#define KOLEJKI_H

#define KOLEJKA_NAZWA "/kolejka"
#define DLUGOSC 20
#define L_KOM 3


mqd_t q_create (const char *, int);
mqd_t q_open (const char *, int);
void q_close (mqd_t);
void q_unlink (const char *);
struct mq_attr q_getattr (mqd_t);
void q_send (mqd_t, const char *);
void q_receive (mqd_t, char *);

#endif