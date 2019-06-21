/*
 * debugFlags.h
 *
 *  Created on: Jun 21, 2019
 *      Author: bigm
 */

#ifndef DEMO_PARALLELLA_DEBUGFLAGS_H_
#define DEMO_PARALLELLA_DEBUGFLAGS_H_

#define cnt_address 0x80803000

#define TASK1_FLAG			2
#define TASK2_FLAG 			0
#define TASK3_FLAG 			4
#define RUNNINGTASK_FLAG 	6
#define DEBUG_FLAG 			7
#define TICK_FLAG			8

void outbuf_init(void );

void traceRunningTask(unsigned taskNum);

void traceTaskPasses(unsigned taskNum, int currentPasses);

void updateTick(void);

void updateDebugFlag(int debugMessage);


#endif /* DEMO_PARALLELLA_DEBUGFLAGS_H_ */
