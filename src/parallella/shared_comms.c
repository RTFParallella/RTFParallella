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

#include <e_lib.h>
#include "shared_comms.h"



unsigned int *allocate_shared_memory(unsigned int offset)
{
    unsigned int *dram_addr = 0;
    /**
     * The shared DRAM memory offset starts at 0x8F000018. This address space is accessible
     * by the Epiphany cores as well as Host. The actual DRAM starts at 0x8F000000. The first
     * 20 bytes are reserved by the FreeRTOS. The next 4 bytes is used to store the time scale.
     * The next 44 bytes will be used to store the BTF trace information. The rest of the memory
     * can be used for storing the shared labels.
     */
    unsigned int *shdram_start_addr = (unsigned int *)SHARED_DRAM_SECTION;
    /* Add offset to get the address */
    dram_addr = (shdram_start_addr + offset);
    return (unsigned int *)dram_addr;
}

//The current SHM access API
void* shm_section_init (SHM_section sec){
    unsigned size_in_bytes = 0;
    typedef unsigned int* cast_type;
    if (sec.sec_type == UINT_8)
    {
        typedef uint8_t* cast_type;
        size_in_bytes = sec.label_count * sizeof(uint8_t);
    }
    else if (sec.sec_type == UINT_16)
    {
        typedef uint16_t* cast_type;
        size_in_bytes = sec.label_count * sizeof(uint16_t);
    }
    else if (sec.sec_type == UINT_32)
    {
        typedef uint32_t* cast_type;
        size_in_bytes = sec.label_count * sizeof(uint32_t);
    }
    else
    {
        size_in_bytes = sec.label_count * sizeof(unsigned int);
    }
    /* check if section fits in shm_dram section of the memory */
    if ((sec.base_addr | size_in_bytes) < 0x01000000 ||
            (sec.base_addr | size_in_bytes) >= 0x02000000)
    {
        /* will cause segmentation fault in Epiphany */
        return NULL;
    }
    cast_type retval;
    /* assign to  the ABSOLUTE address of memory section */
    retval = (cast_type ) (0x8e000000 | sec.base_addr);
    int index = 0;
    for (index = 0; index < sec.label_count; index++){
        retval[index] = 0;
    }
    return retval;
}

int read_shm_section ( unsigned int* x, unsigned indx)
{
    return (int) x[indx];
}

void write_shm_section (unsigned int* x, unsigned indx, int payload)
{
    x[indx] = payload;
}


/**
 * TODO use e_write/ e_read functions and DMA to constrain contention from different cores.
 */
unsigned int shm_section_init_read (SHM_section sec,int index)
{
    unsigned int *retval;
    typedef unsigned int cast_type;
    if (sec.sec_type == UINT_8)
    {
        typedef uint8_t cast_type;
    }
    else if (sec.sec_type == UINT_16)
    {
        typedef uint16_t cast_type;
    }
    else if (sec.sec_type == UINT_32)
    {
        typedef uint32_t cast_type;
    }

    e_write((void*)&e_emem_config, retval, 0, 0, (void *)0x01000000, sizeof(unsigned int));
    return (unsigned int) retval;
}

/*------------------------end of file-------------------------*/
