#include "shell.h"

/**
 * exit_builtin - executes the 'exit' builtin
 *
 * @tok: argument for exit_builtin
 * @input: input
 * @helper: pointer to helper struct
 * Return: void
 */
void exit_builtin(char *tok, char *input, helper_p helper)
{
    int i;

    if (tok)
        i = _atoi(tok);
    else
        i = 0;

    push_hist(helper->hist_head, helper->env);

    if (input) {
        if (input != helper->bufhead)
            free(helper->bufhead);
        free(input);
    }

    clear_hist(&(helper->hist_head));
    
    free_list(helper->env);
    free(helper->printed);
    free(helper->total);
    free(helper->last);
    free(helper->bufsize);
    free(helper->pid);
    
    if (helper->alias)
        free_alist(helper->alias);
    if (helper->args)
        free(helper->args);
    free(helper);
    _exit(i & 255);
}

/**
 * list_env - prints list of environmental variables
 *
 * @environ: pointer to environmental variables
 * Return: 1 on success, -1 otherwise
 */
int list_env(env_p *environ)
{
    env_p walk;

    walk = *environ;

    if (!walk) {
        _puts("Failure to print environmental variables.\n");
        return (-1);
    }

    while (walk) {
        _puts(walk->name);
        _putchar('=');
        _puts(walk->value);
        _putchar('\n');
        walk = walk->next;
    }
    return (1);
}

/**
 * cd_builtin - executes the builtin 'cd' command
 *
 * @args: pointer to list of arguments
 * @helper: pointer to helper struct
 * Return: 1 on success, 0 otherwise
 */
int cd_builtin(char **args, helper_p helper)
{
    char *oldcwd, *home, *tok;

    tok = args[1];
    oldcwd = getcwd(NULL, 100);
    home = (get_env("HOME", helper->env))->value;

    if (tok == NULL) {
        chdir(home);
        set_env("PWD", home, helper->env);
        set_env("OLDPWD", oldcwd, helper->env);
        free(oldcwd);
        return (1);
    }

    if (tok[0] == '-' && tok[1] == '\0') {
        if (get_env("OLDPWD", helper->env) == NULL) {
            _puts("cd: OLDPWD not set.\n");
            free(oldcwd);
            return (0);
        }
        tok = (get_env("OLDPWD", helper->env))->value;
        _puts(tok);
        _putchar(10);
    }

    if (chdir(tok) != -1) {
        set_env("PWD", tok, helper->env);
        set_env("OLDPWD", oldcwd, helper->env);
        free(oldcwd);
        return (1);
    }

    _puts("cd: Invalid folder.\n");
    free(oldcwd);

    return (0);
}

/**
 * unset_env - removes an environmental variable
 *
 * @name: name of environmental variable to remove
 * @head: pointer to environmental variables
 * Return: 1 on success, -1 otherwise
 */
int unset_env(char *name, env_p *head)
{
    int i;
    env_p temp, last;

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

            return (1);
        }
        last = temp;
        temp = temp->next;
        i++;
    }

    _puts("Environmental variable not found.\n");

    return (-1);
}

/**
 * set_env - set environment variable to new value or creates new variable
 *
 * @name: variable name
 * @value: value for @name
 * @head: pointer to list of environmental variables
 * Return: pointer to the new environmental variable
 */
env_p set_env(char *name, char *value, env_p head)
{
    env_p environ;
    char *newval;

    environ = head;
    if (!name) {
        _puts("Invalid variable name.\n");
        return (NULL);
    }

    if (!value) {
        _puts("Invalid value.\n");
        return (NULL);
    }

    environ = get_env(name, head);
    if (environ) {
        newval = _strdup(value);
        if(!newval)
            return (NULL);

        free(environ->value);
        environ->value = newval;
        return (environ);
    }

    return (add_env(&head, name, value));
}
