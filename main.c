#include "shell.h"

/**
 * main - entry point for my_shell program
 *
 * @argc: argument count
 * @argv: arguments passed
 * @envp: pointer to array of environmental variables
 * Return: always 0
 */
int main(int argc, char *argv[], char *envp[])
{
    helper_p helper;
    char *save, *tok, *input, *buf;
    char **args;
    char delim = ' ';

    helper = setup_main(argc, argv, envp);
    signal(SIGINT, sighandler);

    while (1){
        while ((*helper->total) <= 0){
            if (argc == 1 && helper->type == 1)
                prompt();
            buf = get_line(helper->file, helper);
        }
        
        input = malloc_special(*helper->bufsize, helper);
        input = _memcpy(input, buf, (*helper->bufsize));
        (helper->inphead) = input;
        (helper->bufhead) = buf;
        save = NULL;
        args = NULL;
        (*helper->last) = 0;

        while (input != NULL){
            line_count(buf, helper);
            input = parse_alias(input, helper);
            input = parse_dollar(input, helper);
            tok = split_str(input, &delim, &save);
            helper->args = get_args(tok, args, save);
            if (check_builtins(input, helper, helper->args) == 1){
                if (check_local(input, helper, helper->args) == 1){
                    helper->lastExit = check_path(tok, helper->args, helper);
                    if (helper->lastExit != 0){
                        _puts(tok);
                        _puts(": command not found.\n");
                    }
                }
            }

            if (input != NULL){
                free(helper->args);
                helper->args = NULL;
            }

            input = line_check(helper, input);
            save = NULL;
        }

        if (input == NULL && (argc == 2 || helper->type == 0))
            exit_builtin("0", NULL, helper);
    }
}

/**
 * setup_main - populates helper struct; initialize the hist and env struct, grab the parent pid, and determine location
 *
 * @argc: arugment count
 * @argv: arguements passed
 * @envp: pointer to list of environmental variables
 * Return: pointer to helper struct
 */
helper_p setup_main(int argc, char **argv, char **envp)
{
    char *pid;
    env_p head;
    hist_p hist_head;
    helper_p helper;
    alias_p alias;

    pid = _getpid();
    hist_head = NULL;
    head = NULL;
    alias = NULL;

    if (argc > 2 || argv == NULL || envp == NULL){
        _puts("Please run with no argument, or filename to run from script.");
        _exit(9);
    }

    init_env(envp, &head);
    hist_head = pull_hist(&hist_head, head);
    helper = init_helper(head, hist_head, pid);
    helper->alias = alias;

    if (argc == 1)
        helper->file = STDIN_FILENO;
    else if (argc == 2){
        helper->file = open(argv[1], O_RDONLY);
        if (helper->file == -1){
            _puts("Error opening script file: ");
            _puts(argv[1]);
            _putchar('\n');
            _exit(9);
        }
    }

    helper->type = get_type(helper->file);

    return (helper);
}

/**
 * get_type - checks if file is in terminal, or if file is has a pipe
 *
 * @file: file being evaluated
 * Return: 1 if in terminal, 0 if pipe, -1 otherwise
 */
int get_type(int file)
{
    struct stat st;

    fstat(file, &st);
    if (S_ISCHR(st.st_mode))
        return (1);
    else if (S_ISFIFO(st.st_mode))
        return (0);

    return (-1);
}

/**
 * init_helper - fills and intialize values of the helper struct
 *
 * @env: pointer to head of the env variables
 * @hist:  pointer to head of hist list
 * @pid: the pid of the parents process
 * Return: pointer to helper struct
 */
helper_p init_helper(env_p env, hist_p hist, char *pid)
{
    helper_p helper;

    helper = malloc_special(sizeof(helper_t), NULL);
    helper->env = env;
    helper->hist_head = hist;
    helper->printed = malloc_special(sizeof(int) * 1, NULL);
    *(helper->printed) = 0;
    helper->total = malloc_special(sizeof(long) * 1, NULL);
    *(helper->total) = 0;
    helper->bufsize = malloc_special(sizeof(int) * 1, NULL);
    *(helper->bufsize) = 1024;
    helper->last = malloc_special(sizeof(int) * 1, NULL);
    *(helper->last) = 0;
    helper->args = NULL;
    helper->pid = pid;
    helper->lastExit = 0;
    helper->alias = NULL;

    return (helper);
}

/**
 * run - forks program and runs it on the child
 *
 * @name: name of program (including whole path)
 * @argv: pointer to array of arguments
 * @helper: pointer to the helper struct
 * Return: returns -1 if fail, or the exit status of the child
 */
int run(char *name, char *argv[], helper_p helper)
{
    pid_t cpid;
    int cstatus, size;
    char **envs;

    envs = build_env(helper->env, &size);
    cpid = fork();
    if (cpid == -1){
        _puts("Failure while forking\n");
        exit(9);
    }
    else if (cpid == 0){
        execve(name, argv, envs);
        _puts("Attempted to run unknown command: ");
        _puts(name);
        _putchar('\n');
        exit(-1);
    } else {
        wait(&cstatus);
        free_args(envs, size);
        return (cstatus);
    }
}

/**
 * sighandler - the signal handler
 *
 * @signum: signal number corresponding to the signal
 * Return: void
 */
void sighandler(int signum)
{
    (void) signum;
    write(0, "\n", 1);
    prompt();
}
