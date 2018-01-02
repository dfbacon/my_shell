#include "shell.h"


/**
 * add_hist - add new entry to history linked list
 *
 * @total: size of the num of bytes to copy
 * @head: pointer to the head of history linked list
 * @buf: pointer to the buffer to be copied
 *
 * Return: pointer to new head of history linked list
 */
hist_p add_hist(int total, hist_p *head, char *buf)
{
    hist_p new_hist, current;
    int i;

    if (buf[0] == ' ')
        return NULL;

    new_hist = malloc_special(sizeof(hist_t), NULL);
    if (new_hist == NULL)
        return NULL;

    new_hist->entry = malloc_special(sizeof(char) * total, NULL);
    _memcpy(new_hist->entry, buf, total);

    for (i = 0; i <= total - 1; i++) {
        if ((new_hist->entry)[i] == '\n')
            (new_hist->entry)[i] = '\0';
    }
    new_hist->next = NULL;

    if (*head == NULL)
            *head = new_hist;
    else {
        current = *head;
        while (current->next)
            current = current->next;
        current->next = new_hist;
    }

    return (*head);
}

/**
 * clear_hist - clear the history linked list
 *
 * @head: pointer to head of the history linked list
 * Return: void
 */
void clear_hist(hist_p *head)
{
    hist_t *hold;

    while (*head) {
        hold = *head;
        (*head) = (*head)->next;
        free(hold->entry);
        free(hold);
    }
}

/**
 * print_hist - prints out the history linked list
 *
 * @head: pointer to the head of the hist linked list
 * Return: void
 */
void print_hist(hist_p head)
{
    int i;

    i = 0;
    while (head) {
        _putint(i, 0);
        _putchar(' ');
        _puts(head->entry);
        _putchar('\n');
        head = head->next;
        i++;
    }
}

/**
 * push_hist - push history from current sessions into HIST_FILE
 *
 * @h_head: pointer to head of the history link list
 * @e_head: the head to the environment linked list
 * Return: void
 */
void push_hist(hist_p h_head, env_p e_head)
{
    mode_t mode;
    int file, err_w, err_c;
    env_p env_var;
    char *home;

    if (!h_head || !e_head)
        exit(100);

    env_var = get_env("HOME", e_head);
    home = env_var->value;
    home = dir_concat(home, HIST_FILE);
    mode = S_IRUSR | S_IWUSR;
    file = open(home, O_CREAT | O_WRONLY | O_TRUNC, mode);
    if (file == -1) {
        perror("Error opening file: ");
        free(home);
        exit(101);
    }

    while (h_head) {
        if (h_head->entry) {
            err_w = write(file, h_head->entry, _strlen(h_head->entry));
            if (err_w == -1 || err_w != _strlen(h_head->entry)) {
                free(home);
                exit(102);
            }
            
            err_w = write(file, "\n", 1);
            if (err_w == -1 || err_w != 1) {
                free(home);
                exit(103);
            }
        }
        h_head = h_head->next;
    }

    err_c = close(file);
    if (err_c == -1) {
        free(home);
        exit(104);
    }

    free(home);
}

/**
 * pull_hist - pulls entries from history and adds it to history linked list.
 *
 * @h_head: pointer to head of history linked list
 * @e_head: pointer to head of enviroment linked list
 *
 * Return: pointer to head of history list
 */
hist_p pull_hist(hist_p *h_head, env_p e_head)
{
    char delim;
    char *home, *hist_line, *saveptr, *buf;
    env_p env_var;
    hist_p current_node;
    int file, err_c, err_r, buf_len, i;
    struct stat st;

    env_var = get_env("HOME", e_head);
    home = env_var->value;
    home = dir_concat(home, HIST_FILE);
    file = open(home, O_RDONLY, 0744);
    if (file == -1)
        return (*h_head);
    if (stat(home, &st) == 0)
        buf_len = st.st_size;
    else {
        _puts("Failed to get count of hist_file");
        exit(301);
    }

    buf = malloc_special(sizeof(char) * buf_len, NULL);
    err_r = read(file, buf, buf_len);
    if (err_r == -1)
        _puts("unable to read hist_file");

    buf[buf_len - 1] = '\0';
    delim = '\n';
    i = 1;
    hist_line = split_str(buf, &delim, &saveptr);
    while (hist_line) {
            add_hist(_strlen(hist_line) + 1, h_head, hist_line);
            if (i > 4096) {
                    current_node = *h_head;
                    *h_head = (*h_head)->next;
                    free(current_node->entry);
                    free(current_node);
            }

            hist_line = split_str(NULL, &delim, &saveptr);
            i++;
    }
    err_c = close(file);
    if (err_c == -1) {
        _puts("fail to close hist_file");
        exit(302);
    }
    free(buf);
    free(home);

    return (*h_head);
}
