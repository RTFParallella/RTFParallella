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

#ifndef C2C_H
#define C2C_H


#include <stdint.h>
#include "shared_comms.h"
#define dstr_mem_offset_sec_1 			0x4000
#define DSHM_SEC_LABEL_COUNT		    10



/**
 * defines a distributed shared memory section
 *
 * Fields:
 * row		: the row of target core on Epi chip
 * col		: the column of target core on Epi chip
 * base_addr: address of the first label in the section
 * label_count: number of labels in the section
 * sec_type: data type of the section (size of labels in the section)
 *
 */
struct{
	unsigned 		row;
	unsigned 		col;
	unsigned int 	base_addr;
	unsigned 		label_count;
	TYPE			sec_type;
}typedef DSHM_section;

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
void shared_label_write_core	(unsigned row,unsigned col,int label_indx,int payload);


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



/**
 * Initiate the distributed shared label section, this function will assign addresses to labels in a section,
 * and initialize those labels to the value of 256
 *
 * Arguments:
 * sec	:	structure of type DSHM_section containing
 * 			details of the the distributed shared memory section to be initiated
 *
 */
void DSHM_section_init(DSHM_section sec);

/**
 * Read data from a specific label in a ditributed shared memory section
 * This function will read one full label but the result will
 * be cast into unsigned int (4 bytes on this platform)
 *
 * Segmentation fault will occur for addresses outside the shared_dram section of the system
 * check RTFP documentation for details.
 *
 * Arguments:
 * sec			:	struct of the section to be read
 * label_indx	:	index of requested label
 *
 * Return:
 * value of requested label in a distributed shared memory section
 *
 *
 */
unsigned int read_DSHM_section (DSHM_section sec, int label_indx);

/**
 * Write data to a specific label in a distributed shared memory section
 * This function will write one full label but the value will
 * be given as int (4 bytes on this platform) to avoid overflow issues
 *
 * Segmentation fault will occur for addresses outside the shared_dram section of the system
 * check RTFP documentation for details.
 *
 * Arguments:
 * sec			:	struct of the section to be written to
 * label_indx	:	index of requested label
 * payload		:	value to be written (will be cast into data type of target label)
 *
 */
void write_DSHM_section	(DSHM_section sec,int label_indx,int payload);


#endif
