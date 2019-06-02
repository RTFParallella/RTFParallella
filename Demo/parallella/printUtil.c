/*
 * printUtil.c
 *
 *  Created on: May 31, 2019
 *      Author: bigm
 */
#include "printUtil.h"
//communication setup
const char		  ShmName[] = "hello_shm";
const char        Msg[] = "Hello World from core 0x%03x!";
char              buf[256] = { 0 };
e_coreid_t		  coreid;
e_memseg_t   	  emem;
unsigned          my_row;
unsigned          my_col;

uint initializePrintChannel(){
	coreid = e_get_coreid();
	e_coords_from_coreid(coreid, &my_row, &my_col);

	if ( E_OK != e_shm_attach(&emem, ShmName) ) {
		return 1;
	}else {
		return 0;
	}
}

uint printToHost(char printBuf[]){
	if ( emem.size >= strlen(buf) + 1 ) {
		// Write the message (including the null terminating
		// character) to shared memory
		//flush with null message first
		/*char nullBuf[strlen(printBuf)+1];
		uint var;
		for (var = 0; var < strlen(nullBuf); ++var) {
			nullBuf[var]=0;
		}
		e_write((void*)&emem, nullBuf, my_row, my_col, NULL, strlen(nullBuf) + 1);*/
		e_write((void*)&emem, printBuf, my_row, my_col, NULL, strlen(printBuf) + 1);
	} else {
		// Shared memory region is too small for the message
		return 1;
	}
}

