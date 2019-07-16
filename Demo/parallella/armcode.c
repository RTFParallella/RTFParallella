#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <e-hal.h> //hardware abstraction library
#include <time.h>   /* Needed for struct timespec */
#include "debugFlags.h"
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
	unsigned int shared_label_to_read;
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
	fprintf(stderr,"FreeRTOS started \n");
	//fprintf(stderr,"Tick ||Core1 ||Core2 \n");
	addr = cnt_address;
	int pollLoopCounter = 0;
	int taskMessage;
	int prevtaskMessage;
	int prevpollLoopCounter = 0;
	for (pollLoopCounter=0;pollLoopCounter<=40;pollLoopCounter++){
		message[3] = 0;
		e_read(&dev,0,0,addr, &message, sizeof(message));
		e_read(&dev,1,0,addr, &message2, sizeof(message2));
		//e_read(&emem, 0, 0, (off_t) (0x0000), (void *) &(shared_label_to_read), sizeof(shared_label_to_read));
		taskMessage = message[6];
		/*if (message[8]!=message2[8]){
			fprintf(stderr,"cores out of sync\n");
		}
		fprintf(stderr,"%5d||%5d||%5d\n",message[8],message[6],message2[6]);
		*///skip printing if a a tick is missed
		/*if (message[8]> pollLoopCounter){
			fprintf(stderr,"missed tick %3u||\n", message[8]);
			continue;
		}*/
		//fprintf(stderr,"task 1 %3u||", message[2]);
		//fprintf(stderr,"task 2 %3u||", message[0]);
		//fprintf(stderr,"task 3 %3u||", message[4]);
		fprintf(stderr, "tick1 %3d||",message[8]+1);
		fprintf(stderr,"task holding core1 %2u||", message[6]);
		//fprintf(stderr,"debug flag %4u||", message[7]);
		fprintf(stderr, "tick2 %3d||",message2[8]+1);
		fprintf(stderr,"task holding core2 %2u||", message2[6]);
		fprintf(stderr,"\n");
		usleep(READ_PRECISION_US);
		//nsleep(1);
	}

	fprintf(stderr,"----------------------------------------------\n");
	e_close(&dev);
	e_finalize();
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	fprintf(stderr,"demo complete \n ");
	return 0;
}
