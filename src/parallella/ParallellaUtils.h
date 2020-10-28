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

#ifndef SRC_PARALLELLA_PARALLELLAUTILS_H_
#define SRC_PARALLELLA_PARALLELLAUTILS_H_

/**
 * @file ParallellaUtils.h
 * @author Mahmoud Bazzal, Anand Prakash
 * @date 20 May 2020
 * @brief This file declares the sleep timer function to simulate the time taken by each task to complete
 * its processing on the hardware core.
 *
 */

#include "e_lib.h"
/* FreeRTOS imports */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//time units in processor cycles
#define _1MS 700000
#define _1US 700

/**
 * @brief Sleep for a multiple of milliseconds
 *
 * It makes the task to sleep for the provided millisecond. This is used to simulate the computation
 * time taken by the task in a real scenario.
 *
 * @param[in] ticks        :    number of milliseconds to sleep
 * @param[in] taskNum      :    index of task invoking this function (used for tracing during sleep)
 *
 * @return : void
 */
void sleepTimerMs(int ticks,int taskNum);


#endif /* SRC_PARALLELLA_PARALLELLAUTILS_H_ */
