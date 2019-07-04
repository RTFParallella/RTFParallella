/*
 * taskCode.h
 *
 *  Created on: Jul 3, 2019
 *      Author: bigm
 */

#ifndef DEMO_PARALLELLA_TASKCODE_H_
#define DEMO_PARALLELLA_TASKCODE_H_
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "debugFlags.h"
#include "ParallellaUtils.h"
//declare taskHandler functions

void handler5ms();
void handler10ms();
void handler20ms();
//-------
//declare cIn/Out operations
//to be set up from the amalthea model during
//code generation
void cIn5ms();
void cIn10ms();
void cIn20ms();
//-------
void cOut5ms();
void cOut10ms();
void cOut20ms();

#endif /* DEMO_PARALLELLA_TASKCODE_H_ */
