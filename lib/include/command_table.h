#ifndef _COMMAND_TABLE_H_
#define _COMMAND_TABLE_H_

#include <stdbool.h>

/* Maximum number of commands in a command table */
#define MAX_NB_CMDS     (64u)
/* Maximum number of command line arguments in a single command */
#define MAX_NB_CMD_ARGS (128u)
/* Maximum number of input redirection files */
#define MAX_NB_IN_ARGS  (128u)
/* Maximum number of output redirection files */
#define MAX_NB_OUT_ARGS (128u)

/**
 * @brief Single command entry in the command table
 */
typedef struct __cmd_t {

    /* Command arguments */
    char *cmd_args[MAX_NB_CMD_ARGS];

    /* Number of command arguments */
    int nb_cmd_args;

    /* Input redirection file argument */
    char *in_arg;

    /* Boolean to check if the command is input redirected */
    bool is_input_redirected;

    /* Output redirection file argument */
    char *out_arg;

    /* Boolean to check if the command is output redirected */
    bool is_output_redirected;

} cmd_t;

/**
 * @brief Command table
 */
typedef struct __cmd_tab_t {

    /* The entire command line string */
    char *cmd_str;

    /* List of commands */
    cmd_t cmds[MAX_NB_CMDS];

    /* Number of commands */
    int nb_cmds;

    /* Are the commands backgrounded or not */
    bool is_background;

} cmd_tab_t;

void cmd_tab_init(cmd_tab_t *p_cmd_tab);

void cmd_tab_set_str(cmd_tab_t *p_cmd_tab, char *cmd_str);

void cmd_tab_add_cmd(cmd_tab_t *p_cmd_tab);

void cmd_tab_add_cmd_arg(cmd_tab_t *p_cmd_tab, char *cmd_arg);

void cmd_tab_set_in_arg(cmd_tab_t *p_cmd_tab, char *in_arg);

void cmd_tab_set_out_arg(cmd_tab_t *p_cmd_tab, char *out_arg);

void cmd_tab_set_bg(cmd_tab_t *p_cmd_tab);

char *cmd_tab_get_cmd_str(cmd_tab_t *p_cmd_tab);

int cmd_tab_get_nb_cmds(cmd_tab_t *p_cmd_tab);

bool cmd_tab_is_bg(cmd_tab_t *p_cmd_tab);

char **cmd_tab_get_cmd_args(cmd_tab_t *p_cmd_tab, int cmd_i);

int cmd_tab_get_nb_cmd_args(cmd_tab_t *p_cmd_tab, int cmd_i);

bool cmd_tab_is_input_redirected(cmd_tab_t *p_cmd_tab, int cmd_i);

bool cmd_tab_is_output_redirected(cmd_tab_t *p_cmd_tab, int cmd_i);

char *cmd_tab_get_in_arg(cmd_tab_t *p_cmd_tab, int cmd_i);

char *cmd_tab_get_out_arg(cmd_tab_t *p_cmd_tab, int cmd_i);

void cmd_tab_copy(cmd_tab_t *p_cmd_tab_dest, cmd_tab_t *p_cmd_tab_src);

void cmd_tab_deinit(cmd_tab_t *p_cmd_tab);

#endif

