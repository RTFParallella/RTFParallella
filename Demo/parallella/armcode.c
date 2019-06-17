#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <e-hal.h> //hardware abstraction library
int main()
{
	//counters for row and colum, cored id and loop counter
	unsigned   row_loop,col_loop;
	// this will contain the epiphany platform configuration
	e_platform_t epiphany;
	e_epiphany_t dev;
	e_return_stat_t result;
	unsigned int message[9];
	long  message2;
	int loop;
	int addr;
	e_init(NULL); // initialise the system establish connection to the Device
	e_reset_system(); // reset the epiphnay chip
	e_get_platform_info(&epiphany);//gets the configuration info for the parallella platofrm
	//debug flag
	//------------------------------------------------
	//one core within the parallella work group is 1 x 1 i.e single core
	e_open(&dev,0,0,1,1);
	//reset the group
	e_reset_group(&dev);
	//load the group
	result =  e_load("main.elf",&dev,0,0,E_FALSE);
	if (result != E_OK){
		fprintf(stderr,"Error Loading the Epiphany Application 1 %i\n", result);
	}
	e_start_group(&dev);
	fprintf(stderr,"FreeRTOS started \n");
	addr = 0x3000;
	int pollLoopCounter = 0;
	int taskMessage;
	int prevtaskMessage;
	int prevpollLoopCounter = 0;
	for (pollLoopCounter=0;pollLoopCounter<=30;pollLoopCounter++){
		message[3] = 0;
		e_read(&dev,0,0,addr, &message, sizeof(message));
		taskMessage = message[6];
		//only print new status if a different task has been selected
		//if (/*(taskMessage == 1 || taskMessage == 2|| taskMessage == 3) &&*/ prevtaskMessage!=taskMessage){
		prevtaskMessage = taskMessage;
		fprintf(stderr,"task 1 %3u||", message[2]);
		fprintf(stderr,"task 2 %3u||", message[0]);
		fprintf(stderr,"task 3 %3u||", message[4]);
		fprintf(stderr,"task holding core %2u||", message[6]);
		fprintf(stderr,"debug flag %4u||", message[7]);
		fprintf(stderr,"iteration prv task %2d||", ((  pollLoopCounter)-(    prevpollLoopCounter)));
		fprintf(stderr, "tick %3d",message[8]+1);
		prevpollLoopCounter = pollLoopCounter;
		fprintf(stderr,"\n");
		usleep(1000);
		//	}
	}
	fprintf(stderr,"----------------------------------------------\n");
	e_close(&dev);
	e_finalize();
	//----------------------------------------------------------------------------
	//----------------------------------------------------------------------------
	fprintf(stderr,"demo complete \n ");
	return 0;

}
