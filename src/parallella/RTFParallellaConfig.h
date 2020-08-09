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

/**
 * @file RTFParallellaConfig.h
 * @author Anand Prakash
 * @date 19 June 2020
 * @brief This file declares the macros and structures used on Epiphany core to get the trace information,
 *
 */


/* Shared DRAM start address*/
#define SHARED_DRAM_START_ADDRESS                      0x8E000000

/* Shared dram start address offset corresponds to 0x8F000000 global address */
#define SHARED_DRAM_START_OFFSET                       0x01000000

/* The Shared DRAM section as seen by the Epiphany core */
#define SHARED_DRAM_SECTION                            (SHARED_DRAM_START_ADDRESS + SHARED_DRAM_START_OFFSET)

/* Allocate 4KB of shared DRAM for data exchange between host and epiphany cores */
#define SHARED_DRAM_SIZE                               0x00002000

#define RTF_DEBUG_TRACE_COUNT                                  10

/* First five address is used by FreeRTOS porting on Epiphany on shared dram see port.c file. */
#define INPUT_TIMESCALE_OFFSET                                 20

#define SHARED_BTF_DATA_OFFSET                         (INPUT_TIMESCALE_OFFSET + 4)

#define SHARED_LABEL_OFFSET                                0x1000

#define BTF_TRACE_BUFFER_SIZE                                   8

#define GLOBAL_SHARED_LABEL_OFFSET                      sizeof(btf_trace_info)


/* Shared label count */
#define SHM_LABEL_COUNT                                         10

#define DSHM_LABEL_CORE_OFFSET                                  10

/* Start buffer address on epiphany core to store the RTF trace info. */
#define ECORE_RTF_BUFFER_ADDR                                   0x7000

#define DSHM_LABEL_EPI_CORE_OFFSET                              0x7040



#define MUTEX_ROW        1
#define MUTEX_COL        0
#define RING_BUFFER_SIZE 6


extern unsigned int execution_time_scale;



/**
 * Structure to ensure proper synchronization between host and epiphany cores
 * and also within epiphany cores.
 */
typedef struct btf_trace_info_t
{
    int length;                            /**< To ensure that the mutex is initialized */
    unsigned int offset;                    /**< Mutex declaration. Unused on host  */
    unsigned int core_id;                   /**< BTF trace data buffer size which is to be read */
    unsigned int core_write;                /**< Read write operation between epiphany core and host */
} btf_trace_info;



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


typedef enum {
    UINT_8,                    /**< unsigned char type */
    UINT_16,                   /**< unsigned short type */
    UINT_32                    /**< unsigned int type */
} TYPE;


#endif /* SRC_PARALLELLA_RTFPARALLELLACONFIG_H_ */
