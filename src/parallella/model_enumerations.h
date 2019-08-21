/*
 * model_enumerations.h
 *
 *  Created on: Aug 21, 2019
 *      Author: mahmoud
 */

#ifndef SRC_PARALLELLA_MODEL_ENUMERATIONS_H_
#define SRC_PARALLELLA_MODEL_ENUMERATIONS_H_


#define	SHM_section1_label_count 	10

#define	DSHM_section1_label_count	10

#define label_str_len				10

#define task_count					5

#define core_count					2

#define SHM_visible_label_count		2

#define DSHM_visible_label_count	4


void get_SHM_label_name (int index,char *str);

void get_DSHM_label_name(int index,char *str);

void get_task_name(int index,char *str);

void get_visible_label_index(unsigned array[]);


#endif /* SRC_PARALLELLA_MODEL_ENUMERATIONS_H_ */
