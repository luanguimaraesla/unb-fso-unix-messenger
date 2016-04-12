#include "fso_messenger_module.h"
#include "fso_queue_messenger.h"
#include "fso_socket_controller.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Terminal collors
#ifndef COLOR
#define COLOR

#define KNRM  "\x1B[0m"           // NORMAL
#define KRED  "\x1B[31m"          // RED
#define KGRN  "\x1B[32m"          // GREEN
#define COLOR_RECEIVE  "\x1B[33m" // YLW
#define COLOR_SEND  "\x1B[36m"    // CYN
#define CLEAR_SCREEN "\033[H\033[J"

#endif

messenger_module * messenger_module_create(void){
  messenger_module *new = (messenger_module *) malloc (sizeof(messenger_module));
  new->pid_father = getpid();
  new->ready_to_finish = 0;
  return new;
}

//=====================================HEADER=======================================
void init_messenger_header(void){
  msg_mod->role = header;
  
  signal(SIGNAL_MESSAGE_TO_WRITE, get_message);
  signal(SIGNAL_TO_FINISH, ready_to_finish);
  
  char message[MSG_SIZE];
  printf("%s%sHI! WELCOME TO THE FSO MESSENGER!\n%sTYPE 0 TO EXIT\n", CLEAR_SCREEN, KGRN, KRED); 
  sleep(2);

  // TITLE  
  printf("%s%s\t    FULL DUPLEX MESSENGER\n%s\tmessage queues and shared memory%s\n\n",
         CLEAR_SCREEN, KGRN, KRED, KNRM);

  // Infinite loop that asks user for some text to send
  while(1){
    fgets(message, MSG_SIZE, stdin);
    strtok(message, "\n"); // Remove the final "\n"
    if(message[0] == '0' && message[1] == '\0') break;

    fprintf(stderr, "%sSending: \"%s\"\n", COLOR_SEND, message, KNRM);
    send_message(message, SEND_CHANNEL);
    kill(msg_mod->pid_father, SIGNAL_MESSAGE_TO_TRANSMIT);
  }
  kill(msg_mod->pid_father, SIGNAL_TO_FINISH);
}

//================================RECEIVER/TRASMITER================================

void init_messenger_tr(void){
  msg_mod->role = tr;
  char ip[] = "127.0.0.1";
  int port = 5000;

  signal(SIGNAL_MESSAGE_TO_TRANSMIT, get_message);
  signal(SIGNAL_TO_FINISH, ready_to_finish);
  
  init_socket(ip, port);  

  // Infinite loop that verify if there is a new message
  while(!(msg_mod->ready_to_finish)){
    char *message = try_to_receive_message();
    fprintf(stderr, "%sSending to write: \"%s\"\n", COLOR_SEND, message);
    send_message(message, RECEIVE_CHANNEL);
    kill(msg_mod->pid_father, SIGNAL_MESSAGE_TO_WRITE);
    sleep(3);
  }
}

//===================================CONTROLLER====================================

void init_messenger_module(void){
  init_signals();

  // Creating the messenger controller struct
  msg_mod = messenger_module_create();
  msg_mod->role = controller;
   
  // Creating the message queue
  int permission = 0666;
  create_message_queue(permission);

  // Child status
  int header_status = 0;
  int tr_status = 0;
  if((msg_mod->pid_header = fork()) < 0){
    fprintf(stderr, "%sError: module could not create a new header process\n.", KRED);
    exit(1);
  }else if(msg_mod->pid_header == 0){ // Header process
    fprintf(stderr, "%sSuccess: header process created.\n", KGRN);
    init_messenger_header();
    exit(0);
  }else{
    if((msg_mod->pid_tr = fork()) < 0){
      fprintf(stderr, "%sError: module could not create a new tr process\n.", KRED);
      exit(1);
    }else if(msg_mod->pid_tr == 0){ // TR process
      fprintf(stderr, "%sSuccess: TR process created.\n", KGRN);
      init_messenger_tr();
      exit(0);
    }else{
      waitpid(msg_mod->pid_header, &header_status, 0);
      waitpid(msg_mod->pid_tr, &tr_status, 0);
      fprintf(stderr, "%sSuccess: Program finish\n%s", KGRN, KNRM);
      delete_message_queue();
      close_socket();
      exit(0);
    }
  }
}
