#ifndef FSO_MESSENGER_MODULE_SIGNALS
#define FSO_MESSENGER_MODULE_SIGNALS

#include <signal.h>

// signal defines
#define SIGNAL_TO_GET_MESSAGE 10
#define SIGNAL_TO_FINISH 30
#define SIGNAL_TO_KILL_EVERYTHING 12

// signal's functions
void init_signals(void);
void already_to_finish(int signal);
void tell_receiver_child_to_get_message(int signal);
void get_message(int signal);
void tell_receiver_child_to_finish(int signal);
void force_end(int signal);

#endif
