#ifndef _BUILT_IN_H_
#define _BUILT_IN_H_

#include "command_table.h"

/**
 * @brief Built in command types
 */
typedef enum __built_in_cmd_t {

    BUILT_IN_NOT = -1,
    BUILT_IN_FG,
    BUILT_IN_BG,
    BUILT_IN_CD,
    BUILT_IN_JOBS,
    BUILT_IN_KILLPG
} built_in_cmd_t;

built_in_cmd_t is_built_in(cmd_tab_t *p_cmd_tab);

void built_in_exec_cmd_tab(cmd_tab_t *p_cmd_tab, built_in_cmd_t built_in_type);

#endif
