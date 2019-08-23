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
 *       Dortmund University of Applied Sciences and Arts - initial API and implementation
 *******************************************************************************/

#ifndef SRC_PARALLELLA_SHARED_COMMS_H_
#define SRC_PARALLELLA_SHARED_COMMS_H_

#include <stdlib.h>
#include <stdint.h>


#define shared_mem_section	0x01000000

#define shared_mem_section_2	0x01001000

#define shared_mem_section1_label_count 10


/**
 * Initiate the shared label section, this funcion will assign addresses to labels in a section,
 * and initialize those labels to 0
 *
 * Arguments:
 *
 */
void shared_labels_init();

/**
 * write a value to a label in a shared memory(DRAM) section
 *
 * Arguments:
 * label_indx	:	index of the target shared label
 * payload		:	value to write
 *
 */
uint8_t shared_label_write	(int label_indx,int payload);

/**
 * read a value of a label in a shared memory(DRAM) section
 *
 * Arguments:
 * label_indx	:	index of the target shared label
 *
 */
unsigned int shared_label_read(int label_indx);


#endif /* SRC_PARALLELLA_SHARED_COMMS_H_ */
