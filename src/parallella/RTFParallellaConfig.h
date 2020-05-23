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


/* Shared dram start address offset corresponds to 0x8F000000 global address */
#define SHARED_DRAM_START_ADDRESS                       0x01000000

/* Allocate 4KB of shared DRAM for data exchange between host and epiphany cores */
#define SHARED_DRAM_SIZE                                0x00010000

/* First five address is used by FreeRTOS porting on Epiphany on shared dram see port.c file. */
#define INPUT_TIMESCALE_OFFSET                                  20

#define SHARED_DATA_OFFSET                             (INPUT_TIMESCALE_OFFSET + 4)


extern unsigned int execution_time_scale;

/* Pointer to the shared dram buffer */
extern unsigned int shared_dram_start_address;


#endif /* SRC_PARALLELLA_RTFPARALLELLACONFIG_H_ */
