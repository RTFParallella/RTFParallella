#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "c2c.h"
#include <e-lib.h>

extern int M[5];



struct msgBox_t {
    e_mutex_t read ;
    e_mutex_t write;
    char slot;
    c2c_payload l;
} msgBox SECTION("section_mailbox");

QueueHandle_t slots[8] = {0};
unsigned row, col;


//sets row and column
void setMutexes() {
    e_coreid_t thiscore = e_get_coreid();
    e_coords_from_coreid(thiscore, &row, &col);
    //e_mutex_init(row, col, &msgBox.write, NULL);
    //e_mutex_init(row, col, &msgBox.read, NULL);
    msgBox.read= MUTEX_NULL;
    msgBox.write = MUTEX_NULL;
    e_mutex_unlock(row, col, &msgBox.read);
    e_mutex_unlock(row, col, &msgBox.write);
    //reading is now locked, writing open
}

void irqMsg(){
    if(e_mutex_trylock(row, col, &msgBox.read)==0) { // We have got mail!
        if(slots[msgBox.slot]){  //Check if some queue is registered on this slot
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xQueueSendFromISR( slots[msgBox.slot], ( void * ) &msgBox.l, &xHigherPriorityTaskWoken );
            if (xHigherPriorityTaskWoken) {
                // Should check last parameter and yield from ISR (which is not implemented)
            }
        }
        e_mutex_unlock(row, col, &msgBox.write);
    }
}

void setupInterrupt() {
    setMutexes();
}

void sendMsgTo(c2c_msg_t msg, unsigned row, unsigned col, unsigned slot)
{
    struct msgBox_t *dst;
    dst = (struct msgBox_t *) e_get_global_address(row, col, &msgBox);
    //M[3] = &dst;
    int giveup = 0;
    
    taskENTER_CRITICAL();
    //loop till we got the lock
//    while (e_mutex_trylock(row, col, &msgBox.write)!=0) {  //No other core wants to write to this buffer
//        taskEXIT_CRITICAL();  // Make space for beeing preempted
//        giveup++;
//        if(giveup > 10)
//            return;
//        taskYIELD();
//        taskENTER_CRITICAL(); // Block preemption again in case we get the lock
//    }
    
    
    
//    int slotsLeft = getSlotsLeft(dst);
//    if (slotsLeft!=0){
//      sendToAddress(message.data,*dst+(slotsLeft*sizeof(message));
//    
//    }else {
//      while(slotsLeft==0){
//        slotsLeft = getSlotsLeft(dst);
//      }
//      sendToAddress(message.data,*dst+(slotsLeft*sizeof(message));
//    }
    
    
    
    dst->l.msg = msg;
    dst->l.src = e_get_coreid();
    dst->slot=slot;
    
    //e_mutex_unlock(row, col, &msgBox.read);
    // No, we really do not unlock &msgBox.write, the receiver will do that when there is also new space. 'flag' shows that we are done
    
    //Trigger Timer0-Interrupt on that core
    //e_irq_set(row,col,E_MESSAGE_INT);
    taskEXIT_CRITICAL();
} 

void registerForSlot(unsigned slot, QueueHandle_t handler) {
    slots[slot] = handler;
}


void createReceiver() {
    setupInterrupt();
}



unsigned int *outbuf_dstr_shared[10];

int core_write_mutex=0;

void shared_labels_init_core(){
	//shared buffer in core memory
	outbuf_dstr_shared[0] = (unsigned int *) dstr_mem_offset;
	outbuf_dstr_shared[1] = outbuf_dstr_shared[0] + 1;
	outbuf_dstr_shared[2] = outbuf_dstr_shared[1] + 1;
	outbuf_dstr_shared[3] = outbuf_dstr_shared[2] + 1;
	outbuf_dstr_shared[4] = outbuf_dstr_shared[3] + 1;
	outbuf_dstr_shared[5] = outbuf_dstr_shared[4] + 1;
	outbuf_dstr_shared[6] = outbuf_dstr_shared[5] + 1;
	outbuf_dstr_shared[7] = outbuf_dstr_shared[6] + 1;
	outbuf_dstr_shared[8] = outbuf_dstr_shared[7] + 1;
	//initialize buffer
	int i;
	//timer1init();
	for (i=0;i<9;i++){
		*outbuf_dstr_shared[i] =0;
	}
}

void core_shared_space_inti(){


}

uint8_t shared_label_write_core	(unsigned row,unsigned col,int label_indx,int payload){
	uint8_t retval=NULL;
	//*outbuf_dstr_shared[label_indx] = payload;

}

unsigned int shared_label_read_core (unsigned row, unsigned col, int label_indx){

}




