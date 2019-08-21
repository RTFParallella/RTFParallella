/*******************************************************************************
 * Copyright (c) 2019 Dortmund University of Applied Sciences and Arts and others.
 *
 * This program and the accompanying materials are made
 * available under the terms of the Eclipse Public License 2.0
 * which is available at https://www.eclipse.org/legal/epl-2.0/
 *
 * SPDX-License-Identifier: EPL-2.0
 *
 * Contributors:
 *      Dortmund University of Applied Sciences and Arts - initial API and implementation
 *******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
//#include <e-hal.h> //hardware abstraction library
#include <time.h>   /* Needed for struct timespec */
//rtfp imports
#include "c2c.h"
#include "debugFlags.h"
#include "shared_comms.h"

#define READ_PRECISION_US 1000

typedef struct{
	unsigned row;
	unsigned col;
	unsigned num_visible_labels;
}LabelVisual;

void array_init(unsigned array[],unsigned array_size);

void user_config_print_legend(LabelVisual core_config,unsigned array[]);

void user_config_print_values(LabelVisual core_config,unsigned array[],unsigned int values_array[],unsigned int prv_val_array[]);

LabelVisual get_user_input(unsigned indices[]);

void user_config_print_legend_DRAM(unsigned array_length,unsigned array[]);

void user_config_print_values_DRAM(unsigned visible_label_count,unsigned array[],unsigned int values_array[],unsigned int prv_val_array[]);

unsigned get_user_input_DRAM(unsigned indices[]);

int nsleep(long miliseconds);


