#ifndef FSO_MESSENGER_MODULE
#define FSO_MESSENGER_MODULE

#include "fso_messenger_module_structures.h"

typedef struct fso_messenger_module messenger_module;

messenger_module * messenger_module_create(void);
void init_messenger_header(void);
void init_messenger_tr(void);
void init_messenger_module(void);

#endif
