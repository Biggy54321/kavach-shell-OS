#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include "builtin.h"
#include "jobs.h"

#define IS_COMMAND_FG(str)     (!strcmp(str, "fg"))
#define IS_COMMAND_BG(str)     (!strcmp(str, "bg"))
#define IS_COMMAND_CD(str)     (!strcmp(str, "cd"))
#define IS_COMMAND_JOBS(str)   (!strcmp(str, "jobs"))
#define IS_COMMAND_KILLPG(str) (!strcmp(str, "killpg"))

built_in_cmd_t is_built_in(cmd_tab_t *p_cmd_tab) {

    /* Command arguments */
    char **cmd_args = cmd_tab_get_cmd_args(p_cmd_tab, 0);

    /* Check if the command is a builtin */
    if (IS_COMMAND_FG(cmd_args[0])) {

        return BUILT_IN_FG;
    }
    else if (IS_COMMAND_BG(cmd_args[0])) {

        return BUILT_IN_BG;
    }
    else if (IS_COMMAND_CD(cmd_args[0])) {

        return BUILT_IN_CD;
    }
    else if (IS_COMMAND_JOBS(cmd_args[0])) {

        return BUILT_IN_JOBS;
    }
    else if (IS_COMMAND_KILLPG(cmd_args[0])) {

        return BUILT_IN_KILLPG;
    }
    else {

        /* The command is not a built-in */
        return BUILT_IN_NOT;
    }
}

static void __change_directory(char *str) {

    /* Change the directory to the specified argument */
    if (chdir(str)) {

        fprintf(stderr, "kavach: `%s` directory does not exist\n", str);
    }
}

void built_in_exec_cmd_tab(cmd_tab_t *p_cmd_tab, built_in_cmd_t built_in_type) {

    /* Command arguments */
    char **cmd_args = cmd_tab_get_cmd_args(p_cmd_tab, 0);
    /* Command arguments number */
    int nb_cmd_args = cmd_tab_get_nb_cmd_args(p_cmd_tab, 0);

    /* Call the respective built-in functions accordingly */
    if (built_in_type == BUILT_IN_FG) {

        /* Check if we have correct number of arguments */
        if (nb_cmd_args == 2) {

            jobs_signal_init();

            jobs_fg_proc_grp(atoi(cmd_args[1]));
        }
        else {

            fprintf(stderr, "kavach: incorrect number of arguments <fg pid>\n");
        }
    }
    else if (built_in_type == BUILT_IN_BG) {

        /* Check if we have correct number of arguments */
        if (nb_cmd_args == 2) {

            jobs_bg_proc_grp(atoi(cmd_args[1]));
        }
        else {

            fprintf(stderr, "kavach: incorrect number of arguments <bg pid>\n");
        }
    }
    else if (built_in_type == BUILT_IN_CD) {

        /* Check if we have correct number of arguments */
        if (nb_cmd_args == 2) {

            __change_directory(cmd_args[1]);
        }
        else {

            fprintf(stderr, "kavach: incorrect number of arguments <cd path>\n");
        }
    }
    else if (built_in_type == BUILT_IN_JOBS) {

        /* Check if we have correct number of arguments */
        if (nb_cmd_args == 1) {

            jobs_print();
        }
        else {

            fprintf(stderr, "kavach: incorrect number of arguments <jobs>\n");
        }
    }
    else if (built_in_type == BUILT_IN_KILLPG) {

        /* Check if we have correct number of arguments */
        if (nb_cmd_args == 3) {

            jobs_kill_grp(atoi(cmd_args[2]), atoi(cmd_args[1]));
        }
        else {

            fprintf(stderr, "kavach: incorrect number of arguments <killpg sig_nb pid>\n");
        }
    }
}
