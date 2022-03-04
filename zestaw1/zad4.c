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
    int *p = NULL;
    for (p = char_buf; p < len + char_buf; ++p)
    
    {
        if (*p < 32 && *p > 126)
            return false;
    }

    return true;
}

int main()
{
    const char *array = {'H', 'e', 'l', 'l', 'o', '\0'};
    bool result;
    result = drukowalne(array);
    printf("%s\n", result ? "true" : "false");

    return 0;
}