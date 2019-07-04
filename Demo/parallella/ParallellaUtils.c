/*
 * ParallellaUtils.c
 *
 *  Created on: Jul 3, 2019
 *      Author: bigm
 */

#include "ParallellaUtils.h"


void sleepTimerMs(int ticks, int taskNum){
	updateDebugFlag(ticks*10);
	int i;
	for (i=0;i<ticks;i++){
		taskENTER_CRITICAL();
		traceRunningTask(taskNum);
		e_wait(E_CTIMER_0,_1MS);
		taskEXIT_CRITICAL();
	}
}

