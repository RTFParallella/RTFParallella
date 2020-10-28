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

#ifndef SRC_PARALLELLA_MODEL_ENUMERATIONS_H_
#define SRC_PARALLELLA_MODEL_ENUMERATIONS_H_

/**
 * @file model_enumerations.h
 * @author Mahmoud Bazzal, Anand Prakash
 * @date 20 May 2020
 * @brief This file declares and implements the entity table for BTF trace generation. It
 * consists of functions used to store the entities in the tracing framework which is
 * used to generate the BTF trace.
 *
 */


#define SHM_LABEL_COUNT                   10

#define DSHM_LABEL_COUNT                  10

#define LABEL_STRLEN                      32

#define EXEC_TASK_COUNT                    5

#define EXEC_CORE_COUNT                    2

#define SHM_VISIBLE_LABEL_COUNT            2

#define DSHM_VISIBLE_LABEL_COUNT           2

/**
 * @brief Get the string name of DRAM shared label
 *
 * @param[in]     index            :    shared label index in the shared memory section
 * @param[inout]  *str             :    pointer to buffer string that holds the name
 *
 * @return : void
 */
void get_SHM_label_name (int index,char *str);

/**
 * @brief Get the string name of distributed shared label (on a core)
 *
 * @param[in]      index            :    shared label index in the memory section
 * @param[inout]   *str             :    pointer to buffer string that holds the name
 *
 * @return : void
 */
void get_DSHM_label_name(int index,char *str);

/**
 * @brief Get the string name of the task being run
 *
 * @param[in]    index    :    task index in the task_enum array
 * @param[inout] *str     :    pointer to buffer string that holds the name
 *
 * @return : void
 */
void get_task_name(int index,char *str);

/**
 * @brief Get the indices of required labels to show in either shared memory or distributed shared memory
 *
 * @param[inout]     array     :    array buffer that holds the indices
 * @param[in]        mem_type  :    the memory type of indices requested (MEM_TYPE_SHM or MEM_TYPE_DSHM)
 *
 * @return : void
 */
void get_visible_label_index(unsigned array[],unsigned mem_type);


/**
 * @brief Generate the BTF trace entity entry for all the tasks
 *
 * The function is used to store all the tasks entities
 * used in the tasks execution on a heterogeneous platform which is used to
 * generate the BTF header and data section.
 *
 * @return: void
 */
void generate_task_entity_table(void);

/**
 * @brief Generate the BTF trace entity entry for all the runnables
 *
 * The function is used to store all the runnable entities
 * used in the tasks execution on a heterogeneous platform which is used to
 * generate the BTF header and data section.
 *
 * @return: void
 */
void generate_runnable_entity_table(void);

/**
 * @brief Generate the BTF trace entity entry for all the label/signal entities
 *
 * The function is used to store all the shared and distributed label entities
 * used in the tasks execution on a heterogeneous platform which is used to
 * generate the BTF header and data section.
 *
 * @return: void
 */
void generate_signal_entity_table(void);

/**
 * @brief Generate the BTF trace entity entry for all the hardware entities
 *
 * The function is used to store all the hardware entities used in the tasks
 * execution on a heterogeneous platform which is used to generate the BTF
 * header and data section.
 *
 * @return: void
 */
void generate_hw_entity_table(void);


#endif /* SRC_PARALLELLA_MODEL_ENUMERATIONS_H_ */
