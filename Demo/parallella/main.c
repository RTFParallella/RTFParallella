#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "e_lib.h"


//freeRTOS imports
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//#include "c2c.h"

#include "debugFlags.h"


//communicaqtion offsets
//#define cnt_address 0x80803000
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
void sleepTimerMs(int ticks,int taskNum);
void timer1init();
void __attribute__ ((interrupt)) sleepTimerFlagToggle();

//-----------------------------------------------------------------
int passes1 = 0;
int passes2 = 0;
int passes3 = 0;
//----------------------------------------------------------

//---------------------------------------------------
int main(void) {
	//start customRTOS kernel
	//debug interface in shared memory
/*	outbuf[0] = (unsigned int *) cnt_address;
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
	}*/
	outbuf_init();
	//timer initialization
	xTaskCreate( hi, "T1", 0x200, NULL, 3, NULL );
	xTaskCreate( hi2,"T2", 0x200, NULL, 2, NULL );
	xTaskCreate( hi3,"T3", 0x200, NULL, 1, NULL );
	vTaskStartScheduler();
	//*outbuf[3] = 0;
	return EXIT_SUCCESS;
}
//task 5ms handler and helper functions
void hi(void *pvParameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for(;;){
		//*outbuf[7] =700;
		updateDebugFlag(700);
		sleepTimerMs(2,1);
		passes1++;
		//*outbuf[6] = 0;
		traceRunningTask(0);
		//*outbuf[2] =passes1;
		traceTaskPasses(1,passes1);
		vTaskDelayUntil( &xLastWakeTime, 5*1);
		//*outbuf[7] =701;
		updateDebugFlag(700);
	}
}


void hi2(void *pvParameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for(;;){
		*outbuf[7] =800;
		sleepTimerMs(3,2);
		passes2++;
		*outbuf[0] = passes2;
		*outbuf[6] = 0;
		vTaskDelayUntil( &xLastWakeTime,  10*1);
		*outbuf[7] =801;

	}
}

void hi3(void *pvParameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for(;;){
		*outbuf[7] =899;
		sleepTimerMs(5,3);
		passes3++;
		*outbuf[6] = 0;
		*outbuf[4] = passes3;
		vTaskDelayUntil( &xLastWakeTime, 20*1);
		*outbuf[7] =900;
	}
}
//---------------------------------------------------------------------
void sleepTimerMs(int ticks, int taskNum){
	*outbuf[7] =ticks*10;
	int i;
	for (i=0;i<ticks;i++){
		taskENTER_CRITICAL();
		//*outbuf[6] = taskNum;
		traceRunningTask(taskNum);
		e_wait(E_CTIMER_0,_1MS);
		taskEXIT_CRITICAL();
	}
}

void __attribute__ ((interrupt)) sleepTimerFlagToggle(){
	timeDone = true;
}

//---------------------------------------------------------------------
void timer1init(){
	e_irq_attach(E_TIMER1_INT, sleepTimerFlagToggle);
	e_irq_mask(E_TIMER1_INT, E_FALSE);

}
