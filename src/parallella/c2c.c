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


#include "RTFParallellaConfig.h"
#include "c2c.h"

unsigned int *outbuf_dstr_shared[SHM_LABEL_COUNT];

/**
 * Provide the epiphany memory section based on the offset data.
 */
unsigned int *allocate_epiphany_memory(unsigned int offset)
{
    unsigned int *epi_core_addr = 0;
    /* Ensure that the start address is 0x7000 at data bank 3 of each core */
    unsigned int *epi_core_buffer = (unsigned int *)ECORE_RTF_BUFFER_ADDR;
    /* Add offset to get the address */
    epi_core_addr = (epi_core_buffer + offset);
    return (unsigned int *)epi_core_addr;
}


/**
 * Initiate the shared label section, this function will assign addresses to labels in a section,
 * and initialize those labels to 0
 */
void shared_labels_init_core(void){
    /* shared buffer in core memory */
    outbuf_dstr_shared[0] = (unsigned int *) DSHM_LABEL_EPI_CORE_OFFSET;
    /* initialize buffer */
    int index;
    for (index = 0; index < SHM_LABEL_COUNT; index++){
        *outbuf_dstr_shared[index] = 0;
    }

}


/**
 * Write a value to a label in a distributed shared memory section
 */
void shared_label_write_core (unsigned row,unsigned col,int label_indx,int payload)
{
    unsigned int *addr;
    unsigned int* addr_base;
    addr_base = (unsigned int *)get_base_address_core(row,col);
    addr = (unsigned int*) ((unsigned ) addr_base | (unsigned)outbuf_dstr_shared[label_indx]);
    *addr = payload;
}


/**
 * Initialize the distributed shared label section.
 */
void DSHM_section_init(DSHM_section sec)
{
    unsigned int *addr;
    unsigned int* addr_base;
    addr_base = (unsigned int *)get_base_address_core(sec.row,sec.col);
    addr = (unsigned int*) ((unsigned ) addr_base | (unsigned)sec.base_addr);
    for (int i = 0;i<sec.label_count;i++){
        addr[i] = 0;
    }
}


/**
 * Write data to a specific label in a distributed shared memory section
 */
void write_DSHM_section (DSHM_section sec,int label_indx,int payload)
{
    unsigned int *addr;
    unsigned int* addr_base;
    addr_base = (unsigned int *)get_base_address_core(sec.row,sec.col);
    addr = (unsigned int*) ((unsigned ) addr_base | (unsigned)sec.base_addr);
    addr[label_indx] = payload;
}

/**
 * Read data from a specific label in a distributed shared memory section
 */
unsigned int read_DSHM_section (DSHM_section sec, int label_indx)
{
    unsigned int *addr;
    unsigned int* addr_base;
    addr_base = (unsigned int *)get_base_address_core(sec.row,sec.col);
    addr = (unsigned int*) ((unsigned ) addr_base | (unsigned)sec.base_addr);
    return addr[label_indx];
}


/**
 * Read a value of a label in a distributed shared memory section.
 */
unsigned int shared_label_read_core (unsigned row, unsigned col, int label_indx)
{
    unsigned int *addr;
    unsigned int* addr_base;
    addr_base = (unsigned int *)get_base_address_core(row,col);
    addr = (unsigned int*) ((unsigned ) addr_base | (unsigned)outbuf_dstr_shared[label_indx]);
    return (unsigned int)*addr;
}

/**
 * Get the absolute base memory address of a core.
 */
unsigned int get_base_address_core(int row, int col)
{
    uint32_t base_addr = 0x80800000;
    uint32_t col_shift = (uint32_t)((col << 20) | 0);
    uint32_t row_shift = (uint32_t)(((row * 4) << 24) | 0);
    return (base_addr | col_shift | row_shift);
}
