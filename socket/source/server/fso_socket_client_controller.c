#include "fso_socket_controller.h"
#include "fso_queue_messenger.h"
#include "fso_messenger_module_signals.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_socket_control(void){
  // 0 - IP - INTERNET PROTOCOL
  // 1 - ICMP - INTERNET CONTROL MESSAGE PROTOCOL
  // 2 - IGMP - INTERNET GROUP MULTICAST PROTOCOL
  // 3 - GGP - GATEWAY-GATEWAY PROTOCOL
  // 6 - TCP - TRANSMISSION CONTROL PROTOCOL
  // 17 - UDP - USER DATAGRAMA PROTOCOL

  sock.protocol = 0;
  sock.type = SOCK_STREAM;

  // AF_INET      (ARPA INTERNET PROTOCOLS)
  // AF_UNIX      (UNIX INTERNET PROTOCOLS)
  // AF_ISO       (ISO PROTOCOLS)
  // AF_NS        (XEROX NETWORK SYSTEM PROTOCOLS)

  sock.sin_family = AF_INET;
  sock.cli_queue_length = CLIENTS_QUEUE_LENGTH;
  sock.rec_message[0] = WAITING;
}

void create_socket(void){
  if((sock.id = socket(sock.sin_family, sock.type, sock.protocol)) < 0){
      fprintf(stderr, "Error while creating socket\n");
      kill(getpid(), SIGNAL_TO_KILL_EVERYTHING);
  }else{
    fprintf(stderr, "Success: Socket was create.\n");
  }
}

void bind_socket(char ip_address[], int port){
  // Clean the serv_addr variable
  memset((char *)&(sock.serv_addr),0,sizeof(sock.serv_addr));
  sock.serv_addr.sin_family       = sock.sin_family;
  sock.serv_addr.sin_addr.s_addr  = inet_addr(ip_address);
  sock.serv_addr.sin_port         = htons(port);

  if (bind(sock.id, (struct sockaddr *)&(sock.serv_addr), sizeof(sock.serv_addr)) < 0) {
    fprintf(stderr, "Error binding socket.\n");
    kill(getpid(), SIGNAL_TO_KILL_EVERYTHING);
  }else{
    fprintf(stderr, "Success: binding socket.\n");
  }
}

void listen_port(void){
  if(listen(sock.id, 5) < 0){
    fprintf(stderr, "Error listening port.\n");
    kill(getpid(), SIGNAL_TO_KILL_EVERYTHING);
  }else{
    fprintf(stderr, "Success: listening port.\n");
  }
}

void listen_new_client(void){
  int addr_length = sizeof(sock.cli_addr);
  int pid;
  int cli_counter = 0;
  int new_connection;

  turn_write_on();
  if((sock.cli_id = accept(sock.id, (struct sockaddr *) &(sock.cli_addr), &addr_length)) < 0){
    fprintf(stderr, "Error listening client.\n");
    kill(getpid(), SIGNAL_TO_KILL_EVERYTHING);
  }else{
    fprintf(stderr, "Success: listening client.\n");
    new_connection = sock.cli_id;
  }

  if((pthread_create(&client_thread,
                     NULL, listen_client,
                     (void *) new_connection)) < 0){
    fprintf(stderr, "Error creating thread in fuction listen_client.\n");
    kill(getpid(), SIGNAL_TO_KILL_EVERYTHING);
  }else{
    fprintf(stderr, "Success: listening thread created.\n");
  }
  //pthread_join(client_thread);
}

void * listen_client(void * connection){
  char bufin[MSG_SIZE];

  fprintf(stderr, "Success: client connected.\n");
  while(1){
    memset(bufin, 0x0, sizeof(bufin));
    fprintf(stderr, "Success: thread waiting to write on sock.rec_message.\n");
    while(!is_available_to_write()){
      sleep(1);
    };
    fprintf(stderr, "Success: thread enabled to write on sock.rec_message.\n");
    if(recvfrom((int) connection, bufin, sizeof(bufin), 0, NULL, NULL) < 0){
      fprintf(stderr, "Error: thread could not receive any data.\n");
      kill(getpid(), SIGNAL_TO_KILL_EVERYTHING);
    }else{
      fprintf(stderr, "Success: thread received \"%s\".\n", bufin);
    }
    strtok(bufin, "\n");
    if(bufin[0] == '\0') break;
    write_received_message(bufin);
  }

  close_socket();
  fprintf(stderr, "Success: client disconnected.\n");
  kill(msg_mod->pid_father, SIGNAL_TO_FINISH);
  pthread_exit(0);
}

void close_socket(void){
  close(sock.cli_id);
  close(sock.id);
}

void init_socket(char ip_address[], int port){
  create_socket_control();
  create_socket();
  bind_socket(ip_address, port);
  listen_port();
  listen_new_client();
}

char *read_message(void){
  if(!is_available_to_read()){
    fprintf(stderr, "Error: message is not available to read.\n");
    exit(1);
  }else{
    fprintf(stderr, "Success: copying and returning sock.rec_message.\n");
  }
  char *string = (char *) malloc (sizeof(char) * MSG_SIZE);
  char *runner = sock.rec_message + 1;
  char *string_runner = string;

  while(*runner != '\0')
    *(string_runner++) = *(runner++);
  *(string_runner) = '\0';

  fprintf(stderr, "Success: \"%s\" copied.\n", string);
  turn_write_on();
  return string;
}

void write_received_message(char *msg){
  fprintf(stderr, "Coping connection buffer to sock.rec_message.\n");
  strcpy(sock.rec_message + 1, msg);
  fprintf(stderr, "Copied as: \"%s\"\n", sock.rec_message);
  turn_read_on();
  fprintf(stderr, "Readable as: \"%s\"\n", sock.rec_message);
}

char *try_to_receive_message(void){
  fprintf(stderr, "Waiting sock.rec_message be readable.\n");
  while(!is_available_to_read() && !(msg_mod->ready_to_finish)) sleep(1);
  if(msg_mod->ready_to_finish)
    exit(0);
  else
    return read_message();
} 

char *try_to_transmit_message(char *msg){
  int len = sizeof(sock.cli_addr);
  if(sendto(sock.cli_id, msg, MSG_SIZE, 0, (struct sockaddr *) &(sock.cli_addr), len) < 0){
    fprintf(stderr, "Error transmitting message\n");
  }else{
    fprintf(stderr, "Success transmitting message: \"%s\"\n", msg);
  }
}
 
void turn_write_on(void){
  fprintf(stderr, "The sock.rec_message string is writable.\n");
  sock.rec_message[0]  = AVAILABLE_TO_WRITE;
}

void turn_read_on(void){
  fprintf(stderr, "The sock.rec_message string is readable.\n");
  sock.rec_message[0] = AVAILABLE_TO_READ;
}

int is_available_to_write(void){
  return sock.rec_message[0] == AVAILABLE_TO_WRITE;
}

int is_available_to_read(void){
  return sock.rec_message[0] == AVAILABLE_TO_READ; 
}

int is_waiting(void){
  return sock.rec_message[0] == WAITING;
}
