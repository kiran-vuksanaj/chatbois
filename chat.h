#ifndef CHAT_H
#define CHAT_H

struct packet {
  char message[256];
  char username[16];
};

#endif
