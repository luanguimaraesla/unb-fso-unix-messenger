#ifndef FSO_MESSENGER_MODULE_SIGNALS
#define FSO_MESSENGER_MODULE_SIGNALS

#include "fso_messenger_module.h"
#include "fso_socket_controller.h"
#include "fso_queue_messenger.h"
#include <signal.h>

// signal defines
#define SIGNAL_MESSAGE_TO_TRANSMIT 10
#define SIGNAL_MESSAGE_TO_WRITE 16
#define SIGNAL_TO_FINISH 30
#define SIGNAL_TO_KILL_EVERYTHING 12

// signal's functions
void init_signals(void);
void ready_to_finish(int signal);
void tell_receiver_child_to_get_message(int signal);
void tell_header_child_to_write_message(int signal);
void get_message(int signal);
void tell_receiver_child_to_finish(int signal);
void force_end(int signal);

#endif
