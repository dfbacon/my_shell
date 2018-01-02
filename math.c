#include "shell.h"


/**
 * itoa - convert integer to string
 *
 * @n: integer to convert
 * @s: character array
 * Return: int
 */
int itoa(int n, char *s)
{
    int i = 0;

    if (n / 10 != 0)
        i = itoa(n / 10, s);

    else if (n < 0)
        s[i++] = '-';

    s[i++] = _abs(n % 10) + '0';
    s[i] = '\0';

    return (i);
}


/**
 * _abs - return absolute value of a number
 *
 * @num: integer being evaluated
 * Return: absolute value of @num
 */
int _abs(int num)
{
    if (num < 0)
        return num * -1;
    else
        return num;
}
