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

/* Shared label count */
#define SHM_LABEL_COUNT                                         10

#define DSHM_LABEL_EPI_CORE_OFFSET                              0x4000


extern unsigned int execution_time_scale;


#endif /* SRC_PARALLELLA_RTFPARALLELLACONFIG_H_ */
