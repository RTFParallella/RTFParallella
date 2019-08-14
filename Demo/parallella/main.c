//epiphany imports
#include <stdlib.h>
#include "e_lib.h"
//freeRTOS imports
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//utility imports
#include "debugFlags.h"
#include "AmaltheaConverter.h"
#include "taskCode.h"
#include "shared_comms.h"
#include "c2c.h"

//---------------------------------------------
int main(void) {
	//initialize output buffer for debug messages
	outbuf_init();
	shared_labels_init();
	shared_labels_init_core();
	//create Amalthea task objects
	AmaltheaTask t5ms =  createAmaltheaTask(handler5ms,cIn5ms,cOut5ms,5,5,2);
	AmaltheaTask t10ms = createAmaltheaTask(handler10ms,cIn10ms,cOut10ms,10,10,3);
	AmaltheaTask t20ms = createAmaltheaTask(handler20ms,cIn20ms,cOut20ms,20,20,5);
	AmaltheaTask taskList[] = {t5ms,t10ms,t20ms};
	/*int priorities[numTasks];
	int min = 100;
	int minIndx;
	for (int i=0;i<numTasks;i++){
		if (taskList[i].period<min){
			minIndx = i;
		}
	}*/
	//create RTOS task from templates
	createRTOSTask(&t5ms,3,2,16,10,8,10);
	createRTOSTask(&t10ms,2,2,16,10,8,10);
	createRTOSTask(&t20ms,1,2,16,10,8,10);

	/*xTaskCreate(generalizedRTOSTak	,"t5ms"	,configMINIMAL_STACK_SIZE,	&t5ms	,3,NULL);
	xTaskCreate(generalizedRTOSTak	,"t10ms",configMINIMAL_STACK_SIZE,	&t10ms	,2,NULL);
	xTaskCreate(generalizedRTOSTak	,"t20ms",configMINIMAL_STACK_SIZE,	&t20ms	,1,NULL);*/
	vTaskStartScheduler();
	return EXIT_SUCCESS;
}
//---------------------------------------------
//end of file
