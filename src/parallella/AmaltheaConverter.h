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
 *        Dortmund University of Applied Sciences and Arts - initial API and implementation
 *******************************************************************************/

#ifndef SRC_PARALLELLA_AMALTHEACONVERTER_H_
#define SRC_PARALLELLA_AMALTHEACONVERTER_H_

#define PLATFORM_WORD_LENGTH 32

#define numTasks 3

/**
 * Structure to hold tasks according to amalthea model
 */
typedef struct AmaltheaTask_t
{
    unsigned int src_id;
    unsigned int src_instance;
    unsigned int task_id;
    unsigned int task_instance;
    void(* taskHandler)();
    unsigned int executionTime;//in ticks
    unsigned int deadline;        //in ticks
    unsigned int period;        //in ticks
    void(* cInHandler)();
    void(* cOutHandler)();
}AmaltheaTask;


/**
 * communication semantics of tasks
 * if use_LET_COMM_SEMANTICS is defined, the tasks will behave in LET semantics else it will use implicit by default
 */
//#define use_LET_COMM_SEMANTICS

AmaltheaTask createAmaltheaTask(void *taskHandler,void *cInHandler,void *cOutHandler,unsigned int period,unsigned int deadline, unsigned int WCET);

/**
 *    Create the RTOS task that represents a given Amalthea task.
 *    This function can have multiple arguments for all label types used by the task and the number of labels of each type.
 *
 *    Arguments:
 *    task            :    pointer to the AmaltheaTask struct
 *    priority        :    priority of the task (according to RMS, lowesrt perio has highest priority)
 *    argCount        :    number of different types of labels used by this task
 *    label_type_size :    size (in bits) of label type.
 *    label_type_count:    number of labels associated with that type.
 *
 *
 */
void createRTOSTask(AmaltheaTask* task, int priority, int argCount, ...);

/**
 * This function returns the additional stack size (in words) needed for the task to andle its labels
 *
 * Arguments:
 * labelBitCount    :    label size in bits
 * labelCount        :    number of labels
 *
 */
unsigned int calculateStackSize(int labelBitCount, int labelCount);

/**
 *
 *This RTOS task invokes the task handlers and realizes periodic task execution according to
 * Amalthea model
 *
 * Arguments:
 * task        :    instance of AmaltheaTask structure to be invoked
 *
 */
void generalizedRTOSTask(AmaltheaTask task);


#endif /* SRC_PARALLELLA_AMALTHEACONVERTER_H_ */
