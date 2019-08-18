/*
 * shared_comms.h
 *
 *  Created on: Jul 17, 2019
 *      Author: bigm
 */

#ifndef DEMO_PARALLELLA_SHARED_COMMS_H_
#define DEMO_PARALLELLA_SHARED_COMMS_H_

#include <stdlib.h>
#include <stdint.h>
//#include <e-lib.h>


#define shared_mem_section	0x01000000

#define shared_mem_section_2	0x01001000

#define shared_mem_section1_label_count 10





void shared_labels_init();

uint8_t shared_label_write	(int label_indx,int payload);

unsigned int shared_label_read(int label_indx);




#endif /* DEMO_PARALLELLA_SHARED_COMMS_H_ */
