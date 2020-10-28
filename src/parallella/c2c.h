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

/**
 * @file c2c.h
 * @author Mahmoud Bazzal, Anand Prakash
 * @date 17 April 2020
 * @brief This file declares and implements function to communicate between epiphany cores. The
 * functions defined in this file are used for reading from and writing data in the epiphany
 * cores.
 */

#include <stdint.h>
#include "shared_comms.h"


#define DSHM_SEC_LABEL_COUNT            10


/**
 * defines a distributed shared memory section
 *
 */
typedef struct DSHM_section_t {
    unsigned         row;             /**< the row of target core on Epi chip */
    unsigned         col;             /**< the column of target core on Epi chip */
    unsigned int     base_addr;       /**< address of the first label in the section */
    unsigned         label_count;     /**< number of labels in the section */
    TYPE             sec_type;        /**< data type of the section (size of labels in the section) */
} DSHM_section;


/**
 * @brief Provide the epiphany memory section based on the offset data.
 *
 *
 * The epiphany memory section is defined in data bank 3 of each core and
 * start at address 0x7000. The address is returned based on the offset to
 * 0x7000 of each core.
 * This buffer is assigned to stored the RTF parallella legacy trace info. Data bank
 * 3 is used to store the information on each epiphany core. It starts at 0x7000 offset
 * on each epiphany core. Any change in this buffer addressing must be followed with
 * the correct offset set in host application to get the correct values.
 *
 * @param[in] offset : Offset to the epiphany start memory of 0x7000
 *
 * @return : Pointer to the Epiphany address.
 *
 */
unsigned int *allocate_epiphany_memory(unsigned int offset);

/**
 * @brief Initiate the shared label section, this function will assign addresses to labels in a section,
 * and initialize those labels to 0
 *
 * @return : void
 *
 */
void shared_labels_init_core(void);

/**
 * @brief Write a value to a label in a distributed shared memory section
 *
 * @param[in]  row            :    absolute row number of the core
 * @param[in]  col            :    absolute column number of the core
 * @param[in]  label_indx     :    index of the target shared label
 * @param[in]  payload        :    value to write
 *
 * @return : void
 *
 */
void shared_label_write_core    (unsigned row,unsigned col,int label_indx,int payload);


/**
 * @brief Read a value of a label in a distributed shared memory section
 *
 * @param[in] row            :    absolute row number of the core
 * @param[in] col            :    absolute column number of the core
 * @param[in] label_indx     :    index of the target shared label
 *
 * @return : Shared label value at the provided label_indx
 *
 */
unsigned int shared_label_read_core(unsigned row, unsigned col, int label_indx);

/**
 * @brief Get the absolute base memory address of a core.
 *
 * Get he global memory address of the provided epiphany row id and
 * column id.
 *
 * @param[in] row            :    absolute row number of the core
 * @param[in] col            :    absolute column number of the core
 *
 * @return : Global memory address of the Epiphany core.
 */
unsigned int get_base_address_core(int row, int col);



/**
 * @brief Initialize the distributed shared label section.
 *
 * Initiate the distributed shared label section, this function will assign addresses to labels in a section,
 * and initialize those labels to the value of 256
 *
 * @param[in] sec    :    structure of type DSHM_section containing
 *                       details of the the distributed shared memory section to be initiated
 *
 */
void DSHM_section_init(DSHM_section sec);

/**
 * @brief Read data from a specific label in a distributed shared memory section
 *
 * This function will read one full label but the result will
 * be cast into unsigned int (4 bytes on this platform)
 * Segmentation fault will occur for addresses outside the shared_dram section of the system
 * check RTFP documentation for details.
 *
 * @param[in] sec            :    struct of the section to be read
 * @param[in] label_indx     :    index of requested label
 *
 * @return : value of requested label in a distributed shared memory section
 *
 */
unsigned int read_DSHM_section (DSHM_section sec, int label_indx);

/**
 * @brief Write data to a specific label in a distributed shared memory section
 *
 * This function will write one full label but the value will
 * be given as int (4 bytes on this platform) to avoid overflow issues
 * Segmentation fault will occur for addresses outside the shared_dram section of the system
 * check RTFP documentation for details.
 *
 * @param[in] sec            :    struct of the section to be written to
 * @param[in] label_indx     :    index of requested label
 * @param[in] payload        :    value to be written (will be cast into data type of target label)
 *
 * @return : void
 */
void write_DSHM_section    (DSHM_section sec,int label_indx,int payload);


#endif
