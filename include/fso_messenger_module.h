#ifndef FSO_MESSENGER_MODULE
#define FSO_MESSENGER_MODULE

#include "fso_messenger_module_structures.h"

// typedefs
typedef struct fso_messenger_module messenger_module;

// global variables
messenger_module *msg_mod;
char *last_received_message;

// function's headers
messenger_module * messenger_module_create(void);
void init_messenger_header(void);
void run_messenger_tr(void);
void init_messenger_module(void);

#endif
