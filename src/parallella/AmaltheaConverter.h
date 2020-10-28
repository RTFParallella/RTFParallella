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

/**
 * @file AmaltheaConverter.h
 * @author Mahmoud Bazzal, Anand Prakash
 * @date 17 April 2020
 * @brief This file declares and implements function to generate Amalthea task model. The
 * functions defined in this file are used to generate the Amalthea Task model and
 * create a generalized RTOS task which executes periodically in infinite loop over FreeRTOS.
 *
 */


#include "RTFParallellaConfig.h"

#define PLATFORM_WORD_LENGTH 32          /**< Not is used in current implementation. Defines the platform word size */

#define numTasks 3                       /**< Defines number of tasks. Not is current use*/

/**
 * Structure to hold tasks according to amalthea model
 */
typedef struct AmaltheaTask_t
{
    unsigned int src_id;                                     /**< Source ID */
    unsigned int src_instance;                               /**< Source Instance */
    unsigned int task_id;                                    /**< Task ID */
    unsigned int task_instance;                              /**< Task Instance */
    void(* taskHandler)(int  src_id, int src_instance);      /**< Task handler */
    unsigned int executionTime;                              /**< Worst case execution time*/
    unsigned int deadline;                                   /**< Deadline of the task*/
    unsigned int period;                                     /**< Period of the task */
    void(* cInHandler)();                                    /**< cIn handler of the task */
    void(* cOutHandler)();                                   /**< cOut handler of the task */
}AmaltheaTask;


//#define use_LET_COMM_SEMANTICS


/**
 * @brief Generating Amalthea task model
 *
 * The function takes the input arguments and generates the Amalthea
 * task model which is used to create the RTOS tasks.
 *
 * Arguments:
 * @param[in]     taskHandler      : Amalthea task handler
 * @param[in]     cInHandler       : Amalthea cIn handler
 * @param[in]     cOutHandler      : Amalthea cOut handler
 * @param[in]     period           : Time period of the task
 * @param[in]     deadline         : Deadline of the task
 * @param[in]     WCET             : Worst case execution time of the task
 * @param[in]     src_id           : Source ID of the tasks
 * @param[in]     src_instance     : Source Instance of the task
 * @param[in]     task_id          : Task ID
 * @param[in]     task_instance    : Task Instance
 *
 * @return : Amalthea task model
 */
AmaltheaTask createAmaltheaTask(void *taskHandler, void *cInHandler, void *cOutHandler,
        unsigned int period, unsigned int deadline, unsigned int WCET,
        unsigned int src_id, unsigned int src_instance, unsigned int task_id, unsigned int task_instance);

/**
 * @brief Create the RTOS task that represents a given Amalthea task.
 *
 * This function can have multiple arguments for all label types used by the task and the number of labels of each type.
 *
 * @param[in]   task            :    pointer to the AmaltheaTask struct
 * @param[in]   priority        :    priority of the task (according to RMS, lowesrt perio has highest priority)
 * @param[in]   argCount        :    number of different types of labels used by this task
 * @param[in]   label_type_size :    size (in bits) of label type.
 * @param[in]   label_type_count:    number of labels associated with that type.
 *
 *    @return : void
 */
void createRTOSTask(AmaltheaTask* task, int priority, int argCount, ...);

/**
 * @brief This function returns the additional stack size (in words) needed for the task to handle its labels
 *
 * @param[in] labelBitCount    :    label size in bits
 * @param[in] labelCount       :    number of labels
 *
 * @return : stack size
 *
 */
unsigned int calculateStackSize(int labelBitCount, int labelCount);

/**
 *
 *@brief This RTOS task invokes the task handlers and realizes periodic task execution according to
 * Amalthea model
 *
 * @param[in] task        :    instance of AmaltheaTask structure to be invoked
 *
 * @return : void
 *
 */
void generalizedRTOSTask(AmaltheaTask task);


#endif /* SRC_PARALLELLA_AMALTHEACONVERTER_H_ */
