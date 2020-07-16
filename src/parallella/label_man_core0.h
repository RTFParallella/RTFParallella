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

#define num_unique_sections 1

void init_mem_sections(void);

void init_DSHM_sections(void);

//declare cIn/Out operations
//to be set up from the amalthea model during
//code generation
void cIn5ms(int  src_id, int src_instance);
void cIn10ms(int  src_id, int src_instance);
void cIn20ms(int  src_id, int src_instance);
void cIn10msCore2(int  src_id, int src_instance);
void cIn20msCore2(int  src_id, int src_instance);
//-------
void cOut5ms(int  src_id, int src_instance);
void cOut10ms(int  src_id, int src_instance);
void cOut20ms(int  src_id, int src_instance);
void cOut10msCore2(int  src_id, int src_instance);
void cOut20msCore2(int  src_id, int src_instance);






#endif /* SRC_PARALLELLA_LABEL_MAN_CORE0_H_ */
