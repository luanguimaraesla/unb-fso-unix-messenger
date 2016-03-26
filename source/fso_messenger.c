#include "fso_queue_messenger.h"

int main(void){
  int permission = 0666;
  char message[] = "Hello World!";

  create_message_queue(permission);
  send_message(message); 
  receive_message();
  delete_message_queue();
  
  return 0;
}
