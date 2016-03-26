#ifndef FSO_QUEUE_MESSENGER
#define FSO_QUEUE_MESSENGER

#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include "fso_queue_messenger_structures.h" /* including struct fso_msq */

typedef struct fso_msq message_buf;

// Global variables
struct msqid_ds buf;      // to handle remove function
message_buf send_buf;     // to send messages
message_buf receive_buf;  // to receive messages
size_t buf_length;        // stores the size of the string buffer
key_t key;                // keep on mind ftok("/tmp", key) initializes key with unique key value.
int msq_id;               // queue id

// Function's headers
void create_message_queue(int permission);
void delete_message_queue(void);
void send_message(char msg_txt[]);
char *receive_message(void);

#endif
