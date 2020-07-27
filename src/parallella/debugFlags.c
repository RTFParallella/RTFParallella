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

//#define USE_DMA

#ifdef USE_DMA

typedef struct btf_trace_t
{
    unsigned int time;
    unsigned int src;
    unsigned int src_instance;
    unsigned int event_type;
    unsigned int target;
    unsigned int target_instance;
    unsigned int event;
    unsigned int data;
}btf_trace;

#endif /* End of USE_DMA */

e_mutex_t m SECTION(".data_bank3");

#define MUTEX_ROW        1
#define MUTEX_COL        0

/* This buffer is used to store the legacy RTF trace. */
static unsigned int *core_buffer;

/* Variable to set the clock cycle per tick. */
unsigned int execution_time_scale;

static unsigned int scale_factor;

static unsigned int tick_count;

#ifndef USE_DMA
static unsigned int *btf_info;
static btf_trace_info *btf_data;
#endif

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
#ifndef USE_DMA
    unsigned int btf_offset = (sizeof(btf_trace_info) / sizeof(int)) + SHM_LABEL_COUNT;
    btf_data = (btf_trace_info *)allocate_shared_memory(0);
    btf_info = (unsigned int *)allocate_shared_memory(btf_offset + 1);
#endif
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



void traceTaskEvent(int srcID, int srcInstance, btf_trace_event_type type,
        int taskId, int taskInstance, btf_trace_event_name event_name, int data)
{
    unsigned int btf_offset = (sizeof(btf_trace_info) / sizeof(int)) + SHM_LABEL_COUNT;
    unsigned int active_row = 0;
    unsigned int offset = 0;
    unsigned int * trace_buf_addr = NULL;

#ifdef USE_DMA
    unsigned int *btf_info = NULL;
    btf_trace trace;
    btf_trace_info btf_data;
    /* The BTF trace data starts at 0th offset of the shared memory seen by
     * the Epiphany cores */
    unsigned int *btf_data_ptr = (unsigned int *)allocate_shared_memory(0);
    btf_info = (unsigned int *)allocate_shared_memory(btf_offset + 1);
#endif


    e_mutex_lock(MUTEX_ROW, MUTEX_COL, &m);
    /* Wait until data has been read by the host */
#ifdef USE_DMA
    do{
        e_dma_copy(&btf_data, btf_data_ptr, sizeof(btf_trace_info));
    }while(btf_data.core_write == 1);
#else
    while(btf_data->core_write == 1);
#endif

    if((type == TASK_EVENT) && (event_name == PROCESS_TERMINATE))
    {
        traceRunningTask(0);
    }
    else if ((type == TASK_EVENT) && (event_name == PROCESS_START))
    {
        traceRunningTask(taskId);
    }

#ifdef USE_DMA
    btf_data.core_id = e_get_coreid();
    active_row = (e_get_coreid() ^ 0x808) >> 6;
    offset = BTF_TRACE_BUFFER_SIZE * active_row;
    btf_data.offset = active_row;
    trace_buf_addr = btf_info + offset;
    e_dma_wait(E_DMA_1);
    trace.time = tick_count;
    trace.src = srcID;
    trace.src_instance = srcInstance;
    trace.event_type = type;
    trace.target = taskId;
    trace.target_instance = taskInstance;
    trace.event = event_name;
    trace.data = data;
    e_dma_copy(trace_buf_addr, &trace, sizeof(btf_trace));
    e_dma_wait(E_DMA_1);
    btf_data.core_write = 1;
    e_dma_copy(btf_data_ptr, &btf_data, sizeof(btf_trace_info));
#else
    btf_data->core_id = e_get_coreid();
    active_row = (e_get_coreid() ^ 0x808) >> 6;
    offset = BTF_TRACE_BUFFER_SIZE * active_row;
    btf_data->offset = active_row;
    trace_buf_addr = btf_info + offset;
    trace_buf_addr[TIME_FLAG] = tick_count;
    trace_buf_addr[SOURCE_FLAG] = srcID;
    trace_buf_addr[SOURCE_INSTANCE_FLAG] = srcInstance;
    trace_buf_addr[EVENT_TYPE_FLAG] = type;
    trace_buf_addr[TARGET_FLAG] = taskId;
    trace_buf_addr[TARGET_INSTANCE_FLAG] = taskInstance;
    trace_buf_addr[EVENT_FLAG] = event_name;
    trace_buf_addr[DATA_FLAG] = data;
    btf_data->core_write = 1;
#endif

    /* Unlock mutex and wait for the host core to read the data */
    e_mutex_unlock(MUTEX_ROW, MUTEX_COL, &m);
}


