/*
 * printUtil.h
 *
 *  Created on: May 31, 2019
 *      Author: bigm
 */

#ifndef DEMO_PARALLELLA_PRINTUTIL_H_
#define DEMO_PARALLELLA_PRINTUTIL_H_

#include "e-lib.h"
#include <string.h>

#define SHARED_MEM_SECTION_NAME "hello_shm"

uint initializePrintChannel();
uint printToHost(char printBuf[]);


#endif /* DEMO_PARALLELLA_PRINTUTIL_H_ */
