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
#include "debugFlags.h"

#define num_unique_sections 1

void init_mem_sections(void);

void init_DSHM_sections(void);

//declare cIn/Out operations
//to be set up from the amalthea model during
//code generation
#ifdef RFTP_GENERATE_BTF_TRACE
void cIn5ms(int srcId, int srcInstance);
void cIn10ms(int srcId, int srcInstance);
void cIn20ms(int srcId, int srcInstance);
void cIn10msCore2(int srcId, int srcInstance);
void cIn20msCore2(int srcId, int srcInstance);
//-------
void cOut5ms(int srcId, int srcInstance);
void cOut10ms(int srcId, int srcInstance);
void cOut20ms(int srcId, int srcInstance);
void cOut10msCore2(int srcId, int srcInstance);
void cOut20msCore2(int srcId, int srcInstance);
#else
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
#endif






#endif /* SRC_PARALLELLA_LABEL_MAN_CORE0_H_ */
