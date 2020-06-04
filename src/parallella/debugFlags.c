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
static unsigned int *mutex;

static unsigned int *task_sync;

/* btf synchronization structure */
static btf_trace_info *btf_data;

/* This buffer is used to store the btf data */
static unsigned int *btf_info;

/* Variable to set the clock cycle per tick. */
unsigned int execution_time_scale;

static unsigned int scale_factor;

static unsigned int tick_count;

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
    mutex = allocate_shared_memory(offset + BTF_TRACE_BUFFER_SIZE);
    if ((row == 0) && (col == 0))
    {
        mutex[0] = 1;
        btf_data->host_read = 1;
    }
    else
    {
        while(btf_data->host_read == 0)
        {
            /* Wait until the mutex has been initialized */
        }
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
    task_sync = allocate_epiphany_memory(10);
    task_sync[0] = 6;
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
    tick_count = xTaskGetTickCount();
    core_buffer[TICK_FLAG] = tick_count;
}

void updateDebugFlag(int debugMessage)
{
    core_buffer[DEBUG_FLAG] = debugMessage;
}



void traceTaskEvent(int srcID, int srcInstance, btf_trace_event_type type,
        int taskId, int taskInstance, btf_trace_event_name event_name, int data)
{
    /* Lock the mutex before writing to the shared memory */
    while(mutex[0] != 1)
    {
        /* Core to core synchronization */
    }
    mutex[0] = 0;
    while((btf_data->core_write == 1))
    {
        /* Wait until previous task is completed */
    }

    if((type == TASK_EVENT) && (event_name == PROCESS_TERMINATE))
    {
        traceRunningTask(0);
    }
    else if ((type == TASK_EVENT) && (event_name == PROCESS_START))
    {
        traceRunningTask(taskId);
    }
    /* Add a delay to stabilize the mutex. Epiphany core does not have
     * a deterministic behavior if no delay is added */
    btf_info[TIME_FLAG] = tick_count;
    btf_info[SOURCE_FLAG] = srcID;
    btf_info[SOURCE_INSTANCE_FLAG] = srcInstance;
    btf_info[EVENT_TYPE_FLAG] = type;
    btf_info[TARGET_FLAG] = taskId;
    btf_info[TARGET_INSTANCE_FLAG] = taskInstance;
    btf_info[EVENT_FLAG] = event_name;
    btf_info[DATA_FLAG] = data;
    btf_data->core_write = 1;
    /* Wait until data has been read by the host */
    while(btf_data->core_write == 1);
    /* Unlock mutex and wait for the host core to read the data */
    mutex[0] = 1;
}


