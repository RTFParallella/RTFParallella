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
#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

#include "RTFParallellaConfig.h"


#define configCALL_STACK_SIZE              0x50

#define configUSE_PREEMPTION                  1
#define configUSE_TIME_SLICING                0

#define configUSE_IDLE_HOOK                   0
#define configUSE_TICK_HOOK                   0
#define configCPU_CLOCK_HZ                    ( ( unsigned long ) 700000000 )
#define configTICK_RATE_HZ                    ( ( TickType_t ) execution_time_scale )
#define configMAX_PRIORITIES                  ( 5 )
#define configMINIMAL_STACK_SIZE              ( ( unsigned short ) 128) //256 words
#define configTOTAL_HEAP_SIZE                 ( (size_t ) ( 10752) )
#define configMAX_TASK_NAME_LEN               ( 48 )
#define configUSE_TRACE_FACILITY              0
#define configUSE_16_BIT_TICKS                1
#define configIDLE_SHOULD_YIELD               0



#define configUSE_ALTERNATIVE_API             0

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES                 0
#define configMAX_CO_ROUTINE_PRIORITIES       ( 2 )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function. */

#define INCLUDE_vTaskPrioritySet              0
#define INCLUDE_uxTaskPriorityGet             0
#define INCLUDE_vTaskDelete                   0
#define INCLUDE_vTaskCleanUpResources         0
#define INCLUDE_vTaskSuspend                  1
#define INCLUDE_vTaskDelayUntil               1
#define INCLUDE_vTaskDelay                    1
#define INCLUDE_xTaskGetCurrentTaskHandle     1
#define INCLUDE_pcTaskGetTaskName             1

#define C2C_MSG_TYPE int


#endif /* FREERTOS_CONFIG_H */
