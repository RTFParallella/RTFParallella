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


/* Epiphany imports */
#include <stdlib.h>
#include "e_lib.h"

#include "AmaltheaConverter.h"
#include "debugFlags.h"
#include "shared_comms.h"
#include "taskCode.h"
#include "label_man_core0.h"
#include "RTFParallellaConfig.h"

/* FreeRTOS imports */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//---------------------------------------------
int main(void) {
    /* initialize output buffer for debug messages */
    init_btf_mem_section();
    init_task_trace_buffer();
    int ts = get_time_scale_factor();
    init_mem_sections();
    shared_labels_init_core();
    AmaltheaTask t5ms =  createAmaltheaTask(handler5ms ,cIn5ms ,cOut5ms,
            5 * ts, 5 * ts, 3 * ts, HW_CORE0_ID, 0, TASK5MS0_ID, 1);
    AmaltheaTask t10ms = createAmaltheaTask(handler10ms, cIn10ms, cOut10ms,
            10 * ts, 10 * ts, 4 * ts, HW_CORE0_ID, 0, TASK10MS0_ID, 1);
    AmaltheaTask t20ms = createAmaltheaTask(handler20ms, cIn20ms, cOut20ms,
            20 * ts,20 * ts, 6 * ts, HW_CORE0_ID, 0, TASK20MS0_ID, 1);
    /* create RTOS task from templates */
    createRTOSTask(&t5ms, 3, 0);
    createRTOSTask(&t10ms, 2, 0);
    createRTOSTask(&t20ms, 1, 0);
    /* start RTOS scheduler */
    vTaskStartScheduler();
    return EXIT_SUCCESS;
}



