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

#ifndef SRC_PARALLELLA_LABEL_MAN_CORE0_H_
#define SRC_PARALLELLA_LABEL_MAN_CORE0_H_

#include "shared_comms.h"
#include "c2c.h"

/**
 *  Structure to ensure proper synchronization between host and epiphany cores
 * and also within epiphany cores.
 */
typedef struct software_interrupt
{
    int is_init_done;                 /**< To ensure that the mutex is initialized */
    int done;                         /**< To ensure that the mutex is initialized */
#ifdef __EPIPHANY_DEVICE__
    e_mutex_t* mutex;                 /**< Mutex declaration on Epiphany core for synchronization */
#else
    void* mutex;                      /**< Mutex declaration. Unused on host  */
#endif
    int offset;                       /**< Offset of the local memory in epiphany core */
    int row_id;                       /**< Row ID of the Epiphany core */
    int col_id;                       /**< Row ID of the Epiphany core */
} software_interrupt_t;

#define num_unique_sections 1

void init_mem_sections(void);

void init_DSHM_sections(void);

//declare cIn/Out operations
//to be set up from the amalthea model during
//code generation
void cIn5ms();
void cIn10ms();
void cIn20ms();
void cIn10msCore2();
void cIn20msCore2();
//-------
void cOut5ms();
void cOut10ms();
void cOut20ms();
void cOut10msCore2();
void cOut20msCore2();






#endif /* SRC_PARALLELLA_LABEL_MAN_CORE0_H_ */
