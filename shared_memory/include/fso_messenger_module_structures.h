#ifndef FSO_MESSENGER_MODULE_STRUCTURES
#define FSO_MESSENGER_MODULE_STRUCTURES

enum fso_module_role {
  header, tr, controller
};

struct fso_messenger_module {
  int ready_to_finish; // signal
  int pid_father;     // father pid
  int pid_header;     // user interface
  int pid_tr;         // transmitter/receiver
  enum fso_module_role role;
};

#endif
