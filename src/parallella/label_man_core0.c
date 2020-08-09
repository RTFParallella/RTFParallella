/*******************************************************************************
 *     Copyright (c) 2019 Dortmund University of Applied Sciences and Arts and others.
 *    
 *     This program and the accompanying materials are made
 *     available under the terms of the Eclipse Public License 2.0
 *     which is available at https://www.eclipse.org/legal/epl-2.0/
 *    
 *     SPDX-License-Identifier: EPL-2.0
 *    
 *     Contributors:
 *          Dortmund University of Applied Sciences and Arts - initial API and implementation
 *******************************************************************************/
#include "e-lib.h"
#include "RTFParallellaConfig.h"
#include "shared_comms.h"
#include "c2c.h"
#include "label_man_core0.h"

DSHM_section sec1_core_00 = {0,0,DSHM_LABEL_EPI_CORE_OFFSET,10};
DSHM_section sec1_core_10 = {1,0,DSHM_LABEL_EPI_CORE_OFFSET,10};

int label5_10_00;

int label5_10_00_copy1;

int shared_label_1 = 0;
int shared_label_2 = 97;
int shared_label_10 = 0;

extern int passes1;


/* Pointer declaration for global shared memory section */
static unsigned int* sec1_global_pointer;


/*initialize shm sections and assign pointers */

void init_mem_sections(void)
{
    sec1_global_pointer = allocate_shared_memory(SHARED_LABEL_OFFSET/sizeof(int));
    for (int index = 0; index < SHM_LABEL_COUNT; index++)
    {
        sec1_global_pointer[index] = 0;
    }
}


void init_DSHM_sections(void){
    DSHM_section_init(sec1_core_00);
    DSHM_section_init(sec1_core_10);
}

//-------
void cIn5ms(){
    label5_10_00_copy1 = label5_10_00;
    shared_label_2 = 97 + passes1;
}
void cIn10ms(){

}
void cIn20ms(){

}

void cIn10msCore2(){
    shared_label_1 = read_DSHM_section(sec1_core_00,0);
}
void cIn20msCore2(){
    shared_label_10 = read_shm_section(sec1_global_pointer,0);
}

//-------
void cOut5ms(){
    label5_10_00 = label5_10_00_copy1;
    write_DSHM_section(sec1_core_10,0,shared_label_2);
}
void cOut10ms()
{
    write_shm_section(sec1_global_pointer,0,3);
}
void cOut20ms(){

}
void cOut10msCore2(){
    shared_label_1++;
    write_shm_section(sec1_global_pointer,1,shared_label_1);
}
void cOut20msCore2(){
    write_shm_section(sec1_global_pointer,2,shared_label_10 + 1);
}
