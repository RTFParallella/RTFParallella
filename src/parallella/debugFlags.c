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

#include "debugFlags.h"

#include "FreeRTOS.h"
#include "task.h"

unsigned int *outbuf[10];

//#ifdef RFTP_GENERATE_BTF_TRACE
unsigned int *btf_trace_buf[BTF_TRACE_BUFFER_SIZE];
static unsigned int counter;


static void btf_trace_buf_init(void);
//#endif
/*
 * initialize output buffer with the addresses to array elements
 */
void outbuf_init(void ){
    //This will be the same on each core
    //debug interface in shared memory
    outbuf[0] = (unsigned int *) cnt_address;
    outbuf[1] = outbuf[0] + 1;
    outbuf[2] = outbuf[1] + 1;
    outbuf[3] = outbuf[2] + 1;
    outbuf[4] = outbuf[3] + 1;
    outbuf[5] = outbuf[4] + 1;
    outbuf[6] = outbuf[5] + 1;
    outbuf[7] = outbuf[6] + 1;
    outbuf[8] = outbuf[7] + 1;
    *outbuf[1] = 200;
    //initialize buffer
    int i;
    //timer1init();
    for (i=0;i<9;i++){
        *outbuf[i] = 0;
    }
//#ifdef RFTP_GENERATE_BTF_TRACE
    btf_trace_buf_init();
//#endif
}


//#ifdef RFTP_GENERATE_BTF_TRACE
/**
 * Function to initialize the BTF trace buffer.
 *
 * Arguments:
 * void
 *
 * Return: void
 */
static void btf_trace_buf_init(void)
{
    int index;
    btf_trace_buf[0] = (unsigned int *) btf_trace_address;
    //initialize buffer
    for (index = 0;index < BTF_TRACE_BUFFER_SIZE; index++){
        *btf_trace_buf[index] = 0x00;
    }
}

//#endif

void traceRunningTask(unsigned taskNum){
    *outbuf[RUNNINGTASK_FLAG] = taskNum;
}

void traceTaskPasses(unsigned taskNum, int currentPasses){
    if (taskNum == 1){
        *outbuf[TASK1_FLAG] = currentPasses;
    }else if (taskNum == 2){
        *outbuf[TASK2_FLAG] = currentPasses;
    }else if (taskNum == 3){
        *outbuf[TASK3_FLAG] = currentPasses;
    }
}

void updateTick(void){
    *outbuf[TICK_FLAG] = xTaskGetTickCount();
    *btf_trace_buf[TIME_FLAG] = xTaskGetTickCount();
}

void updateDebugFlag(int debugMessage){
    *outbuf[DEBUG_FLAG] = debugMessage;
    //*btf_trace_buf[SOURCE_FLAG] = counter++;
    //*btf_trace_buf[SOURCE_INSTANCE_FLAG] = counter++;
    //*btf_trace_buf[EVENT_TYPE_FLAG] = counter++;
    //*btf_trace_buf[TARGET_FLAG] = counter++;
    //*btf_trace_buf[TARGET_INSTANCE_FLAG] = counter++;
    //*btf_trace_buf[EVENT_FLAG] = counter++;
    //*btf_trace_buf[DATA_FLAG] = counter++;
}

//#ifdef RFTP_GENERATE_BTF_TRACE
void updateBTFTraceBuffer(int srcID, int srcInstance, btf_trace_event_type type,
        int targetId, int targetInstance, btf_trace_event_name event_name, int data)
{
    //*btf_trace_buf[TIME_FLAG] = xTaskGetTickCount();
    *btf_trace_buf[SOURCE_FLAG] = srcID;
    *btf_trace_buf[SOURCE_INSTANCE_FLAG] = srcInstance;
    *btf_trace_buf[EVENT_TYPE_FLAG] = type;
    *btf_trace_buf[TARGET_FLAG] = targetId;
    *btf_trace_buf[TARGET_INSTANCE_FLAG] = targetInstance;
    *btf_trace_buf[EVENT_FLAG] = event_name;
    *btf_trace_buf[DATA_FLAG] = data;
}
//#endif



