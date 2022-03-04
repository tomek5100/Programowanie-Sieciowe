#include <stdlib.h>
#include <stdio.h>

int main()
{
    int size = 50;
    int liczby[size];
    int *i = NULL;
    for (i = liczby; i < liczby + size; ++i)
    {
        scanf("%d", i);
        if (*i == 0) //wyłuskanie wartości spod adresu *i
            break;
    }
    for (i = liczby; i < liczby + size; ++i)
    {
        if(*i > 10 && *i < 100)
            printf("%d\n", *i);
    }

    return 0;
}