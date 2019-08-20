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

#include "include/shared_comms.h"

unsigned int *outbuf_shared[shared_mem_section1_label_count];

void shared_labels_init(){
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
	for (i=0;i<shared_mem_section1_label_count;i++){
		*outbuf_shared[i] =0;
	}
}

/*void shared_labels_init_multi(){

}*/



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
