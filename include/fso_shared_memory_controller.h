#ifndef FSO_SHARED_MEMORY_CONTROLLER
#define FSO_SHARED_MEMORY_CONTROLLER

#include "fso_shared_memory_controller_structures.h"

#define KEY 1235
#define NUMBER_OF_CHANNELS 2
#define AVAILABLE_TO_READ '*'
#define AVAILABLE_TO_WRITE '-'
#define WAITING_ANOTHER '~'

shm_control *shared_memory;

void create_shm_control(int permission);
char *try_to_transmit_message(char *msg);
char *try_to_receive_message(void);
void create_shared_memory(int permission);
void delete_shared_memory(void);
int init_segment(void);
void write_segment(char *string);
char *read_segment(void);
int is_available_to_read(void);
int is_available_to_write(void);
char * get_shmaddr_to_receive(void);
char * get_shmaddr_to_transmit(void);
void turn_read_on(void);
void turn_write_on(void);
void create_segment(void);
void attach_segment(void);

#endif
