/*******************************************************************************
 *  * Copyright (c) 2019 Dortmund University of Applied Sciences and Arts and others.
 *  * 
 *  * This program and the accompanying materials are made
 *  * available under the terms of the Eclipse Public License 2.0
 *  * which is available at https://www.eclipse.org/legal/epl-2.0/
 *  * 
 *  * SPDX-License-Identifier: EPL-2.0
 *  * 
 *  * Contributors:
 *  *     Dortmund University of Applied Sciences and Arts - initial API and implementation
 *******************************************************************************/

#include "include/ParallellaUtils.h"

void sleepTimerMs(int ticks, int taskNum){
	updateDebugFlag(ticks*10);
	int i;
	for (i=0;i<ticks;i++){
		taskENTER_CRITICAL();
		traceRunningTask(taskNum);
		e_wait(E_CTIMER_0,_1MS);
		taskEXIT_CRITICAL();
	}
}

