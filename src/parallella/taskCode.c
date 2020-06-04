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

#include "taskCode.h"
#include "e_lib.h"
#include "debugFlags.h"
#include "RTFParallellaConfig.h"
#include "trace_utils_BTF.h"

/* define global variables to be
 * accessed through a specific task (context)
 */
//task recurrence counters
int passes1 = 1;
int passes2 = 1;
int passes3 = 1;
int passes4 = 1;
int passes5 = 1;



//define tasks and copy operations here
//-------
void handler5ms(int src_id, int src_instance)
{
    int localLabel;
    updateDebugFlag(700);
    traceTaskEvent(src_id, src_instance, RUNNABLE_EVENT, RUNNABLE_HANDLER5MS0_ID,
            passes1, PROCESS_START, 0);
    sleepTimerMs(1,1);
    traceTaskEvent(src_id, src_instance, RUNNABLE_EVENT, RUNNABLE_HANDLER5MS0_ID,
            passes1, PROCESS_TERMINATE, 0);
    passes1++;
    traceTaskPasses(1,passes1);
}

void handler10ms(int src_id, int src_instance)
{
    updateDebugFlag(800);
    traceTaskEvent(src_id, src_instance, RUNNABLE_EVENT, RUNNABLE_HANDLER10MS0_ID,
            passes2, PROCESS_START, 0);
    sleepTimerMs(2,2);
    traceTaskEvent(src_id, src_instance, RUNNABLE_EVENT, RUNNABLE_HANDLER10MS0_ID,
            passes2, PROCESS_TERMINATE, 0);
    passes2++;
    traceTaskPasses(2,passes2);
}

void handler20ms(int src_id, int src_instance)
{
    updateDebugFlag(899);
    traceTaskEvent(src_id, src_instance, RUNNABLE_EVENT, RUNNABLE_HANDLER20MS0_ID,
            passes3, PROCESS_START, 0);
    sleepTimerMs(4,3);
    traceTaskEvent(src_id, src_instance, RUNNABLE_EVENT, RUNNABLE_HANDLER20MS0_ID,
            passes3, PROCESS_TERMINATE, 0);
    passes3++;
    traceTaskPasses(3,passes3);

}

void handler10msCore2(int src_id, int src_instance)
{
    updateDebugFlag(899);
    traceTaskEvent(src_id, src_instance, RUNNABLE_EVENT, RUNNABLE_HANDLER10MS1_ID,
            passes4, PROCESS_START, 0);
    sleepTimerMs(3,4);
    traceTaskEvent(src_id, src_instance, RUNNABLE_EVENT, RUNNABLE_HANDLER10MS1_ID,
            passes4, PROCESS_TERMINATE, 0);
    passes4++;
    traceTaskPasses(2,passes4);
}

void handler20msCore2(int src_id, int src_instance)
{
    updateDebugFlag(899);
    traceTaskEvent(src_id, src_instance, RUNNABLE_EVENT, RUNNABLE_HANDLER20MS1_ID,
            passes5, PROCESS_START, 0);
    sleepTimerMs(7,5);
    traceTaskEvent(src_id, src_instance, RUNNABLE_EVENT, RUNNABLE_HANDLER20MS1_ID,
            passes5, PROCESS_TERMINATE, 0);
    passes5++;
    traceTaskPasses(3,passes5);
}






