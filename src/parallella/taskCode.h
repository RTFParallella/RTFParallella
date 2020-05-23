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

#ifndef SRC_PARALLELLA_TASKCODE_H_
#define SRC_PARALLELLA_TASKCODE_H_
#include "c2c.h"
#include "ParallellaUtils.h"
#include "shared_comms.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

//declare taskHandler functions

void handler5ms();
void handler10ms();
void handler20ms();
void handler10msCore2();
void handler20msCore2();
//-------


#endif /* SRC_PARALLELLA_TASKCODE_H_ */
