#ifndef FSO_SOCKET_CONTROLLER_STRUCTURES
#define FSO_SOCKET_CONTROLLER_STRUCTURES

#include "fso_queue_messenger.h"
#include <netinet/in.h>

typedef struct fso_socket{
  int id;
  int cli_id;
  int cli_queue_length;
  struct sockaddr_in serv_addr;
  struct sockaddr_in cli_addr;
  char rec_message[MSG_SIZE];
  int protocol;
  int type;
  int sin_family;   
}socket_control;

#endif
