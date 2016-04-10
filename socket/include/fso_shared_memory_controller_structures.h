#ifndef FSO_SHARED_MEMORY_CONTROLLER_STRUCTURES
#define FSO_SHARED_MEMORY_CONTROLLER_STRUCTURES

#include <sys/shm.h>
#include <sys/types.h>

typedef int shmid_t;

typedef struct shm_controller{
  key_t key;
  shmid_t id;
  size_t size;
  char *addr;
  int creation_flags;
  int module_id;
}shm_control;

#endif
