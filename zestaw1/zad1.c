#include <stdlib.h>
#include <stdio.h>

int main()
{
    int size = 50;
    int liczby[size];
    for (int i = 0; i < size; i++)
    {
        scanf("%d", &liczby[i]);
        if (liczby[i] == 0)
            break;
    }
    for (int i = 0; i < size; i++)
    {
        if(liczby[i] > 10 && liczby[i] < 100)
            printf("%d", liczby[i]);
    }

    return 0;
}