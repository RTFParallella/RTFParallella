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
#include "c2c.h"
#include "shared_comms.h"
#include "trace_utils_BTF.h"

#include "FreeRTOS.h"
#include "task.h"

/* This buffer is used to store the legacy RTF trace. */
static unsigned int *core_buffer;

/* Mutex lock for core to core synchronization */
static e_mutex_t *mutex;

/* btf synchronization structure */
static btf_trace_info *btf_data;

/* This buffer is used to store the btf data */
static unsigned int *btf_info;

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


void init_mutex(unsigned int row, unsigned int col, unsigned int core_id)
{
    /* The BTF trace data starts at 0th offset of the shared memory seen by
     * the Epiphany cores */
    btf_data = (btf_trace_info *)allocate_shared_memory(0);
    unsigned int offset = (sizeof(btf_trace_info) / sizeof(int)) + SHM_LABEL_COUNT;
    btf_info = (unsigned int *)allocate_shared_memory(offset);
    /* initialize mutex in core 0 */
    if (core_id == 0)
    {
        mutex = allocate_epiphany_memory(EPI_CORE_MUTEX_OFFSET);
        btf_data->mutex = e_get_global_address(row, col, mutex);
        e_mutex_init(row, col, btf_data->mutex, NULL);
        btf_data->is_init_done = 1;
    }
    else
    {
        while(btf_data->is_init_done == 0);
    }
}

/*
 * initialize output buffer with the addresses to array elements
 */
void init_task_trace_buffer(void )
{
    get_execution_time_scale();
    /* Initialize buffer */
    int index;
    /* The core buffer starts at the 0th offset of memory bank 3 of each core */
    core_buffer = allocate_epiphany_memory(0);
    /* The first 10 values are used to store the legacy RTF information. */
    for (index = 0;index < RTF_DEBUG_TRACE_COUNT; index++)
    {
        core_buffer[index] = 0;
    }
}

void traceRunningTask(unsigned taskNum)
{
    core_buffer[RUNNINGTASK_FLAG] = taskNum;
}

void traceTaskPasses(unsigned taskNum, int currentPasses)
{
    if (taskNum == 1)
    {
        core_buffer[TASK1_FLAG] = currentPasses;
    }
    else if (taskNum == 2)
    {
        core_buffer[TASK2_FLAG] = currentPasses;
    }
    else if (taskNum == 3)
    {
        core_buffer[TASK3_FLAG] = currentPasses;
    }
}

void updateTick(void)
{
    core_buffer[TICK_FLAG] = xTaskGetTickCount();
}

void updateDebugFlag(int debugMessage)
{
    core_buffer[DEBUG_FLAG] = debugMessage;
}

void traceTaskEvent(int srcID, int srcInstance, btf_trace_event_type type,
        int taskId, int taskInstance, btf_trace_event_name event_name, int data)
{
    int delay = 0;
    unsigned int *btf_info;
    /* Lock the mutex before writing to the shared memory */
    e_mutex_lock(0, 0, btf_data->mutex);
    /* Add a delay to stabilize the mutex. Epiphany core does not have
     * a deterministic behavior if no delay is added */
    for (delay = 0; delay < 500000; delay++);
    btf_info[TIME_FLAG] = core_buffer[TICK_FLAG];
    btf_info[SOURCE_FLAG] = srcID;
    btf_info[SOURCE_INSTANCE_FLAG] = srcInstance;
    btf_info[EVENT_TYPE_FLAG] = type;
    btf_info[EVENT_TYPE_FLAG] = taskId;
    btf_info[EVENT_TYPE_FLAG] = taskInstance;
    btf_info[EVENT_TYPE_FLAG] = event_name;
    btf_info[EVENT_TYPE_FLAG] = data;
    /* Unlock mutex and wait for the host core to read the data */
    e_mutex_unlock(0, 0, btf_data->mutex);
    while(btf_data->rw_operation == 1);
}


