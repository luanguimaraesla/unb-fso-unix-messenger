#include "fso_messenger_module_signals.h"
#include "fso_shared_memory_controller.h"
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
  signal(SIGNAL_MESSAGE_TO_TRANSMIT,
         tell_receiver_child_to_get_message);
  signal(SIGNAL_MESSAGE_TO_WRITE,
         tell_header_child_to_write_message);
  signal(SIGNAL_TO_FINISH,
         tell_receiver_child_to_finish);        
  signal(SIGNAL_TO_KILL_EVERYTHING,
         force_end);
}

void ready_to_finish(int signal){
  msg_mod->ready_to_finish = 1;
} 

void get_message(int signal){
  if(msg_mod->role == tr){ // transmit if role is trasmitter
    current_sent_message = receive_message(SEND_CHANNEL);
    fprintf(stderr, "%sTransmitting: \"%s\"\n", COLOR_RECEIVE, current_sent_message);
    try_to_transmit_message(current_sent_message);
  }else if(msg_mod->role == header){ // print if role is header
    current_got_message = receive_message(RECEIVE_CHANNEL);
    fprintf(stderr, "%sReceived: \"%s\"\n", COLOR_RECEIVE, current_got_message);
    printf("%30s%s%s\n",COLOR_RECEIVE, current_got_message, KNRM);
  }else{
    fprintf(stderr, "%sError: wrong role to get message.\n%s", KRED, KNRM);
  }
}

void tell_receiver_child_to_finish(int signal){
  // Send signal ro tell child receiver that the
  // process must be finished
  kill(msg_mod->pid_tr, SIGNAL_TO_FINISH);
}

void tell_receiver_child_to_get_message(int signal){
  // Send signal to tell child receiver that the 
  // message in the queue is ready to be read
  kill(msg_mod->pid_tr, SIGNAL_MESSAGE_TO_TRANSMIT);
}

void tell_header_child_to_write_message(int signal){
  //Send signal to tell the header that the
  // message in the queue is ready to be written
  kill(msg_mod->pid_header, SIGNAL_MESSAGE_TO_WRITE);
}

void force_end(int signal){
  fprintf(stderr, "%sSIGNAL TO KILL EVERYTHING%s\n", KRED, KNRM);
  kill(msg_mod->pid_tr, 9);
  kill(msg_mod->pid_header, 9);
  delete_message_queue();
  delete_shared_memory();
  exit(0); 
}
