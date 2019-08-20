/*******************************************************************************
 *  * Copyright (c) 2019 Dortmund University of Applied Sciences and Arts and others.
 *  * 
 *  * This program and the accompanying materials are made
 *  * available under the terms of the Eclipse Public License 2.0
 *  * which is available at https://www.eclipse.org/legal/epl-2.0/
 *  * 
 *  * SPDX-License-Identifier: EPL-2.0
 *  * 
 *  * Contributors:
 *  *     Dortmund University of Applied Sciences and Arts - initial API and implementation
 *******************************************************************************/
//epiphany imports
#include <stdlib.h>

#include "../rtfparallella_framework/shared_comms.h"
#include "e_lib.h"
#include "taskCode.h"
//freeRTOS imports
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "AmaltheaConverter.h"
#include "c2c.h"
#include "debugFlags.h"
//utility imports
//---------------------------------------------
int main(void) {
	//initialize output buffer for debug messages
	outbuf_init();
	shared_labels_init();
	shared_labels_init_core();
	//create Amalthea task objects
	AmaltheaTask t20ms = createAmaltheaTask(handler10msCore2,cIn10msCore2,cOut10msCore2,10,10,7);
	AmaltheaTask t40ms = createAmaltheaTask(handler20msCore2,cIn20msCore2,cOut20msCore2,20,20,10);
	//create RTOS task from templates
	createRTOSTask(&t20ms,2,0);
	createRTOSTask(&t40ms,1,0);
	vTaskStartScheduler();
	return EXIT_SUCCESS;
}
//---------------------------------------------
//end of file
