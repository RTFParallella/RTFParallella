/*******************************************************************************
 *   Copyright (c) 2019 Dortmund University of Applied Sciences and Arts and others.
 *  
 *   This program and the accompanying materials are made
 *   available under the terms of the Eclipse Public License 2.0
 *   which is available at https://www.eclipse.org/legal/epl-2.0/
 *  
 *   SPDX-License-Identifier: EPL-2.0
 *  
 *   Contributors:
 *        Dortmund University of Applied Sciences and Arts - initial API and implementation
 *******************************************************************************/

#ifndef SRC_PARALLELLA_DEBUGFLAGS_H_
#define SRC_PARALLELLA_DEBUGFLAGS_H_


#define cnt_address     0x3000

#define TASK1_FLAG             2
#define TASK2_FLAG             0
#define TASK3_FLAG             4
#define TASK4_FLAG             0
#define TASK5_FLAG             4
#define RUNNINGTASK_FLAG       6
#define DEBUG_FLAG             7
#define TICK_FLAG              8


/**
 *  Structure to ensure proper synchronization between host and epiphany cores
 * and also within epiphany cores.
 */
typedef struct software_interrupt
{
    int is_init_done;                 /**< To ensure that the mutex is initialized */
    int core_write_done;              /**< To ensure that the mutex is initialized */
    int host_read_done;               /**< To ensure that the mutex is initialized */
#ifdef __EPIPHANY_DEVICE__
    e_mutex_t* mutex;                 /**< Mutex declaration on Epiphany core for synchronization */
#else
    void* mutex;                      /**< Mutex declaration. Unused on host  */
#endif
    int offset;                       /**< Offset of the local memory in epiphany core */
    int row_id;                       /**< Row ID of the Epiphany core */
    int col_id;                       /**< Row ID of the Epiphany core */
} software_interrupt_t;


/**
 * initialize output buffer in core memory
 *
 * Arguments:
 *
 */
void init_task_trace_buffer(void );

/**
 * write the index of the running task to output buffer
 *
 * Arguments:
 * taskNum            :    index of the task
 *
 */
void traceRunningTask(unsigned taskNum);

/**
 * write the task instance (job) to output buffer
 *
 * Arguments:
 * taskNum            :    index of the task
 * currentPasses    :    instance of task (job number)
 *
 */
void traceTaskPasses(unsigned taskNum, int currentPasses);

/**
 * update RTOS tick value in output buffer
 *
 * Arguments:
 *
 */
void updateTick(void);

/**
 * Write a custom value to the output buffer for code coverage debugging
 *
 * Arguments:
 * debugMessage            :    message to be written
 *
 */
void updateDebugFlag(int debugMessage);

#endif /* SRC_PARALLELLA_DEBUGFLAGS_H_ */
