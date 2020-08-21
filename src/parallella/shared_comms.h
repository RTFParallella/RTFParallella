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

#ifndef SRC_PARALLELLA_SHARED_COMMS_H_
#define SRC_PARALLELLA_SHARED_COMMS_H_

/**
 * @file shared_comms.h
 * @author Mahmoud Bazzal, Anand Prakash
 * @date 13 April 2020
 * @brief This file declares and implements function to read and write data to shared memory. The
 * functions defined in this file are used for reading from and writing data in the shared memory
 * which can be used by host core or epiphany cores.
 */

#include <stdlib.h>
#include <stdint.h>
#include "RTFParallellaConfig.h"


#define shared_mem_section    0x01001000

#define shared_mem_section_2    0x01001000

#define shared_mem_section1_label_count 10

#define SHM_DEFINED_SPACE    256 //number of words in the shared memory section



/**
 * defines a shared memory section
 */
typedef struct SHM_section_t
{
    unsigned int     base_addr;    /**< address of the first label in the section */
    unsigned         label_count;  /**< number of labels in the section */
    TYPE            sec_type;     /**< data type of the section (size of labels in the section) */
}SHM_section;


/**
 * @brief The function provides the pointer to the global address to the shared DRAM memory.
 *
 * The shared DRAM memory offset starts at 0x8F000018. This address space is accessible
 * by the Epiphany cores as well as Host. The actual DRAM starts at 0x8F000000. The first
 * 20 bytes are reserved by the FreeRTOS. The next 4 bytes is used to store the time scale.
 * The next 44 bytes will be used to store the BTF trace information. The rest of the memory
 * can be used for storing the shared labels.
 *
 * @param[in] offset : The offset from the shared dram start address.
 *
 * @return           : Pointer to the shared DRAM memory.
 *
 */
unsigned int *allocate_shared_memory(unsigned int offset);

/**
 * @brief Function to initialize the shared memory area.
 *
 * Initiate the shared label section, this function will assign addresses to labels in a section,
 * and initialize those labels to the value of 256
 * If the requested section does not fit in the system's shared_dram, a null
 * pointer will be returned
 *
 * @param[in] sec  :   structure of type SHM_section containing
 *                     details of the the shared memory section to be initiated
 *
 * @return:   pointer to the initiated shared memory label
 *
 */
void* shm_section_init (SHM_section sec);

/**
 * @brief Read data from a specific label in a shared memory section.
 *
 * This function will read one full label but the result will
 * be cast into int (4 bytes on this platform)
 *
 * Segmentation fault will occur for addresses outside the shared_dram section of the system
 * check RTFP documentation for details.
 *
 * @param[in] x    :   pointer to the section to be read
 * @param[in] indx :   index of requested label
 *
 * @return:  value of requested label in a shared memory section
 *
 */
int read_shm_section ( unsigned int* x, unsigned indx);

/**
 * @brief Write data to a specific label in a shared memory section.
 *
 * This function will write one full label but the value will
 * be given as int (4 bytes on this platform) to avoid overflow issues
 *
 * Segmentation fault will occur for addresses outside the shared_dram section of the system
 * check RTFP documentation for details.
 *
 * @param[in] x        :   pointer to the section to be written to
 * @param[in] indx     :   index of requested label
 * @param[in] payload  :   value to be written (will be cast into data type of target label)
 *
 * @return : void
 *
 */
void write_shm_section (unsigned int* x, unsigned indx, int payload);

/**
 * @brief This function is obsolete. Use "read_shm_section" for reading shared memory area.
 */
unsigned int shm_section_init_read (SHM_section sec,int index);


#endif /* SRC_PARALLELLA_SHARED_COMMS_H_ */
