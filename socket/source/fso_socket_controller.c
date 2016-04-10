#include "fso_socket_controller.h"
#include "fso_queue_messenger.h"
#include "fso_messenger_module_signals.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

void create_socket_control(void){
  sock = (socket_control *) malloc (sizeof (socket_control));
  
  // 0 - IP - INTERNET PROTOCOL
  // 1 - ICMP - INTERNET CONTROL MESSAGE PROTOCOL
  // 2 - IGMP - INTERNET GROUP MULTICAST PROTOCOL
  // 3 - GGP - GATEWAY-GATEWAY PROTOCOL
  // 6 - TCP - TRANSMISSION CONTROL PROTOCOL
  // 17 - UDP - USER DATAGRAMA PROTOCOL

  sock->protocol = 0;
  sock->type = SOCK_STREAM;

  // AF_INET      (ARPA INTERNET PROTOCOLS)
  // AF_UNIX      (UNIX INTERNET PROTOCOLS)
  // AF_ISO       (ISO PROTOCOLS)
  // AF_NS        (XEROX NETWORK SYSTEM PROTOCOLS)

  sock->sin_family = AF_INET;
  sock->cli_queue_length = CLIENTS_QUEUE_LENGTH;
}

void create_socket(void){
  if((sock->id = socket(sock->sin_family, sock->type, sock->protocol)) < 0){
      fprintf(stderr, "Error while creating socket\n");
      kill(getpid(), SIGNAL_TO_KILL_EVERYTHING);
  }else{
    fprintf(stderr, "Success: Socket was create.\n");
  }
}

void bind_socket(char ip_address[], int port){
  // Clean the serv_addr variable
  memset((char *)&sock->serv_addr,0,sizeof(sock->serv_addr));
  sock->serv_addr.sin_family       = sock->sin_family;
  sock->serv_addr.sin_addr.s_addr  = inet_addr(ip_address);
  sock->serv_addr.sin_port         = htons(port);

  if (bind(sock->id, (struct sockaddr *)&(sock->serv_addr), sizeof(sock->serv_addr)) < 0) {
    fprintf(stderr, "Error binding socket.\n");
    kill(getpid(), SIGNAL_TO_KILL_EVERYTHING);
  }else{
    fprintf(stderr, "Success: binding socket.\n");
  }
}

void listen_port(void){
  if(listen(sock->id, sock->cli_queue_length) < 0){
    fprintf(stderr, "Error listening port.\n");
    kill(getpid(), SIGNAL_TO_KILL_EVERYTHING);
  }else{
    fprintf(stderr, "Success: binding socket.\n");
  }
}

void listen_new_clients(void){
  int addr_length = sizeof(sock->cli_addr);
  int new_connection;
  int pid;

  if((new_connection = accept(sock->id, (struct sockaddr *) &(sock->cli_addr), &addr_length)) < 0){
    fprintf(stderr, "Error listening client.\n");
    kill(getpid(), SIGNAL_TO_KILL_EVERYTHING);
  }else{
    fprintf(stderr, "Success: listening client.\n");
  }

  while(1){
    if((pid = fork()) < 0){
      fprintf(stderr, "Error forking in fuction listen_client.\n");
      kill(getpid(), SIGNAL_TO_KILL_EVERYTHING);
    }else{
      fprintf(stderr, "Success: listening process forked.\n");
      // Child
      if(!pid){
        fprintf(stderr, "Success: client connected.\n");
        listen_client(new_connection, sock->cli_addr);
        fprintf(stderr, "Success: client disconnected.\n");
        exit(0);
      }else{
        fprintf(stderr, "Waiting connections.\n");
      }
    }
  }
}

void listen_client(int connection, struct sockaddr_in cli_addr){
  char bufin[MSG_SIZE];
  while(1){
    memset(&bufin, 0x0, sizeof(bufin));
    recv(connection, &bufin, sizeof(bufin), 0);
    if (strncmp(bufin, "FIM", 3) == 0)
      break;
  }
  close(connection);
}

void init_socket(char ip_address[], int port){
  create_socket_control();
  create_socket(ip_address, port);
  bind_socket();
  listen_port();
  listen_new_clients();
}

char *try_to_receive_message(void){
  while(!(msg_mod->ready_to_finish)) sleep(1);
  return read_segment();
} 

char *try_to_transmit_message(char *msg){
  while((is_waiting() || !is_available_to_write()) && !(msg_mod->ready_to_finish)) sleep(1);
  write_segment(msg);
} 
