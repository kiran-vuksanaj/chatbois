#include "networking.h"
#include "chat.h"

void receive_messages(int socketfd,int pipefd);
void send_input(struct packet template,int socketfd);

int main(int argc, char *argv[]){
  int f,server_socket;
  if(argc < 2){
    printf("usage: ./chat <username> [<ip>]\n");
    return EINVAL;
  }
  char username[16];
  strncpy(username,argv[1],16);
  char *IPv4 = argc > 2 ? argv[2] : TEST_IP ;
  server_socket = client_setup(IPv4);
  f = fork();
  int msgpipe[2];
  pipe(msgpipe);
  struct packet template;
  strncpy(template.username,username,16);
  f = fork();
  if(!f) send_input(template,server_socket);
  struct packet buf;
  printf("\e[s[ready to get messages]\n");
  while(1){
    read(server_socket,&buf,sizeof(struct packet));
    printf("\e[u\e[K[%s]: %s\n...$ ",buf.username,buf.message);
  }
}


void send_input(struct packet template,int socketfd){
  char buf[256];
  while(1){
    fgets(buf,256,stdin);
    *(strchr(buf,'\n')) = '\0';
    strncpy(template.message,buf,256);
    write(socketfd,&template,sizeof(struct packet));
  }
  exit(0);
}
