/*
 * shared_comms.c
 *
 *  Created on: Jul 17, 2019
 *      Author: bigm
 */

#include "shared_comms.h"

unsigned int *outbuf_shared[10];

void shared_labels_init(){
	//debug interface in shared memory
	outbuf_shared[0] = (unsigned int *) shared_mem_section;
	outbuf_shared[1] = outbuf_shared[0] + 1;
	outbuf_shared[2] = outbuf_shared[1] + 1;
	outbuf_shared[3] = outbuf_shared[2] + 1;
	outbuf_shared[4] = outbuf_shared[3] + 1;
	outbuf_shared[5] = outbuf_shared[4] + 1;
	outbuf_shared[6] = outbuf_shared[5] + 1;
	outbuf_shared[7] = outbuf_shared[6] + 1;
	outbuf_shared[8] = outbuf_shared[7] + 1;
	//initialize buffer
	int i;
	//timer1init();
	for (i=0;i<9;i++){
		*outbuf_shared[i] =0;
	}
}

uint8_t shared_label_write(int label_indx,int payload){
	uint8_t retval=NULL;
	*outbuf_shared[label_indx] = payload;
	/*if (payload == *outbuf_shared[label_indx]){
		retval = 1;
	}*/
	return retval;
}


unsigned int shared_label_read(int label_indx){
	return *outbuf_shared[label_indx];
}



//-------------------end of file-------------------------//
