#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "executor.h"
#include "jobs.h"

/* Returns the file descriptor to be used for reading by the ith command,
 * given fds has all the required number of pipe fds */
#define GET_RD_END_OF_CMD(fds, i)               \
    ({                                          \
        fds[(2 * ((i) - 1) + 0)];               \
    })
/* Returns the file descriptor to be used for writing by the ith command,
 * given fds has all the required number of pipe fds */
#define GET_WR_END_OF_CMD(fds, i)               \
    ({                                          \
        fds[(2 * (i) + 1)];                     \
    })

/* Opens the file in read mode */
#define OPEN_RD(file)                           \
    ({                                          \
        open(file, O_RDONLY);                   \
    })

/* Opens the file in write mode */
#define OPEN_WR(file)                                       \
    ({                                                      \
        open(file, O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);  \
    })

#define WRITE_ERROR_CMD(cmd)                                    \
    ({                                                          \
        fprintf(stderr, "kavach: `%s` command failed\n", cmd[0]); \
    })

/* Expands the command in order to pass it to execvp system call */
#define EXEC(args)                              \
    ({                                          \
        execvp(args[0], args);                  \
    })

/**
 * @brief Executes the command present in the command table
 * @param[in] p_cmd_tab Pointer to the command table instance
 */
void executor_exec_cmd_tab(cmd_tab_t *p_cmd_tab) {

    /* Index for traversing the ith command in the command table */
    int cmd_i;
    int cmd_j;

    /* Index for traversing the pipes */
    int pipe_i;

    /* Get the number of commands in the command table */
    int nb_cmds = cmd_tab_get_nb_cmds(p_cmd_tab);

    /* Allocate the memory for the pipes */
    int *cmd_pipes = (int *)malloc(2 * (nb_cmds + 1) * sizeof(int));

    /* Variable to store the process id of the child */
    pid_t child_pid;

    /* Variable to store the process group for the commands */
    pid_t group_pid = -1;

    /* Deinitialize any previously linked handlers */
    jobs_signal_deinit();

    /* For every pair of pipe file descriptor */
    for (pipe_i = 0; pipe_i < (nb_cmds + 1); pipe_i++) {

        /* Initialize the pipes */
        pipe(cmd_pipes + 2 * pipe_i);
    }

    /* Increment the pointer to the pipes array */
    cmd_pipes += 2;

    /* Link the input of first pipe to standard input */
    dup2(STDIN_FILENO, GET_RD_END_OF_CMD(cmd_pipes, 0));

    /* Link the output of last pipe to standard output */
    dup2(STDOUT_FILENO, GET_WR_END_OF_CMD(cmd_pipes, nb_cmds - 1));

    /* For every command in the command table */
    for (cmd_i = 0; cmd_i < nb_cmds; cmd_i++) {

        /* Fork to create a copy process */
        if (!(child_pid = fork())) {

            /* If the process group id is not set */
            if (group_pid == -1) {

                /* Change the process group id of the child to itself */
                setpgid(child_pid, 0);
            }
            else {

                /* Change the process group if the child to child leader */
                setpgid(child_pid, group_pid);
            }

            /* Prepare the input source for the ith command */
            if (cmd_tab_is_input_redirected(p_cmd_tab, cmd_i)) {

                /* Duplicate the input file argument descriptor  */
                dup2(OPEN_RD(cmd_tab_get_in_arg(p_cmd_tab, cmd_i)), STDIN_FILENO);
            }
            else {

                /* Duplicate the pipe file descriptor */
                dup2(GET_RD_END_OF_CMD(cmd_pipes, cmd_i), STDIN_FILENO);
            }

            /* Prepare the output source for the ith command */
            if (cmd_tab_is_output_redirected(p_cmd_tab, cmd_i)) {

                /* Duplicate the output file argument descriptor  */
                dup2(OPEN_WR(cmd_tab_get_out_arg(p_cmd_tab, cmd_i)), STDOUT_FILENO);
            }
            else {

                /* Duplicate the pipe file descriptor */
                dup2(GET_WR_END_OF_CMD(cmd_pipes, cmd_i), STDOUT_FILENO);
            }

            /* For each of the next command */
            for (cmd_j = cmd_i + 1; cmd_j < nb_cmds; cmd_j++) {

                /* Close the read end */
                close(GET_RD_END_OF_CMD(cmd_pipes, cmd_j));
                /* Close the write end */
                close(GET_WR_END_OF_CMD(cmd_pipes, cmd_j));
            }

            /* Execute the requested command */
            if (EXEC(cmd_tab_get_cmd_args(p_cmd_tab, cmd_i))) {

                /* Print the error to the standard error */
                WRITE_ERROR_CMD(cmd_tab_get_cmd_args(p_cmd_tab, cmd_i));

                /* Free the command table */
                cmd_tab_deinit(p_cmd_tab);

                /* Exit the child process */
                exit(0);
            }
        }
        else {

            /* If the process group id is not set */
            if (group_pid == -1) {

                /* Update process group id to the current child process id */
                group_pid = child_pid;

                /* Create a new job */
                jobs_add_proc_grp(group_pid, p_cmd_tab);
            }

            /* Update the process group id of the current child to the
             * process group id */
            setpgid(child_pid, group_pid);

            /* Add the process to the job */
            jobs_add_proc(group_pid, child_pid);

            /* Close the read end of the current command */
            close(GET_RD_END_OF_CMD(cmd_pipes, cmd_i));
            /* Close the write end of the current command */
            close(GET_WR_END_OF_CMD(cmd_pipes, cmd_i));
        }
    }

    /* If the process group is not backgrounded */
    if (!cmd_tab_is_bg(p_cmd_tab)) {

        /* Initialize the job signals */
        jobs_signal_init();

        /* Make the child process group as the foreground group */
        jobs_fg_proc_grp(group_pid);
    }

    /* Free the memory allocated to the pipes */
    free(cmd_pipes -= 2);
}
