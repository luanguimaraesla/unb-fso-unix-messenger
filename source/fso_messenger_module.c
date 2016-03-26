#include "fso_messenger_module.h"
#include "fso_queue_messenger.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

messenger_module * messenger_module_create(void){
  messenger_module *new = (messenger_module *) malloc (sizeof(messenger_module));
  return new;
}

void init_messenger_header(void){
  int permission = 0666;
  char message[] = "Trying to connect.";
  create_message_queue(permission);
  send_message(message);
}

void init_messenger_tr(void){
  printf("Received: \"%s\"\n", receive_message());
}

void init_messenger_module(void){
  messenger_module *msg_mod = messenger_module_create();

  if((msg_mod->pid_header = fork()) < 0){
    fprintf(stderr, "Error: module could not create a new header process\n.");
    exit(1);
  }else if(msg_mod->pid_header == 0){ // Header process
    fprintf(stderr, "Success: header process created.\n");
    init_messenger_header();
    exit(0);
  }else{
    if((msg_mod->pid_tr = fork()) < 0){
      fprintf(stderr, "Error: module could not create a new tr process\n.");
      exit(1);
    }else if(msg_mod->pid_tr == 0){ // TR process
      fprintf(stderr, "Success: TR process created.\n");
      init_messenger_tr();
      exit(0);
    }else{
      waitpid(msg_mod->pid_header);
      waitpid(msg_mod->pid_tr);
      fprintf(stderr, "Success: Program finish\n");
      delete_message_queue();
      exit(0);
    }
  }
}

