#ifndef C2C_H
#define C2C_H

#ifndef C2C_MSG_TYPE
  typedef int c2c_msg_t;
#else
  typedef C2C_MSG_TYPE c2c_msg_t;
#endif



/*

typedef struct{
  c2c_msg_t msg;
  int src;
} c2c_payload;

void sendMsgTo(c2c_msg_t msg, unsigned row, unsigned col, unsigned slot);
void registerForSlot(unsigned slot, QueueHandle_t handler);
void createReceiver();

*/
#define dstr_mem_offset 0x4000

void shared_labels_init_core();

void core_shared_space_inti();

uint8_t shared_label_write_core	(unsigned row,unsigned col,int label_indx,int payload);

unsigned int shared_label_read_core(unsigned row, unsigned col, int label_indx);




#endif
