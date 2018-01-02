#include "shell.h"


/**
 * prompt - prints a prompt
 *
 * Return: void
 */
void prompt(void)
{
    char *cwd;

    cwd = getcwd(NULL, 100);
    _puts(cwd);
    _putchar('$');
    _putchar(' ');
    free(cwd);
}

/**
 * _puts - prints a string to stdout
 *
 * @str: string to print
 * Return: void
 */
void _puts(char *str)
{
    int i;

    if (!str)
        return;

    i = 0;
    while (str[i] != '\0')
        _putchar(str[i++]);
}

/**
 * _putchar - writes a single character to stdout
 *
 * @c: The character to print
 * Return: 1 on success, -1 otherwise
 */
int _putchar(char c)
{
    return write(1, &c, 1);
}

/**
 * _putint - prints an integer
 *
 * @i: int to print
 * @flag: set flag to 0. To account for i == 0
 * Return: void
 */
void _putint(int i, int flag)
{
    if (i != 0) {
        _putint(i / 10, 1);
        _putchar(i % 10 + '0');
    }
    else if (i == 0 && flag == 0)
        _putchar('0');
}
