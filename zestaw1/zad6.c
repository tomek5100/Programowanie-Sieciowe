#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    int filedes = NULL;
    filedes = open("tekst.txt", O_RDONLY);
    return 0;
}