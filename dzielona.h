#ifndef DZIELONA_H
#define DZIELONA_H

int create_PD (const char *name);
int open_PD (const char *name);
void size_PD (int fd, off_t length);
void close_PD (int fd);
void del_PD (const char *name);
void *map_PD (int fd, size_t length);
void del_map_PD (void *addr, size_t length);

#endif