/*
    Akshay Satish Suryavanshi
    26001B
    summary: Minishell is a simple Linux shell implemented in C that supports built-in and external commands.
            It uses system calls like fork(), execvp(), waitpid(), chdir(), and signal handling to execute commands.

*/
#include "header.h"

char *external_commands[MAX_EXTERNAL_COMMANDS];

int main(void)
{
    //system call
    system("clear");

    char prompt[PROMPT_LEN] = "minishell:~$";
    char input_string[MAX_INPUT_LEN];

    signal(SIGINT, signal_handler);
    signal(SIGTSTP, signal_handler);
    signal(SIGCHLD, signal_handler);

    extract_external_commands(external_commands);

    scan_input(prompt, input_string);

    return 0;
}