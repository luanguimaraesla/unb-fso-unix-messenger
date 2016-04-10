#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>

#define MSQ_ERR -1

key_t key; // ftok("/tmp", key)  Initialize key with unique key value.
int queue_id;
struct msqid_ds buf;

void create_message_queue(int permission){
    // msgget(key, flags) returns a new queue if there is no queue with key
    // IPC_CREAT create a queue if it no exists
    // When called with both the IPC_CREAT and IPC_EXCL flags, the function fails if the facility already exists.
    // that is, if multiple processes are trying to create a queue with the same facility, only one wins.

    if((queue_id = msgget(ftok("/tmp", key), IPC_CREAT | IPC_EXCL | permission)) == MSQ_ERR){
      printf("Sorry, error creating message queue.\n");
      exit(1);
    }else{
      printf("Success. The message queue has been created\n");
    }
}

int main(){

  create_message_queue(0666);

    // The msgctl() function alters the permissions and other characteristics of a message queue.
    // int msgctl(int msqid, int cmd, struct msqid_ds *buf )
  if(msgctl(queue_id, IPC_RMID, &buf) == MSQ_ERR){ // remove message_queue
    printf("Error deleting message queue.\n");
    exit(1);
  }else{
    printf("Success deleting message queue.\n");
  }
  
  return 0;

}
