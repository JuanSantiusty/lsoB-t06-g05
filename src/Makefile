##VARIABLES
CC = gcc
CFLAGS = -Wall -g
TARGET_SERVER = unix_server
TARGET_CLIENT = unix_client
OBJS = unixsocketUtils.o
##Directiva principal
all: $(TARGET_SERVER) $(TARGET_CLIENT)
##Actividades
$(TARGET_SERVER): principalServer.o $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET_SERVER) principalServer.o $(OBJS)

$(TARGET_CLIENT): principalClient.o $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET_CLIENT) principalClient.o $(OBJS)

principalServer.o: principalServer.c unixsocket.h
	$(CC) $(CFLAGS) -c principalServer.c

principalClient.o: principalClient.c unixsocket.h
	$(CC) $(CFLAGS) -c principalClient.c

unixsocketUtils.o: unixsocketUtils.c unixsocket.h
	$(CC) $(CFLAGS) -c unixsocketUtils.c
##Directiva phony
clean:
	rm -f $(TARGET_SERVER) $(TARGET_CLIENT) *.o /tmp/unix_socket
