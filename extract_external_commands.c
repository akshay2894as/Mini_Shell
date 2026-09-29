#include "header.h"

void extract_external_commands(char **external_commands)
{
    int fd;
    char ch;
    int count = 0;
    int i = 0;
    int j = 0;

    fd = open("ext_commands.txt", O_RDONLY);

    if (fd == -1)
    {
        perror("open");
        external_commands[0] = NULL;
        return;
    }

    /* FIRST PASS: count characters per line (ignore '\r') */
    while (read(fd, &ch, 1) > 0)
    {
        if (ch == '\r')
        {
            continue;
        }

        if (ch == '\n')
        {
            if (i >= MAX_EXTERNAL_COMMANDS - 1)
            {
                break; /* don't overflow the fixed-size array */
            }

            external_commands[i] = malloc(count + 1);
            i++;
            count = 0;
        }
        else
        {
            count++;
        }
    }

    /* LAST LINE (no trailing newline) */
    if (count > 0 && i < MAX_EXTERNAL_COMMANDS - 1)
    {
        external_commands[i] = malloc(count + 1);
        i++;
    }

    external_commands[i] = NULL;

    /* SECOND PASS: copy characters (ignore '\r') */
    lseek(fd, 0, SEEK_SET);

    i = 0;
    j = 0;

    while (read(fd, &ch, 1) > 0 && external_commands[i] != NULL)
    {
        if (ch == '\r')
        {
            continue;
        }

        if (ch == '\n')
        {
            external_commands[i][j] = '\0';
            i++;
            j = 0;
        }
        else
        {
            external_commands[i][j] = ch;
            j++;
        }
    }

    if (external_commands[i] != NULL && j > 0)
    {
        external_commands[i][j] = '\0';
    }

    close(fd);
}

void execute_external_commands(char *input_string)
{
    char *first_cmd_args[20];
    char *second_cmd_args[20];
    int i;

    char *pipe_pos = strchr(input_string, '|');

    if (pipe_pos == NULL)
    {
        /* ---- NO PIPE: run normally ---- */
        i = 0;
        first_cmd_args[i] = strtok(input_string, " ");
        while (first_cmd_args[i] != NULL)
        {
            i++;
            first_cmd_args[i] = strtok(NULL, " ");
        }

        execvp(first_cmd_args[0], first_cmd_args);
        perror("execvp");
        exit(1);
    }
    else
    {
        /* ---- PIPE PRESENT: split into two command strings ---- */
        *pipe_pos = '\0';
        char *first_part = input_string;
        char *second_part = pipe_pos + 1;

        i = 0;
        first_cmd_args[i] = strtok(first_part, " ");
        while (first_cmd_args[i] != NULL)
        {
            i++;
            first_cmd_args[i] = strtok(NULL, " ");
        }

        i = 0;
        second_cmd_args[i] = strtok(second_part, " ");
        while (second_cmd_args[i] != NULL)
        {
            i++;
            second_cmd_args[i] = strtok(NULL, " ");
        }

        int fd[2];

        if (pipe(fd) == -1)
        {
            perror("pipe");
            exit(1);
        }

        pid_t pid1 = fork();

        if (pid1 == 0)
        {
            close(fd[0]);
            dup2(fd[1], STDOUT_FILENO);
            close(fd[1]);

            execvp(first_cmd_args[0], first_cmd_args);
            perror("execvp");
            exit(1);
        }

        pid_t pid2 = fork();

        if (pid2 == 0)
        {
            close(fd[1]);
            dup2(fd[0], STDIN_FILENO);
            close(fd[0]);

            execvp(second_cmd_args[0], second_cmd_args);
            perror("execvp");
            exit(1);
        }

        close(fd[0]);
        close(fd[1]);

        waitpid(pid1, NULL, 0);

        int second_status;
        waitpid(pid2, &second_status, 0);
        if (WIFEXITED(second_status))
        {
            exit(WEXITSTATUS(second_status));
        }

        exit(1);
    }
}