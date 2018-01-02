#include "shell.h"

/**
 * _strlen - determine the length of a given string
 *
 * @s: string being evaluated
 * Return: length of @s
 */
int _strlen(char *s)
{
    int i;

    i = 0;
    while (s[i] != '\0')
        i++;
    return i;
}

/**
 * _strcpy - copies a string to a buffer
 *
 * @src: source string to be copied
 * @dest: buffer to copy @src to
 * Return: pointer to @src
 */
char *_strcpy(char *dest, char *src)
{
    int i;
    char *retval;

    i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }

    dest[i] = src[i];
    retval = dest;

    return retval;
}

/**
 * _strdup - duplicates a string
 *
 * @str: string to duplicate
 * Return: pointer to duplicated @str
 */
char *_strdup(char *str)
{
    char *dupe;

    if (!str)
        return NULL;

    dupe = malloc_special(_strlen(str) + 1, NULL);
    if (!dupe)
        return NULL;

    _strcpy(dupe, str);

    return dupe;
}

/**
 * str_concat - concatenates two strings
 *
 * @s1: first string
 * @s2: second string
 * Return: pointer to new string
 */
char *str_concat(char *s1, char *s2)
{
    char *new;
    int i, j, size;

    if (!s1)
        s1 = "";
    if (!s2)
        s2 = "";

    size = (_strlen(s1) + _strlen(s2) + 1);
    new = malloc_special(size, NULL);
    if (!new)
        return NULL;

    _strcpy(new, s1);
    i = _strlen(s1);
    for (i = _strlen(s1), j = 0; i != size && s2[j] != '\0'; i++, j++)
        new[i] = s2[j];
    new[i] = '\0';

    return new;
}

/**
 * _strcmp - compares two strings
 *
 * @s1: first string
 * @s2: second string
 * Return: difference in ASCII value between @s1 and @s2
 */
int _strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    if (s1[i] == '\0' || s2[i] == '\0')
        return -1;

    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i])
            return ((s1[i] - '0') - (s2[i] - '0'));
        i++;
    }

    return (0);
}

/**
 * _strcat - concatenate two  strings, @src appended to end of @dest
 *
 * @dest: destination string
 * @src: source string
 * Return: returns finished string
 */
char *_strcat(char *dest, char *src)
{
    int i, j;

    i = j = 0;
    while (dest[i] != '\0')
        i++;

    while (src[j] != '\0')
        dest[i++] = src[j++];
    dest[i] = '\0';

    return dest;
}

/**
 * split_str - tokenize a string by delimiter
 *
 * @str: string to tokenize
 * @delim: deliminator
 * @saveptr: ptr used to save locations
 * Return: pointer to new token
 */
char *split_str(char *str, const char *delim, char **saveptr)
{
    char *tok;
    int i;

    if (!str) {
        if (!*saveptr)
            return NULL;
        str = *saveptr;
    }

    i = 0;
    tok = str;
    while (*str != '\0' && *str != EOF && *str != *delim) {
        str++;
        i++;
    }

    if (*str == '\0')
        *saveptr = NULL;
    else {
        if (saveptr)
            *saveptr = str + 1;
    }
    tok[i] = '\0';

    return tok;
}

/**
 * all_strcmp - compares two strings; evaluates null terminator as well
 *
 * @s1: first string
 * @s2: second string
 * Return: difference in ASCII value between string 1 and string 2
 */
int all_strcmp(char *s1, char *s2)
{
    int i;

    i = 0;
    if (s1[i] == '\0' || s2[i] == '\0')
        return -1;

    while (s1[i] != '\0' && s2[i] != '\0') {
        if (s1[i] != s2[i])
            return ((s1[i] - '0') - (s2[i] - '0'));
        i++;
    }

    if (s1[i] == '\0' && s2[i] == '\0')
        return 0;

    return i;
}

/**
 * dir_concat - concatenates two strings; appends with '/'
 *
 * @s1: first string
 * @s2: second string
 * Return: pointer to new string, NULL
 */
char *dir_concat(char *s1, char *s2)
{
    char *new;
    int i, j, size;

    if (!s1)
        s1 = "";
    if (!s2)
        s2 = "";

    size = (_strlen(s1) + _strlen(s2) + 3);
    new = malloc_special(size, NULL);
    if (!new)
        return NULL;

    _strcpy(new, s1);
    i = _strlen(s1);
    new[i] = '/';
    for (i = (_strlen(s1) + 1), j = 0; i != size && s2[j] != '\0'; i++, j++)
        new[i] = s2[j];
    new[i] = '\0';

    return new;
}

/**
 * _atoi - copy of atoi(); converts a character array to an integer
 *
 * @s: character array to convert
 * Return: integer from array, or 0
 */
int _atoi(char *s)
{
    int retval, i, neg;

    i = 0;
    retval = 0;
    neg = -1;
    while (s[i] != '\0' && (s[i] < '0' || s[i] > '9')) {
        if (s[i] == '-')
            neg *= -1;
        i++;
    }

    while (s[i] != '\0' && (s[i] >= '0' && s[i] <= '9'))
        retval = (retval * 10) - (s[i++] - '0');

    return retval * neg;
}
