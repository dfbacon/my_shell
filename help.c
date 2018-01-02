#include "shell.h"


/**
 * help_builtin - finds the desired help page of the builtin
 *
 * @file: a string of the desired help page
 * Return: 1 if page found, -1 otherwise
 */
int help_builtin(char *file)
{
    if (!file || file[0] == '\0') {
        _puts("Select a built-in: alias, cd, env, exit, help, history, setenv, unsetenv\n");
        return (-1);
    }

    if (_strcmp(file, "exit") == 0)
        help_exit();
    else if (_strcmp(file, "env") == 0)
        help_env();
    else if (_strcmp(file, "setenv") == 0)
        help_set_env();
    else if(_strcmp(file, "unsetenv") == 0)
        help_unset_env();
    else if (_strcmp(file, "cd") == 0)
        help_cd();
    else if (_strcmp(file, "help") == 0)
        help_help();
    else if (_strcmp(file, "alias") == 0)
        help_alias();
    else if (_strcmp(file, "history") == 0)
        help_history();
    else {
        _puts("Not a built-in of my_shell.\n");
        return (-1);
    }
    return (1);
}

/**
 * help_exit - prints the exit help page.
 *
 * Return: void
 */
void help_exit(void)
{
    _puts("exit: exit [n]\n\n");
    _puts("  Exit the shell.\n\n");
    _puts("  Exits the shell with a status of N. If N is omitted,\n");
    _puts("  the exit status is that of the last command executed\n");
}

/**
 * help_env - prints the env help page.
 *
 * Return: void
 */
void help_env(void)
{
    _puts("env: env  [-] [NAME=VALUE]... [COMMAND [ARG]...]\n\n");
    _puts("  Prints out the current environment\n\n");
    _puts("  Exit Status:\n");
    _puts("  Returns success unless an invalid option is given\n");
    _puts("  or an error occurs.\n");
}

/**
 * help_set_env - prints the set_env help page
 *
 * Return: void
 */
void help_set_env(void)
{
    _puts("set_env: set_env [VARIABLE] [VALUE]/\n\n");

    _puts("  The set_env() built-in adds the variable name to the\n");
    _puts("  environment with the value value, if name does not\n");
    _puts("  already exist. If name does exist in the environment\n");
    _puts("  , then its value is changed to value if overwrite is\n");
    _puts("  nonzero; if overwrite is zero, then the value of name\n");
    _puts("  is not changed. This function makes copies of the\n");
    _puts("  strings pointed to by name and value.\n\n");
    _puts("  VARIABLE:\n");
    _puts("    VARIABLE: the name of the variable to be set\n");
    _puts("    VALUE: the value of the variable as either a single\n");
    _puts("           word or a quoted string\n\n");

    _puts("  Exit Status:\n");
    _puts("    The set_env builtin returns zero on success, or -1\n");
    _puts("    on error, with errno set to indicate the cause of\n");
    _puts("    the error.\n");
}

/**
 * help_unset_env - prints the unset_env help page
 *
 * Return: void
 */
void help_unset_env(void)
{
    _puts("unset_env: unset_env [VARIABLE]\n\n");

    _puts("  The unset_env() built-in deletes the variable name\n");
    _puts("  from the environment. If name does not exist in the\n");
    _puts("  environment, then the functionsucceeds, and the\n");
    _puts("  environment is unchanged.\n\n");
    _puts("  VARIABLE:\n");
    _puts("    VARIABLE: the name of the variable to be set]\n\n");
    _puts("  Exit Status:\n");
    _puts("    The unsetenv builtin returns 1 on success, or -1 on\n");
    _puts("    error, with errno set to indicate the cause of the\n");
    _puts("    error.\n");
}

/**
 * help_cd - prints the cd help page
 *
 * Return: void
 */
void help_cd(void)
{
    _puts("help: help [PATTERN ...]\n\n");

    _puts("  Display information about builtin commands. Displays\n");
    _puts("  brief summaries of builtin commands. If PATTERN is\n");
    _puts("  specified, gives detailed help on all commands\n");
    _puts("  matching PATTERN, otherwise the list of help topics\n");
    _puts("  is printed.\n\n");
    _puts("  Arguments:\n");
    _puts("    PATTERN: Pattern specifiying a help topic\n\n");
    _puts("  Exit Status:\n");
    _puts("    Returns success unless PATTERN is not found or an\n");
    _puts("    invalid option is given.\n");
}

/**
 * help_help - prints the 'help' help page
 *
 * Return: void
 */
void help_help(void)
{
    _puts("help: help [PATTERN ...]\n\n");

    _puts("  Display information about builtin commands.\n\n");
    _puts("  Displays brief summaries of builtin commands. If\n");
    _puts("  PATTERN is specified, gives detailed help on all\n");
    _puts("  commands matching PATTERN, otherwise the list of help\n");
    _puts("  topics is printed.\n\n");
    _puts("  Arguments:\n");
    _puts("    PATTERN: Pattern specifiying a help topic\n\n");
    _puts("  Exit Status:\n");
    _puts("    Returns success unless PATTERN is not found or an\n");
    _puts("    invalid option is given.\n");
}

/**
 * help_alias - prints the alias help
 *
 * Return: void
 */
void help_alias(void)
{
    _puts("alias: alias [name[=value] ... ]\n\n");

    _puts("  Define or display aliases.\n\n");
    _puts("  Without arguments, `alias' prints the list of aliases\n");
    _puts("  in the reusable form `alias NAME=VALUE' on standard\n");
    _puts("  output. Otherwise, an alias is defined for each NAME\n");
    _puts("  whose VALUE is given. A trailing space in VALUE\n");
    _puts("  causes the next word to be checked for alias\n");
    _puts("  substitution when the alias is expanded.\n\n");
    _puts("  Exit Status:\n");
    _puts("    alias returns true unless a NAME is supplied for\n");
    _puts("    which no alias has been defined.\n");
}

/**
 * help_history - prints the history help
 *
 * Return: void
 */
void help_history(void)
{
    _puts("history: history\n\n");

    _puts("  Display the history list with line numbers.\n\n");
    _puts("  Exit Status:\n");
    _puts("    Returns success unless an invalid option is given\n");
    _puts("    or an error occurs.\n");
}
