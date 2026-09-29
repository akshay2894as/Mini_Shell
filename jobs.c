#include "header.h"

job job_list[MAX_JOBS];
int num_jobs = 0;
int job_count = 0;

void add_job(pid_t pid, const char *command, const char *status_str)
{
    if (num_jobs >= MAX_JOBS)
    {
        fprintf(stderr, "job table full\n");
        return;
    }

    job_count++;

    job_list[num_jobs].job_number = job_count;
    job_list[num_jobs].pid = pid;

    strncpy(job_list[num_jobs].command, command, MAX_CMD_LEN - 1);
    job_list[num_jobs].command[MAX_CMD_LEN - 1] = '\0';

    strncpy(job_list[num_jobs].status, status_str, sizeof(job_list[num_jobs].status) - 1);
    job_list[num_jobs].status[sizeof(job_list[num_jobs].status) - 1] = '\0';

    num_jobs++;
}

void delete_job(pid_t pid)
{
    int i;
    int found = -1;

    for (i = 0; i < num_jobs; i++)
    {
        if (job_list[i].pid == pid)
        {
            found = i;
            break;
        }
    }

    if (found == -1)
    {
        return;
    }

    for (i = found; i < num_jobs - 1; i++)
    {
        job_list[i] = job_list[i + 1];
    }

    num_jobs--;
}

job *get_last_job(void)
{
    if (num_jobs == 0)
    {
        return NULL;
    }

    return &job_list[num_jobs - 1];
}

void print_jobs(void)
{
    int i;

    for (i = 0; i < num_jobs; i++)
    {
        char marker = (i == num_jobs - 1) ? '+' : '-';
        printf("[%d]%c  %-22s %s\n", job_list[i].job_number, marker,
               job_list[i].status, job_list[i].command);
    }
}