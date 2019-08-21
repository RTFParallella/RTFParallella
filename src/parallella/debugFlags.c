/*******************************************************************************
 * Copyright (c) 2019 Dortmund University of Applied Sciences and Arts and others.
 *
 * This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License 2.0
 * which is available at https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *      Dortmund University of Applied Sciences and Arts - initial API and implementation
 *******************************************************************************/

#include "debugFlags.h"

#include "FreeRTOS.h"
#include "task.h"


unsigned int *outbuf[10];

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
		*outbuf[i] =0;
	}
}

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
}

void updateDebugFlag(int debugMessage){
	*outbuf[DEBUG_FLAG] = debugMessage;
}






