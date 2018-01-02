#include "shell.h"


/**
 * init_env - turns the list of environmental variables into a linked list
 *
 * @environ: pointer to array of environmental variables
 * @head: pointer to head of environmental variable list
 * Return: pointer to head of linked list
 */
env_t *init_env(char **environ, env_p *head)
{
    env_p newnode, tempnode;
    char *saveptr, *tok, *temp;
    char equals = '=';
    char newline = '\n';
    int i;

    i = 0;
    while (environ[i]) {
        newnode = malloc_special(sizeof(env_t), NULL);
        if (!newnode)
            return NULL;

        temp = _strdup(environ[i]);
        tok = split_str(temp, &equals, &saveptr);
        if (tok)
            newnode->name = _strdup(tok);

        tok = split_str(NULL, &newline, &saveptr);
        if (tok)
            newnode->value = _strdup(tok);
        free(temp);
        if (*head == NULL) {
            newnode->next = *head;
            *head = newnode;
        } else {
            newnode->next = NULL;
            tempnode = *head;
            while (tempnode->next)
                tempnode = tempnode->next;
            tempnode->next = newnode;
        }
        i++;
    }

    return *head;
}

/**
 * build_env - build two-dimensional array from environmental variable linked list
 *
 * @head: pointer to head of environmental variable linked list
 * @size: size of @head list
 * Return: pointer to new array
 */
char **build_env(env_p head, int *size)
{
    env_p temp;
    char **envs;
    char *env, *env2;
    int i;

    temp = head;
    i = 0;
    while (temp) {
        i++;
        temp = temp->next;
    }

    *size = i;
    temp = head;
    envs = malloc_special(sizeof(char *) * (i + 1), NULL);
   
    i = 0;
    while (temp) {
        env = _strdup(temp->name);
        env2 = str_concat(env, "=");
        free(env);
        env = str_concat(env2, temp->value);
        envs[i] = _strdup(env);
        free(env);
        free(env2);
        temp = temp->next;
        i++;
    }
    envs[i] = NULL;
    
    return envs;
}

/**
 * free_list - frees a linked list
 *
 * @head: pointer to head of linked list
 * Return: void
 */
void free_list(env_p head)
{
    env_p temp;

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
 * add_env - adds node to end of a linked list
 *
 * @head: pointer to head of linked list
 * @name: name of new environmental variable
 * @value: value for @name
 * Return: pointer to the new node, or NULL
 */
env_p add_env(env_p *head, char *name, char *value)
{
    env_p newnode, tempnode;

    if (!head || !name || !value)
        return NULL;

    newnode = malloc_special(sizeof(env_t), NULL);
    if (!newnode)
        return NULL;

    newnode->name = _strdup(name);
    newnode->value = _strdup(value);
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

    return newnode;
}

/**
 * get_env - locates an environmental variable
 *
 * @name: name of environment variable to search for
 * @head: pointer to head of environmental variable linked list
 * Return: pointer to @name, or NULL
 */
env_p get_env(char *name, env_p head)
{
    env_p environ;

    environ = head;
    while (environ) {
        if (all_strcmp(name, environ->name) == 0)
            return environ;
        environ = environ->next;
    }

    return NULL;
}
