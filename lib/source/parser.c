#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "parser.h"
#include "str_util.h"

/* Maximum token string length */
#define MAX_TOKEN_SIZE (512u)
/* String to store the tokens */
char g_tok_str[MAX_TOKEN_SIZE];
/* Index to traverse the token string */
int g_tok_i;
/* Parser state */
parser_state_t g_state;
/* Parser expected argument type */
parser_arg_type_t g_arg_type;

/**
 * @brief Function to perform action if the current state if INIT
 * @param[in] Pointer to the command table instance
 * @param[in] cmd_ch The current character from the command string
 * @return PARSER_OK On success
 * @return PARSER_GRAMMAR_ERR On invalid syntax/grammar
 * @return PARSER_CHARACTER_ERR On unsupported character
 */
static inline parser_err_t __parser_action_init(
        cmd_tab_t *p_cmd_tab,
        char cmd_ch) {

    parser_err_t ret_err;

    if (IS_WHITESPACE(cmd_ch) ||
        IS_NULL(cmd_ch)) {

        /* Return success */
        ret_err = PARSER_OK;
    }
    else if (IS_INPUT_REDIREC_OP(cmd_ch)  ||
             IS_OUTPUT_REDIREC_OP(cmd_ch) ||
             IS_PIPE_OP(cmd_ch)) {

        /* Return error */
        ret_err = PARSER_GRAMMAR_ERR;
    }
    else if (IS_VALID_IDENTIFIER(cmd_ch)) {

        /* Add a new command */
        cmd_tab_add_cmd(p_cmd_tab);
        /* Update the token string index */
        g_tok_i = 0;
        /* Update the token string */
        g_tok_str[g_tok_i++] = cmd_ch;
        /* Update the state */
        g_state = PARSER_STATE_ARGS;
        /* Update the expected argument type */
        g_arg_type = ARG_TYPE_CMD;
        /* Return success */
        ret_err = PARSER_OK;
    }
    else {

        /* Return error */
        ret_err = PARSER_CHARACTER_ERR;
    }

    return ret_err;
}

/**
 * @brief Function to perform action if the current state if ARGS
 * @param[in] Pointer to the command table instance
 * @param[in] cmd_ch The current character from the command string
 * @return PARSER_OK On success
 * @return PARSER_GRAMMAR_ERR On invalid syntax/grammar
 * @return PARSER_CHARACTER_ERR On unsupported character
 */
static inline parser_err_t __parser_action_args(
        cmd_tab_t *p_cmd_tab,
        char cmd_ch) {

    parser_err_t ret_err;

    /* If the character is the token terminator */
    if (IS_WHITESPACE(cmd_ch)        ||
        IS_INPUT_REDIREC_OP(cmd_ch)  ||
        IS_OUTPUT_REDIREC_OP(cmd_ch) ||
        IS_PIPE_OP(cmd_ch)           ||
        IS_BACKGROUND_OP(cmd_ch)     ||
        IS_NULL(cmd_ch)) {

        /* Add the end of string character */
        g_tok_str[g_tok_i] = '\0';
        /* Add the token depending on the argument type exepected  */
        if (g_arg_type == ARG_TYPE_CMD) {
            cmd_tab_add_cmd_arg(p_cmd_tab, g_tok_str);
        }
        else if (g_arg_type == ARG_TYPE_IN)  {
            cmd_tab_set_in_arg(p_cmd_tab, g_tok_str);
        }
        else if (g_arg_type == ARG_TYPE_OUT) {
            cmd_tab_set_out_arg(p_cmd_tab, g_tok_str);
        }

        /* Update the states depending on the terminator */
        if (IS_WHITESPACE(cmd_ch)) {

            /* Update the argument type */
            g_arg_type = ARG_TYPE_CMD;
            /* Update the parser state */
            g_state = PARSER_STATE_WHITE;
            /* Return success */
            ret_err = PARSER_OK;
        }
        else if (IS_INPUT_REDIREC_OP(cmd_ch)) {

            /* Update the argument type */
            g_arg_type = ARG_TYPE_IN;
            /* Update the parser state */
            g_state = PARSER_STATE_SPECIAL;
            /* Return success */
            ret_err = PARSER_OK;
        }
        else if (IS_OUTPUT_REDIREC_OP(cmd_ch)) {

            /* Update the argument type */
            g_arg_type = ARG_TYPE_OUT;
            /* Update the parser state */
            g_state = PARSER_STATE_SPECIAL;
            /* Return success */
            ret_err = PARSER_OK;
        }
        else if (IS_PIPE_OP(cmd_ch)) {

            /* Add a new command (pipe indicates end
             * of previous one) */
            cmd_tab_add_cmd(p_cmd_tab);
            /* Update the argument type */
            g_arg_type = ARG_TYPE_CMD;
            /* Update the parser state */
            g_state = PARSER_STATE_SPECIAL;
            /* Return success */
            ret_err = PARSER_OK;
        }
        else if (IS_NULL(cmd_ch)) {

            /* Add a new command */
            cmd_tab_add_cmd(p_cmd_tab);
            /* Return success */
            ret_err = PARSER_OK;
        }
        else if (IS_BACKGROUND_OP(cmd_ch)) {

            /* Set the backgrounded status for the commands */
            cmd_tab_set_bg(p_cmd_tab);
            /* Update the parser state */
            g_state = PARSER_STATE_BACKGROUND;
            /* Return success */
            ret_err = PARSER_OK;
        }
    }
    else if (IS_VALID_IDENTIFIER(cmd_ch)) {

        /* Add the character to the token string */
        g_tok_str[g_tok_i++] = cmd_ch;
        /* Return success */
        ret_err = PARSER_OK;
    }
    else {

        /* Return error */
        ret_err = PARSER_CHARACTER_ERR;
    }

    return ret_err;
}

/**
 * @brief Function to perform action if the current state if WHITE
 * @param[in] Pointer to the command table instance
 * @param[in] cmd_ch The current character from the command string
 * @return PARSER_OK On success
 * @return PARSER_GRAMMAR_ERR On invalid syntax/grammar
 * @return PARSER_CHARACTER_ERR On unsupported character
 */
static inline parser_err_t __parser_action_white(
        cmd_tab_t *p_cmd_tab,
        char cmd_ch) {

    parser_err_t ret_err;

    /* Initalize the token string index */
    g_tok_i = 0;

    if (IS_WHITESPACE(cmd_ch)) {

        /* Return success */
        ret_err = PARSER_OK;
    }
    else if (IS_INPUT_REDIREC_OP(cmd_ch)) {

        /* Update the expected argument type */
        g_arg_type = ARG_TYPE_IN;
        /* Update the state */
        g_state = PARSER_STATE_SPECIAL;
        /* Return success */
        ret_err = PARSER_OK;
    }
    else if (IS_OUTPUT_REDIREC_OP(cmd_ch)) {

        /* Update the expected argument type */
        g_arg_type = ARG_TYPE_OUT;
        /* Update the state */
        g_state = PARSER_STATE_SPECIAL;
        /* Return success */
        ret_err = PARSER_OK;
    }
    else if (IS_PIPE_OP(cmd_ch)) {

        /* Add a new command to the command table */
        cmd_tab_add_cmd(p_cmd_tab);
        /* Update the state */
        g_state = PARSER_STATE_SPECIAL;
        /* Return success */
        ret_err = PARSER_OK;
    }
    else if (IS_NULL(cmd_ch)) {

        /* Add a new command to the command table */
        cmd_tab_add_cmd(p_cmd_tab);
        /* Return success */
        ret_err = PARSER_OK;
    }
    else if (IS_VALID_IDENTIFIER(cmd_ch)) {

        /* Save the character in token string */
        g_tok_str[g_tok_i++] = cmd_ch;
        /* Update the state */
        g_state = PARSER_STATE_ARGS;
        /* Return success */
        ret_err = PARSER_OK;
    }
    else if (IS_BACKGROUND_OP(cmd_ch)) {

        /* Set the backgrounded status for the commands */
        cmd_tab_set_bg(p_cmd_tab);
        /* Update the parser state */
        g_state = PARSER_STATE_BACKGROUND;
        /* Return success */
        ret_err = PARSER_OK;
    }
    else {

        /* Return error */
        ret_err = PARSER_CHARACTER_ERR;
    }

    return ret_err;
}

/**
 * @brief Function to perform action if the current state if SPECIAL
 * @param[in] Pointer to the command table instance
 * @param[in] cmd_ch The current character from the command string
 * @return PARSER_OK On success
 * @return PARSER_GRAMMAR_ERR On invalid syntax/grammar
 * @return PARSER_CHARACTER_ERR On unsupported character
 */
static inline parser_err_t __parser_action_special(
        cmd_tab_t *p_cmd_tab,
        char cmd_ch) {

    parser_err_t ret_err;

    /* Initalize the token string index */
    g_tok_i = 0;

    if (IS_WHITESPACE(cmd_ch)) {

        /* Return success */
        ret_err = PARSER_OK;
    }
    else if (IS_INPUT_REDIREC_OP(cmd_ch)  ||
             IS_OUTPUT_REDIREC_OP(cmd_ch) ||
             IS_PIPE_OP(cmd_ch)           ||
             IS_BACKGROUND_OP(cmd_ch)     ||
             IS_NULL(cmd_ch)) {

        /* Return failure */
        ret_err = PARSER_GRAMMAR_ERR;
    }
    else if (IS_VALID_IDENTIFIER(cmd_ch)) {

        /* Save the character in token string */
        g_tok_str[g_tok_i++] = cmd_ch;
        /* Update the state */
        g_state = PARSER_STATE_ARGS;
        /* Return success */
        ret_err = PARSER_OK;
    }
    else {

        /* Return error */
        ret_err = PARSER_CHARACTER_ERR;
    }

    return ret_err;
}

/**
 * @brief Function to perform action if the current state if BACKGROUND
 * @param[in] Pointer to the command table instance
 * @param[in] cmd_ch The current character from the command string
 * @return PARSER_OK On success
 * @return PARSER_GRAMMAR_ERR On invalid syntax/grammar
 * @return PARSER_CHARACTER_ERR On unsupported character
 */
static inline parser_err_t __parser_action_background(
        cmd_tab_t *p_cmd_tab,
        char cmd_ch) {

    parser_err_t ret_err;

    if (IS_WHITESPACE(cmd_ch)) {

        /* Return success */
        ret_err = PARSER_OK;
    }
    else if (IS_NULL(cmd_ch)) {

        /* Add a new command to the command table */
        cmd_tab_add_cmd(p_cmd_tab);
        /* Return success */
        ret_err = PARSER_OK;
    }
    else if (IS_INPUT_REDIREC_OP(cmd_ch)  ||
             IS_OUTPUT_REDIREC_OP(cmd_ch) ||
             IS_BACKGROUND_OP(cmd_ch)     ||
             IS_PIPE_OP(cmd_ch)           ||
             IS_VALID_IDENTIFIER(cmd_ch)) {

        /* Return error */
        ret_err = PARSER_GRAMMAR_ERR;
    }
    else {

        ret_err = PARSER_CHARACTER_ERR;
    }

    return ret_err;
}

/**
 * @brief Sets the command table appropriately, given the entire command line
 *        string
 * @param[in] Pointer to the command table instance
 * @param[in] cmd_str Command line string
 * @return PARSER_OK On success
 * @return PARSER_GRAMMAR_ERR On invalid syntax/grammar
 * @return PARSER_CHARACTER_ERR On unsupported character
 */
parser_err_t parser_set_cmd_tab(cmd_tab_t *p_cmd_tab, char *cmd_str) {

    /* Index for traversing the command string */
    int cmd_i = 0;

    /* Initialize the function pointers for performing actions depending
     * on the current state */
    parser_err_t (*action[NB_PARSER_STATES])(cmd_tab_t *, char) =
            {__parser_action_init,
             __parser_action_args,
             __parser_action_white,
             __parser_action_special,
             __parser_action_background};

    /* Error number of the state actions */
    parser_err_t ret_err;

    /* Initialize the initial state of the parser */
    g_state = PARSER_STATE_INIT;

    /* Set the command line string in the command table  */
    cmd_tab_set_str(p_cmd_tab, cmd_str);

    /* For each character */
    while (cmd_i <= strlen(cmd_str)) {

        /* Perform the action depending on the current state */
        ret_err = action[g_state](p_cmd_tab, cmd_str[cmd_i]);

        /* If the action resulted in an error */
        if (ret_err == PARSER_GRAMMAR_ERR) {

            /* Print the error */
            fprintf(stderr, "kavach: parser grammar error occurred near `%c`\n", cmd_str[cmd_i]);

            /* Then return with error */
            return ret_err;
        }
        else if (ret_err == PARSER_CHARACTER_ERR) {

            /* Print the error */
            fprintf(stderr, "kavach: parser character error occurred near `%c`\n", cmd_str[cmd_i]);


            /* Then return with error */
            return ret_err;
        }

        /* Increment the command string index */
        cmd_i++;
    }

    /* Return with success */
    return PARSER_OK;
}
