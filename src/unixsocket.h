#ifndef UNIX_SOCKET_H
#define UNIX_SOCKET_H

#define SOCKET_PATH "/tmp/unix_socket"
#define BUFFER_SIZE 256

void create_unix_server();
void create_unix_client();

#endif
