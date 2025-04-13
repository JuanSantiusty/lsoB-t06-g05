#ifndef UNIX_SOCKET_H
#define UNIX_SOCKET_H

#define BUFFER_SIZE 256
#define MAX_LENGTH 40

void create_unix_server(char SOCKET_PATH[]);
void create_unix_client(char SOCKET_PATH[]);

#endif
