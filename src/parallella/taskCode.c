/*******************************************************************************
 *   Copyright (c) 2019 Dortmund University of Applied Sciences and Arts and others.
 *   
 *   This program and the accompanying materials are made
 *   available under the terms of the Eclipse Public License 2.0
 *   which is available at https://www.eclipse.org/legal/epl-2.0/
 *   
 *   SPDX-License-Identifier: EPL-2.0
 *   
 *   Contributors:
 *       Dortmund University of Applied Sciences and Arts - initial API and implementation
 *******************************************************************************/

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

int label5_10_00;

int label5_10_00_copy1;

int shared_label_1;
int shared_label_2 = 97;

//define tasks and copy operations here
//-------
void handler5ms(){
	int localLabel;
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
	sleepTimerMs(3,4);
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
	label5_10_00_copy1 = label5_10_00;
	shared_label_2 = 97 + passes1;
}
void cIn10ms(){

}
void cIn20ms(){

}

void cIn10msCore2(){
	shared_label_1 = shared_label_read_core(1,0,0);
}
void cIn20msCore2(){

}

//-------
void cOut5ms(){
	label5_10_00 = label5_10_00_copy1;
	shared_label_write_core(1,0,0,shared_label_2);
}
void cOut10ms(){
	shared_label_write(0,2);
}
void cOut20ms(){

}
void cOut10msCore2(){
	shared_label_1++;
	shared_label_write(1,shared_label_1);
}
void cOut20msCore2(){

}
