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

unsigned int *outbuf_dstr_shared[10];

void shared_labels_init_core(){
    //shared buffer in core memory
    outbuf_dstr_shared[0] = (unsigned int *) dstr_mem_offset_sec_1;
    //initialize buffer
    int i;
    for (i = 0;i < 10; i++){
        *outbuf_dstr_shared[i] = 0;
    }

}

void shared_label_write_core (unsigned row,unsigned col,int label_indx,int payload){
    unsigned int *addr;
    unsigned int* addr_base;
    addr_base = (unsigned int *)get_base_address_core(row,col);
    addr = (unsigned int*) ((unsigned ) addr_base | (unsigned)outbuf_dstr_shared[label_indx]);
    *addr = payload;
}


void DSHM_section_init(DSHM_section sec){
    unsigned int *addr;
    unsigned int* addr_base;
    addr_base = (unsigned int *)get_base_address_core(sec.row,sec.col);
    addr = (unsigned int*) ((unsigned ) addr_base | (unsigned)sec.base_addr);
    for (int i = 0;i<sec.label_count;i++){
        addr[i] = 0;
    }
}


void write_DSHM_section (DSHM_section sec,int label_indx,int payload){
    unsigned int *addr;
    unsigned int* addr_base;
    addr_base = (unsigned int *)get_base_address_core(sec.row,sec.col);
    addr = (unsigned int*) ((unsigned ) addr_base | (unsigned)sec.base_addr);
    addr[label_indx] = payload;
}

unsigned int read_DSHM_section (DSHM_section sec, int label_indx){
    unsigned int *addr;
    unsigned int* addr_base;
    addr_base = (unsigned int *)get_base_address_core(sec.row,sec.col);
    addr = (unsigned int*) ((unsigned ) addr_base | (unsigned)sec.base_addr);
    return addr[label_indx];
}


unsigned int shared_label_read_core (unsigned row, unsigned col, int label_indx){
    unsigned int *addr;
    unsigned int* addr_base;
    addr_base = (unsigned int *)get_base_address_core(row,col);
    addr = (unsigned int*) ((unsigned ) addr_base | (unsigned)outbuf_dstr_shared[label_indx]);
    return (unsigned int)*addr;
}


unsigned int get_base_address_core(int row, int col)
{
    uint32_t base_addr = 0x80800000;
    uint32_t col_shift = (uint32_t)((col << 20) | 0);
    uint32_t row_shift = (uint32_t)(((row * 4) << 24) | 0);
    return (base_addr | col_shift | row_shift);
}
