#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main()
{
    int filedes1;
    if ((filedes1 = open("tekst.txt", O_RDONLY, 0644)) == -1)
    {
        perror("Open1 error\n");
        exit(EXIT_FAILURE);
    }

    int filedes2;
    if ((filedes2 = open("kopia.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
    {
        perror("Open2 error\n");
        exit(EXIT_FAILURE);
    }

    int size = 64;
    int nbytes = 32;
    char bufor[size];
    char output[2 * size];
    int readed;
    while ((readed = read(filedes1, bufor, nbytes)) > 0)
    {
        if (write(filedes2, bufor, readed) == -1)
        {
            perror("Write error\n");
            exit(EXIT_FAILURE);
        }
        sprintf(output, "Zapisano: %s  ||| o wielkosci %d bajtow\n", bufor, readed);
        if (write(STDOUT_FILENO, output, sizeof(char) * strlen(output)) == -1)
        {
            perror("Write on screen error\n");
            exit(EXIT_FAILURE);
        }
        memset(bufor, '\0', size); // czyscimy bufor
    }
    if (readed == -1)
    {
        perror("Read error\n");
        exit(EXIT_FAILURE);
    }

    // zamykamy deskryptory
    if (close(filedes1) == -1)
    {
        perror("Close1 error\n");
        exit(EXIT_FAILURE);
    }
    if (close(filedes2) == -1)
    {
        perror("Close2 error\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}