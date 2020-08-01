#ifndef _JOBS_H_
#define _JOBS_H_

#include "command_table.h"

/* Maximum number of processes in a group */
#define MAX_PROCS_IN_GRP  (128u)

/**
 * @brief Job structure to hold information of single job (or a process group)
 */
typedef struct job_t {

    /* Command table corresponding to the job */
    cmd_tab_t cmd_tab;

    /* Process group id */
    int gpid;

    /* All the process ids in the group */
    int pids[MAX_PROCS_IN_GRP];

    /* Number of processes */
    int nb_pids;

    /* Number of processes completed */
    int nb_procs_comp;

} job_t;

void jobs_init();

void jobs_signal_init();

void jobs_signal_deinit();

void jobs_add_proc_grp(int gpid, cmd_tab_t *p_cmd_tab);

void jobs_add_proc(int gpid, int pid);

void jobs_fg_proc_grp(int pid);

void jobs_bg_proc_grp(int pid);

void jobs_mark_proc_comp(int pid, bool do_print);

void jobs_print();

void jobs_kill_grp(int pid, int sig_num);

#endif
