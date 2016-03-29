#include "fso_messenger_module_signals.h"
#include "fso_messenger_module.h"
#include "fso_queue_messenger.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef COLOR
#define COLOR

#define KNRM  "\x1B[0m"           // NORMAL
#define KRED  "\x1B[31m"          // RED
#define KGRN  "\x1B[32m"          // GREEN
#define COLOR_RECEIVE  "\x1B[33m" // YLW
#define COLOR_SEND  "\x1B[36m"    // CYN

#endif

void init_signals(void){
  // Initialize the signal to comunicate between child processes
  signal(SIGNAL_TO_GET_MESSAGE,
         tell_receiver_child_to_get_message);
  signal(SIGNAL_TO_FINISH,
         tell_receiver_child_to_finish);        
  signal(SIGNAL_TO_KILL_EVERYTHING,
         force_end);
}

void already_to_finish(int signal){
  msg_mod->already_to_finish = 1;
} 

void get_message(int signal){
  last_received_message = receive_message(SEND_CHANNEL);
  printf("%sReceived: \"%s\"\n", COLOR_RECEIVE, last_received_message); 
}

void tell_receiver_child_to_finish(int signal){
  // Send signal ro tell child receiver that the
  // process must be finished
  kill(msg_mod->pid_tr, SIGNAL_TO_FINISH);
}

void tell_receiver_child_to_get_message(int signal){
  // Send signal to tell child receiver that the 
  // message in the queue is already to be read
  kill(msg_mod->pid_tr, SIGNAL_TO_GET_MESSAGE);
}

void force_end(int signal){
  fprintf(stderr, "%sSIGNAL TO KILL EVERYTHING%s\n", KRED, KNRM);
  kill(msg_mod->pid_tr, 9);
  kill(msg_mod->pid_header, 9);
  delete_message_queue();
  exit(0); 
}
