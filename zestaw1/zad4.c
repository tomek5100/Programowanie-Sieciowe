#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

bool drukowalne(const void *buf)
{
    const char *char_buf = buf;
    for (int i = 0; i < sizeof(buf); i++)
    {
        if (char_buf[i] < 32 && char_buf[i] > 126)
            return false;
    }
    return true;
}

bool drukowalne_pointers(const void *buf)
{
    const char *char_buf = buf;
    for (int i = 0; i < sizeof(buf); i++)
    { // wyłuskujemy wartosc spod adresu *(char_buf + i)
        if (*(char_buf + i) < 32 || *(char_buf + i) > 126)
        {
            return false;
        }
    }
    return true;
}

int main()
{
    //to jest chyba zle, sprawdzic znaczenie String w kontekscie jezyka C
    const char array_t[] = {'H', 'e', 'l', 'l', 'o', '\0'};
    const char array_true[] = {33, 34, 35, 46, 63};
    const char array_false[] = {10, 36, 55, -73};

    printf("%s\n", drukowalne(array_t) ? "true" : "false");
    printf("%s\n", drukowalne(array_true) ? "true" : "false");
    printf("%s\n", drukowalne(array_false) ? "true" : "false");
    printf("%s\n", drukowalne_pointers(array_t) ? "true" : "false");
    printf("%s\n", drukowalne_pointers(array_true) ? "true" : "false");
    printf("%s\n", drukowalne_pointers(array_false) ? "true" : "false");

    return 0;
}