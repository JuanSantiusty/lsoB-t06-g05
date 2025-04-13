#include "unixsocket.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void create_unix_server() {
    int server_fd, client_fd;
    struct sockaddr_un server_addr;

    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Error al crear socket del servidor");
        exit(EXIT_FAILURE);
    }

    unlink(SOCKET_PATH);
    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1) {
        perror("Error en bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 5) == -1) {
        perror("Error en listen");
        exit(EXIT_FAILURE);
    }

    printf("Servidor UNIX esperando conexiones...\n");
    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1) {
        perror("Error en accept");
        exit(EXIT_FAILURE);
    }

    char buffer[BUFFER_SIZE];
    read(client_fd, buffer, BUFFER_SIZE);
    printf("Mensaje recibido: %s\n", buffer);

    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
}

void create_unix_client() {
    int client_fd;
    struct sockaddr_un server_addr;
    
    client_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("Error al crear socket del cliente");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(struct sockaddr_un));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_un)) == -1) {
        perror("Error en connect");
        exit(EXIT_FAILURE);
    }

    char *message = "Hola desde el cliente UNIX!";
    write(client_fd, message, strlen(message) + 1);
    
    close(client_fd);
}
