#include <stdio.h>
#include <stdlib.h>
#include "sema.h"
#include <sys/ipc.h>
#define KEY 24601

int main(){
  int semid = sem_config(KEY,IPC_CREAT|IPC_EXCL|0644,4, 1,4,2,3 );
  printf("successful config: %d\n",semid);
}
