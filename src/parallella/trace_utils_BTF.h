/*******************************************************************************
 *   Copyright (c) 2020 Dortmund University of Applied Sciences and Arts and others.
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

#ifndef SRC_PARALLELLA_TRACE_UTILS_BTF_H_
#define SRC_PARALLELLA_TRACE_UTILS_BTF_H_

/**
 * @file trace_utils_BTF.h
 * @author Anand Prakash
 * @date 23 May 2020
 * @brief This file declares and implement the BTF trace framework. It
 * consists of functions used to generate the trace information of the tasks, runnables
 * shared label access and hardware info in the BTF trace format.
 *
 * @see https://wiki.eclipse.org/images/e/e6/TA_BTF_Specification_2.1.3_Eclipse_Auto_IWG.pdf
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "RTFParallellaConfig.h"

#define RFTP_GENERATE_BTF_TRACE 0x01

#define BTF_TRACE_TRUE     0x01
#define BTF_TRACE_FALSE    0x00


/* BTF trace flags */
#define BTF_TRACE_BUFFER_SIZE           8
#define TIME_FLAG                       0
#define SOURCE_FLAG                     1
#define SOURCE_INSTANCE_FLAG            2
#define EVENT_TYPE_FLAG                 3
#define TARGET_FLAG                     4
#define TARGET_INSTANCE_FLAG            5
#define EVENT_FLAG                      6
#define DATA_FLAG                       7

/* Enum for event type ID */
typedef enum btf_trace_event_type_t
{
    TASK_EVENT,                        /**< BTF Task Event*/
    INT_SERVICE_ROUTINE_EVENT,         /**< BTF ISR Event*/
    RUNNABLE_EVENT,                    /**< BTF Runnable Event*/
    INS_BLOCK_EVENT,                   /**< BTF INS Block Event*/
    STIMULUS_EVENT,                    /**< BTF Stimulus Event*/
    ECU_EVENT,                         /**< BTF ECU Event*/
    PROCESSOR_EVENT,                   /**< BTF Processor Event*/
    CORE_EVENT,                        /**< BTF Hardware Core Event*/
    SCHEDULER_EVENT,                   /**< BTF Scheduler Event*/
    SIGNAL_EVENT,                      /**< BTF Signal Event for reading/writing labels*/
    SEMAPHORE_EVENT,                   /**< BTF Semaphore Event*/
    SIMULATION_EVENT                   /**< BTF Simulation Event*/
} btf_trace_event_type;

/* Enum for event name */
typedef enum btf_trace_event_name_t
{
    INIT = -1,            /**< Dummy Init Event name*/
    PROCESS_START,        /**< Starting a process/task/runnable*/
    PROCESS_TERMINATE,    /**< Terminating a process/task/runnable*/
    PROCESS_PREEMPT,      /**< Preempt a process/task*/
    PROCESS_SUSPEND,      /**< Suspend a runnable event*/
    PROCESS_RESUME,       /**< Resume a process/task/runnable*/
    SIGNAL_READ,          /**< Read event for signal/label*/
    SIGNAL_WRITE          /**< Write signal for signal/label*/
} btf_trace_event_name;


/**
 * Structure to hold BTF Header
 */
typedef struct btf_trace_header_config_t
{
    uint32_t timescale;           /**< This sets the scale of the time e.g 10, 100 etc.. */
    uint8_t creator[64];          /**< Target device on which the trace is generated */
    uint8_t modelfile[512];       /**< Model file used to generate the trace */
    uint8_t timeunit[4];          /**< Time unit e.g ns, ms, us, s..*/
} btf_trace_header_config_t;

/* BTF structure for storing the entity configuration */
typedef struct btf_trace_entity_entry_t
{
    uint16_t entity_id;                 /**< Entity ID to get the entity name */
    int16_t instance;                   /**< Current instance of the entity */
    btf_trace_event_name state;         /**< Current state of the entity */
    btf_trace_event_type entity_type;   /**< Entity type to get the source*/
    uint8_t entity_name[64];            /**< Entity name */

} btf_trace_entity_entry;

/* Structure to hold BTF trace data in task stack for processing */
typedef struct btf_trace_data_t
{
    int32_t ticks;                     /**< Tick count */
    int32_t srcId;                     /**< Source Id */
    int32_t srcInstance;               /**< Instance of the source */
    int32_t eventTypeId;               /**< Type of event Runnable , Task etc.. */
    int32_t taskId;                    /**< Task Id */
    int32_t taskInstance;              /**< Instance of the task */
    int32_t eventState;                /**< State of the event */
    int32_t data;                      /**< Notes */
} btf_trace_data;

/* Entity table structure */
typedef struct btf_trace_entity_table_t
{
    uint16_t is_occupied;                 /**< If 0, entry is available else not available */
    btf_trace_entity_entry entity_data;   /**< Entity details */
} btf_trace_entity_table;

/**
 * @brief Function to get the file name of the trace file along with the
 * absolute path.
 *
 * Arguments:
 * @param[inout] trace_file_path  : Pointer to the buffer where the BTF trace file path
 *                                  is stored.
 *
 * @return: void
 */
void get_btf_trace_file_path(char *trace_file_path);

/**
 * @brief Parse the command line arguments for generating the BTF trace file
 *
 * The provided parameters are used to configure the trace file required to
 * be generated. For example the trace file path, model file used to generate the
 * trace, device name and time scale.
 *
 * Arguments:
 * @param[in] argc  : The count for the number of arguments passed
 * @param[in] argv  : Pointer to the list of arguments
 *
 * @return: The integer value of the timescale used for the task execution.
 */
int parse_btf_trace_arguments(int argc, char **argv);

/**
 * @brief This function is responsible for writing the BTF trace header information.
 *
 * Function to write BTF header data to the trace file. It writes the version, creator,
 * input model file, time scale and timestamp section of the header file. It also writes
 * the entity table, type table and entity type table used in the task model.
 *
 * Arguments:
 * @param[in] stream  : File pointer to the stream where the data has to be
 *                      written.
 *
 * @return            : void
 */
void write_btf_trace_header_config(FILE *stream);

/**
 * @brief This function to write entity type in BTF header data.
 *
 * The function defines what kinds of entities are supported in the BTF
 * trace generated. It consists of entity type such as Tasks, Signals, Runnables
 * along with their IDs.
 * Refer to below link for more details:
 * https://wiki.eclipse.org/images/e/e6/TA_BTF_Specification_2.1.3_Eclipse_Auto_IWG.pdf
 *
 * Arguments:
 * @param[in] stream  : File pointer to the stream where the data has to be
 *                      written.
 * @param[in] type    : Type of the entity i.e. TASK, RUNNABLE, STIMULUS etc..
 *
 * @return: void
 */
void write_btf_trace_header_entity_type(FILE *stream, btf_trace_event_type type);

/**
 * @brief Function to write entity type in BTF header data
 *
 * The function writes the list of tasks, runnables, shared labels, cores in a
 * tabular format. It combines the entity type and entity type table.
 * Refer to below link for more details:
 * https://wiki.eclipse.org/images/e/e6/TA_BTF_Specification_2.1.3_Eclipse_Auto_IWG.pdf
 *
 * Arguments:
 * @param[in] stream  : File pointer to the stream where the data has to be
 *                     written.
 *
 * @return: void
 */
void write_btf_trace_header_entity_table(FILE *stream);

/**
 * @brief  This function writes the entity type table in the BTF header.
 *
 * The function writes the list of tasks, runnables, shared labels, cores in a
 * tabular format. It consists of the tasks, runnables and shared labels executed
 * on the specified cores along with their IDs.
 * Refer to below link for more details:
 * https://wiki.eclipse.org/images/e/e6/TA_BTF_Specification_2.1.3_Eclipse_Auto_IWG.pdf
 *
 * Arguments:
 * @param[in] stream  : File pointer to the stream where the data has to be
 *                      written.
 *
 * @return: void
 */
void write_btf_trace_header_entity_type_table(FILE *stream);

/**
 * @brief This function is used to store the entity information of all the
 * tasks, runnables and labels.
 *
 * Store the entity metadata which can be used to generate the entity
 * type and entity type table. Also this table entry is used to decode the
 * tasks and runnables information received from the Parallella framework.
 *
 * Arguments:
 * @param[in] typeId  : Unique entity type ID
 * @param[in] type    : Entity type..e.g TASK, RUNNABLE etc..
 * @param[in] name    : Entity name
 *
 * @return: void
 */
void store_entity_entry(entity_id typeId, btf_trace_event_type type, const char *name);

/**
 * @brief Function to write the data section of the BTF
 *
 * The function is responsible for writing the BTF trace data section in
 * CSV format which can be interpreted by the trace visualizing tools such as
 * Eclipse trace compass. Currently the support is provided for only two cores.
 * However, this can be extended further for multiple cores.
 *
 * Arguments:
 * @param[in] stream        : File pointer to the stream where the data has to be
 *                            written.
 * @param[in] core_id       : Core ID on which the task operations are performed
 * @param[in] data_buffer   : Data buffer containing the BTF trace information.
 *
 * @return: void
 */
void write_btf_trace_data(FILE *stream, uint8_t core_id, unsigned int * data_buffer);

#endif /* SRC_PARALLELLA_TRACE_UTILS_BTF_H_ */
