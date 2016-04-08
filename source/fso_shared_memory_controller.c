#include "fso_shared_memory_controller.h"
#include "fso_queue_messenger.h"
#include "fso_messenger_module_signals.h"
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>

void create_shm_control(int permission){
  shared_memory = (shm_control *) malloc (sizeof (shm_control));
  shared_memory->key = (key_t) KEY;
  shared_memory->size = (MSG_SIZE * NUMBER_OF_CHANNELS);
  shared_memory->creation_flags = IPC_CREAT | permission; 
}

void create_segment(void){
  if((shared_memory->id = shmget(
                            ftok("/tmp",shared_memory->key),
                            shared_memory->size,
                            shared_memory->creation_flags)) < 0){
      fprintf(stderr, "Error while creating shared memory");
      kill(getpid(), SIGNAL_TO_KILL_EVERYTHING);
  }else{
    fprintf(stderr, "Success: Shared memory was create.\n");
  }
}

void attach_segment(void){
  if((shared_memory->addr = shmat(shared_memory->id, NULL, 0)) == (char *) -1){
    fprintf(stderr, "Error: cannot attach the segment");
    exit(1);
  }else{
    init_segment();
    fprintf(stderr, "Success: shared memory segment attached\n");
  }
}

int init_segment(void){
  if(*(shared_memory->addr) == WAITING_ANOTHER &&
     *(shared_memory->addr + MSG_SIZE) == WAITING_ANOTHER){
    shared_memory->module_id = 1;
    *(shared_memory->addr) = AVAILABLE_TO_WRITE;
    *(shared_memory->addr + MSG_SIZE) = AVAILABLE_TO_WRITE;
    return 1;
  }else{
    shared_memory->module_id = 0;
    *(shared_memory->addr) = WAITING_ANOTHER;
    *(shared_memory->addr + MSG_SIZE) = WAITING_ANOTHER;
    return 0;
  }
}

void turn_write_on(void){
  // Tell other process it is available to read in it's space
  *(get_shmaddr_to_receive()) = AVAILABLE_TO_WRITE;
}

void turn_read_on(void){
  // Tell other process it is available to write in my space
  *(get_shmaddr_to_transmit()) = AVAILABLE_TO_READ;
}

int is_available_to_write(void){
  return *(get_shmaddr_to_transmit()) == AVAILABLE_TO_WRITE;
}

int is_available_to_read(void){
  return *(get_shmaddr_to_receive()) == AVAILABLE_TO_READ; 
}

char * get_shmaddr_to_transmit(void){
  // write in the space of another process
  return shared_memory->addr + (1 - shared_memory->module_id) * MSG_SIZE;
}

char * get_shmaddr_to_receive(void){
  // read in my space
  return shared_memory->addr + shared_memory->module_id * MSG_SIZE;
}

char *read_segment(void){
  if(!is_available_to_read()){
    fprintf(stderr, "Error, the segment isn't available to be read.\n");
    exit(1);
  }

  char *runner = get_shmaddr_to_receive() + 1;
  char *string = (char *) malloc (sizeof(char) * MSG_SIZE);
  char *str_runner = string;
  while(*runner != '\0')
    *(str_runner++) = *(runner++);
  *str_runner = '\0';
  turn_write_on();
  return string;
}

void write_segment(char *string){  
  if(!is_available_to_write()){
    fprintf(stderr, "Error, the segment isn't available to be written.\n");
    exit(1);
  }

  char *runner = get_shmaddr_to_transmit() + 1;  
  while(*string != '\0')
    *(runner++) = *(string++);
  *runner = '\0';
  turn_read_on();
}

void delete_shared_memory(void){
  if(shmctl(shared_memory->id, IPC_RMID, 0) < 0){
    fprintf(stderr, "Error, the segment couldn't be removed.\n");
    exit(1);
  }else{
    fprintf(stderr, "Success: the segment was deleted.\n");
  }
}

void create_shared_memory(int permission){
  create_shm_control(permission);
  create_segment();
  attach_segment();
}

char *try_to_receive_message(void){
  while(!is_available_to_read()) sleep(1);
  return read_segment();
} 

char *try_to_transmit_message(char *msg){
  while(!is_available_to_write()) sleep(1);
  write_segment(msg);
} 
