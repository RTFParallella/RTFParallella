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

#include "model_enumerations.h"
#include "RTFParallellaConfig.h"
#include "trace_utils_BTF.h"


static const char SHM_sec1_enum [SHM_LABEL_COUNT][LABEL_STRLEN] =
{
    "sh_label_a",
    "sh_label_b",
    "sh_label_c",
    "sh_label_d",
    "sh_label_e",
    "sh_label_f",
    "sh_label_g",
    "sh_label_h",
    "sh_label_i",
    "sh_label_j"
};

static const char DSHM_sec1_enum [DSHM_LABEL_COUNT][LABEL_STRLEN] =
{
    "DSH_labelA",
    "DSH_labelB",
    "DSH_labelC",
    "DSH_labelD",
    "DSH_labelE",
    "DSH_labelF",
    "DSH_labelG",
    "DSH_labelH",
    "DSH_labelI",
    "DSH_labelJ"
};

static const char task_enum [EXEC_TASK_COUNT +1][LABEL_STRLEN] =
{
    "[idle]",
    "Task5ms0",
    "Task10ms0",
    "Task20ms0",
    "Task10ms1",
    "Task20ms1"
};


static const char runnable_enum[][LABEL_STRLEN] =
{
    "handler5ms",
    "handler10ms",
    "handler20ms",
    "handler10msCore2",
    "handler20msCore2",
};

static const char hw_enum[][LABEL_STRLEN] = {
    "Core_0",
    "Core_1"
};


static unsigned int DSHM_visible_labels [DSHM_VISIBLE_LABEL_COUNT] = {0,1};
static unsigned int SHM_visible_labels [SHM_VISIBLE_LABEL_COUNT] = {0,1};



void get_SHM_label_name (int index,char str[])
{
    for (int pos = 0; pos < LABEL_STRLEN; pos++)
    {
        str[pos] = SHM_sec1_enum[index][pos];
    }
}

void get_DSHM_label_name(int index,char str[])
{
    for (int pos = 0; pos < LABEL_STRLEN; pos++)
    {
        str[pos] = DSHM_sec1_enum[index][pos];
    }
}

void get_task_name(int index,char *str)
{
    for (int pos = 0; pos < LABEL_STRLEN; pos++)
    {
        str[pos] = task_enum[index][pos];
    }
}

void get_visible_label_index(unsigned array[],unsigned mem_type)
{
    int index = 0;
    if (mem_type == 0)
    {
        for (index = 0; index < SHM_VISIBLE_LABEL_COUNT; index++)
        {
            array[index] = SHM_visible_labels[index];
        }
    }
    for (index = 0;  index < DSHM_VISIBLE_LABEL_COUNT; index++)
    {
        array[index] = DSHM_visible_labels[index];
    }
}


void generate_task_entity_table(void)
{
    store_entity_entry(IDLE_TASK_ID, TASK_EVENT, task_enum[0]);
    store_entity_entry(TASK5MS0_ID, TASK_EVENT, task_enum[1]);
    store_entity_entry(TASK10MS0_ID, TASK_EVENT, task_enum[2]);
    store_entity_entry(TASK20MS0_ID, TASK_EVENT, task_enum[3]);
    store_entity_entry(TASK10MS1_ID, TASK_EVENT, task_enum[4]);
    store_entity_entry(TASK20MS1_ID, TASK_EVENT, task_enum[5]);
}

void generate_runnable_entity_table(void)
{
    store_entity_entry(RUNNABLE_HANDLER5MS0_ID, RUNNABLE_EVENT, runnable_enum[0]);
    store_entity_entry(RUNNABLE_HANDLER10MS0_ID, RUNNABLE_EVENT, runnable_enum[1]);
    store_entity_entry(RUNNABLE_HANDLER20MS0_ID, RUNNABLE_EVENT, runnable_enum[2]);
    store_entity_entry(RUNNABLE_HANDLER10MS1_ID, RUNNABLE_EVENT, runnable_enum[3]);
    store_entity_entry(RUNNABLE_HANDLER20MS1_ID, RUNNABLE_EVENT, runnable_enum[4]);
}

void generate_signal_entity_table(void)
{
    store_entity_entry(SH_LABEL_A_ID, SIGNAL_EVENT, SHM_sec1_enum[0]);
    store_entity_entry(DSH_LABEL_B_ID, SIGNAL_EVENT, SHM_sec1_enum[1]);
    store_entity_entry(DSH_LABEL_A_ID, SIGNAL_EVENT, DSHM_sec1_enum[0]);
    store_entity_entry(DSH_LABEL_B_ID, SIGNAL_EVENT, DSHM_sec1_enum[1]);
}

void generate_hw_entity_table(void)
{
    store_entity_entry(HW_CORE0_ID, CORE_EVENT, hw_enum[0]);
    store_entity_entry(HW_CORE1_ID, CORE_EVENT, hw_enum[1]);
}
