#ifndef FSO_SOCKET_CLIENT_CONTROLLER
#define FSO_SOCKET_CLIENT_CONTROLLER

#include "fso_socket_controller_structures.h"

// Socket headers
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define CLIENTS_QUEUE_LENGTH 5
#define AVAILABLE_TO_READ '*'
#define AVAILABLE_TO_WRITE '-'
#define WAITING '~'


// Global function
socket_control sock;
pthread_t server_thread;

void create_socket_control(void);
void create_socket(void);
void connect_to_socket(char ip_address[], int port);
void open_conversation(void);
void * init_conversation(void * connection);
void close_socket(void);
void init_socket(char ip_address[], int port);
char *read_message(void);
void write_received_message(char *msg);
char *try_to_receive_message(void);
char *try_to_transmit_message(char *msg);
void turn_write_on(void);
void turn_read_on(void);
int is_available_to_write(void);
int is_available_to_read(void);
int is_waiting(void);

#endif
