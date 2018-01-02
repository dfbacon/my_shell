#include "shell.h"


/**
 * in_strcat - 'concatenates' a string to the inside of a buffer
 *
 * @buf: buffer to being targeted
 * @string: string to insert; must be null-terminated
 * @bufsize: size of @buf
 * @insert: index of @buf where @string is to be inserted
 * Return: pointer to modified @buf
 */
char *in_strcat(char *buf, char *string, int *bufsize, int insert)
{
    char *newbuf;
    int newsize;

    newsize = *bufsize + _strlen(string);

    newbuf = malloc_special(newsize * sizeof(char), NULL);
    _memset(newbuf, '\0', newsize);
    _memcpy(newbuf, buf, insert);
    _memcpy(newbuf + insert, string, _strlen(string));
    _memcpy(newbuf + insert  + _strlen(string), buf + insert, *bufsize - insert);
    *bufsize = newsize;

    return (newbuf);
}

/**
 * slice_string - cuts a substring out of a string and resizes it
 *
 * @buf: buffer being evaluated
 * @bufsize: size of @buf
 * @slicesize: size of the 'slice' to be removed
 * @index: location in @buf to slice
 * Return: pointer to modified @buf
 */
char *slice_string(char *buf, int *bufsize, int slicesize, int index)
{
    char *newbuf;
    int newsize;

    newsize = *bufsize - slicesize;
    newbuf = malloc_special(newsize * sizeof(char), NULL);
    _memset(newbuf, '\0', newsize);
    _memcpy(newbuf, buf, index);

    if (buf[index + slicesize] != '\0')
        _memcpy(newbuf + index, buf + index + slicesize, *bufsize - index - slicesize);

    *bufsize = newsize;
    return (newbuf);
}
