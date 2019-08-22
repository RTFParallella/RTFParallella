/*******************************************************************************
 * Copyright (c) 2019 IBM Corporation and others.
 *
 * This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License 2.0
 * which accompanies this distribution, and is available at
 * https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *     IBM Corporation - initial API and implementation
 *******************************************************************************/
#ifndef SRC_PARALLELLA_MODEL_ENUMERATIONS_H_
#define SRC_PARALLELLA_MODEL_ENUMERATIONS_H_


#define	SHM_section1_label_count 	10

#define	DSHM_section1_label_count	10

#define label_str_len				10

#define task_count					5

#define core_count					2

#define SHM_visible_label_count		2

#define DSHM_visible_label_count	2

/**
 * Get the string name of DRAM shared label
 *
 * Arguments:
 * index			:	shared label index in the shared memory section
 * *str				:	pointer to buffer string that holds the name
 */
void get_SHM_label_name (int index,char *str);

/**
 * Get the string name of distributed shared label (on a core)
 *
 * Arguments:
 * index			:	shared label index in the memory section
 * *str				:	pointer to buffer string that holds the name
 */
void get_DSHM_label_name(int index,char *str);

/**
 * Get the string name of the task being run
 *
 * Arguments:
 * index			:	task index in the task_enum array
 * *str				:	pointer to buffer string that holds the name
 */
void get_task_name(int index,char *str);

/**
 * Get the indices of required labels to show in either shared memory or distributed shared memory
 *
 * Arguments:
 * array			:	array buffer that holds the indices
 * mem_type			:	the memory type of indices requested (MEM_TYPE_SHM or MEM_TYPE_DSHM)
 */
void get_visible_label_index(unsigned array[],unsigned mem_type);


#endif /* SRC_PARALLELLA_MODEL_ENUMERATIONS_H_ */
