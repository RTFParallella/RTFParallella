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

//epiphany imports
#include <stdlib.h>

#include "e_lib.h"

#include "AmaltheaConverter.h"
#include "c2c.h"
#include "debugFlags.h"
#include "shared_comms.h"
#include "taskCode.h"
#include "RTFParallellaConfig.h"

#include "label_man_core0.h"
/* FreeRTOS imports */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"


int main(void) {
    /* Initialize output buffer for debug messages */
    unsigned int row, col, core_id;
    e_coords_from_coreid(e_get_coreid(), &row, &col);
    core_id = ((row * 4) + col);
    init_mutex(row, col, core_id);
    init_task_trace_buffer();
    int ts = get_time_scale_factor();
    init_mem_sections();
    shared_labels_init_core();
    /* create Amalthea task objects */
    AmaltheaTask t20ms = createAmaltheaTask(handler10msCore2, cIn10msCore2, cOut10msCore2,
            10 * ts, 10 * ts, 7 * ts, HW_CORE1_ID, 0, TASK10MS1_ID, 1);
    AmaltheaTask t40ms = createAmaltheaTask(handler20msCore2, cIn20msCore2, cOut20msCore2,
            20 * ts, 20 * ts, 12 * ts, HW_CORE1_ID, 0, TASK20MS1_ID, 1);
    /* create RTOS task from templates */
    createRTOSTask(&t20ms, 2, 0);
    createRTOSTask(&t40ms, 1, 0);
    /* start RTOS scheduler */
    vTaskStartScheduler();
    return EXIT_SUCCESS;
}

