#include "header.h"

int    status;
pid_t  pid;
char  *global_prompt;
char   current_command[MAX_CMD_LEN] = "";

char *get_command(char *input_string)
{
    int len = 0;

    while (input_string[len] != ' ' && input_string[len] != '\0')
    {
        len++;
    }

    char *cmd = malloc(len + 1);
    if (cmd == NULL)
    {
        perror("malloc");
        exit(1);
    }

    memcpy(cmd, input_string, len);
    cmd[len] = '\0';

    return cmd;
}

int check_command_type(char *command)
{
    int i;

    for (i = 0; builtins[i] != NULL; i++)
    {
        if (strcmp(command, builtins[i]) == 0)
        {
            return BUILTIN;
        }
    }

    for (i = 0; external_commands[i] != NULL; i++)
    {
        if (strcmp(command, external_commands[i]) == 0)
        {
            return EXTERNAL;
        }
    }

    return NO_COMMAND;
}

void execute_internal_commands(char *input_string)
{
    char *cmd = get_command(input_string);

    if (strcmp(cmd, "jobs") == 0)
    {
        print_jobs();
    }
    else if (strcmp(cmd, "fg") == 0)
    {
        job *last = get_last_job();

        if (last == NULL)
        {
            printf("fg: no current job\n");
        }
        else
        {
            pid_t fg_pid = last->pid;

            printf("%s\n", last->command);

            strncpy(current_command, last->command, MAX_CMD_LEN - 1);
            current_command[MAX_CMD_LEN - 1] = '\0';

            kill(fg_pid, SIGCONT);

            pid = fg_pid;
            waitpid(fg_pid, &status, WUNTRACED);

            if (WIFSTOPPED(status))
            {
                strncpy(last->status, "Stopped", sizeof(last->status) - 1);
                printf("\n[%d]+  %-22s %s\n", last->job_number, "Stopped", last->command);
            }
            else
            {
                delete_job(fg_pid);
            }

            pid = 0;
        }
    }
    else if (strcmp(cmd, "bg") == 0)
    {
        job *last = get_last_job();

        if (last == NULL)
        {
            printf("bg: no current job\n");
        }
        else
        {
            printf("[%d]+ %s &\n", last->job_number, last->command);
            kill(last->pid, SIGCONT);
            strncpy(last->status, "Running", sizeof(last->status) - 1);
        }
    }
    else if (strcmp(cmd, "exit") == 0)
    {
        free(cmd);
        exit(0);
    }
    else if (strcmp(cmd, "pwd") == 0)
    {
        char cwd[250];

        if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s\n", cwd);
        }
        else
        {
            perror("pwd");
        }
    }
    else if (strcmp(cmd, "cd") == 0)
    {
        char *path = input_string + 2;

        while (*path == ' ')
        {
            path++;
        }

        if (*path == '\0')
        {
            path = getenv("HOME");
        }

        if (path == NULL || chdir(path) != 0)
        {
            perror("cd");
        }
    }
    else if (strcmp(input_string, "echo $$") == 0)
    {
        printf("%d\n", getpid());
    }
    else if (strcmp(input_string, "echo $?") == 0)
    {
        if (WIFEXITED(status))
        {
            printf("%d\n", WEXITSTATUS(status));
        }
    }
    else if (strcmp(input_string, "echo $SHELL") == 0)
    {
        char *shell_env = getenv("SHELL");
        printf("%s\n", shell_env != NULL ? shell_env : "");
    }

    free(cmd);
}

void copy_change(char *prompt, char *input_string)
{
    int i;

    for (i = 0; input_string[i] != '\0'; i++)
    {
        if (input_string[i] == ' ')
        {
            printf("PS1: command not found\n");
            return;
        }
    }

    if (strlen(input_string + 4) >= PROMPT_LEN)
    {
        printf("PS1: value too long\n");
        return;
    }

    strcpy(prompt, input_string + 4);
}

void scan_input(char *prompt, char *input_string)
{
    global_prompt = prompt;

    while (1)
    {
        char cwd[250];

        if (getcwd(cwd, sizeof(cwd)) == NULL)
        {
            perror("getcwd");
            strcpy(cwd, "?");
        }
        printf(ANSI_COLOR_GREEN "%s:" ANSI_COLOR_RESET
               ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET
               ANSI_COLOR_WHITE "$ " ANSI_COLOR_RESET,
               global_prompt, cwd);
        // printf(ANSI_COLOR_GREEN "%s:" ANSI_COLOR_RESET

        //        global_prompt);

        scanf(" %[^\n]", input_string);
        if (strncmp(input_string, "PS1=", 4) == 0)
        {
            copy_change(prompt, input_string);
            continue;
        }

        char *cmd = get_command(input_string);
        int type = check_command_type(cmd);
        free(cmd);

        if (type == BUILTIN)
        {
            execute_internal_commands(input_string);
        }
        else if (type == EXTERNAL)
        {
            strncpy(current_command, input_string, MAX_CMD_LEN - 1);
            current_command[MAX_CMD_LEN - 1] = '\0';

            pid = fork();

            if (pid == 0)
            {
                signal(SIGINT, SIG_DFL);
                signal(SIGTSTP, SIG_DFL);
                execute_external_commands(input_string);
            }
            else if (pid > 0)
            {
                waitpid(pid, &status, WUNTRACED);
                pid = 0;
            }
            else
            {
                perror("fork");
            }
        }
        else
        {
            printf("%s: command not found\n", input_string);
        }
    }
}

void signal_handler(int sig_num)
{
    if (sig_num == SIGINT)
    {
        if (pid == 0)
        {
            printf("\n%s", global_prompt);
            fflush(stdout);
        }
    }
    else if (sig_num == SIGTSTP)
    {
        if (pid > 0)
        {
            add_job(pid, current_command, "Stopped");
            printf("\n[%d]+  %-22s %s\n", job_count, "Stopped", current_command);
            fflush(stdout);
        }
    }
    else if (sig_num == SIGCHLD)
    {
        pid_t finished_pid = waitpid(-1, &status, WNOHANG);

        if (finished_pid > 0)
        {
            delete_job(finished_pid);
        }
    }
}
