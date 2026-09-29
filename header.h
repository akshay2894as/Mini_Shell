#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUILTIN     1
#define EXTERNAL    2
#define NO_COMMAND  0

#define MAX_JOBS              64
#define MAX_CMD_LEN           100
#define MAX_INPUT_LEN         256
#define PROMPT_LEN            25
#define MAX_EXTERNAL_COMMANDS 200

#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_WHITE   "\x1b[37m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct job
{
    int   job_number;
    pid_t pid;
    char  command[MAX_CMD_LEN];
    char  status[20];
} job;

/* ---- globals shared across files ---- */
extern int    status;
extern pid_t  pid;
extern char  *global_prompt;
extern char   current_command[MAX_CMD_LEN];

extern char  *builtins[];
extern char  *external_commands[];

extern job job_list[MAX_JOBS];
extern int num_jobs;
extern int job_count;

/* ---- external_commands.c ---- */
void extract_external_commands(char **external_commands);
void execute_external_commands(char *input_string);

/* ---- jobs.c ---- */
void add_job(pid_t pid, const char *command, const char *status_str);
void delete_job(pid_t pid);
job *get_last_job(void);
void print_jobs(void);

/* ---- shell.c ---- */
char *get_command(char *input_string);
int   check_command_type(char *command);
void  execute_internal_commands(char *input_string);
void  copy_change(char *prompt, char *input_string);
void  scan_input(char *prompt, char *input_string);
void  signal_handler(int sig_num);

#endif