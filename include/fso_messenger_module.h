#ifndef FSO_MESSENGER_MODULE
#define FSO_MESSENGER_MODULE

#include "fso_messenger_module_signals.h"
#include "fso_messenger_module_structures.h"

// typedefs
typedef struct fso_messenger_module messenger_module;

// global variables
messenger_module *msg_mod;
char *current_got_message;
char *current_sent_message;

// function's headers
messenger_module * messenger_module_create(void);
void init_messenger_header(void);
void run_messenger_header_catcher(void);
void init_messenger_tr(void);
void run_messenger_tr_catcher(void);
void init_messenger_module(void);

#endif
