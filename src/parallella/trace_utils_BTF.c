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
#include <getopt.h>
#include <time.h>
#include "trace_utils_BTF.h"


/*-------------------------GOLBAL VARIABLES--------------------*/
const char btf_trace_version = "#version 1.0";

/*-------------------------STATIC GOLBAL VARIABLES--------------------*/
static btf_trace_header_config_t btf_header;
static uint8_t output_trace_path[512];

/*-------------------------CONST VARIABLES---------------------------*/
const uint8_t event_type[][8] = {
        "T",
        "ISR",
        "R",
        "IB",
        "STI",
        "ECU",
        "P",
        "C",
        "SCHED",
        "SIG",
        "SEM",
        "SIM"
};

const uint8_t event_name[][16] = {
        "start",
        "terminate",
        "read",
        "write"
};


/*-------------------------STATIC FUNCTIONS----------------------------*/
static void print_usage(void);
static void get_trace_timestamp(uint8_t *buffer);



static int validate_timescale(char *scale)
{
	return BTF_TRACE_TRUE;
}

/* Function to get the current time of creation of btf trace file */
static void get_trace_timestamp(uint8_t *buffer)
{
    time_t timer;
    struct tm* tm_info;
    time(&timer);
    tm_info = localtime(&timer);
    /* The total number of characters to display time is 26 */
    strftime(buffer, 26, "%Y-%m-%d %H:%M:%SZ", tm_info);
}

/* Function to display to usage of the command line parameters */
static void print_usage(void)
{
    fprintf(stdout, "Usage:\n");
    fprintf(stdout,"\t[-t|--trace-btf]=<Output trace file name.>\n");
    fprintf(stdout,"\t[-m|--model-file]=<Model file name used to generate the trace file.>\n");
    fprintf(stdout,"\t[-s|--scale]=<Timing scale used to generate the trace file.>\n");
    fprintf(stdout,"\t[-d|--device]=<Device target on which the trace file is generated.>\n");
    fprintf(stdout,"\t[-h|--help]=<Print the help message.>\n");
    fflush(stdout);
}


/**
 * Parse the command line arguments for generating the BTF trace file
 *
 * Arguments:
 * @in_param argc  : The count for the number of arguments passed
 * @in_param argv  : Pointer to the list of arguments
 *
 * Return: void
 */
void  parse_btf_trace_arguments(int argc, char **argv)
{
    int opt= 0;

    //Specifying the expected options
    //The two options l and b expect numbers as argument
    static const struct option long_options[] = {
        {"trace-btf", required_argument, NULL, 't' },
        {"model-file", required_argument, NULL,  'm' },
        {"scale", optional_argument, NULL, 's'},
        {"device", required_argument, NULL, 'd'},
        {"help", no_argument, NULL, 'h'},
        {NULL, 0, NULL, 0}
    };

    int long_index =0;
    while ((opt = getopt_long(argc, argv,"t:m:s:d:",
                   long_options, &long_index )) != -1) {
        switch (opt) {
             case 't' :
                 strncpy((char *)output_trace_path, (const char *)optarg, sizeof(output_trace_path));
                 break;
             case 'm' :
                 strncpy((char *)btf_header.modelfile, (const char *)optarg, sizeof(btf_header.modelfile));
                 break;
             case 'd' :
                 strncpy((char *)btf_header.creator, (const char *)optarg, sizeof(btf_header.creator));
                 break;
             case 's' :
                 strncpy((char *)btf_header.timescale, (const char *)optarg, sizeof(btf_header.timescale));
                 int retval = validate_timescale(btf_header.timescale);
                 if (retval == BTF_TRACE_FALSE)
                 {
                     fprintf(stdout,"Invalid timescale\n");
                     fflush(stdout);
                     exit(EXIT_FAILURE);
                 }
                 break;
             case 'h' :
                 print_usage();
                 exit(EXIT_SUCCESS);
             case '?' :
             default  :
                 print_usage();
                 exit(EXIT_FAILURE);
        }
    }
}

/**
 * Function to write BTF header data to the trace file.
 * It writes the version, creator, input model file,
 * timescale and timestamp section of the header file.
 *
 * Arguments:
 * @in_param stream  : File pointer to the stream where the data has to be
 *                     written.
 *
 * Return: void
 */
void write_btf_trace_header_config(FILE *stream)
{
    if (stream == NULL)
    {
        return;
    }
    /* 32 bytes is enough to store the timescale format */
    uint8_t time_string[32] = {0};
    fprintf(stream, "%s\n", btf_trace_version);
    fprintf(stream, "#creator %s\n", btf_header.creator);
    get_trace_timestamp(time_string);
    fprintf(stream, "#creationdate %s\n", time_string);
    fprintf(stream, "#inputFile %s\n", btf_header.modelfile);
    fprintf(stream, "#timescale %s\n", btf_header.timescale);
    fflush(stream);
}



/**
 * Function to initialize the BTF trace buffer.
 *
 * Arguments:
 * @in_param btf_trace_buf  : Pointer to the BTF trace buffer.
 * @in_param address        : Starting offset of the BTF trace buffer.
 *
 * Return: void
 */
void btf_trace_buf_init(int **btf_trace_buf, int address)
{
    int index;
    btf_trace_buf[0] = (unsigned int *) address;
    btf_trace_buf[1] = btf_trace_buf[0] + 1;
    btf_trace_buf[2] = btf_trace_buf[1] + 1;
    btf_trace_buf[3] = btf_trace_buf[2] + 1;
    btf_trace_buf[4] = btf_trace_buf[3] + 1;
    btf_trace_buf[5] = btf_trace_buf[4] + 1;
    btf_trace_buf[6] = btf_trace_buf[5] + 1;
    btf_trace_buf[7] = btf_trace_buf[6] + 1;
    //initialize buffer
    for (index = 0;index < BTF_TRACE_BUFFER_SIZE; index++){
        *btf_trace_buf[index] = 0x00;
    }
}
