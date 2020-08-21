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
#include "AmaltheaConverter.h"
#include "debugFlags.h"
#include "trace_utils_BTF.h"

#include "FreeRTOS.h"
#include "task.h"
#include <stdarg.h>


/**
 * Generating Amalthea task model
 */
AmaltheaTask createAmaltheaTask(void *taskHandler,void *cInHandler,void *cOutHandler,
        unsigned int period,unsigned int deadline, unsigned int WCET,
        unsigned int src_id, unsigned int src_instance, unsigned int task_id, unsigned int task_instance)
{
    if (WCET >= period)
    {
        AmaltheaTask retValNull = {0, 0, 0, 0, NULL, 0, 0, 0, NULL, NULL};
        return retValNull;
    }else
    {
        AmaltheaTask retVal = {src_id, src_instance, task_id, task_instance,
                taskHandler, WCET, deadline, period, cInHandler, cOutHandler};
        return retVal;
    }
}



/**
 * This function returns the additional stack size (in words) needed for the task to handle its labels
 *
 */

unsigned int calculateStackSize(int labelBitCount, int labelCount)
{
    return ((labelBitCount * labelCount) / PLATFORM_WORD_LENGTH);
}


/**
 * Create the RTOS task that represents a given Amalthea task.
 */
void createRTOSTask(AmaltheaTask* task, int priority, int argCount, ...)
{
    /* cycle through the stack arguments and add the needed numbers to the stack */
    int stack_size = 0;
    /* make sure the stack size is at least big enough to run the task. */
    stack_size += configMINIMAL_STACK_SIZE;
    /* create the RTOS task with the generalized form */
    xTaskCreate((TaskFunction_t)generalizedRTOSTask, "Task", stack_size, &(*task), priority, NULL);
}


#ifdef use_LET_COMM_SEMANTICS
void generalizedRTOSTask(AmaltheaTask task){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    //task.cInHandler();
    for (;;){
        //execute cIn
        task.cInHandler();
        task.taskHandler();
        vTaskDelayUntil( &xLastWakeTime, task.period);
        task.cOutHandler();
    }
}
#else
/**
 * This RTOS task invokes the task handlers and realizes periodic task execution according to
 * Amalthea model
 */
void generalizedRTOSTask(AmaltheaTask task){
    TickType_t xLastWakeTime = xTaskGetTickCount();
    for (;;)
    {
        /* CDGen specific code changes. It will be made consistent in future. */
#ifndef CDGEN_BTF_TRACE
        traceTaskEvent(task.src_id, task.src_instance, TASK_EVENT, task.task_id,
                                task.task_instance, PROCESS_START, 0);
        task.cInHandler();
        task.taskHandler(task.task_id, task.task_instance);
        task.cOutHandler();
        traceTaskEvent(task.src_id, task.src_instance, TASK_EVENT, task.task_id,
                                task.task_instance, PROCESS_TERMINATE, 0);
        vTaskDelayUntil( &xLastWakeTime, task.period);
        task.task_instance++;
#else
        task.cInHandler();
        task.taskHandler(task.src_id, task.src_instance);
        task.cOutHandler();
        vTaskDelayUntil( &xLastWakeTime, task.period);
#endif
    }
}

#endif











