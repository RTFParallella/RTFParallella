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

#include "host_utils.h"

void array_init(unsigned array[],unsigned array_size)
{
    for (int index = 0; index < array_size; index++)
    {
        array[index] = 0;
    }
}


void user_config_print_legend(LabelVisual core_config,unsigned array[])
{
    for (int index = 0; index < core_config.num_visible_labels; index++)
    {
        fprintf(stderr,"(L,%u,%u,%u)|", core_config.row, core_config.col, array[index]);
    }
}

void user_config_print_values(LabelVisual core_config,unsigned array[],
        unsigned int values_array[],unsigned int prv_val_array[])
{
    for(int index = 0; index < core_config.num_visible_labels; index++)
    {
        int val_ind = array[index];
        unsigned int val_to_print = values_array[val_ind];
        if (val_to_print == prv_val_array[val_ind])
        {
            fprintf(stderr,"  ----   |");
        }
        else
        {
            fprintf(stderr,"  %4d   |",val_to_print);
            prv_val_array[val_ind] = val_to_print;
        }
    }
}

LabelVisual get_user_input(unsigned indices[])
{
    unsigned row_in,col_in,label_num;
    printf("Enter row and column of core to read ");
    scanf("%u,%u",&row_in,&col_in);
    printf("Enter number of visible labels");
    scanf("%u",&label_num);
    unsigned visible_label_array[label_num];
    printf("Enter indices of labels\n");
    for (int index = 0; index < label_num; index++)
    {
        scanf("%u",&indices[index]);
    }
    LabelVisual retVal;
    retVal.row = row_in;
    retVal.col = col_in;
    retVal.num_visible_labels = label_num;
    return retVal;
}


void user_config_print_legend_auto(unsigned array_length,unsigned array[])
{
    for (int index = 0; index < array_length; index++)
    {
        fprintf(stderr,"(F,%u)|",array[index]);
    }
}

void user_config_print_values_auto(unsigned visible_label_count,unsigned array[],
        unsigned int values_array[],unsigned int prv_val_array[])
{
    for(int index = 0; index < visible_label_count; index++)
    {
        int val_ind = array[index];
        unsigned int val_to_print = values_array[val_ind];
        if (val_to_print == prv_val_array[val_ind])
        {
            fprintf(stderr,"            |");
        }
        else
        {
            fprintf(stderr," %10d |",val_to_print);
            prv_val_array[val_ind] = val_to_print;
        }
    }
}

unsigned get_user_input_DRAM(unsigned indices[])
{
    unsigned label_num;
    printf("Enter number of visible labels( shared mem)");
    scanf("%u",&label_num);
    unsigned visible_label_array[label_num];
    printf("Enter indices of labels\n");
    for (int index = 0; index < label_num; index++)
    {
        scanf("%u",&indices[index]);
    }
    return label_num;
}

void print_legend_enum (unsigned label_count,unsigned label_positions[], unsigned memory_type)
{
    char label_name[LABEL_STRLEN] = {0};
    int index = 0;

    if (memory_type == MEM_TYPE_SHM)
    {
        for (index = 0; index < label_count; index++)
        {
            /* Get the shared label name */
            get_SHM_label_name(index,label_name);
            fprintf(stderr," %s |",label_name);
        }
    }
    else
    {
        for (index = 0; index < label_count; index++)
        {
            /* Get the distributed shared label name */
            get_DSHM_label_name(index, label_name);
            fprintf(stderr," %s |", label_name);
        }
    }
    fflush(stderr);
}


int nsleep(long miliseconds)
{
   struct timespec req, rem;
   if(miliseconds > 999)
   {
       /* Must be Non-Negative */
        req.tv_sec = (int)(miliseconds / 1000);
        /* Must be in range of 0 to 999999999 */
        req.tv_nsec = (miliseconds - ((long)req.tv_sec * 1000)) * 1000000;
   }
   else
   {
       /* Must be Non-Negative */
        req.tv_sec = 0;
        /* Must be in range of 0 to 999999999 */
        req.tv_nsec = miliseconds * 1000000;
   }
   return nanosleep(&req , &rem);
}
