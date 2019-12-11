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

#ifndef SRC_PARALLELLA_TRACE_UTILS_BTF_H_
#define SRC_PARALLELLA_TRACE_UTILS_BTF_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BTF_TRACE_TRUE     0x01
#define BTF_TRACE_FALSE    0x00


/* BTF trace flags */
#define BTF_TRACE_BUFFER_SIZE           8
#define SOURCE_FLAG                     0
#define SOURCE_INSTANCE_FLAG            1
#define EVENT_TYPE_FLAG                 2
#define TARGET_FLAG                     3
#define TARGET_INSTANCE_FLAG            4
#define EVENT_FLAG                      5
#define TIME_FLAG                       6
#define DATA_FLAG                       7


/**
 * Structure to hold BTF Header
 */
typedef struct btf_trace_header_config_t
{
    uint8_t creator[64];          /* Target device on which the trace is generated */
    uint8_t modelfile[512];       /* Model file used to generate the trace */
    uint8_t timescale[4];         /* Time scale e.g ns, ms, us, s..*/
} btf_trace_header_config_t;

typedef enum btf_trace_event_type_t
{
    T,
    ISR,
    R,
    IB,
    STI,
    ECU,
    P,
    C,
    SCHED,
    SIG,
    SEM,
    SIM
} btf_trace_event_type;

typedef enum btf_trace_event_name_t
{
    START,
    TERMINATE,
    READ,
    WRITE
} btf_trace_event_name;


/* Function to parse the command line arguments for generating the BTF trace file. */
void parse_btf_trace_arguments(int argc, char **argv);

/* Function to write the trace header config */
void write_btf_trace_header_config(FILE *stream);

/* Function to initialize the BTF trace buffer */
void btf_trace_buf_init(int **btf_trace_buf, int address);

#endif /* SRC_PARALLELLA_TRACE_UTILS_BTF_H_ */
