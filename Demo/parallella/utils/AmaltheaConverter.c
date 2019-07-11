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
	/*TickType_t xLastWakeTime = xTaskGetTickCount();
		for (;;){
			//traceTaskStatus(1,task->executionTime);
			//execute cIn
			//task->cInHandler();
			//task->taskHandler();
			sleepTimerMs(task->executionTime,task->executionTime);
			//task->cOutHandler();
			traceTaskStatus(0,task->executionTime);
			vTaskDelayUntil( &xLastWakeTime, task->period);
		}*/
}

/*
void createRTOSTask(AmaltheaTask *task){
	xTaskCreate(generalizedRTOSTak,"T1",configMINIMAL_STACK_SIZE,&task,4,NULL);
}
*/




