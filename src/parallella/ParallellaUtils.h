/*
 * ParallellaUtils.h
 *
 *  Created on: Jul 3, 2019
 *      Author: bigm
 */

#ifndef DEMO_PARALLELLA_PARALLELLAUTILS_H_
#define DEMO_PARALLELLA_PARALLELLAUTILS_H_

#include "e_lib.h"
//freeRTOS imports
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "debugFlags.h"

//time units in processor cycles
#define _1MS 700000
#define _1US 700


void sleepTimerMs(int ticks,int taskNum);


#endif /* DEMO_PARALLELLA_PARALLELLAUTILS_H_ */
