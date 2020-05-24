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
#include "RTFParallellaConfig.h"
#include "debugFlags.h"
#include "FreeRTOS.h"
#include "task.h"

/**
 * This buffer is assigned to stored the RTF parallella legacy trace info. Data bank
 * 3 is used to store the information on each epiphany core. It starts at 0x6000 offset
 * on each epiphany core. Any change in this buffer addressing must be followed with
 * the correct offset set in host application to get the correct values.
 */
static unsigned int trace_buffer[32] __attribute__((section(".data_bank3")));

/* Variable to set the clock cycle per tick. */
unsigned int execution_time_scale;

static unsigned int scale_factor;

unsigned int get_time_scale_factor(void)
{
    return (scale_factor > 0 ) ? scale_factor : 1;
}

/**
 * Gets the execution time from the global memory. The global assigned address in shared
 * memory is 0x8F000014. The shared DRAM buffer starts at 0x8F000000. The first 20 bytes
 * is used by the port.c file in FreeRTOS to store some interrupt information.
 */
static void get_execution_time_scale(void)
{
    /* 1000 corresponds to 1 ms which is considered as max resolution */
    uint32_t max_resolution = 1000;
    unsigned int* time_scale = (unsigned int *)(SHARED_DRAM_START_ADDRESS |
                                                SHARED_DRAM_START_OFFSET  |
                                                INPUT_TIMESCALE_OFFSET);
    scale_factor = ( max_resolution / time_scale[0] );
    execution_time_scale = ( scale_factor * max_resolution );
}

/*
 * initialize output buffer with the addresses to array elements
 */
void init_task_trace_buffer(void )
{
    get_execution_time_scale();
    /* Initialize buffer */
    int index;
    /* The first 10 values are used to store the legacy RTF information. */
    for (index = 0;index < RTF_DEBUG_TRACE_COUNT; index++)
    {
        trace_buffer[index] = 0;
    }
}

void traceRunningTask(unsigned taskNum)
{
    trace_buffer[RUNNINGTASK_FLAG] = taskNum;
}

void traceTaskPasses(unsigned taskNum, int currentPasses)
{
    if (taskNum == 1)
    {
        trace_buffer[TASK1_FLAG] = currentPasses;
    }
    else if (taskNum == 2)
    {
        trace_buffer[TASK2_FLAG] = currentPasses;
    }
    else if (taskNum == 3)
    {
        trace_buffer[TASK3_FLAG] = currentPasses;
    }
}

void updateTick(void)
{
    trace_buffer[TICK_FLAG] = xTaskGetTickCount();
}

void updateDebugFlag(int debugMessage)
{
    trace_buffer[DEBUG_FLAG] = debugMessage;
}


