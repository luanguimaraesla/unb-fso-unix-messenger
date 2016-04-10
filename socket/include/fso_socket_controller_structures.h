#ifndef FSO_SOCKET_CONTROLLER_STRUCTURES
#define FSO_SOCKET_CONTROLLER_STRUCTURES

#include <netinet/in.h>

typedef struct fso_socket{
  int id;
  struct sockaddr_in serv_addr;
  struct sockaddr_in cli_addr; 
}socket_control;

#endif
