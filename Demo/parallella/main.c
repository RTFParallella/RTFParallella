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
//---------------------------------------------
//time units in processor cycles
#define _1MS 700000
#define _1US 700
//---------------------------------------------
//Task handlers
void task5ms(void *pvParameters);
void task10ms(void *pvParameters);
void task20ms(void *pvParameters);
//sleep function
void sleepTimerMs(int ticks,int taskNum);
//---------------------------------------------
void handler5ms(){
/*	updateDebugFlag(700);
	sleepTimerMs(2,1);
	passes1++;
	traceRunningTask(0);
	traceTaskPasses(1,passes1);*/
}
void handler10ms(){
	/*updateDebugFlag(800);
	sleepTimerMs(3,2);
	passes2++;
	traceTaskPasses(2,passes2);
	traceRunningTask(0);*/

}
void handler20ms(){

}
//-------
void cIn5ms(){

}
void cIn10ms(){

}
void cIn20ms(){

}
//-------
void cOut5ms(){

}
void cOut10ms(){

}
void cOut20ms(){

}
//-------

//task recurrence counters
int passes1 = 0;
int passes2 = 0;
int passes3 = 0;
//---------------------------------------------
int main(void) {
	//initialize output buffer for debug messages
	outbuf_init();
	//createAmaltheaTask()




	xTaskCreate( task5ms, "T1", 0x200, NULL, 3, NULL );
	xTaskCreate( task10ms,"T2", 0x200, NULL, 2, NULL );
	xTaskCreate( task20ms,"T3", 0x200, NULL, 1, NULL );
	vTaskStartScheduler();
	return EXIT_SUCCESS;
}
//---------------------------------------------
//task 5ms handler
void task5ms(void *pvParameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for(;;){
		updateDebugFlag(700);
		sleepTimerMs(2,1);
		passes1++;
		traceRunningTask(0);
		traceTaskPasses(1,passes1);
		vTaskDelayUntil( &xLastWakeTime, 5*1);
		updateDebugFlag(700);
	}
}
//---------------------------------------------
//task 10ms handler
void task10ms(void *pvParameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for(;;){
		updateDebugFlag(800);
		sleepTimerMs(3,2);
		passes2++;
		traceTaskPasses(2,passes2);
		traceRunningTask(0);
		vTaskDelayUntil( &xLastWakeTime,  10*1);
		updateDebugFlag(801);
	}
}
//---------------------------------------------
//task 20ms handler
void task20ms(void *pvParameters){
	TickType_t xLastWakeTime = xTaskGetTickCount();
	for(;;){
		updateDebugFlag(899);
		sleepTimerMs(5,3);
		passes3++;
		traceRunningTask(0);
		traceTaskPasses(3,passes3);
		vTaskDelayUntil( &xLastWakeTime, 20*1);
		updateDebugFlag(900);
	}
}
//---------------------------------------------
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

//end of file
