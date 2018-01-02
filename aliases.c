#include "shell.h"


/**
 * alias_builtin - determines what to do with alias input
 *
 * @args: two-dimensional array of arguments
 * @helper: pointer to helper struct
 * Return: void
 */
void alias_builtin(char **args, helper_p helper)
{
    int i;

    if (!args[1]) {
        list_alias(helper);
        return;
    }

    for (i = 0; args[1][i] != '\0'; i++) {
        if (args[1][i] == '=') {
            set_alias(args, helper, &helper->alias);
            return;
        }
    }

    if (get_alias(args[1], helper->alias) == NULL)
        _puts("No such alias exists.\n");
}

/**
 * list_alias - lists available aliases
 *
 * @helper: pointer to helper struct
 * Return: always 1
 */
int list_alias(helper_p helper)
{
    alias_p walk;

    walk = helper->alias;
    if (!walk) {
        _puts("No aliases.\n");
        return (1);
    }

    while (walk) {
        _puts("alias ");
        _puts(walk->name);
        _putchar('=');
        _putchar('\'');
        _puts(walk->value);
        _putchar('\'');
        _putchar('\n');
        walk = walk->next;
    }

    return (1);
}

/**
 * set_alias - adds an alias
 *
 * @args: two-dimensional array of arguments
 * @helper: pointer to helper struct
 * @head: pointer to head of alias linked list
 * Return: pointer to new alias_p node, or NULL
 */
alias_p set_alias(char **args, helper_p helper, alias_p *head)
{
    alias_p newnode, tempnode;
    char *value, *save, *name;
    char *space = " ";
    int i;

    name = split_str(args[1], "=", &save);
    if (get_alias(name, helper->alias) != NULL)
        remove_alias(name, &helper->alias);

    value = malloc_special(1000, helper);
    _memset(value, '\0', 1000);
    _strcat(value, save);
    _strcat(value, space);
    for (i = 2; args[i] != NULL; i++) {
        _strcat(value, args[i]);
        _strcat(value, space);
    }

    value[_strlen(value) - 1] = '\0';
    if (!head || !name || !value)
        return NULL;

    newnode = malloc_special(sizeof(alias_t), NULL);
    if (!newnode)
        return NULL;

    newnode->name = _strdup(name);
    newnode->value = value;
    if (!*head) {
        newnode->next = *head;
        *head = newnode;
    } else {
        newnode->next = NULL;
        tempnode = *head;
        while (tempnode->next)
            tempnode = tempnode->next;
        tempnode->next = newnode;
    }

    return (newnode);
}

/**
 * get_alias - checks to see if alias exists
 *
 * @name: environment variable to search for
 * @head: pointer to head of the alias linked list
 * Return: pointer to @name node, or NULL
 */
alias_p get_alias(char *name, alias_p head)
{
    alias_p alist;

    alist = head;
    while (alist) {
        if (all_strcmp(name, alist->name) == 0)
            return alist;
        alist = alist->next;
    }

    return NULL;
}

/**
 * free_alist - frees a alias_t linked list
 *
 * @head: pointer to head of alias_t linked list
 * Return: void
 */
void free_alist(alias_p head)
{
    alias_p temp;

    temp = head;
    while (head) {
        temp = head;
        head = head->next;
        free(temp->name);
        free(temp->value);
        free(temp);
    }
    free(head);
}

/**
 * remove_alias - removes an alias
 *
 * @head: pointer to head of alist
 * @name: name of alias
 * Return: 1 on success, -1 otherwise
 */
int remove_alias(char *name, alias_p *head)
{
    int i;
    alias_p temp, last;

    temp = *head;
    if (!name) {
        _puts("Invalid name.\n");
        return (-1);
    }

    i = 0;
    while (temp) {
        if (_strcmp(name, temp->name) == 0) {
            if (i == 0) {
                *head = temp->next;
                free(temp->name);
                free(temp->value);
                free(temp);
            } else {
                last->next = temp->next;
                free(temp->name);
                free(temp->value);
                free(temp);
            }
            return 1;
        }

        last = temp;
        temp = temp->next;
        i++;
    }
    _puts("No such alias.\n");

    return -1;
}

/**
 * parse_alias - parses aliases
 *
 * @helper: pointer to helper struct
 * @buf: buffer being evaluated
 * Return: pointer to new buffer
 */
char *parse_alias(char *buf, helper_p helper)
{
    alias_p alias;
    char *newbuf;
    int newsize;
    char *name;
    int i;

    name = malloc(*helper->bufsize);
    if (!name)
        return NULL;
    i = 0;
    while (buf[i] != ' ' && buf[i] != ';' && buf[i] != '\0') {
        name[i] = buf[i];
        i++;
    }
    name[i] = '\0';
    alias = get_alias(name, helper->alias);
    if (!alias) {
        free(name);
        return (buf);
    }

    newsize = _strlen(alias->value) + *helper->bufsize;
    newbuf = malloc_special(newsize, helper);
    _memset(newbuf, '\0', newsize);
    newbuf = _memcpy(newbuf, alias->value, _strlen(alias->value));
    _memcpy(newbuf + _strlen(alias->value) + 1, buf + _strlen(name), (*helper->bufsize) - (_strlen(name) + 10));
    newbuf[_strlen(alias->value)] = ' ';
    free(helper->inphead);
    free(name);
    helper->inphead = newbuf;

    return (newbuf);
}
