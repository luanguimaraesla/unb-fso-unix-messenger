#ifndef FSO_SHARED_MEMORY_CONTROLLER
#define FSO_SHARED_MEMORY_CONTROLLER

#include "fso_shared_memory_controller_structures.h"

#define KEY 122
#define NUMBER_OF_CHANNELS 2
#define AVAILABLE_TO_READ '*'
#define AVAILABLE_TO_WRITE '-'

shm_control *shared_memory;

void create_shm_control(void);
char *try_to_transmit_message(char *msg);
char *try_to_receive_message(void);
void create_shared_memory(void);
void delete_segment(void);
void write_segment(char *string);
char *read_segment(void);
int is_available_to_read(void);
int is_available_to_write(void);
void turn_read_on(void);
void turn_write_on(void);
void create_segment(void);
void attach_segment(void);

#endif
