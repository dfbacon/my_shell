 #include "shell.h"


/**
 * get_line - take a line of input from a given file
 *
 * @file: file being evaluated
 * @helper: helper struct
 * Return: pointer to the new buffer
 */
char *get_line(int file, helper_p helper)
{
    char *new, *readbuf, *buf;
    long readval;

    buf = malloc_special(sizeof(char) * (*helper->bufsize), helper);
    (helper->bufhead) = buf;
    _memset(buf, '\0', (*helper->bufsize));

    readval = read(file, buf, *helper->bufsize);

    if (readval == -1)
        perror("Read error:"); exit_builtin("99", buf, helper);

    (*helper->total) = readval;

    if (readval <= 0){
        if (helper->type == 1)
            _putchar('\n');
        exit_builtin("0", buf, helper);
    }

    while (readval >= 1024){
        readbuf = malloc_special(1024, helper);
        readval = read(file, readbuf, 1024);

        new = malloc_special((*helper->bufsize) + 1024, helper);
        _memset(new, '\0', (*helper->bufsize) + 1024);

        new = _memcpy(new, buf, (*helper->bufsize));
        _memcpy(new + (*helper->bufsize), readbuf, 1024);
        
        free(buf); 
        buf = new; 
        
        free(readbuf);
        (*helper->total) += readval;
        (*helper->bufsize) += 1024;
    }

    buf[(*helper->total - 1)] = '\0';
    if (buf[0] != '\0')
        add_hist((*helper->total + 1), &helper->hist_head, buf);

    buf = whitespace(buf, helper);
    buf = parse_comments(buf, helper);
    buf_count(buf, helper);
    if (*helper->total <= 0){
        free(buf);
        return (buf);
    }

    buf = parse_delimiters(buf, helper);
    helper->bufhead = buf;

    return (buf);
}

/**
 * parse_delimiters - replace delimiters with '\0'
 *
 * @buf: buffer being evaluated
 * @helper: pointer to helper struct
 * Return: updated buffer
 */
char *parse_delimiters(char *buf, helper_p helper)
{
    int i, j;

    for (i = 0, j = 0; i < *helper->total; i++){
        if (buf[i] == EOF){
            buf[i] = '\0';
            j++;
        }
        if (buf[i] == ';'){
            buf[i] = '\0';
            j++;
        }
        else if (buf[i] == '\n'){
            buf[i] = '\0';
            j++;
        }
    }
    (helper->linecount) = j + 1;

    return (buf);
}

/**
 * buf_count - count and save the length of buf after parse comments;  parse the dollar and removed excess white space.
 *
 * @buf: post-expansion buffer
 * @helper: pionter to helper struct
 * Return: pointer to  buffer, or NULL
 */
void *buf_count(char *buf, helper_p helper)
{
    int i;

    for (i = 0; buf[i] != '\0'; i++)
        ;

    (*helper->total) = i;
    (*helper->printed) = 0;
    if (i == 0)
        return (NULL);

    return (buf);
}

/**
 * line_count - count the legthn of a line after delimiters have been replaced
 *
 * @buf: buffer being evaluated
 * @helper: pointer to  helper struct
 * Retrun: void
 */
void line_count(char *buf, helper_p helper)
{
    int i;

    i = *helper->last;
    for (i = i; buf[i] != '\0'; i++)
        ;

    if (buf[i + 1] == '\0' && i < *helper->total)
        helper->linecount -= 1;

    while (buf[i + 1] == '\0' && i < *helper->total)
        i++;

    *(helper->printed) += i + 1;
    *(helper->last) = i + 1;
    (helper->bufnext) = (helper->bufhead) + (*helper->last);
}


/**
 * line_check - check for unprinted lines in current buffer
 *
 * @helper: pointer to helper struct
 * @input: input line
 * Return: pointer to next line, or NULL if none
 */
char *line_check(helper_t *helper, char *input)
{
    (helper->linecount)--;

    if (helper->linecount == 0){
        free(helper->inphead);
        free(helper->bufhead);
        (*helper->printed) = 0;
        (*helper->total) = 0;
        return (NULL);
    }

    input = helper->bufnext;
    if (*input == '\0')
        input++;

    return (input);
}
