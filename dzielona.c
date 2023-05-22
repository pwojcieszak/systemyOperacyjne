#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int create_PD (const char *name) 
{
	int deskryptor = shm_open(name, O_RDWR | O_CREAT | O_EXCL, 0777);
	if (deskryptor == -1) 
    {
		perror ("blad tworzenia pamieci dzielonej");
		_exit (EXIT_FAILURE);
	} 
	return deskryptor;
}

int open_PD (const char *name) {
	int deskryptor = shm_open(name, O_RDWR, 0777);
	if (deskryptor == -1) {
		perror ("blad otwierania pamieci dzielonej");
		exit (EXIT_FAILURE);
	} else {
		return deskryptor;
	}
}

void size_PD (int fd, off_t length) 
{
	if (ftruncate(fd, length) == -1) 
    {
		perror ("blad inicjowania pamieci dzielonej");
		_exit (EXIT_FAILURE);
	}
}

void close_PD (int fd) 
{
	if (close(fd) == -1) 
    {
		perror ("blad zamykania pamieci dzielonej");
		_exit (EXIT_FAILURE);
	}
}

void del_PD (const char *name) 
{
	if (shm_unlink(name) == -1) 
    {
		perror ("blad usuwania pamieci dzielonej");
		_exit (EXIT_FAILURE);
	}
}

void *map_PD (int fd, size_t length) 
{
	void *adres = mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (adres == MAP_FAILED) {
		perror ("blad odwzorowania pamieci dzielonej");
		_exit (EXIT_FAILURE);
	}
	return adres;
}

void del_map_PD (void *addr, size_t length) 
{
	if (munmap(addr, length) == -1) 
    {
		perror ("blad usuwania odwzorowania pamieci dzielonej");
		_exit (EXIT_FAILURE);
	}
}