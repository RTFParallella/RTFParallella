/*******************************************************************************
 *  * Copyright (c) 2019 Dortmund University of Applied Sciences and Arts and others.
 *  * 
 *  * This program and the accompanying materials are made
 *  * available under the terms of the Eclipse Public License 2.0
 *  * which is available at https://www.eclipse.org/legal/epl-2.0/
 *  * 
 *  * SPDX-License-Identifier: EPL-2.0
 *  * 
 *  * Contributors:
 *  *     Dortmund University of Applied Sciences and Arts - initial API and implementation
 *******************************************************************************/
#ifndef C2C_H
#define C2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#define dstr_mem_offset_sec_1 			0x4000
#define dstr_mem_sec_1_label_count		10
#define dstr_mem_sec_1_label_type		unsigned int


void shared_labels_init_core();

void core_shared_space_inti();

uint8_t shared_label_write_core	(unsigned row,unsigned col,int label_indx,int payload);

unsigned int shared_label_read_core(unsigned row, unsigned col, int label_indx);

unsigned int get_base_address_core(int row, int col);



#endif
