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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <e-hal.h> //hardware abstraction library
#include <time.h>   /* Needed for struct timespec */
#include <e-loader.h>

#include "RTFParallellaConfig.h"
#include "c2c.h"
#include "debugFlags.h"
#include "shared_comms.h"
#include "host_utils.h"
#include "model_enumerations.h"

unsigned int shared_label_to_read[10];
unsigned int shared_label_core_00[DSHM_SEC_LABEL_COUNT];
unsigned int shared_label_core_10[DSHM_SEC_LABEL_COUNT];

int main()
{
	int label_enable_count_core0 = 0;
	printf("RTFP demo host code\n");
	//setup visualization config for the first core
	unsigned index_array1[DSHM_SEC_LABEL_COUNT];
	unsigned index_array1_prv_val[DSHM_SEC_LABEL_COUNT];
	array_init(index_array1,DSHM_SEC_LABEL_COUNT);
	array_init(index_array1_prv_val,DSHM_SEC_LABEL_COUNT);
	LabelVisual core1 = get_user_input(index_array1);
	//setup visualization config for the first core
	unsigned index_array2[DSHM_SEC_LABEL_COUNT];
	unsigned index_array2_prv_val[DSHM_SEC_LABEL_COUNT];
	array_init(index_array2,DSHM_SEC_LABEL_COUNT);
	array_init(index_array2_prv_val,DSHM_SEC_LABEL_COUNT);
	LabelVisual core2 = get_user_input(index_array2);
	//steup visualization for shared memeory
	unsigned index_array_DRAM[shared_mem_section1_label_count];
	unsigned index_array_prv_DRAM[shared_mem_section1_label_count];
	array_init(index_array_DRAM,shared_mem_section1_label_count);
	array_init(index_array_prv_DRAM,shared_mem_section1_label_count);
	unsigned dram_indices = get_user_input_DRAM(index_array_DRAM);

	//counters for row and column, cored id and loop counter
	unsigned   row_loop,col_loop;
	// this will contain the epiphany platform configuration
	e_platform_t epiphany;
	e_epiphany_t dev;
	e_return_stat_t result;
	unsigned int message[9];
	unsigned int message2[9];
	int loop;
	int addr;
	e_mem_t emem;
	e_init(NULL); // initialise the system establish connection to the Device
	//initial ecore is set to 0,0
	/*
	 * reserve shared mem sections
	 * one section for each shared label in the model
	 * shared mem address space starts at 0x0100_0000
	 * and ends at 0x3FFF_FFFF
	 *
	 */
	e_alloc(&emem, shared_mem_section , sizeof(shared_label_to_read));
	e_reset_system(); // reset the epiphnay chip
	e_get_platform_info(&epiphany);//gets the configuration info for the parallella platofrm
	//debug flag
	//------------------------------------------------
	//one core within the parallella work group is 1 x 1 i.e single core
	e_open(&dev,0,0,2,1); //2 rows one column rectangle size 2*1
	//reset the group
	e_reset_group(&dev);
	e_return_stat_t result1;
	e_return_stat_t result2;
	//load the group
	result1 =  e_load("core0_main.elf",&dev,0,0,E_FALSE);
	result2 =  e_load("core1_main.elf",&dev,1,0,E_FALSE);
	if (result1 != E_OK || result2 != E_OK){
		fprintf(stderr,"Error Loading the Epiphany Application 1 %i\n", result);
	}
	e_start_group(&dev);
	fprintf(stderr,"Legend: \n");
	fprintf(stderr,"--------\n");
	fprintf(stderr," Tick				 -> RTOS tick \n");
	fprintf(stderr," Core 1 -> (0,0) \n");
	fprintf(stderr," Core 2 -> (1,0) \n");
	fprintf(stderr," THC 				 -> Task Holding Core(row, column) \n");
	fprintf(stderr," L 					 -> Local core memory \n");
	fprintf(stderr," F 					 -> Foreign memory (DRAM) \n");
	fprintf(stderr,"(L,row,column,indx)  -> Label of index _indx_ on Local core memory(row, column) \n");
	fprintf(stderr,"----------------------------------------------\n");

	fprintf(stderr,"RFTP demo started \n");
	addr = cnt_address;
	int pollLoopCounter = 0;
	int taskMessage;
	int prevtaskMessage;
	int prevpollLoopCounter = 0;
	unsigned int chainLatencyEndIndicator = 0;
	unsigned int chainLatencyStartIndicator = 10e6;
	unsigned int lat1 = 0;
	int label_to_feed_in = 97;
	fprintf(stderr,"===========================================================================\n");
	fprintf(stderr,"     | Tasks being executed| Shared labels' values |\n");
	fprintf(stderr,"tick | Core 1   | Core 2   |");
	user_config_print_legend(core1,index_array1);
	user_config_print_legend(core2,index_array2);
	user_config_print_legend_auto(dram_indices,index_array_DRAM);

	fprintf(stderr,"\n");
	fprintf(stderr,"===========================================================================\n");

	int prev1,prev2,prev3;
	for (pollLoopCounter=0;pollLoopCounter<=40;pollLoopCounter++){
		message[3] = 0;
		e_read(&dev,0,0,addr, &message, sizeof(message));
		e_read(&dev,0,0,DSHM_LABEL_EPI_CORE_OFFSET, &shared_label_core_00, sizeof(shared_label_core_00));
		e_read(&dev,1,0,addr, &message2, sizeof(message2));
		e_read(&dev,1,0,DSHM_LABEL_EPI_CORE_OFFSET, &shared_label_core_10, sizeof(shared_label_core_10));
		e_read(&emem,0,0,0x00, &shared_label_to_read, 10*sizeof(unsigned int));
		if (message[8]!= message2[8] ){
			//fprintf(stderr,"NIS->");
		}
		fprintf(stderr, "%4d |",message[8]+1);
		fprintf(stderr,"   %4u   |", message[6]);
		fprintf(stderr,"   %4u   |", message2[6]);
		user_config_print_values(core1,index_array1,shared_label_core_00,index_array1_prv_val);
		user_config_print_values(core2,index_array2,shared_label_core_10,index_array2_prv_val);
		user_config_print_values_auto(dram_indices,index_array_DRAM,shared_label_to_read,index_array_prv_DRAM);
		fprintf(stderr,"\n");
		nsleep(1);
	}
	fprintf(stderr,"----------------------------------------------\n");
	e_close(&dev);
	e_finalize();
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	fprintf(stderr,"RFTP demo complete \n ");
	return 0;
}

