#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"

char *int_to_str(int n)
{
    int i = 0, sign = n;
    char c;
    char *s = malloc(12); // 11 digits + sign + '\0'

    if (s == NULL)
    {
        return NULL; // Allocation failed
    }

    if (sign < 0)
    {
        n = -n;
        s[i++] = '-';
    }

    do
    {
        s[i++] = n % 10 + '0';
        n /= 10;
    } while (n > 0);

    s[i] = '\0';

    // Reverse the string
    int len = strlen(s);
    for (int j = 0; j < len / 2; j++)
    {
        c = s[j];
        s[j] = s[len - j - 1];
        s[len - j - 1] = c;
    }

    return s;
}

void crlf_to_lf(char *str)
{
    char *p = str;
    char *q = str;
    while (*p)
    {
        if (*p == '\r' && *(p + 1) == '\n')
        {
            p++;
        }
        *q++ = *p++;
    }
    *q = '\0';
}