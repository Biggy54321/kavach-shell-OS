#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "command_table.h"
#include "parser.h"
#include "executor.h"
#include "prompt.h"
#include "jobs.h"
#include "builtin.h"

/* Maximum command line string input length */
#define MAX_CMD_STR_LEN (1024u)

/* Declaration of gets (to prevent the warning) */
char *gets(char *);

/**
 * @brief Reads a command line string, parses to find meaningful commands,
 *        arguments and executes them accordingly
 */
int main() {

    /* Create the command table */
    cmd_tab_t cmd_tab;

    /* Create the command string */
    char cmd_str[MAX_CMD_STR_LEN];

    /* Variable to store the type of built-in command */
    built_in_cmd_t built_in_type;

    /* Create a new session for the shell */
    setsid();

    /* Initialize the jobs */
    jobs_init();

    while (1) {

        /* Initialize the prompt */
        prompt_signal_init();

        /* Print the prompt */
        prompt_print();

        /* Input the command line string from the user */
        if (!gets(cmd_str)) {

            /* Exit if EOF (Ctrl-D) is entered */
            exit(0);
        }

        /* Init command table */
        cmd_tab_init(&cmd_tab);

        /* Run the parser on the given string to set the command table */
        if (parser_set_cmd_tab(&cmd_tab, cmd_str) == PARSER_OK) {

            /* If the command is a built-in */
            if ((built_in_type = is_built_in(&cmd_tab)) != BUILT_IN_NOT) {

                /* Call the required built-in function */
                built_in_exec_cmd_tab(&cmd_tab, built_in_type);
            }
            else {

                /* If not built-in then fork-exec it */
                executor_exec_cmd_tab(&cmd_tab);
            }
        }

        /* Deinit the command table */
        cmd_tab_deinit(&cmd_tab);
    }

    return 0;
}
