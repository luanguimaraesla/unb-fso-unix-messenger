#ifndef FSO_MESSENGER_MODULE
#define FSO_MESSENGER_MODULE

#include "fso_messenger_module_structures.h"
#include <signal.h>

// signal defines
#define SIGNAL_TO_GET_MESSAGE 10
#define SIGNAL_TO_FINISH 30
#define SIGNAL_TO_KILL_EVERYTHING 12

// typedefs
typedef struct fso_messenger_module messenger_module;

// global variables
messenger_module *msg_mod;
char *last_received_message;

// signal's functions
void already_to_finish(int signal);
void tell_receiver_child_to_get_message(int signal);
void get_message(int signal);
void tell_receiver_child_to_finish(int signal);
void force_end(int signal);

// function's headers
messenger_module * messenger_module_create(void);
void init_messenger_header(void);
void run_messenger_tr(void);
void init_messenger_module(void);

#endif
