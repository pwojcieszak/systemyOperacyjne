#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include "kolejki.h"

mqd_t q_create (const char *name, int flag) 
{
    struct mq_attr attr;
	attr.mq_maxmsg = L_KOM;
	attr.mq_msgsize = DLUGOSC;
	mqd_t des = mq_open(name, O_CREAT | O_EXCL | flag, 0777, &attr);
	if (des == -1) 
    {
		perror ("błąd tworzenia kolejki");
		exit (EXIT_FAILURE);
	}
	return des;
}

mqd_t q_open (const char *name, int flag) 
{
	mqd_t des = mq_open(name, flag);
	if (des == -1) 
    {
		perror ("błąd otwierania kolejki");
		exit (EXIT_FAILURE);
	}
	return des;
}

void q_close (mqd_t mq_des) 
{
	if (mq_close(mq_des) == -1) 
    {
		perror ("błąd zamykania kolejki");
		exit (EXIT_FAILURE);
	}
}

void q_unlink (const char *name) 
{
	if (mq_unlink(name) == -1) 
    {
		perror ("błąd usuwania kolejki");
		exit (EXIT_FAILURE);
	}
}

struct mq_attr q_getattr (mqd_t mq_des) 
{
	struct mq_attr atrybuty;
	if (mq_getattr(mq_des, &atrybuty) == -1) 
    {
		perror ("błąd pobierania atrybutów kolejki");
		exit (EXIT_FAILURE);
	}
	return atrybuty;
}

void q_send (mqd_t mq_des, const char *msg_ptr) 
{
	if (mq_send(mq_des, msg_ptr, DLUGOSC, 1) == -1) 
    {
		perror ("błąd wysyłania komunikatu");
		exit (EXIT_FAILURE);
	}
}

void q_receive (mqd_t mq_des, char *msg_ptr) 
{
	if (mq_receive(mq_des, msg_ptr, DLUGOSC, NULL) == -1) 
    {
		perror ("błąd odbierania komunikatu");
		exit (EXIT_FAILURE);
	}
}



