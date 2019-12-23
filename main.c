#include <stdio.h>
#include <stdlib.h>
#include "sema.h"
#include <sys/ipc.h>
#define KEY 2323

int main(){
  int semid = sem_config(KEY,IPC_CREAT|IPC_EXCL,4, 1,4,2,3 );
  printf("successful config: %d",semid);
}
