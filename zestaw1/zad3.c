#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

bool drukowalne(const void *buf, int len)
{
    const char *char_buf = buf; // rzutowanie na typ const char
    for (int i = 0; i < len; i++)
    {
        if (char_buf[i] < 32 || char_buf[i] > 126)
            return false;
    }
    return true;
}

bool drukowalne_pointers(const void *buf, int len)
{
    const char *char_buf = buf;
    for (int i = 0; i < len; i++)
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

    const char array_true[5] = {33, 34, 35, 46, 63};
    const char array_false[5] = {10, 36, 55, -73};
    printf("%s\n", drukowalne(array_true, 5) ? "true" : "false");
    printf("%s\n", drukowalne(array_false, 4) ? "true" : "false");
    printf("%s\n", drukowalne_pointers(array_true, 5) ? "true" : "false");
    printf("%s\n", drukowalne_pointers(array_false, 4) ? "true" : "false");

    return 0;
}