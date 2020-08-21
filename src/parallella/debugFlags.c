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

/**< Mutex declaration for synchronization within cores */
static e_mutex_t m;

/**< This buffer is used to store the legacy RTF trace. */
static unsigned int *core_buffer;

/* Variable to set the clock cycle per tick. */
unsigned int execution_time_scale;

/**< Time scale factor per tick */
static unsigned int scale_factor;

static unsigned int tick_count;

static unsigned char buffer_offset;

static unsigned int *btf_info;

static btf_trace_info *btf_data;

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


void init_btf_mem_section(void)
{
    unsigned int offset = SHARED_BTF_DATA_OFFSET/sizeof(int);
    unsigned int btf_offset = (sizeof(btf_trace_info) / sizeof(int)) + offset;
    btf_data = (btf_trace_info *)allocate_shared_memory(offset);
    btf_info = (unsigned int *)allocate_shared_memory(btf_offset);
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
    tick_count = xTaskGetTickCount();
    core_buffer[TICK_FLAG] = tick_count;
}

void updateDebugFlag(int debugMessage)
{
    core_buffer[DEBUG_FLAG] = debugMessage;
}


/** Signal the host to read the BTF trace metadata */
void signalHost(void)
{
    btf_trace_info btf_data_info;
    unsigned int active_row = (e_get_coreid() ^ 0x808) >> 6;
    e_mutex_lock(MUTEX_ROW, MUTEX_COL, &m);
    do{
        e_dma_copy(&btf_data_info, btf_data, sizeof(btf_trace_info));
    }while(btf_data_info.core_write == 1);
    btf_data_info.offset = BTF_TRACE_BUFFER_SIZE * (active_row * RING_BUFFER_SIZE);
    btf_data_info.core_id = active_row;
    btf_data_info.length = buffer_offset;
    btf_data_info.core_write = 1;
    e_dma_copy(btf_data, &btf_data_info, sizeof(btf_trace_info));
    e_mutex_unlock(MUTEX_ROW, MUTEX_COL, &m);
    buffer_offset = 0;
}


/* Dump the BTF trace event */
void traceTaskEvent(int srcID, int srcInstance, btf_trace_event_type type,
        int taskId, int taskInstance, btf_trace_event_name event_name, int data)
{
    unsigned char active_row = 0;
    unsigned short offset = 0;
    unsigned char index = 0;
    unsigned int * trace_buf_addr = NULL;
    btf_trace_data trace;

    if((type == TASK_EVENT) && (event_name == PROCESS_TERMINATE))
    {
        traceRunningTask(0);
    }
    else if ((type == TASK_EVENT) && (event_name == PROCESS_START))
    {
        traceRunningTask(taskId);
    }

    active_row = (e_get_coreid() ^ 0x808) >> 6;
    index = buffer_offset % RING_BUFFER_SIZE;
    if (index == 0){
        buffer_offset = 0;
    }
    offset = BTF_TRACE_BUFFER_SIZE * ((active_row * RING_BUFFER_SIZE) + index) ;
    buffer_offset++;
    trace_buf_addr = btf_info + offset;
    trace.ticks = tick_count;
    trace.srcId = srcID;
    trace.srcInstance = srcInstance;
    trace.eventTypeId = type;
    trace.taskId = taskId;
    trace.taskInstance = taskInstance;
    trace.eventState = event_name;
    trace.data = data;
    e_dma_copy(trace_buf_addr, &trace, sizeof(btf_trace_data));
}


