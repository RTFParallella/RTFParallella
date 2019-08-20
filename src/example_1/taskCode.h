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

#ifndef SRC_PARALLELLA_TASKCODE_H_
#define SRC_PARALLELLA_TASKCODE_H_
#include "../rtfparallella_framework/shared_comms.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "c2c.h"
#include "debugFlags.h"
#include "ParallellaUtils.h"

//declare taskHandler functions

void handler5ms();
void handler10ms();
void handler20ms();
void handler10msCore2();
void handler20msCore2();
//-------
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

#endif /* SRC_PARALLELLA_TASKCODE_H_ */
