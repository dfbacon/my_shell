#include "shell.h"


/**
 * parse_comments - remove comments from a given buffer
 *
 * @buf: buffer being evaluated
 * @helper: pointer to helper struct
 * Return: pointer to parsed @buf
 */
char *parse_comments(char *buf, helper_p helper)
{
    int i;

    for (i = 0; i < _strlen(buf); i++) {
        if (buf[i] == '#') {
            if (i == 0 || is_delimiter(buf[i - 1]) || buf[i - 1] == ' ') {
                while (buf[i] != '\0' && _strlen(buf) > i)
                    buf = slice_string(buf, helper->bufsize, 1, i);
            }
        }
    }

    return (buf);
}

/**
 * delete_buffer - delete section of the given buffer
 *
 * @buf: buffer
 * @helper: pointer to helper struct
 * @index: index to begin deleting
 * @times: number of bytes to delete
 *
 * Return: pointer to modified @buf
 */
char *delete_buffer(char *buf, helper_p helper, int index, int times)
{
    char *newbuf;

    newbuf = slice_string(buf, helper->bufsize, times, index);
    free(buf);
    buf = newbuf;

    return (buf);
}

/**
 * is_delimiter - checks to see if a character is a delimiter
 *
 * @c: char to check
 * Return: 1 if delim, 0 otherwise
 */
int is_delimiter(char c)
{
    if (c == ';' || c == '\n' || c == '\0')
        return (1);
    else
        return (0);
}

/**
 * is_whitespace - checks to see if given char is whitespace
 *
 * @c: char to check
 * Return: 1 if yes, 0 otherwise
 */
int is_whitespace(char c)
{
    if (c == ' ' || c == '\n' || c == '\t')
        return (1);
    else
        return (0);
}

/**
 * _getpid - gets the PID of currently running process
 *
 * Return: current PID
 */
char *_getpid(void)
{
    pid_t pid;
    char *cpid, *dir, *buf, *save, *tok, *newbuf;
    int file, status, readval;

    pid = fork();
    if (pid > 0) {
        cpid = malloc_special(sizeof(long) * sizeof(char), NULL);
        cpid[0] = '\0';
        dir = malloc_special(100, NULL);
        dir[0] = '\0';
        itoa(pid, cpid);
        _strcat(dir, "/proc/");
        _strcat(dir, cpid);
        _strcat(dir, "/stat");
        file = open(dir, O_RDONLY);
        buf = malloc_special(1024, NULL);
        readval = read(file, buf, 1024);
        if (readval < 0) {
            perror("Error reading from /proc/: ");
            return (NULL);
        }
        tok = split_str(buf, " ", &save);
        tok = split_str(NULL, " ", &save);
        tok = split_str(NULL, " ", &save);
        tok = split_str(NULL, " ", &save);
        wait(&status);
    } else
        _exit(0);

    free(dir);
    free(cpid);

    newbuf = malloc_special(_strlen(tok) + 1, NULL);
    _memcpy(newbuf, tok, _strlen(tok) + 1);
    free(buf);
    close(file);

    return (newbuf);
}

/**
 * whitespace - remove whitespace from a given buffer
 *
 * @buf: buffer being evaluated
 * @helper: pointer to helper struct
 * Return: modified @buf
 */
char *whitespace(char *buf, helper_p helper)
{
    unsigned int i;

    for (i = 0; i <= *helper->total && buf[i] != '\0'; i++) {
        for (i = i; buf[i] == ' '; i++) {
            if (i == 0 || buf[i - 1] == '\n')
                while (buf[i] == ' ')
                    buf = delete_buffer(buf, helper, i, 1);
            if (i == 0 && (buf[i + 1] == ';' || buf[i + 1] == '\0' || buf[i + 1] == ' ' || buf[i + 1] == '\n')) {
                while (buf[i] == ' ')
                    buf = delete_buffer(buf, helper, i, 1);
            }
            else if (i > 0 && buf[i] == ' '  && buf[i - 1] == ';')
                while (buf[i] == ' ')
                    buf = delete_buffer(buf, helper, i, 1);
        }
    }

    return (buf);
}

/**
 * parse_dollar - parses dollarsigns from a buffer
 *
 * @buf: buffer being evaluated
 * @helper: pointer to helper struct
 * Return: pointer to modified @buf
 */
char *parse_dollar(char *buf, helper_p helper)
{
    char *name, *newbuf;
    env_p envname, env;
    int i, j, k, start;
    
    start = 0;
    env = helper->env;

    for (i = 0; i < _strlen(buf); i++) {
        if (buf[i] == '$') {
            for (j = 0, start = ++i; !is_delimiter(buf[i]) && (!is_whitespace(buf[i])) && buf[i] != '$' && buf[i] != '?'; i++, j++)
                ;
            if (j == 0 && buf[i] == '$')
                buf = insert_pid(buf, helper, start);
            if (j == 0 && buf[i] == '?')
                buf = insert_exit_status(buf, helper, start);
            else if (j != 0) {
                name = malloc_special((j + 1) * (sizeof(char)), helper);

                for (k = start, j = 0; k != i; j++, k++)
                    name[j] = buf[k];

                name[j] = '\0';
                envname = get_env(name, env);

                if (!envname) {
                    newbuf = slice_string(buf, helper->bufsize, _strlen(name) + 1, start - 1);
                    free(helper->inphead);
                    buf = newbuf;
                    helper->inphead = buf;
                } else
                    buf = insert_env(buf, envname->value, helper, start, name);
                free(name);
            }
        }

        if (start != 0) {
            i = start;
            start = 0;
        }
    }

    return (buf);
}

/**
 * insert_env - helper function for parse_dollar(); insert the value of an environmental variable
 *
 * @buf: buffer to insert into
 * @value: value to insert
 * @helper: pointer to helper struct
 * @start: location in @buf to insert
 * @name: variable name
 * Return: pointer to modified @buf or NULL
 */
char *insert_env(char *buf, char *value, helper_p helper, int start, char *name)
{
    char *newbuf;

    newbuf = slice_string(buf, helper->bufsize, _strlen(name) + 1, start - 1);
    buf = in_strcat(newbuf, value, helper->bufsize, start - 1);
    free(newbuf);
    free(helper->inphead);
    (helper->inphead) = buf;

    return (buf);
}

/**
 * insert_pid - helper function for parse_dollar(); insert latest PID
 *
 * @buf: buffer to insert into
 * @helper: pointer to helper struct
 * @start: location in @buf to insert
 * Return: pointer to modified buffer or NULL
 */
char *insert_pid(char *buf, helper_p helper, int start)
{
    char *newbuf;
    
    newbuf = slice_string(buf, helper->bufsize, 2, start - 1);
    buf = in_strcat(newbuf, helper->pid, helper->bufsize, start - 1);
    free(newbuf);
    free(helper->inphead);
    (helper->inphead) = buf;

    return (buf);
}

/**
 * insert_exit_status - helper function for parse_dollar(); insert the last exit status
 *
 * @buf: buffer to insert into
 * @helper: pointer helper struct
 * @start: location in @buf to insert
 * Return: pointer to modified @buf or NULL
 */
char *insert_exit_status(char *buf, helper_p helper, int start)
{
    char *newbuf, *name;

    name = malloc_special(100, helper);
    itoa(helper->lastExit, name);
    newbuf = slice_string(buf, helper->bufsize, 2, start - 1);
    buf = in_strcat(newbuf, name, helper->bufsize, start - 1);
    free(newbuf);
    free(helper->inphead);
    (helper->inphead) = buf;
    free(name);

    return (buf);
}
