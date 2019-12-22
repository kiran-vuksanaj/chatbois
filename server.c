#include "networking.h"
#include "chat.h"

#include "listen_pipe.h"
void receive_inbound(int client_socket,int merge_fd);
void pass_messages(int merge_fd,int *client_sockets);

int get_blank_idx(int *ary,int n){
  while(--n >= 0 && ary[n]);
  return n;
}

int main() {
  int client_sockets[8];
  int merger_pid = 0;
  int f;
  int listen_socket = server_setup();
  int merge[2];
  pipe(merge);
  while(1){
    int blank_idx = get_blank_idx(client_sockets,8);
    client_sockets[blank_idx] = server_connect(listen_socket);
    f = fork();
    if(!f){
      printf("[subserver %d] initialized to receive\n",getpid());
      close(merge[READ]);
      receive_inbound(client_sockets[blank_idx],merge[WRITE]);
    }
    if(merger_pid) kill(merger_pid,SIGTERM);
    printf("[server] (re)launching merger process\n");
    merger_pid = fork();
    if(!merger_pid) {
      close(merge[WRITE]);
      pass_messages(merge[READ],client_sockets);
    }
  }
  return 0;
}

void receive_inbound(int client_socket,int merge_fd){
  struct packet packet;
  int i;
  while( read(client_socket,&packet,sizeof(struct packet)) ){
    printf("[subserver %d] received a packet\n",getpid());
    // semaphore get
    write(merge_fd,&packet,sizeof(packet));
    // semaphore drop
  }
  exit(0);
}

void pass_messages(int merge_fd,int *client_sockets){
  int i;
  struct packet packet;
  while( read(merge_fd,&packet,sizeof(struct packet)) ){
    for(i=0;i<8;i++){
      if(client_sockets[i]){
	write(client_sockets[i],&packet,sizeof(struct packet));
      }
    }
  }
  exit(0);
}
