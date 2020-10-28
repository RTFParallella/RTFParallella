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

/**
 * @file debugFlags.h
 * @author Mahmoud Bazzal, Anand Prakash
 * @date 10 April 2020
 * @brief This file declares the debug trace of the application running on Epiphany core.The
 * debug trace consists in two forms. One is the visual form which is the legacy
 * RTFParallella trace and can be seen while executing the application on Adapteva Parallella.
 * The other is the BTF trace dump which can be viewed on Eclipse Trace Compass.
 *
 */

#include "trace_utils_BTF.h"

#define cnt_address     0x3000

#define TASK1_FLAG             2
#define TASK2_FLAG             0
#define TASK3_FLAG             4
#define TASK4_FLAG             0
#define TASK5_FLAG             4
#define RUNNINGTASK_FLAG       6
#define DEBUG_FLAG             7
#define TICK_FLAG              8


#define BTF_TRACE_BUFFER_SIZE  8



/**
 * @brief Get the time scaling factor for tick count
 *
 *
 * @return : Scale factor
 *
 */
unsigned int get_time_scale_factor(void);

/**
 * @brief Initialize memory section for storing BTR trace data and metadata
 *
 * The function does not take any arguments. It initializes the BTF memory section
 * from the shared memory area.
 * @return : void
 *
 */

void init_btf_mem_section(void);

/**
 * @brief Initialize output buffer in core memory
 *
 * The function initializes the epiphany core memory section for dumping the legacy
 * RTParallella trace.
 *
 * @return : void
 *
 *
 */
void init_task_trace_buffer(void );

/**
 * @brief Write the index of the running task to output buffer
 *
 * The function writes the ID of the current task in execution to the Epiphany core memory
 *
 * @param[in] taskNum            :    index of the task
 *
 * @return : void
 *
 */
void traceRunningTask(unsigned taskNum);

/**
 * @brief write the task instance (job) to output buffer
 *
 * The function writes the task instance number  of the current task in
 * execution to the Epiphany core memory.
 *
 * @param[in] taskNum            :    index of the task
 * @param[in] currentPasses    :    instance of task (job number)
 *
 */
void traceTaskPasses(unsigned taskNum, int currentPasses);

/**
 * @brief Update RTOS tick value in output buffer
 *
 * @return : void
 *
 */
void updateTick(void);

/**
 * @brief Write a custom value to the output buffer for code coverage debugging
 *
 *
 * @param[in] debugMessage            :    message to be written
 *
 *@return : void
 */
void updateDebugFlag(int debugMessage);


/**
 * @brief Signal the host core to read the memory.
 *
 * The function locks the shared memory address using the epiphany mutex implementation.
 * It waits until wait the host core processor has read the data.
 * It then dumps the BTF trace metadata to the shared memory and and unlock the mutex.
 * DMA channel 1 is used to dump the trace metadata.
 *
 * @return : void
 *
 */
void signalHost(void);


/**
 * @brief Write the BTF trace data
 *
 * The function dumps the BTF trace data to the shared memory..
 * DMA channel 1 is used to dump the trace data.
 *
 * @param[in] srcID          : Source ID of the task.
 * @param[in] srcInstance    : Source instance of the task.
 * @param[in] type           : Event type.
 * @param[in] taskId         : Task Id.
 * @param[in] taskInstance   : Task instance.
 * @param[in] event_name     : Name of the event.
 * @param[in] data           : Notes or shared label value
 *
 * @return : void
 *
 */
void traceTaskEvent(int srcID, int srcInstance, btf_trace_event_type type,
        int taskId, int taskInstance, btf_trace_event_name event_name, int data);

#endif /* SRC_PARALLELLA_DEBUGFLAGS_H_ */
