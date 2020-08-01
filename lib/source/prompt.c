#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/* Current working directory string size */
#define CWD_STR_SIZE (128u)

/* Prototypes for the handlers */
static void __sigint_handler(int sig_num);

/**
 * @brief Initialize the signal handlers for the prompt
 */
void prompt_signal_init() {

    /* Initialize the SIGINT handler */
    signal(SIGINT, __sigint_handler);

    /* Initialize the SIGTSTP handler */
    signal(SIGTSTP, SIG_IGN);

    /* Initialize the SIGTTOU handler */
    signal(SIGTTOU, SIG_DFL);

    /* Initialize the SIGCHLD handler */
    signal(SIGCHLD, SIG_DFL);
}

/**
 * @brief Print the prompt for the shell
 */
void prompt_print() {

    /* Define the current working directory string size */
    char cwd_str[CWD_STR_SIZE];

    /* Get the current working directory */
    getcwd(cwd_str, CWD_STR_SIZE);

    /* Print the prompt string */
    printf("╭─[ %s ]\n╰─O ", cwd_str);

    /* Flush the output */
    fflush(stdout);
}

/**
 * @brief SIGINT handler
 * @param[in] sig_num Signal number
 */
static void __sigint_handler(int sig_num) {

    /* Print newline */
    printf("\n");

    /* Print the prompt */
    prompt_print();
}
