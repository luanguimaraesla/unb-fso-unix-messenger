#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>

#define MSQ_ERR -1
#define MSG_SIZE 128

// Structs
typedef struct mymsg{
  long mtype;         /* message type */
  char mtext[MSG_SIZE]; /* message text of length MSG_SIZE */
}message_buf;


// Variables
struct msqid_ds buf; // to remove function
message_buf sbuf; // to send
message_buf rbuf; // to receive
size_t buf_length;
key_t key; // ftok("/tmp", key)  Initialize key with unique key value.
int queue_id;

// Function headers
void create_message_queue(int permission);
void delete_message_queue(void);
void send_message(char msg_txt[]);
char *receive_message(void);

// Main function
int main(){
  int permission = 0666;
  char message[] = "Hello World!";

  create_message_queue(permission);
  send_message(message); 
  receive_message();
  delete_message_queue();
  
  return 0;
}

// Message queue functions
void send_message(char msg_txt[]){
  // int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
  sbuf.mtype = 1; // Message type = 1
  strcpy(sbuf.mtext, msg_txt); // Copying message
  buf_length = strlen(sbuf.mtext) + 1 ; // Setting size of message buffer

  if(msgsnd(queue_id, &sbuf, buf_length, IPC_NOWAIT) < 0){
    printf("Error sending message.\n");
  }else{
    printf("Message: \"%s\" sent\n", sbuf.mtext);
  }
}

char *receive_message(void){
  // int msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp,int msgflg);
  
  if(msgrcv(queue_id, &rbuf, MSG_SIZE, 1, 0) < 0){
    printf("Error getting message.\n");
  }else{
    printf("Got message: \"%s\"\n", rbuf.mtext);
    return rbuf.mtext;
  }
  return NULL;
}

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

void delete_message_queue(void){
  // The msgctl() function alters the permissions and other characteristics of a message queue.
  // int msgctl(int msqid, int cmd, struct msqid_ds *buf )
  if(msgctl(queue_id, IPC_RMID, &buf) == MSQ_ERR){ // remove message_queue
    printf("Error deleting message queue.\n");
    exit(1);
  }else{
    printf("Success deleting message queue.\n");
  } 
}
