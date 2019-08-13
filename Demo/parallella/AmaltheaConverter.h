/*
 * AmaltheaConverter.h
 *
 *  Created on: Jun 23, 2019
 *      Author: bigm
 */

#ifndef DEMO_PARALLELLA_AMALTHEACONVERTER_H_
#define DEMO_PARALLELLA_AMALTHEACONVERTER_H_

#define PLATFORM_WORD_LENGTH 32

#define numTasks 3

//task typedef
typedef struct{
	unsigned isDone;
	unsigned isReady;
	void(* taskHandler)();
	unsigned executionTime;//in ticks
	unsigned deadline;		//in ticks
	unsigned period;		//in ticks
	void(* cInHandler)();
	void(* cOutHandler)();
}AmaltheaTask;


//choose communication semantics
//#define use_LET_COMM_SEMANTICS

AmaltheaTask createAmaltheaTask(void *taskHandler,void *cInHandler,void *cOutHandler,unsigned int period,unsigned int deadline, unsigned int WCET);

void createRTOSTask(AmaltheaTask* task, int priority, int label_size, int labelCount);

unsigned int calculateStackSize(int labelBitCount, int labelCount);

void generalizedRTOSTak(AmaltheaTask task);


#endif /* DEMO_PARALLELLA_AMALTHEACONVERTER_H_ */
