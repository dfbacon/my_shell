#ifndef __SHELL_H__
#define __SHELL_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFSIZE 1024
#define ARGS 256
#define PATHSIZE 10
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define HIST_FILE ".my_shell_history"

/**
 * struct env_s - struct to store environmental variables
 *
 * @name: environment variable name
 * @value: environment variable value
 * @next: the pointer to the next node
 */
typedef struct env_s{
    char *name;
    char *value;
    struct env_s *next;
} env_t, *env_p;

/**
 * struct alias_s - struct to store aliases
 *
 * @name: alias name
 * @value: alias value
 * @next: pointer to the next node
 */
typedef struct alias_s{
    char *name;
    char *value;
    struct alias_s *next;
} alias_t, *alias_p;

/**
 * struct hist_s - struct to store user's session history
 *
 * @entry: pointer user's session
 * @next: pointer to next node
 */
typedef struct hist_s{
    char *entry;
    struct hist_s *next;
} hist_t, *hist_p;

/**
 * struct helper_s - a helper struct
 *
 * @bufhead: ptr to the start of the buffer
 * @hist_head: head to the hist linked list
 * @env: head to the env hist linke list
 * @total: the total bytes read from get_line
 * @bufsize: the bufsize
 * @printed: the number of bytes that have printed
 * @last: holds the index for the start of the next line
 * @pid: the pid
 * @lastExit: the status of a fork
 * @file: holds the value of the file descriptor
 * @type: if file is in terminal or a pipe
 * @linecount: current linecount
 * @bufnext: pointer for next location in the buffer
 * @inphead: pointer to the head of the input list
 * @args: arguments, tokenized
 * @alias: alias list head
 *
 * Thanks and credit to Tim Britton for this.
 */
typedef struct helper_s{
    alias_t *alias;
    int linecount;
    char **args;
    char *bufnext;
    char *inphead;
    char *bufhead;
    hist_t *hist_head;
    env_t *env;
    long *total;
    int *bufsize;
    int *printed;
    int *last;
    char *pid;
    int lastExit;
    int file;
    int type;
} helper_t, *helper_p;

/* get_line.c */
char *get_line(int file, helper_p helper);
char *line_check(helper_p helper, char *input);
char *parse_delimiters(char *buf, helper_p helper);
void *buf_count(char *buf, helper_p helper);
void line_count(char *buf, helper_p helper);

/* builtins.c */
void exit_builtin(char *tok, char *input, helper_p helper);
int list_env(env_p *environ);
int cd_builtin(char **args, helper_p helper);
int unset_env(char *name, env_p *head);
env_p set_env(char *name, char *value, env_t *head);

/* path.c */
int check_builtins(char *tok, helper_p helper, char **args);
int check_local(char *tok, helper_p helper, char **args);
int check_path(char *input, char **args, helper_p helper);
void free_args(char **args, int size);
char **get_args(char *tok, char *argv[], char *save);

/* mem.c */
char *_memcpy(char *dest, char *src, unsigned int n);
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size);
void *malloc_special(unsigned int b, helper_p helper);
char *_memset(char *s, char b, unsigned int);

/* parsing.c */
char *parse_dollar(char *buf, helper_p helper);
char *whitespace(char *buf, helper_p helper);
char *parse_comments(char *buf, helper_p helper);
char *del_buffer(char *buf, helper_p helper, int index, int times);
int is_delimiter(char c);
int is_whitespace(char c);
char *_getpid(void);
char *insert_env(char *a, char *value, helper_p helper, int, char *name);
char *insert_pid(char *buf, helper_p helper, int start);
char *insert_exit_status(char *buf, helper_p helper, int start);

/* buffer_manipulation.c */
char *in_strcat(char *buf, char *string, int *bufsize, int insert);
char *slice_string(char *buf, int *bufsize, int slicesize, int index);

/* env.c */
env_p init_env(char **environ, env_p *head);
char **build_env(env_p head, int *size);
void free_list(env_p head);
env_p add_env(env_p *head, char *name, char *value);
env_p get_env(char *name, env_p head);

/* main.c */
helper_p init_helper(env_t *env, hist_p hist, char *pid);
int run(char *name, char *argv[], helper_p helper);
int get_type(int file);
helper_p setup_main(int argc, char **argv, char **envp);
void sighandler(int signum);

/* strings.c */
int _strlen(char *s);
char *_strcpy(char *dest, char *src);
char *_strdup(char *str);
char *str_concat(char *s1, char *s2);
int _strcmp(char *s1, char *s2);
char *_strcat(char *dest, char *src);
char *split_str(char *str, const char *delim, char **saveptr);
int all_strcmp(char *s1, char *s2);
char *dir_concat(char *s1, char *s2);
int _atoi(char *s);

/* io.c */
void prompt(void);
void _puts(char *str);
int _putchar(char c);
void _putint(int i, int flag);

/* math.c */
int itoa(int n, char *s);
int _abs(int num);

/* history.c */
hist_p add_hist(int total, hist_p *head, char *buf);
void clear_hist(hist_p *head);
void print_hist(hist_p head);
void push_hist(hist_p h_head, env_p e_head);
hist_p pull_hist(hist_p *h_head, env_p e_head);

/* help.c */
int help_builtin(char *file);
void help_exit(void);
void help_env(void);
void help_set_env(void);
void help_unset_env(void);
void help_cd(void);
void help_help(void);
void help_alias(void);
void help_history(void);

/* aliases.c */
void alias_builtin(char **args, helper_p helper);
int list_alias(helper_p helper);
alias_p set_alias(char **args, helper_p helper, alias_p *head);
alias_p get_alias(char *name, alias_p head);
void free_alist(alias_p head);
int remove_alias(char *name, alias_p *head);
char *parse_alias(char *buf, helper_p helper);


#endif /* __SHELL_H__ */
