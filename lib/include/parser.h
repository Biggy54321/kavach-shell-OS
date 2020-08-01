#ifndef _PARSER_H_
#define _PARSER_H_

#include "command_table.h"

/**
 * @brief Parser state
 */
typedef enum __parser_state_t {

    PARSER_STATE_INIT = 0,
    PARSER_STATE_ARGS,
    PARSER_STATE_WHITE,
    PARSER_STATE_SPECIAL,
    PARSER_STATE_BACKGROUND

} parser_state_t;

#define NB_PARSER_STATES (5u)

/**
 * @brief Type of argument the parser is expecting
 */
typedef enum __parser_arg_type_t {

    ARG_TYPE_CMD = 0,
    ARG_TYPE_IN,
    ARG_TYPE_OUT

} parser_arg_type_t;

/**
 * @brief Parser return error numbers
 */
typedef enum __parser_err_t {

    PARSER_OK = 0,
    PARSER_GRAMMAR_ERR,
    PARSER_CHARACTER_ERR

} parser_err_t;

parser_err_t parser_set_cmd_tab(cmd_tab_t *p_cmd_tab, char *cmd_str);

#endif
