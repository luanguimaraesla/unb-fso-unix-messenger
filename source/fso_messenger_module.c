#include "fso_messenger_module.h"
#include "fso_queue_messenger.h"
#include "fso_messenger_module_signals.h"
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

#endif

messenger_module * messenger_module_create(void){
  messenger_module *new = (messenger_module *) malloc (sizeof(messenger_module));
  new->pid_father = getpid();
  new->already_to_finish = 0;
  return new;
}

//=====================================HEADER=======================================
void init_messenger_header(void){
  char message[MSG_SIZE] = "Trying to connect.";
  fprintf(stderr, "%sSending: \"%s\"\n", COLOR_SEND, message);
  send_message(message, SEND_CHANNEL);
  sleep(1);
  kill(msg_mod->pid_father, SIGNAL_TO_GET_MESSAGE);
  

  // Infinite loop that asks user for some text to send
  while(1){
    printf("%sType: %s", COLOR_SEND, KNRM);
    sleep(1);
    fgets(message, MSG_SIZE, stdin);
    strtok(message, "\n"); // Remove the final "\n"

    // Finish the program if user types 0
    if(message[0] == '0' && message[1] == '\0') break;

    fprintf(stderr, "%sSending: \"%s\"\n", COLOR_SEND, message);
    send_message(message, SEND_CHANNEL);
    kill(msg_mod->pid_father, SIGNAL_TO_GET_MESSAGE);
  }
  kill(msg_mod->pid_father, SIGNAL_TO_FINISH);
}

//================================RECEIVER/TRASMITER================================
void run_messenger_tr(void){
  signal(SIGNAL_TO_GET_MESSAGE, get_message);
  signal(SIGNAL_TO_FINISH, already_to_finish);
  while(!(msg_mod->already_to_finish)){
    sleep(1);
  }
}


//===================================CONTROLLER====================================
void init_messenger_module(void){
  init_signals();

  // Creating the messenger controller struct
  msg_mod = messenger_module_create();
   
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
      run_messenger_tr();
      exit(0);
    }else{
      waitpid(msg_mod->pid_header, &header_status, 0);
      waitpid(msg_mod->pid_tr, &tr_status, 0);
      fprintf(stderr, "%sSuccess: Program finish\n%s", KGRN, KNRM);
      delete_message_queue();
      exit(0);
    }
  }
}
