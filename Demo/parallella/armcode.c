#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <e-hal.h> //hardware abstraction library
#include <time.h>   /* Needed for struct timespec */
#include "debugFlags.h"
#include "shared_comms.h"
#define READ_PRECISION_US 1000

int nsleep(long miliseconds){
   struct timespec req, rem;
   if(miliseconds > 999){
        req.tv_sec = (int)(miliseconds / 1000);                            /* Must be Non-Negative */
        req.tv_nsec = (miliseconds - ((long)req.tv_sec * 1000)) * 1000000; /* Must be in range of 0 to 999999999 */
   } else {
        req.tv_sec = 0;                         /* Must be Non-Negative */
        req.tv_nsec = miliseconds * 1000000;    /* Must be in range of 0 to 999999999 */
   }
   return nanosleep(&req , &rem);
}

int main()
{
	unsigned int shared_label_to_read[10];
	//unsigned int shared_label_to_read1;
	//shared_label_to_read = (unsigned int *) shared_mem_section;
	//counters for row and colum, cored id and loop counter
	unsigned   row_loop,col_loop;
	// this will contain the epiphany platform configuration
	e_platform_t epiphany;
	e_epiphany_t dev;
	e_return_stat_t result;
	unsigned int message[9];
	unsigned int message2[9];
	//long  message2;
	int loop;
	int addr;
	e_mem_t emem;
	e_init(NULL); // initialise the system establish connection to the Device
	//initial ecore is set to 0,0
	/*
	 * reserve shared mem sections
	 * one section for each shared label in the model
	 * shared mem address space starts at 0x0100_0000
	 * and ends at 0x3FFF_FFFF
	 *
	 */
	e_alloc(&emem, shared_mem_section , sizeof(shared_label_to_read));
	e_reset_system(); // reset the epiphnay chip
	e_get_platform_info(&epiphany);//gets the configuration info for the parallella platofrm
	//debug flag
	//------------------------------------------------
	//one core within the parallella work group is 1 x 1 i.e single core
	e_open(&dev,0,0,2,1); //2 rows one column rectangle size 2*1
	//reset the group
	e_reset_group(&dev);
	e_return_stat_t result1;
	e_return_stat_t result2;
	//load the group
	result1 =  e_load("main.elf",&dev,0,0,E_FALSE);
	result2 =  e_load("main_2.elf",&dev,1,0,E_FALSE);
	if (result1 != E_OK || result2 != E_OK){
		fprintf(stderr,"Error Loading the Epiphany Application 1 %i\n", result);
	}
	e_start_group(&dev);
	fprintf(stderr,"RFTP demo started \n");
	addr = cnt_address;
	int pollLoopCounter = 0;
	int taskMessage;
	int prevtaskMessage;
	int prevpollLoopCounter = 0;
	unsigned int chainLatencyEndIndicator = 0;
	unsigned int chainLatencyStartIndicator = 10e6;
	for (pollLoopCounter=0;pollLoopCounter<=40;pollLoopCounter++){
		message[3] = 0;
		e_read(&dev,0,0,addr, &message, sizeof(message));
		e_read(&dev,1,0,addr, &message2, sizeof(message2));
		e_read(&emem,0,0,0x00, &shared_label_to_read, sizeof(shared_label_to_read));
		taskMessage = message[6];
		fprintf(stderr, "tick1 %3d||",message[8]+1);
		fprintf(stderr,"task holding core1 %2u||", message[6]);
		fprintf(stderr, "tick2 %3d||",message2[8]+1);
		fprintf(stderr,"task holding core2 %2u||", message2[6]);
		fprintf(stderr,"shared label %2c||", shared_label_to_read[0]);
		fprintf(stderr,"shared label1 %2c||", shared_label_to_read[1]);
		if (shared_label_to_read[0] != chainLatencyStartIndicator){
			chainLatencyStartIndicator = shared_label_to_read[0];
			fprintf(stderr,"<== chain start");
		}
		if (shared_label_to_read[1] != chainLatencyEndIndicator && shared_label_to_read[1]>1){
			chainLatencyEndIndicator = shared_label_to_read[1];
			fprintf(stderr,"<== chain end");
		}
		fprintf(stderr,"\n");
		usleep(READ_PRECISION_US);
		//nsleep(1);
	}
	fprintf(stderr,"----------------------------------------------\n");
	e_close(&dev);
	e_finalize();
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	fprintf(stderr,"RFTP demo complete \n ");
	return 0;
}

