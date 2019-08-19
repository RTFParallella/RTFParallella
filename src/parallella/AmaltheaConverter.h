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
/*
 *	Create the RTOS task that represents a given Amalthea task.
 *	This function can have multiple arguments for all label types used by the task and the number of labels of each type.
 *
 *	Arguments:
 *	task			:	pointer to the AmaltheaTask struct
 *	priority		:	priority of the task (according to RMS, lowesrt perio has highest priority)
 *	argCount		:	number of different types of labels used by this task
 *	label_type_size :	size (in bits) of label type.
 *	label_type_count:	number of labels associated with that type.
 *
 *
 */

void createRTOSTask(AmaltheaTask* task, int priority, int argCount, ...);

/*
 * This function returns the additional stack size (in words) needed for the task to andle its labels
 *
 * Arguments:
 * labelBitCount	:	label size in bits
 * labelCount		:	number of labels
 */
unsigned int calculateStackSize(int labelBitCount, int labelCount);

void generalizedRTOSTak(AmaltheaTask task);


#endif /* DEMO_PARALLELLA_AMALTHEACONVERTER_H_ */
