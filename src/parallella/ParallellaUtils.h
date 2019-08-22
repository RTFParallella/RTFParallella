/*******************************************************************************
 * Copyright (c) 2019 Dortmund University of Applied Sciences and Arts and others.
 *
 * This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License 2.0
 * which is available at https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *      Dortmund University of Applied Sciences and Arts - initial API and implementation
 *******************************************************************************/

#ifndef SRC_PARALLELLA_PARALLELLAUTILS_H_
#define SRC_PARALLELLA_PARALLELLAUTILS_H_

#include "debugFlags.h"
#include "e_lib.h"
//freeRTOS imports
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//time units in processor cycles
#define _1MS 700000
#define _1US 700

/**
 * sleep for a multiple of milliseconds
 *
 * Arguments:
 * ticks		:	number of milliseconds to sleep
 * taskNum		:	index of task invoking this function (used for tracing during sleep)
 *
 */
void sleepTimerMs(int ticks,int taskNum);


#endif /* SRC_PARALLELLA_PARALLELLAUTILS_H_ */
