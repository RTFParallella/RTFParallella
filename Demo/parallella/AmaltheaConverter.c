/*
 * AmaltheaConverter.c
 *
 *  Created on: Jun 23, 2019
 *      Author: bigm
 */


#include "AmaltheaConverter.h"

#include "debugFlags.h"
#include "FreeRTOS.h"
#include "task.h"

AmaltheaTask createAmaltheaTask(void *taskHandler,void *cInHandler,void *cOutHandler,unsigned int period,unsigned int deadline, unsigned int WCET){
	if (WCET >= period){
		AmaltheaTask retValNull = {0,0,NULL,0,0,0,NULL,NULL};
		return retValNull;
	}else{
		AmaltheaTask retVal = {0,0,taskHandler,WCET,deadline,period,cInHandler,cOutHandler};
		return retVal;
	}
}

void generalizedRTOSTak(AmaltheaTask task){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for (;;){
		//execute cIn
		task.cInHandler();
		task.taskHandler();
		task.cOutHandler();
		vTaskDelayUntil( &xLastWakeTime, task.period);
	}
}




