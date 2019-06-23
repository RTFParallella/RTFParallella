#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "e_lib.h"


//freeRTOS imports
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
//#include "c2c.h"

//RMSscheduler include

#include "scheduler.h"



//communicaqtion offsets
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
void task5ms(void *pvParameters);
void task10ms(void *pvParameters);
void task20ms(void *pvParameters);
void sleepTimerMs(int ticks);
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
  	//timer1init();
  	for (i=0;i<9;i++){
     		*outbuf[i] =0;
  	}
  
  
  	char c1 = 'a';
	char c2 = 'b';
	char c3 = 'c';
	char c4 = 'e';
	vSchedulerInit();
  

	vSchedulerPeriodicTaskCreate(task5ms, "t1", configMINIMAL_STACK_SIZE, &c1, 1, NULL, 0, 5, 2, 5);
	vSchedulerPeriodicTaskCreate(task10ms, "t2", configMINIMAL_STACK_SIZE, &c2, 2, NULL, 0, 10, 4, 10);
	vSchedulerPeriodicTaskCreate(task20ms, "t3", configMINIMAL_STACK_SIZE, &c3, 3, NULL, 0, 20, 5, 20);
  
  
  
  
	//timer initialization
//  xTaskCreate( hi, "T1", 512, NULL, 4, NULL );
//	xTaskCreate( hi2,"T2", 512, NULL, 3, NULL );
//  xTaskCreate( hi3,"T3", 512, NULL, 2, NULL );
	vTaskStartScheduler();
	*outbuf[3] = 0;
	return EXIT_SUCCESS;
}
//task 5ms handler and helper functions
void task5ms(void *pvParameters){
  TickType_t xLastWakeTime;
  for(;;){
     *outbuf[6] =1;
     passes1++;
     *outbuf[2] =passes1;
     *outbuf[7] =700;
     sleepTimerMs(1);
 //    vTaskDelayUntil( &xLastWakeTime, 5*1);
 //    taskYIELD();
     *outbuf[7] =701;
 	}
}


void task10ms(void *pvParameters){
  TickType_t xLastWakeTime;
  for(;;){
    *outbuf[6] =2;
    passes2++;
    *outbuf[0] = passes2;
    *outbuf[7] =800;
    sleepTimerMs(3);
  //  vTaskDelayUntil( &xLastWakeTime,  10*1);
  //  taskYIELD();
    *outbuf[7] =801;
	}
}

void task20ms(void *pvParameters){
  TickType_t xLastWakeTime;
  for(;;){
    *outbuf[6] =3;
    passes3++;
    *outbuf[4] = passes3;
    *outbuf[7] =899;
    sleepTimerMs(4);
  //  vTaskDelayUntil( &xLastWakeTime, 15*1);
  //  taskYIELD();
    *outbuf[7] =900;
	}
}
//---------------------------------------------------------------------
void sleepTimerMs(int ticks){
  *outbuf[7] =ticks*10;
  e_wait(E_CTIMER_0,ticks*_1MS);
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
