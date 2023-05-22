#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 3)
	{
		perror("Podano za malo argumentow. Skladnia '<nazwa_programu> <potok> <plik>'");
		exit (EXIT_FAILURE);
	}
    char *potok = argv[1];
    char *plik = argv[2];
    //otwarcie potoku
    int we = open(plik, O_RDONLY);
    char buffor_in[100];
    if(we==-1)
    {
        perror ("2Blad open.\n");
        exit(EXIT_FAILURE);
        
    }
    int wy = open(potok, O_WRONLY | O_CREAT, 0777);
    if(wy==-1)
    {
        perror ("2Blad open.\n");
        exit(EXIT_FAILURE);
        
    }
    int bajty_prod = read(we, buffor_in, 50);
    if(bajty_prod==-1)
    {
        perror ("3Blad read.\n");
        exit (EXIT_FAILURE);
        
    }
    while(bajty_prod>0)
    {
        printf("\nWysylane dane \n");
        if(write(1, buffor_in, bajty_prod)==-1)
        {
            perror("Blad write\n");
            exit(EXIT_FAILURE);
            
        }
        printf("\n"); 
        if(write(wy, buffor_in, bajty_prod)==-1)
        {
            perror ("4Blad read.\n");
            exit (EXIT_FAILURE);
                    
        }
        sleep(1);
        bajty_prod = read(we, buffor_in, 50);
    }
    if(close(wy)==-1)
    {
        perror ("5Blad close.\n");
        exit (EXIT_FAILURE);
       
    }
    if(close(we)==-1)
    {
        perror ("5Blad close.\n");
        exit (EXIT_FAILURE);    
    }
    exit(EXIT_SUCCESS);
}