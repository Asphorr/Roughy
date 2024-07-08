#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"

void scheduler_init(void);
void scheduler_add_task(task_t *task);
void scheduler_remove_task(task_t *task);
void scheduler_run(void);
void task_switch(task_t *task);

#endif // SCHEDULER_H
