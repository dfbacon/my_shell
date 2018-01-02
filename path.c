#include "shell.h"


/**
 * check_local - checking if input token is local
 *
 * @tok: tokenized input
 * @helper: pointer to helper struct
 * @args: tokenized argument list
 * Return: 0 if success, 1 if fail, and 127 if no such file found
 */
int check_local(char *tok, helper_p helper, char **args)
{
    struct stat st;

    if ((tok[0] == '.' && tok[1] == '/') || tok[0] == '/') {
        if (tok[0] == '.')
            tok = tok + 2;

        if (stat(tok, &st) == 0 && (st.st_mode & S_IXUSR) && S_ISREG(st.st_mode)) {
            helper->lastExit = run(tok, args, helper);
            return (0);
        } else {
            if (tok[0] != '/')
                tok = (tok - 2);
            
            _puts(tok);
            _puts(": No such file or directory.\n");
            helper->lastExit = 127;
            return (127);
        }
    }

    return (1);
}

/**
 * check_builtins - checks for builtin commands
 *
 * @tok: input string being evaluated
 * @helper: pointer to helper struct
 * @args: tokenized argument list
 *
 * Return: returns 0 on success, 1 otherwise
 */
int check_builtins(char *tok, helper_p helper, char **args)
{
    hist_p hist_head;

    hist_head = helper->hist_head;

    if (all_strcmp(tok, "env") == 0)
        list_env(&helper->env);
    else if (all_strcmp(tok, "exit") == 0)
        exit_builtin(args[1], tok, helper);
    else if (all_strcmp(tok, "setenv") == 0)
        set_env(args[1], args[2], helper->env);
    else if (all_strcmp(tok, "unsetenv") == 0)
        unset_env(args[1], &helper->env);
    else if (all_strcmp(tok, "history") == 0)
        print_hist(hist_head);
    else if (all_strcmp(tok, "cd") == 0)
        cd_builtin(args, helper);
    else if (all_strcmp(tok, "help") == 0)
        help_builtin(args[1]);
    else if (all_strcmp(tok, "alias") == 0)
        alias_builtin(args, helper);
    else if (all_strcmp(tok, "unalias") == 0)
        remove_alias(args[1], &helper->alias);
    else
        return (1);

    return (0);
}

/**
 * check_path - check for program located in $PATH
 *
 * @input: input string beint evaluated
 * @helper: pointter to helper struct
 * @args: tokenized argument list
 * Return: returns 1 if @input in $PATH, 0 otherwise
 */
int check_path(char *input, char **args, helper_p helper)
{
    int j, retval;
    char *temp, *path[PATHSIZE], *tok, *pathsave, *paths, *cwd;
    char colon = ':';

    temp = NULL;
    cwd = malloc_special(100, helper);
    getcwd(cwd, 100);
    retval = 0;
    
    if (get_env("PATH", helper->env)) {
        if (!input || input[0] == '\0') {
            free(cwd);
            return (-1);
        }

        paths = _strdup((get_env("PATH", helper->env))->value);
        tok = split_str(paths, &colon, &pathsave);

        for (j = 0; tok != NULL; j++) {
            path[j] = tok;
            tok = split_str(NULL, &colon, &pathsave);
        }

        path[j] = NULL;
        for (j = 0; path[j] != NULL; j++) {
            if (path[j][0] == '\0')
                temp = dir_concat(cwd, input);
            else
                temp = dir_concat(path[j], input);

            if (access(temp, X_OK) == 0) {
                helper->lastExit = run(temp, args, helper);
                break;
            }

            if (temp != NULL) {
                free(temp);
                temp = NULL;
            }
        }
        if (!path[j])
            retval = 127;
    }

    free(cwd);
    free(paths);
    
    if (temp)
        free(temp);

    return (retval);
}

/**
 * free_args - free a two-dimensional array
 *
 * @args: pointer to array of arguments
 * @size: size of environmental variable array
 * Return: void
 */
void free_args(char **args, int size)
{
    int i;

    for (i = 0; i < size; i++)
            free(args[i]);
    free(args);
}

/**
 * get_args - create a two-dimensional array; last argument set to null terminator
 *
 * @tok: tokenized input
 * @argv: argv for main
 * @save: save pointer for arguments
 * Return: pointer to array of arguments
 */
char **get_args(char *tok, char *argv[], char *save)
{
    char **args;
    char *arg;
    char delim = ' ';
    int i;

    (void) argv;

    arg = split_str(NULL, &delim, &save);
    args = malloc_special(sizeof(char *) * 1000, NULL); /* value of 1000 is arbitrary */
    args[0] = tok;
    i = 1;

    while (arg) {
        if (arg[0] != '\0')
            args[i++] = arg;
        arg = split_str(NULL, &delim, &save);
    }
    args[i] = NULL;

    return (args);
}
