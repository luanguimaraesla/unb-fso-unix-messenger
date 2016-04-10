#ifndef FSO_SOCKET_CONTROLLER
#define FSO_SOCKET_CONTROLLER

#include "fso_socket_controller_structures.h"

// Socket headers
#include <sys/types.h>
#include <sys/sockets.h>
#include <netdb.h>

#define CLIENTS_QUEUE_LENGTH 5

// Global function
socket_control *sock;


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
int is_waiting(void);
char * get_shmaddr_to_receive(void);
char * get_shmaddr_to_transmit(void);
void turn_read_on(void);
void turn_write_on(void);
void create_segment(void);
void attach_segment(void);

#endif
