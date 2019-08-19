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
#include "shared_comms.h"
#include "c2c.h"

//declare taskHandler functions

void handler5ms();
void handler10ms();
void handler20ms();
void handler10msCore2();
void handler20msCore2();
//-------
//declare cIn/Out operations
//to be set up from the amalthea model during
//code generation
void cIn5ms();
void cIn10ms();
void cIn20ms();
void cIn10msCore2();
void cIn20msCore2();
//-------
void cOut5ms();
void cOut10ms();
void cOut20ms();
void cOut10msCore2();
void cOut20msCore2();

#endif /* DEMO_PARALLELLA_TASKCODE_H_ */
