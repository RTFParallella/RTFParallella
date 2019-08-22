/*******************************************************************************
 * Copyright (c) 2019 Dortmund University of Applied Sciences and Arts and others.
 *
 * This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License 2.0
 * which is available at https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *      Dortmund University of Applied Sciences and Arts - initial API and implementation
 *******************************************************************************/

#ifndef C2C_H
#define C2C_H


#include <stdint.h>
#define dstr_mem_offset_sec_1 			0x4000
#define dstr_mem_sec_1_label_count		10
#define dstr_mem_sec_1_label_type		unsigned int

/**
 * Initiate the shared label section, this funcion will assign addresses to labels in a section,
 * and initialize those labels to 0
 *
 * Arguments:
 *
 */
void shared_labels_init_core();

/**
 * write a value to a label in a distributed shared memory section
 *
 * Arguments:
 * row			:	absolute row number of the core
 * col			:	absolute column number of the core
 * label_indx	:	index of the target shared label
 * payload		:	value to write
 *
 */
uint8_t shared_label_write_core	(unsigned row,unsigned col,int label_indx,int payload);

/**
 * read a value of a label in a distributed shared memory section
 *
 * Arguments:
 * row			:	absolute row number of the core
 * col			:	absolute column number of the core
 * label_indx	:	index of the target shared label
 *
 */
unsigned int shared_label_read_core(unsigned row, unsigned col, int label_indx);

/**
 * get the absolute base memory address of a core
 *
 * Arguments:
 * row			:	absolute row number of the core
 * col			:	absolute column number of the core
 *
 */
unsigned int get_base_address_core(int row, int col);



#endif
