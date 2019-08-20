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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <e-hal.h> //hardware abstraction library
#include <time.h>   /* Needed for struct timespec */

#include "../rtfparallella_framework/c2c.h"
#include "../rtfparallella_framework/debugFlags.h"
#include "../rtfparallella_framework/shared_comms.h"

#define READ_PRECISION_US 1000

typedef struct{
	unsigned row;
	unsigned col;
	unsigned num_visible_labels;
}LabelVisual;

void array_init(unsigned array[],unsigned array_size){
	for (int i=0;i<array_size;i++){
		array[i] = 0;
	}
}

void user_config_print_legend(LabelVisual core_config,unsigned array[]){
	for (int i=0;i<core_config.num_visible_labels;i++){
		fprintf(stderr,"(L,%u,%u,%u)|",core_config.row,core_config.col,array[i]);
	}
}

void user_config_print_values(LabelVisual core_config,unsigned array[],unsigned int values_array[],unsigned int prv_val_array[]){
	for(int i=0;i<core_config.num_visible_labels;i++){
		int val_ind = array[i];
		unsigned int val_to_print = values_array[val_ind];
		if (val_to_print == prv_val_array[val_ind]){
			fprintf(stderr,"  ----   |");
		}else {
			fprintf(stderr,"  %4d   |",val_to_print);
			prv_val_array[val_ind] = val_to_print;
		}

	}
}

LabelVisual get_user_input(unsigned indices[]){
	unsigned row_in,col_in,label_num;
	printf("Enter row and column of core to read ");
	scanf("%u,%u",&row_in,&col_in);
	printf("Enter number of visible labels");
	scanf("%u",&label_num);
	unsigned visible_label_array[label_num];
	printf("Enter indices of labels\n");
	for (int i=0;i<label_num;i++){
		scanf("%u",&indices[i]);
	}
	LabelVisual retVal;
	retVal.row = row_in;
	retVal.col = col_in;
	retVal.num_visible_labels = label_num;
	/*for (int i=0;i<label_num;i++){
		printf("%u",indices[i]);
	}*/
	return retVal;
}


void user_config_print_legend_DRAM(unsigned array_length,unsigned array[]){
	for (int i=0;i<array_length;i++){
		fprintf(stderr,"(F,%u)|",array[i]);
	}
}

void user_config_print_values_DRAM(unsigned visible_label_count,unsigned array[],unsigned int values_array[],unsigned int prv_val_array[]){
	for(int i=0;i<visible_label_count;i++){
		int val_ind = array[i];
		unsigned int val_to_print = values_array[val_ind];
		if (val_to_print == prv_val_array[val_ind]){
			fprintf(stderr," ----|");
		}else {
			fprintf(stderr,"%4d |",val_to_print);
			prv_val_array[val_ind] = val_to_print;
		}
	}
}

unsigned get_user_input_DRAM(unsigned indices[]){
	unsigned label_num;
	printf("Enter number of visible labels( shared mem)");
	scanf("%u",&label_num);
	unsigned visible_label_array[label_num];
	printf("Enter indices of labels\n");
	for (int i=0;i<label_num;i++){
		scanf("%u",&indices[i]);
	}
	/*for (int i=0;i<label_num;i++){
		printf("%u",indices[i]);
	}*/
	return label_num;
}

int nsleep(long miliseconds){
   struct timespec req, rem;
   if(miliseconds > 999){
        req.tv_sec = (int)(miliseconds / 1000);                            /* Must be Non-Negative */
        req.tv_nsec = (miliseconds - ((long)req.tv_sec * 1000)) * 1000000; /* Must be in range of 0 to 999999999 */
   } else {
        req.tv_sec = 0;                         /* Must be Non-Negative */
        req.tv_nsec = miliseconds * 1000000;    /* Must be in range of 0 to 999999999 */
   }
   return nanosleep(&req , &rem);
}


unsigned int shared_label_to_read[10];
unsigned int shared_label_core_00[dstr_mem_sec_1_label_count];
unsigned int shared_label_core_10[dstr_mem_sec_1_label_count];

int main()
{
	int label_enable_count_core0 = 0;
	printf("RTFP demo host code\n");
	//setup visualization config for the first core
	unsigned index_array1[dstr_mem_sec_1_label_count];
	unsigned index_array1_prv_val[dstr_mem_sec_1_label_count];
	array_init(index_array1,dstr_mem_sec_1_label_count);
	array_init(index_array1_prv_val,dstr_mem_sec_1_label_count);
	LabelVisual core1 = get_user_input(index_array1);
	//setup visualization config for the first core
	unsigned index_array2[dstr_mem_sec_1_label_count];
	unsigned index_array2_prv_val[dstr_mem_sec_1_label_count];
	array_init(index_array2,dstr_mem_sec_1_label_count);
	array_init(index_array2_prv_val,dstr_mem_sec_1_label_count);
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
	result1 =  e_load("main.elf",&dev,0,0,E_FALSE);
	result2 =  e_load("main_2.elf",&dev,1,0,E_FALSE);
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
	user_config_print_legend_DRAM(dram_indices,index_array_DRAM);

	fprintf(stderr,"\n");
	fprintf(stderr,"===========================================================================\n");

	int prev1,prev2,prev3;
	for (pollLoopCounter=0;pollLoopCounter<=40;pollLoopCounter++){
		message[3] = 0;
		e_read(&dev,0,0,addr, &message, sizeof(message));
		e_read(&dev,0,0,dstr_mem_offset_sec_1, &shared_label_core_00, sizeof(shared_label_core_00));
		e_read(&dev,1,0,addr, &message2, sizeof(message2));
		e_read(&dev,1,0,dstr_mem_offset_sec_1, &shared_label_core_10, sizeof(shared_label_core_10));
		e_read(&emem,0,0,0x00, &shared_label_to_read, sizeof(shared_label_to_read));
		if (message[8]!= message2[8] ){
			//fprintf(stderr,"NIS->");
		}
		fprintf(stderr, "%4d |",message[8]+1);
		fprintf(stderr,"   %4u   |", message[6]);
		fprintf(stderr,"   %4u   |", message2[6]);
		user_config_print_values(core1,index_array1,shared_label_core_00,index_array1_prv_val);
		user_config_print_values(core2,index_array2,shared_label_core_10,index_array2_prv_val);
		user_config_print_values_DRAM(dram_indices,index_array_DRAM,shared_label_to_read,index_array_prv_DRAM);
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

