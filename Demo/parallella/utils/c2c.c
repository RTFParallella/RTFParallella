#include "utils/c2c.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
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