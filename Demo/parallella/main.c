//epiphany imports
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "e_lib.h"
//freeRTOS imports
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//utility imports
#include "debugFlags.h"
#include "AmaltheaConverter.h"
#include "taskCode.h"
//---------------------------------------------
int main(void) {
	//initialize output buffer for debug messages
	outbuf_init();
	//create Amalthea task objects
	AmaltheaTask t5ms =  createAmaltheaTask(handler5ms,cIn5ms,cOut5ms,5,5,2);
	AmaltheaTask t10ms = createAmaltheaTask(handler10ms,cIn10ms,cOut10ms,10,10,3);
	AmaltheaTask t20ms = createAmaltheaTask(handler20ms,cIn20ms,cOut20ms,20,20,5);
	//create RTOS task from templates
	xTaskCreate(generalizedRTOSTak	,"t5ms"	,configMINIMAL_STACK_SIZE,	&t5ms	,3,NULL);
	xTaskCreate(generalizedRTOSTak	,"t10ms",configMINIMAL_STACK_SIZE,	&t10ms	,2,NULL);
	xTaskCreate(generalizedRTOSTak	,"t20ms",configMINIMAL_STACK_SIZE,	&t20ms	,1,NULL);
	vTaskStartScheduler();
	return EXIT_SUCCESS;
}
//---------------------------------------------
//end of file
