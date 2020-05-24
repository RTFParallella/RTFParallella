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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>   /* Needed for struct timespec */
#include <e-hal.h>  /* hardware abstraction library */
#include <e-loader.h>

#include "RTFParallellaConfig.h"
#include "c2c.h"
#include "debugFlags.h"
#include "shared_comms.h"
#include "host_utils.h"
#include "model_enumerations.h"
#include "trace_utils_BTF.h"

unsigned int shared_label_to_read[SHM_LABEL_COUNT];
unsigned int shared_label_core_00[DSHM_SEC_LABEL_COUNT];
unsigned int shared_label_core_10[DSHM_SEC_LABEL_COUNT];
unsigned int shared_label_core[EXEC_CORE_COUNT][DSHM_SEC_LABEL_COUNT];
unsigned int shared_dram_start_address = SHARED_DRAM_START_OFFSET;


static void construct_btf_trace_header(FILE *stream)
{
    write_btf_trace_header_config(stream);
    write_btf_trace_header_entity_type(stream, TASK_EVENT);
    write_btf_trace_header_entity_type(stream, RUNNABLE_EVENT);
    write_btf_trace_header_entity_type(stream, SIGNAL_EVENT);
    generate_task_entity_table();
    generate_runnable_entity_table();
    generate_signal_entity_table();
    generate_hw_entity_table();
    write_btf_trace_header_entity_table(stream);
    write_btf_trace_header_entity_type_table(stream);
}


/* Entry point of the application running on the HOST Core. */
int main(int argc, char *argv[])
{
    /* This will contain the epiphany platform configuration */
    e_platform_t epiphany;
    e_epiphany_t dev;
    e_mem_t emem;
    unsigned int ecore0[RTF_DEBUG_TRACE_COUNT];
    unsigned int ecore1[RTF_DEBUG_TRACE_COUNT];
    unsigned int labelVisual_perCore[EXEC_CORE_COUNT][DSHM_VISIBLE_LABEL_COUNT];
    unsigned int prv_val_preCore[EXEC_CORE_COUNT][DSHM_VISIBLE_LABEL_COUNT];
    unsigned int labelVisual_SHM[SHM_VISIBLE_LABEL_COUNT];
    unsigned int prv_val_SHM[SHM_VISIBLE_LABEL_COUNT];
    int index = 0;
    btf_trace_info trace_info;

    trace_info.is_init_done = 0;
    trace_info.rw_operation = -1;
    for(index = 0; index < BTF_TRACE_BUFFER_SIZE; index++)
    {
        trace_info.data[index] = 0;
    }

    FILE *fp_to_trace = NULL;
    int scale_factor = parse_btf_trace_arguments(argc, argv);
    char trace_file_path[512] = {0};
    get_btf_trace_file_path(trace_file_path);
    if (strlen((const char *)trace_file_path) != 0)
    {
        fp_to_trace = fopen((const char *)trace_file_path, "w+");
        if (fp_to_trace == NULL)
        {
            fprintf(stderr,"Output redirected to stderr\n");
            fp_to_trace = stderr;
        }
    }
    else
    {
        fprintf(stderr,"Output redirected to stderr\n");
        fp_to_trace = stderr;
    }
    construct_btf_trace_header(fp_to_trace);

    for (index = 0;index < EXEC_CORE_COUNT; index++)
    {
        /* Get the visible label index per core for epiphany device */
        get_visible_label_index(labelVisual_perCore[index],MEM_TYPE_DSHM);
    }

    /* Get the visible label index for the variables stored in shared memory */
    get_visible_label_index(labelVisual_SHM,MEM_TYPE_SHM);

    for (index = 0;index < EXEC_CORE_COUNT; index++)
    {
        printf ("@%d : ",index);
        for (int count = 0; count < DSHM_VISIBLE_LABEL_COUNT; count++)
        {
            printf("L%d ",labelVisual_perCore[index][count]);
        }
        printf("\n");
    }

    fprintf(stderr,"======================================================================="
                    "=======================================================================\n");
    fprintf(stderr,"        |   Tasks being executed  |                                 "
            "      observed labels values                                          |\n");
    fprintf(stderr,"%8s|%12s|%12s|"," tick ","   Core 1   ","   Core 2   ");
    for (index = 0;index < EXEC_CORE_COUNT;index++)
    {
        /* Get the device shared Label name*/
        print_legend_enum(DSHM_VISIBLE_LABEL_COUNT, labelVisual_perCore[index], MEM_TYPE_DSHM);
    }

    /* Get the shared label name */
    print_legend_enum(SHM_VISIBLE_LABEL_COUNT + 2, labelVisual_SHM, MEM_TYPE_SHM);
    fprintf(stderr,"\n");
    fprintf(stderr,"======================================================================="
                    "=======================================================================\n");

    e_init(NULL);

    /* Reserve the memory for the data in the shared dram region to be shared between
     * host and epiphany core. The dram offset starts at 0x01000000 which corresponds
     * to the global address as 0x8F000000. */
    if (E_OK != e_alloc(&emem, (unsigned int)shared_dram_start_address , SHARED_DRAM_SIZE))
    {
        fprintf(stderr, "Error in reserving the shared dram buffer\n");
    }


    /* Reset the system and get the platform information. */
    e_reset_system();
    e_get_platform_info(&epiphany);

    /* one core within the parallella work group is 1 x 1 i.e single core
     * 2 rows one column rectangle size 2*1.
     */
    e_open(&dev,0,0,2,1);
    /* reset the group */
    e_reset_group(&dev);
    /* load the group */
    if (E_OK != e_load("core0_main.elf", &dev, 0, 0, E_FALSE))
    {
        fprintf(stderr,"Error Loading the Epiphany Application on core with row=0 and col=0\n");
    }
    if (E_OK != e_load("core1_main.elf", &dev, 1, 0, E_FALSE))
    {
        fprintf(stderr,"Error Loading the Epiphany Application on core with row=1 and col=0\n");
    }
    /* Write the time unit used for the configuration of the clock cycle per tick */
    if (sizeof(int) != e_write(&emem, 0, 0, INPUT_TIMESCALE_OFFSET, &scale_factor, sizeof(int)))
    {
        fprintf(stderr, "Error in writing to the shared dram buffer\n");
    }

    /* Write the initialized trace buffer values to the shared memory */
    if (sizeof(btf_trace_info) != e_write(&emem, 0, 0, SHARED_BTF_DATA_OFFSET, &trace_info, sizeof(btf_trace_info)))
    {
        fprintf(stderr, "Error in writing to the shared dram buffer\n");
    }

    e_start_group(&dev);
    int pollLoopCounter = 0;
    char buffer1[LABEL_STRLEN] = {0};
    char buffer2[LABEL_STRLEN] = {0};

    for (pollLoopCounter = 0; pollLoopCounter <= 50; pollLoopCounter++)
    {
        e_read(&dev, 0, 0, ECORE_RTF_BUFFER_ADDR, ecore0, sizeof(ecore0));
        e_read(&dev, 0, 0, DSHM_LABEL_EPI_CORE_OFFSET, &shared_label_core[0],
                            sizeof(shared_label_core_00));
        e_read(&dev, 1, 0, ECORE_RTF_BUFFER_ADDR, ecore1, sizeof(ecore1));
        e_read(&dev, 1, 0, DSHM_LABEL_EPI_CORE_OFFSET, &shared_label_core[1],
                            sizeof(shared_label_core_10));
        e_read(&emem, 0, 0, SHARED_BTF_DATA_OFFSET + sizeof(btf_trace_info) ,
                            shared_label_to_read, sizeof(shared_label_to_read));

        /* Check the tick count of both the messages */
        if (ecore0[8]!= ecore1[8] )
        {
            /* Left empty intentionally */
        }
        get_task_name(ecore0[6],buffer1);
        get_task_name(ecore1[6],buffer2);

        fprintf(stderr," %6d | %10s | %10s | ", ((ecore0[8] + 1) * scale_factor), buffer1, buffer2);

        for (index = 0;index < EXEC_CORE_COUNT; index++)
        {
            user_config_print_values_auto(DSHM_VISIBLE_LABEL_COUNT,
                    labelVisual_perCore[index], shared_label_core[index], prv_val_preCore[index]);
        }

        for (index = 0; index < (SHM_VISIBLE_LABEL_COUNT + 2); index++)
        {
            fprintf(stderr," %10d |",shared_label_to_read[index]);
        }

        fprintf(stderr,"\n");
        nsleep(1);
    }
    fprintf(stderr,"----------------------------------------------\n");
    if (fp_to_trace != NULL)
    {
        fclose(fp_to_trace);
        fp_to_trace = NULL;
    }
    e_close(&dev);
    e_finalize();
    fprintf(stderr,"RFTP demo complete \n ");
    return 0;
}



