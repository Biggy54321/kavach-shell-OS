#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "jobs.h"
#include "prompt.h"

/* Maximum number of jobs supported by the shell */
#define MAX_NB_OF_JOBS  (128u)

/* Global array of jobs */
job_t g_jobs[MAX_NB_OF_JOBS];
/* Global count of number of jobs */
int g_nb_jobs;

/**
 * @brief Get the index of the job which contains the specified pid
 * @param[in] pid Process id to be searched
 * @return Index in the #g_jobs array which contains #pid
 */
static int __get_idx_from_pid(int pid) {

    int job_i;

    int pid_i;

    /* For each job */
    for (job_i = 0; job_i < g_nb_jobs; job_i++) {

        /* For each pid it contains */
        for (pid_i = 0; pid_i < g_jobs[job_i].nb_pids; pid_i++) {

            /* If the required pid is found */
            if (g_jobs[job_i].pids[pid_i] == pid) {

                /* Return the group pid */
                return job_i;
            }
        }
    }

    return -1;
}

/**
 * @brief Get the index of the job which has the specified gpid
 * @param[in] pid Process group id to be searched
 * @return Index in the #g_jobs array which has #gpid
 */
static int __get_idx_from_gpid(int gpid) {

    int job_i;

    /* For every job */
    for (job_i = 0; job_i < g_nb_jobs; job_i++) {

        /* If the requested gpid matched the current job's gpid */
        if (g_jobs[job_i].gpid == gpid) {

            /* Return group pid */
            return job_i;
        }
    }

    return -1;
}

/**
 * @brief Waits for the child so that the PCB entry for that child is removed
 * @param[in] sig_num Signal number
 */
static void __sigchld_handler(int sig_num) {

    int pid;
    int idx;
    int nb_procs_comp;

    /* Wait for any child, but do not halt if no child's state has changed */
    pid = waitpid(WAIT_ANY, 0, WNOHANG);

    /* Mark the pid as complete */
    jobs_mark_proc_comp(pid, true);
}

/**
 * @brief Initialize the jobs global variables
 */
void jobs_init() {

    /* Set the number of jobs to zero */
    g_nb_jobs = 0;
}

/**
 * @brief Initialize the signal handlers for the executor
 */
void jobs_signal_init() {

    /* Initialize the SIGINT handler */
    signal(SIGINT, SIG_IGN);

    /* Initialize the SIGTSTP handler */
    signal(SIGTSTP, SIG_IGN);

    /* Initialize the SIGTTOU handler to be ignored */
    signal(SIGTTOU, SIG_IGN);

    /* Initialize the SIGCHLD handler */
    signal(SIGCHLD, __sigchld_handler);
}

/**
 * @brief Reset the signal handlers, to their default functions
 */
void jobs_signal_deinit() {

    /* Reset SIGINT handler */
    signal(SIGINT, SIG_DFL);

    /* Reset SIGTSTP handler */
    signal(SIGTSTP, SIG_DFL);

    /* Reset SIGTTOU handler */
    signal(SIGTTOU, SIG_DFL);

    /* Reset the SIGCHLD handler */
    signal(SIGCHLD, __sigchld_handler);
}

/**
 * @brief Creates a new job for the specified process group
 * @param[in] Process group id
 * @param[in] cmd_tab Command table for the group
 */
void jobs_add_proc_grp(int gpid, cmd_tab_t *p_cmd_tab) {

    /* Initialize a new job for the new process group */
    g_jobs[g_nb_jobs].gpid = gpid;

    /* Initialize the command table for the process group */
    /* g_jobs[g_nb_jobs].cmd_tab = *p_cmd_tab; */
    cmd_tab_copy(&g_jobs[g_nb_jobs].cmd_tab, p_cmd_tab);

    /* Initialize the number of processes currently in the group */
    g_jobs[g_nb_jobs].nb_pids = 0;

    /* Initialize the number of processes completed */
    g_jobs[g_nb_jobs].nb_procs_comp = 0;

    /* Increment the number of jobs */
    g_nb_jobs++;
}

/**
 * @brief Adds the process to the specified process group
 * @param[in] gpid Process group id
 * @param[in] pid Process id
 */
void jobs_add_proc(int gpid, int pid) {

    int idx;

    /* Get the index from the group id */
    idx = __get_idx_from_gpid(gpid);

    /* If gpid is not found */
    if (idx == -1) {

        return;
    }

    /* Add the process to the process' list */
    g_jobs[idx].pids[g_jobs[idx].nb_pids] = pid;

    /* Increment the nubmer of pids in the process' list */
    g_jobs[idx].nb_pids++;
}

/**
 * @brief Moves the group in which the specified pid lies, to the foreground
 * @param[in] pid Process id
 */
void jobs_fg_proc_grp(int pid) {

    int cmd_i;
    int idx;
    int nb_cmds;
    int gpid;
    int cpid;
    int status;
    /* String to store the controlling terminal name */
    char tty_name[128];
    /* File descriptor for the controlling terminal */
    int tty_fd;

    /* Get the controlling terminal name */
    ctermid(tty_name);

    /* Open the controlling terminal file */
    tty_fd = open(tty_name, O_RDONLY);

    /* Get the index of jobs for the given process */
    idx = __get_idx_from_pid(pid);

    /* If the pid is not found */
    if (idx == -1) {

        return;
    }

    /* Get the group pid */
    gpid = g_jobs[idx].gpid;

    /* Send a stop signal to the entire process group */
    killpg(gpid, SIGTSTP);

    /* Make the entire child process group as foreground process group */
    tcsetpgrp(tty_fd, gpid);

    /* Send a continuation signal to the entire process group */
    killpg(gpid, SIGCONT);

    /* Get the number of commands for the job */
    nb_cmds = cmd_tab_get_nb_cmds(&g_jobs[idx].cmd_tab);

    /* For each of the child process */
    for (cmd_i = 0; cmd_i < nb_cmds; cmd_i++) {

        /* Wait till the child process either terminates/suspends */
        cpid = waitpid(-gpid, &status, WUNTRACED);

        /* If the process exited normally or by a signal */
        if (WIFEXITED(status) || WIFSIGNALED(status)) {

            /* Mark the job complete */
            jobs_mark_proc_comp(pid, false);
        }
        else if (WIFSTOPPED(status)) {

            /* Print the suspended job */
            printf("\n[%d] - %d suspended (%s)\n", idx, cpid,
                   cmd_tab_get_cmd_str(&g_jobs[idx].cmd_tab));
        }
    }

    /* Make the current (parent process) as the foreground process group */
    tcsetpgrp(tty_fd, getpgid(getpid()));
}

/**
 * @brief Moves the group in which the specified pid lies, to the background
 * @param[in] Process id
 */
void jobs_bg_proc_grp(int pid) {

    int idx;
    int gpid;

    /* Get the index using the given pid */
    idx = __get_idx_from_pid(pid);

    /* If pid not found */
    if (idx == -1) {

        return;
    }

    /* Get the process group id */
    gpid = g_jobs[idx].gpid;

    /* Send a signal to the entire process group */
    killpg(gpid, SIGCONT);
}

/**
 * @brief Marks the specified pid as completed
 * @param[in] pid Process id
 * @param[in] do_print Whether to print the debug information
 */
void jobs_mark_proc_comp(int pid, bool do_print) {

    int idx;
    int nb_procs_comp;

    /* If a valid pid is returned by wait */
    if (pid < 1) {

        return;
    }

    /* Get the process group id from the pid */
    idx = __get_idx_from_pid(pid);

    /* If pid not found */
    if (idx == -1) {

        return;
    }

    /* Increment the number of completed processes */
    nb_procs_comp = ++g_jobs[idx].nb_procs_comp;

    /* If the number of completed processes is same as the number of commands */
    if (nb_procs_comp == cmd_tab_get_nb_cmds(&g_jobs[idx].cmd_tab)) {

        if (do_print) {

            /* Print the completed job */
            printf("\n[%d] - %d done (%s)\n", idx, g_jobs[idx].gpid,
                   cmd_tab_get_cmd_str(&g_jobs[idx].cmd_tab));

            /* Print the prompt */
            prompt_print();
        }

        /* Deallocate the memory of the command table */
        cmd_tab_deinit(&g_jobs[idx].cmd_tab);

        /* Remove the process group entry from the job list */
        for (; idx < g_nb_jobs - 1; idx++) {

            /* Shift the jobs to the left */
            g_jobs[idx] = g_jobs[idx + 1];
        }

        /* Decrement the number of jobs */
        g_nb_jobs--;
    }
}

/**
 * @brief Prints the jobs maintained by the shell
 */
void jobs_print() {

    int job_i;

    /* Print the headers */
    printf("JOB_ID\tPGID\tCOMMAND\n");

    /* For every job */
    for (job_i = 0; job_i < g_nb_jobs; job_i++) {

        /* Print the job index */
        printf("[%d]\t", job_i);

        /* Print the process group id */
        printf("%d\t", g_jobs[job_i].gpid);

        /* Print the command string */
        printf("%s\n", cmd_tab_get_cmd_str(&g_jobs[job_i].cmd_tab));
    }
}

/**
 * @brief Kills the entire process group in which the specified pid lies
 * @param[in] pid Process id
 * @param[in] sig_num Signal number
 */
void jobs_kill_grp(int pid, int sig_num) {

    /* Get the index of the job from the global array */
    int idx = __get_idx_from_pid(pid);

    /* Send the signal to the process group */
    killpg(g_jobs[idx].gpid, sig_num);
}
