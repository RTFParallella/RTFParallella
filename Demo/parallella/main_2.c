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
	//AmaltheaTask t5ms =  createAmaltheaTask(handler5ms,cIn5ms,cOut5ms,5,5,2);
	AmaltheaTask t20ms = createAmaltheaTask(handler10msCore2,cIn10msCore2,cOut10msCore2,10,10,7);
	AmaltheaTask t40ms = createAmaltheaTask(handler20msCore2,cIn20msCore2,cOut20msCore2,20,20,10);
	//create RTOS task from templates
	//xTaskCreate(generalizedRTOSTak	,"t5ms"	,configMINIMAL_STACK_SIZE,	&t5ms	,3,NULL);
	xTaskCreate(generalizedRTOSTak	,"t21ms",configMINIMAL_STACK_SIZE,	&t20ms	,3,NULL);
	xTaskCreate(generalizedRTOSTak	,"t41ms",configMINIMAL_STACK_SIZE,	&t40ms	,2,NULL);
	vTaskStartScheduler();
	return EXIT_SUCCESS;
}
//---------------------------------------------
//end of file
