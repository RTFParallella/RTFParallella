/*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "e_lib.h"
//special import 

#include "test.h"

//freeRTOS imports
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//#include "c2c.h"
//communication offsets
#define cnt_address 0x80803000
unsigned int *outbuf[10];
//internal offsets
#define _1MS 700000
#define _1US 700

long clockTicks = 0;

bool timeDone= false;

int firstTime=0;
//Task handles
//function declaration
void hi(void *pvParameters);
void hi2(void *pvParameters);
void hi3(void *pvParameters);
void sleepTimerMs(int ticks);
void timer1init();
void __attribute__ ((interrupt)) sleepTimerFlagToggle();
void rms_handler(void *pvParameters);

//-----------------------------------------------------------------
int passes1 = 0;
int passes2 = 0;
int passes3 = 0;
//----------------------------------------------------------
//define new host output structure
char ShmName[] = "printfOutBuf";
char taskHoldingCore[] = "task %d is holding the core presently";
char buf[256] = {0};
e_memseg_t emem;
int runningTaskID;

unsigned          my_row;
unsigned          my_col;
void printString(void);
//---------------------------------------------------
int main(void) {
	//attach shared memory name to corresponding segment

	//start customRTOS kernel
	//debug interface in shared memory
	//printString();
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
	if ( E_OK != e_shm_attach(&emem, ShmName) ) {
		*outbuf[1] = 300;
			return EXIT_FAILURE;
		}
		e_coreid_t coreid;
		//get core IDs
		coreid = e_get_coreid();
		e_coords_from_coreid(coreid, &my_row, &my_col);
		snprintf(buf, sizeof(buf), taskHoldingCore, 1);
		e_write((void*)&emem, buf, my_row, my_col, NULL, strlen(buf) + 1);
	//timer1init();
	for (i=0;i<9;i++){
		*outbuf[i] =0;
	}
	//check this
	testFunction();



	//timer initialization
	xTaskCreate( hi, "T1", 512, NULL, 4, NULL );
	//xTaskCreate( hi2,"T2", 512, NULL, 3, NULL );
	//xTaskCreate( hi3,"T3", 512, NULL, 2, NULL );
	//xTaskCreate(rms_handler,"RMS_TASK",256,NULL,configMAX_PRIORITIES,NULL);

	vTaskStartScheduler();
	*outbuf[3] = 0;
	return EXIT_SUCCESS;
}
//task 5ms handler and helper functions
void hi(void *pvParameters){
	TickType_t xLastWakeTime;


	for(;;){
		//printString();
		snprintf(buf, sizeof(buf), taskHoldingCore, 1);
			e_write((void*)&emem, buf, my_row, my_col, NULL, strlen(buf) + 1);
		//*outbuf[6] =1;
		passes1++;
		//*outbuf[2] =passes1;
		//*outbuf[7] =700;
		//printString();
		//sleepTimerMs(1);
		//vTaskDelayUntil( &xLastWakeTime, 5);
		//*outbuf[7] =701;
	}
}


void hi2(void *pvParameters){
	TickType_t xLastWakeTime;
	for(;;){
		*outbuf[6] =2;
		passes2++;
		*outbuf[0] = passes2;
		*outbuf[7] =800;
		sleepTimerMs(2);
		vTaskDelayUntil( &xLastWakeTime,  10);
		*outbuf[7] =801;
	}
}

void hi3(void *pvParameters){
	TickType_t xLastWakeTime;
	for(;;){
		*outbuf[6] =3;
		passes3++;
		*outbuf[4] = passes3;
		*outbuf[7] =899;
		sleepTimerMs(3);
		vTaskDelayUntil( &xLastWakeTime, 20);
		*outbuf[7] =900;
	}
}
//---------------------------------------------------------------------
void sleepTimerMs(int ticks){
	*outbuf[7] =ticks*10;
	e_wait(E_CTIMER_0,ticks*_1US);
	*outbuf[7] =(ticks*1000)+1;
}

void __attribute__ ((interrupt)) sleepTimerFlagToggle(){
	timeDone = true;
}

//---------------------------------------------------------------------
void timer1init(){
	e_irq_attach(E_TIMER1_INT, sleepTimerFlagToggle);
	e_irq_mask(E_TIMER1_INT, E_FALSE);

}


void vOtherFunction( TaskFunction_t pxTaskCode )
{
BaseType_t xReturned;
TaskHandle_t xHandle = NULL;

     Create the task, storing the handle.
    xReturned = xTaskCreate(
                    pxTaskCode,        Function that implements the task.
                    "RUNNING_TASK",           Text name for the task.
                    512,       Stack size in words, not bytes.
                    ( void * ) 1,     Parameter passed into the task.
                    tskIDLE_PRIORITY, Priority at which the task is created.
                    &xHandle );       Used to pass out the created task's handle.

    if( xReturned == pdPASS )
    {
         The task was created.  Use the task's handle to delete the task.
        vTaskDelete( xHandle );
    }
}

void rms_handler(void *pvParameters){

	for (;;){
		//traverse task handle linked list
		//get the task with highest local prio
		//
	}
}

void printString(void){
	snprintf(buf, sizeof(buf), taskHoldingCore, 1);
	e_write((void*)&emem, buf, my_row, my_col, NULL, strlen(buf) + 1);

}


//void printString(char *string []){
//	int size=sizeof(string);
//	int i;
//	for(i=0;i<size;i++){
//		*outbuf[0] = string[i];
//	}
//}

*/







/*
  e_hello_world.c

  Copyright (C) 2012 Adapteva, Inc.
  Contributed by Yaniv Sapir <yaniv@adapteva.com>

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program, see the file COPYING.  If not, see
  <http://www.gnu.org/licenses/>.
*/

// This is the DEVICE side of the Hello World example.
// The host may load this program to any eCore. When
// launched, the program queries the CoreID and prints
// a message identifying itself to the shared external
// memory buffer.

//C includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//E-SDK includes
#include "e_lib.h"
//FreeRTOS includes
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "printUtil.h"


//internal offsets
#define _1MS 700000
#define _1US 700
//task declaration
void hi(void *pvParameters);
void hi2(void *pvParameters);
void hi3(void *pvParameters);
void hi4(void *pvParameters);
void schedulerTask(void *pvParameters);

//utility function declaration
void sleepTimerMs(int ticks);
//void printToHost(char printBuf[]);
//task instance counters
int passes1 = 0;
int passes2 = 0;
int passes3 = 0;
int passes4 = 0;
/*//communication setup
const char		  ShmName[] = "hello_shm";
const char        Msg[] = "Hello World from core 0x%03x!";
char              buf[256] = { 0 };
e_coreid_t		  coreid;
e_memseg_t   	  emem;
unsigned          my_row;
unsigned          my_col;*/




TaskHandle_t task5ms_handle;
TaskHandle_t task10ms_handle;
TaskHandle_t task20ms_handle;
TaskHandle_t task40ms_handle;
//TaskHandle_t tasksched_handle;

int main(void) {
	// Who am I? Query the CoreID from hardware.
/*
	coreid = e_get_coreid();
	e_coords_from_coreid(coreid, &my_row, &my_col);

	if ( E_OK != e_shm_attach(&emem, ShmName) ) {
		return EXIT_FAILURE;
	}

	// Attach to the shm segment
	snprintf(buf, sizeof(buf), Msg, coreid);
*/

	/*if ( emem.size >= strlen(buf) + 1 ) {
		// Write the message (including the null terminating
		// character) to shared memory
		//e_write((void*)&emem, buf, my_row, my_col, NULL, strlen(buf) + 1);
	} else {
		// Shared memory region is too small for the message
		return EXIT_FAILURE;
	}*/
	initializePrintChannel();
	xTaskCreate( hi	, "T1", configMINIMAL_STACK_SIZE, NULL, 5,task5ms_handle );
	xTaskCreate( hi2, "T2", configMINIMAL_STACK_SIZE, NULL, 4, task10ms_handle);
	xTaskCreate( hi3, "T3", configMINIMAL_STACK_SIZE, NULL, 3, task20ms_handle );
	xTaskCreate( hi4, "T4", configMINIMAL_STACK_SIZE, NULL, 2, task40ms_handle );
	vTaskStartScheduler();
	//hi(NULL);

	return EXIT_SUCCESS;
}


void hi(void *pvParameters){
	TickType_t xLastWakeTime;
	for(;;){
		passes1++;
		char bufLocal[] = "task 1: instance# %2d  holding core at tick %2d";
		TickType_t currentTickCount;
		taskENTER_CRITICAL();
		currentTickCount = xTaskGetTickCount();//*passes1;
		snprintf(bufLocal, sizeof(bufLocal), bufLocal,passes1, currentTickCount);
		printToHost(bufLocal);
		taskEXIT_CRITICAL();
		//e_write((void*)&emem, bufLocal, my_row, my_col, NULL, strlen(bufLocal) + 1);
		sleepTimerMs(pdMSTOTICKS(2));
		//vTaskDelay(10);
		vTaskDelayUntil( &xLastWakeTime, pdMSTOTICKS(5));
	}
}

void hi2(void *pvParameters){
	TickType_t xLastWakeTime;
	for(;;){
		passes2++;
		char bufLocal[] = "task 2: instance# %2d  holding core at tick %2d";
		TickType_t currentTickCount;
		taskENTER_CRITICAL();
		currentTickCount = xTaskGetTickCount();//*passes2;
		snprintf(bufLocal, sizeof(bufLocal), bufLocal,passes2, currentTickCount);
		printToHost(bufLocal);
		taskEXIT_CRITICAL();
		sleepTimerMs(pdMSTOTICKS(4));
		vTaskDelayUntil( &xLastWakeTime, pdMSTOTICKS(10));
	}
}

void hi3(void *pvParameters){
	TickType_t xLastWakeTime;
	for(;;){
		passes3++;
		char bufLocal[] = "task 3: instance# %2d  holding core at tick %2d";
		TickType_t currentTickCount;
		taskENTER_CRITICAL();
		currentTickCount = xTaskGetTickCount();//*passes3;
		snprintf(bufLocal, sizeof(bufLocal), bufLocal,passes3, currentTickCount);
		printToHost(bufLocal);
		taskEXIT_CRITICAL();
		//e_write((void*)&emem, bufLocal, my_row, my_col, NULL, strlen(bufLocal) + 1);
		sleepTimerMs(pdMSTOTICKS(8));
		vTaskDelayUntil( &xLastWakeTime, pdMSTOTICKS(20));
	}
}
void hi4(void *pvParameters){
	TickType_t xLastWakeTime;
	for (;;){
		passes4++;
		char bufLocal[] = "task 4: instance# %2d  holding core at tick %2d";
		TickType_t currentTickCount;
		taskENTER_CRITICAL();
		currentTickCount = xTaskGetTickCount();//*passes3;
		snprintf(bufLocal, sizeof(bufLocal), bufLocal,passes4, currentTickCount);
		printToHost(bufLocal);
		taskEXIT_CRITICAL();
		//e_write((void*)&emem, bufLocal, my_row, my_col, NULL, strlen(bufLocal) + 1);
		sleepTimerMs(pdMSTOTICKS(16));
		vTaskDelayUntil( &xLastWakeTime, pdMSTOTICKS(40));
	}
}

void traceTask(void *pvParameters){



}


void sleepTimerMs(int ticks){
	int var;
	for (var = 0; var < ticks; ++var) {
		//var*var*var;
		//e_wait(E_CTIMER_0,_1MS);
        taskENTER_CRITICAL();
        vTaskDelay(pdMSTOTICKS(1));
        taskEXIT_CRITICAL();
		//xTaskIncrementTick();
        //vTaskSwitchContext();
		//hello
	}
}



