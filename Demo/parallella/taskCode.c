/*
 * taskCode.c
 *
 *  Created on: Jun 21, 2019
 *      Author: bigm
 */

#include "taskCode.h"




/* define global variables to be
 * accessed through a specific task (context)
 */
//task recurrence counters
int passes1 = 0;
int passes2 = 0;
int passes3 = 0;
int passes4 = 0;
int passes5 = 0;

//extern unsigned int shared_label1;


//define tasks and copy operations here
//-------
void handler5ms(){
	updateDebugFlag(700);
	sleepTimerMs(2,1);
	passes1++;
	traceTaskPasses(1,passes1);
	traceRunningTask(0);
}

void handler10ms(){
	updateDebugFlag(800);
	sleepTimerMs(3,2);
	passes2++;
	traceTaskPasses(2,passes2);
	traceRunningTask(0);

}

void handler20ms(){
	updateDebugFlag(899);
	sleepTimerMs(5,3);
	passes3++;
	traceRunningTask(0);
	traceTaskPasses(3,passes3);

}

void handler10msCore2(){
	updateDebugFlag(899);
	sleepTimerMs(5,4);
	passes4++;
	//shared_label1 = shared_label_to_read;
	traceRunningTask(0);
	traceTaskPasses(2,passes4);

}

void handler20msCore2(){
	updateDebugFlag(899);
	sleepTimerMs(7,5);
	passes5++;
	traceRunningTask(0);
	traceTaskPasses(3,passes5);
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
