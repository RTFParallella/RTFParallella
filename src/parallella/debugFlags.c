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

#include <e-lib.h>
#include "debugFlags.h"
#include "FreeRTOS.h"
#include "task.h"

unsigned int trace_buffer[1024] __attribute__((section(".data_bank3")));
unsigned int execution_time_scale;
unsigned int* time_scale = (unsigned int *)0x8f000014;
/*
 * initialize output buffer with the addresses to array elements
 */
void init_task_trace_buffer(void )
{
    execution_time_scale = time_scale[0];
    /* initialize buffer */
    int index;
    /* The first */
    for (index = 0;index < 10; index++)
    {
        trace_buffer[index] = 0;
    }
}

void traceRunningTask(unsigned taskNum){

    trace_buffer[RUNNINGTASK_FLAG] = taskNum;
}

void traceTaskPasses(unsigned taskNum, int currentPasses){
    if (taskNum == 1){
        trace_buffer[TASK1_FLAG] = currentPasses;
    }else if (taskNum == 2){
        trace_buffer[TASK2_FLAG] = currentPasses;
    }else if (taskNum == 3){
        trace_buffer[TASK3_FLAG] = currentPasses;
    }
}

void updateTick(void){
    trace_buffer[TICK_FLAG] = xTaskGetTickCount();
}

void updateDebugFlag(int debugMessage){
    trace_buffer[DEBUG_FLAG] = debugMessage;
}







