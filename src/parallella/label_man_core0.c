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
#include "label_man_core0.h"

DSHM_section sec1_core_00 = {0,0,0x4000,10};
DSHM_section sec1_core_10 = {1,0,0x4000,10};

unsigned int execution_time_scale;

/* Pointer declaration for global shared memory section */
unsigned int* sec1_global_pointer;
unsigned int shared_dram_start_address SECTION("shared_dram");
unsigned int* time_scale = 0x8f000014;

//initialize shm sections and assign pointers

void init_mem_sections(void){
    execution_time_scale = time_scale[0];
    sec1_global_pointer = &shared_dram_start_address;
    for (int i = 0;i<10;i++){
        sec1_global_pointer[i] = 0;
    }
}


void init_DSHM_sections(void){
    DSHM_section_init(sec1_core_00);
    DSHM_section_init(sec1_core_10);
}



//extern unsigned int shared_label1;

int label5_10_00;

int label5_10_00_copy1;

int shared_label_1;
int shared_label_2 = 97;
int shared_label_10 = 0;

extern int passes1;

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
    //shared_label_1 = shared_label_read_core(1,0,0);
}
void cIn20msCore2(){
    shared_label_10 = read_shm_section(sec1_global_pointer,0);
}

//-------
void cOut5ms(){
    label5_10_00 = label5_10_00_copy1;
    write_DSHM_section(sec1_core_10,0,shared_label_2);
    //shared_label_write_core(1,0,0,shared_label_2);
}
void cOut10ms(){
    write_shm_section(sec1_global_pointer,0,3);
    //shared_label_write(0,2);
}
void cOut20ms(){

}
void cOut10msCore2(){
    shared_label_1++;
    write_shm_section(sec1_global_pointer,1,shared_label_1);
    //shared_label_write(1,shared_label_1);
}
void cOut20msCore2(){
    write_shm_section(sec1_global_pointer,3,shared_label_10 + 1);
    //shared_label_write(3,shared_label_10 + 1);
}
