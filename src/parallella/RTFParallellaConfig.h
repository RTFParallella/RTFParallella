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

#ifndef SRC_PARALLELLA_RTFPARALLELLACONFIG_H_
#define SRC_PARALLELLA_RTFPARALLELLACONFIG_H_

/* Shared DRAM start address*/
#define SHARED_DRAM_START_ADDRESS                      0x8E000000

/* Shared dram start address offset corresponds to 0x8F000000 global address */
#define SHARED_DRAM_START_OFFSET                       0x01000000

/* Allocate 4KB of shared DRAM for data exchange between host and epiphany cores */
#define SHARED_DRAM_SIZE                               0x00001000

/* The Shared DRAM section as seen by the Epiphany core */
#define SHARED_DRAM_SECTION                            0x8F000018

/* Start buffer address on epiphany core to store the RTF trace info. */
#define ECORE_RTF_BUFFER_ADDR                           0x6000

#define RTF_DEBUG_TRACE_COUNT                               10

/* First five address is used by FreeRTOS porting on Epiphany on shared dram see port.c file. */
#define INPUT_TIMESCALE_OFFSET                              20

#define SHARED_BTF_DATA_OFFSET                         (INPUT_TIMESCALE_OFFSET + 4)

#define BTF_TRACE_BUFFER_SIZE                                    8

#define GLOBAL_SHARED_LABEL_OFFSET                              11

#define EPI_CORE_MUTEX_OFFSET                                   16

/* Shared label count */
#define SHM_LABEL_COUNT                                         10

#define DSHM_LABEL_EPI_CORE_OFFSET                              0x4000


extern unsigned int execution_time_scale;


/* Enum for entity type ID */
typedef enum entity_id_t
{
    /* 0 to 15 entity ID is reserved for TASKS. */
    IDLE_TASK_ID = 0,
    TASK5MS0_ID,
    TASK10MS0_ID,
    TASK20MS0_ID,
    TASK10MS1_ID,
    TASK20MS1_ID,


    /* 16 to 64 entity ID reserved for RUNNABLES */
    RUNNABLE_HANDLER5MS0_ID = 16,
    RUNNABLE_HANDLER10MS0_ID,
    RUNNABLE_HANDLER20MS0_ID,
    RUNNABLE_HANDLER10MS1_ID,
    RUNNABLE_HANDLER20MS1_ID,

    /* 64 to 256 reserved for LABELS */
    SH_LABEL_A_ID = 64,
    SH_LABEL_B_ID,
    DSH_LABEL_A_ID,
    DSH_LABEL_B_ID,

    /* 256 to 264 reserved for HARDWARE */
    HW_CORE0_ID = 256,
    HW_CORE1_ID
} entity_id;


#endif /* SRC_PARALLELLA_RTFPARALLELLACONFIG_H_ */
