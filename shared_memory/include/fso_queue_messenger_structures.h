#ifndef FSO_QUEUE_MESSENGER_STRUCTURES
#define FSO_QUEUE_MESSENGER_STRUCTURES

#define MSG_SIZE 128

struct fso_msq{
  long mtype;         /* message type */
  char mtext[MSG_SIZE]; /* message text of length MSG_SIZE */
};

#endif
