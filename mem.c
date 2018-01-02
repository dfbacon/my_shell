#include "shell.h"


/**
 * _realloc - copy of realloc()
 *
 * @ptr: pointer to move data from
 * @old_size: size of current pointer
 * @new_size: size of new pointer
 * Return: pointer to new allocated memory, NULL otherwise
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
    char *newspace;
    unsigned int i;

    if (new_size == old_size)
        return (ptr);

    if (ptr == NULL) {
        newspace = malloc(new_size);
        if (!newspace)
            return (NULL);
        return (newspace);
    }

    if (new_size == 0 && ptr) {
            free(ptr);
            return (NULL);
    }

    newspace = malloc(new_size);
    if (!newspace)
        return (NULL);

    for (i = 0; i < old_size && i < new_size; i++)
        newspace[i] = ((char *)ptr)[i];
    free(ptr);

    return newspace;
}

/**
 * _memcpy - copies memory location in src to destination
 *
 * @dest: destination
 * @src: source
 * @n: number of bytes
 * Return: pointer to @dest
 */
char *_memcpy(char *dest, char *src, unsigned int n)
{
    int i;

    i = 0;
    while (i < (int)n) {
        dest[i] = src[i];
        i++;
    }
    
    return (dest);
}

/**
 * malloc_special - copy of malloc() that attempts to free on failure
 *
 * @b: size to malloc
 * @helper: pointer to helper struct
 * Return: returns malloc'd space, or nothing on failure (exits)
 */
void *malloc_special(unsigned int b, helper_p helper)
{
    void *pointer;

    pointer = malloc(b);
    if (!pointer) {
        perror("Failed to allocate memory\n");
        if (!helper)
            _exit(98);
        exit_builtin("98", NULL, helper);
    }

    return (pointer);
}

/**
 * _memset - fills memory with constant bytes
 *
 * @s: location to fill
 * @b: char to fill @s with
 * @n: number of bytes to fill
 * Return: pointer to filled @s
 */
char *_memset(char *s, char b, unsigned int n)
{
    int i;

    i = 0;
    while (n > 0) {
        s[i] = b;
        i++;
        n--;
    }

    return (s);
}
