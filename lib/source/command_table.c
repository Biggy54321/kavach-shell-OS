#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/command_table.h"

/**
 * @brief Initialize the command table (sets the variables to base values)
 * @param[out] p_cmd_tab Pointer to command table object
 */
void cmd_tab_init(cmd_tab_t *p_cmd_tab) {

    /* Set the string to NULL */
    p_cmd_tab->cmd_str = NULL;

    /* Set the number of commands to -1 */
    p_cmd_tab->nb_cmds = -1;

    /* Set the background status */
    p_cmd_tab->is_background = false;
}

/**
 * @brief Set the command line string for the command table
 * @param[out] p_cmd_tab Pointer to command table object
 * @param[in] cmd_str Command line string
 */
void cmd_tab_set_str(cmd_tab_t *p_cmd_tab, char *cmd_str) {

    /* Set the string */
    p_cmd_tab->cmd_str = strdup(cmd_str);
}

/**
 * @brief Add a new empty command to the table
 * @param[out] p_cmd_tab Pointer to command table object
 */
void cmd_tab_add_cmd(cmd_tab_t *p_cmd_tab) {

    /* If the command table has atleast one command */
    if (p_cmd_tab->nb_cmds > -1) {
        /* Set the current command's last argument as NULL */
        cmd_tab_add_cmd_arg(p_cmd_tab, NULL);
    }

    /* Increment the commands count */
    p_cmd_tab->nb_cmds++;

    /* Initialize the number of command line arguments for the command to zero */
    p_cmd_tab->cmds[p_cmd_tab->nb_cmds].nb_cmd_args = 0;

    /* Initialize the input argument string */
    p_cmd_tab->cmds[p_cmd_tab->nb_cmds].in_arg = NULL;

    /* Initialize the status of input redirection */
    p_cmd_tab->cmds[p_cmd_tab->nb_cmds].is_input_redirected = false;

    /* Initialize the input argument string */
    p_cmd_tab->cmds[p_cmd_tab->nb_cmds].in_arg = NULL;

    /* Initialize the status of output redirection */
    p_cmd_tab->cmds[p_cmd_tab->nb_cmds].is_output_redirected = false;
}

/**
 * @brief Add a command line argument to the current command in the table
 * @param[out] p_cmd_tab Pointer to command table object
 * @param[in] cmd_arg Command line argument
 */
void cmd_tab_add_cmd_arg(cmd_tab_t *p_cmd_tab, char *cmd_arg) {

    /* Get the starting address of the command line arguments list */
    char **cmd_args = p_cmd_tab->cmds[p_cmd_tab->nb_cmds].cmd_args;
    /* Get the current number of command arguments */
    int nb_cmd_args = p_cmd_tab->cmds[p_cmd_tab->nb_cmds].nb_cmd_args;

    /* Add the command line argument to the current command */
    cmd_args[nb_cmd_args] = (cmd_arg) ? strdup(cmd_arg) : NULL;

    /* Increment the number of command line arguments */
    p_cmd_tab->cmds[p_cmd_tab->nb_cmds].nb_cmd_args++;
}

/**
 * @brief Add a input redirection file to the command table
 * @param[out] p_cmd_tab Pointer to command table object
 * @param[in] in_arg Input redirection file argument
 */
void cmd_tab_set_in_arg(cmd_tab_t *p_cmd_tab, char *in_arg) {

    /* If the command is already input redirected */
    if(p_cmd_tab->cmds[p_cmd_tab->nb_cmds].is_input_redirected) {

        /* Deallocate the previously allocated string */
        free(p_cmd_tab->cmds[p_cmd_tab->nb_cmds].in_arg);
    }

    /* Allocate the new string */
    p_cmd_tab->cmds[p_cmd_tab->nb_cmds].in_arg = (in_arg) ? strdup(in_arg) : NULL;

    /* Update the input redirection status */
    p_cmd_tab->cmds[p_cmd_tab->nb_cmds].is_input_redirected = true;
}

/**
 * @brief Add a output redirection file to the command table
 * @param[out] p_cmd_tab Pointer to command table object
 * @param[in] out_arg Output redirection file argument
 */
void cmd_tab_set_out_arg(cmd_tab_t *p_cmd_tab, char *out_arg) {

    /* If the command is already input redirected */
    if(p_cmd_tab->cmds[p_cmd_tab->nb_cmds].is_output_redirected) {

        /* Deallocate the previously allocated string */
        free(p_cmd_tab->cmds[p_cmd_tab->nb_cmds].out_arg);
    }

    /* Allocate the new string */
    p_cmd_tab->cmds[p_cmd_tab->nb_cmds].out_arg = (out_arg) ? strdup(out_arg) : NULL;

    /* Update the input redirection status */
    p_cmd_tab->cmds[p_cmd_tab->nb_cmds].is_output_redirected = true;
}

/**
 * @brief Informs that the commands are to be run in background
 * @param[out] p_cmd_tab Pointer to command table object
 */
void cmd_tab_set_bg(cmd_tab_t *p_cmd_tab) {

    /* Set the flag to true */
    p_cmd_tab->is_background = true;
}

/**
 * @brief Returns the command line string entered by the user
 * @param[in] p_cmd_tab Pointer to command table object
 * @return Pointer to dynamically allocated string
 */
char *cmd_tab_get_cmd_str(cmd_tab_t *p_cmd_tab) {

    /* Return the copy of the command string */
    return strdup(p_cmd_tab->cmd_str);
}

/**
 * @brief Returns the number of commands in the command table
 * @param[in] p_cmd_tab Pointer to command table object
 * @return Integer number
 */
int cmd_tab_get_nb_cmds(cmd_tab_t *p_cmd_tab) {

    /* Return the number of commands */
    return p_cmd_tab->nb_cmds;
}

/**
 * @brief Returns the background status of the commands
 * @param[in] p_cmd_tab Pointer to command table object
 * @return Boolean status
 */
bool cmd_tab_is_bg(cmd_tab_t *p_cmd_tab) {

    /* Return the background status */
    return p_cmd_tab->is_background;
}

/**
 * @brief Returns the command arguments for the specified command
 * @param[in] p_cmd_tab Pointer to command table object
 * @param[in] cmd_i The ith command in the command table
 * @return Array of strings (NULL terminated)
 */
char **cmd_tab_get_cmd_args(cmd_tab_t *p_cmd_tab, int cmd_i) {

    /* Return the ith command arguments */
    return p_cmd_tab->cmds[cmd_i].cmd_args;
}

/**
 * @brief Returns the number of command arguments for the specified command
 * @param[in] p_cmd_tab Pointer to command table object
 * @param[in] cmd_i The ith command in the command table
 * @return Integer number of arguments (except the NULL)
 */
int cmd_tab_get_nb_cmd_args(cmd_tab_t *p_cmd_tab, int cmd_i) {

    /* Return the number of command arguments (excepting the last NULL) */
    return p_cmd_tab->cmds[cmd_i].nb_cmd_args - 1;
}

/**
 * @brief Returns the input redirection status for the specified command
 * @param[in] p_cmd_tab Pointer to command table object
 * @param[in] cmd_i The ith command in the command table
 * @return Boolean status
 */
bool cmd_tab_is_input_redirected(cmd_tab_t *p_cmd_tab, int cmd_i) {

    /* Return the input redirection status */
    return p_cmd_tab->cmds[cmd_i].is_input_redirected;
}

/**
 * @brief Returns the output redirection status for the specified command
 * @param[in] p_cmd_tab Pointer to command table object
 * @param[in] cmd_i The ith command in the command table
 * @return Boolean status
 */
bool cmd_tab_is_output_redirected(cmd_tab_t *p_cmd_tab, int cmd_i) {

    /* Return the output redirection status */
    return p_cmd_tab->cmds[cmd_i].is_output_redirected;
}

/**
 * @brief Returns the input redirection argument for the specified command
 * @param[in] p_cmd_tab Pointer to command table object
 * @param[in] cmd_i The ith command in the command table
 * @return Dynamically allocated input argument string
 */
char *cmd_tab_get_in_arg(cmd_tab_t *p_cmd_tab, int cmd_i) {

    /* Return the input redirected file name */
    return strdup(p_cmd_tab->cmds[cmd_i].in_arg);
}

/**
 * @brief Returns the output redirection argument for the specified command
 * @param[in] p_cmd_tab Pointer to command table object
 * @param[in] cmd_i The ith command in the command table
 * @return Dynamically allocated output argument string
 */
char *cmd_tab_get_out_arg(cmd_tab_t *p_cmd_tab, int cmd_i) {

    /* Return the output redirected file name */
    return strdup(p_cmd_tab->cmds[cmd_i].out_arg);
}

/**
 * @brief Copies one command table to another (allocating new memory)
 * @param[out] p_cmd_tab_dest Destination command table
 * @param[in] p_cmd_tab_src Source command table
 */
void cmd_tab_copy(cmd_tab_t *p_cmd_tab_dest, cmd_tab_t *p_cmd_tab_src) {

    int i;
    int j;

    /* Copy the command string */
    if (p_cmd_tab_src->cmd_str) {
        p_cmd_tab_dest->cmd_str = strdup(p_cmd_tab_src->cmd_str);
    }
    else {
        p_cmd_tab_dest->cmd_str = NULL;
    }

    /* Copy the number of commands */
    p_cmd_tab_dest->nb_cmds = p_cmd_tab_src->nb_cmds;

    /* Copy the command arguments */
    for (i = 0; i < p_cmd_tab_src->nb_cmds; i++) {

        /* Copy the number of command arguments */
        p_cmd_tab_dest->cmds[i].nb_cmd_args = p_cmd_tab_src->cmds[i].nb_cmd_args;

        /* Copy the command arguments */
        for (j = 0; j < p_cmd_tab_src->cmds[i].nb_cmd_args - 1; j++) {

            p_cmd_tab_dest->cmds[i].cmd_args[j] = strdup(p_cmd_tab_src->cmds[i].cmd_args[j]);
        }

        /* Make the last command argument as NULL */
        p_cmd_tab_dest->cmds[i].cmd_args[j] = NULL;

        /* Copy the input redirection status  */
        p_cmd_tab_dest->cmds[i].is_input_redirected = p_cmd_tab_src->cmds[i].is_input_redirected;

        /* Copy the input redirection file name */
        if (p_cmd_tab_src->cmds[i].in_arg) {
            p_cmd_tab_dest->cmds[i].in_arg = strdup(p_cmd_tab_src->cmds[i].in_arg);
        }
        else {
            p_cmd_tab_dest->cmds[i].in_arg = NULL;
        }

        /* Copy the output redirection status  */
        p_cmd_tab_dest->cmds[i].is_output_redirected = p_cmd_tab_src->cmds[i].is_output_redirected;

        /* Copy the output redirection file name */
        if (p_cmd_tab_src->cmds[i].out_arg) {
            p_cmd_tab_dest->cmds[i].out_arg = strdup(p_cmd_tab_src->cmds[i].out_arg);
        }
        else {
            p_cmd_tab_dest->cmds[i].out_arg = NULL;
        }
    }

    /* Copy the background status */
    p_cmd_tab_dest->is_background = p_cmd_tab_src->is_background;
}

/**
 * @brief Deallocates the memory assinged to the command table
 * @param[out] p_cmd_tab Pointer to command table object
 */
void cmd_tab_deinit(cmd_tab_t *p_cmd_tab) {

    int i;
    int j;

    /* Free the command line string */
    free(p_cmd_tab->cmd_str);

    /* For each command in the command table */
    for (i = 0; i < p_cmd_tab->nb_cmds; i++) {

        /* For each command line arguments of the current command */
        for (j = 0; j < p_cmd_tab->cmds[i].nb_cmd_args; j++) {

            /* Free the command line argument string */
            free(p_cmd_tab->cmds[i].cmd_args[j]);
        }

        /* Set the number of command line arguments to zero */
        p_cmd_tab->cmds[i].nb_cmd_args = 0;

        /* If the command was input redirected */
        if (p_cmd_tab->cmds[i].is_input_redirected) {

            /* Free the input file argument */
            free(p_cmd_tab->cmds[i].in_arg);

            /* Update the input redirection status */
            p_cmd_tab->cmds[i].is_input_redirected = false;
        }

        /* If the command was output redirected */
        if (p_cmd_tab->cmds[i].is_output_redirected) {

            /* Free the output file argument */
            free(p_cmd_tab->cmds[i].out_arg);

            /* Update the output redirection status */
            p_cmd_tab->cmds[i].is_output_redirected = false;
        }
    }

    /* Reinitialize the command table */
    cmd_tab_init(p_cmd_tab);
}
