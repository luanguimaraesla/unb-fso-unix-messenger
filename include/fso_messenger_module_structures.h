#ifndef FSO_MESSENGER_MODULE_STRUCTURES
#define FSO_MESSENGER_MODULE_STRUCTURES

struct fso_messenger_module {
  int already_to_finish; // signal
  int pid_father;     // father pid
  int pid_header;     // user interface
  int pid_tr;         // transmitter/receiver
};

#endif
