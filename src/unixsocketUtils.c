#include "unixsocket.h"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>

time_t hora_inicio;
volatile sig_atomic_t keep_running = 1;

void create_unix_server(char SOCKET_PATH []) {
    int server_fd, client_fd;
    struct sockaddr_un server_addr;
    signal(SIGTERM, manejador_sigterm);
    hora_inicio = time(NULL);

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
    do{
    read(client_fd, buffer, BUFFER_SIZE);
    printf("Mensaje recibido: %s\n", buffer);
    }while(!strncmp(buffer,"/exit",5)==0);
    printf("Cliente cerro la Sesion");
    close(client_fd);
    printf("\n[Servidor] Cliente cerró la sesión. Esperando SIGTERM para finalizar...\n");
    while(keep_running) {
      sleep(1);
    }
    close(server_fd);
    unlink(SOCKET_PATH);
}

void create_unix_client(char SOCKET_PATH []) {
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

    char mensaje[MAX_LENGTH];
    do{
    fgets(mensaje,MAX_LENGTH,stdin);
    write(client_fd, mensaje, strlen(mensaje) + 1);
    }while(!strncmp(mensaje,"/exit",5)==0);
    
    close(client_fd);
}

void manejador_sigterm(int sig) {
      int log_num = 1;
      FILE *temp = fopen("log-servidor.txt", "r");
      if (temp != NULL) {
          char linea[128];
          while (fgets(linea, sizeof(linea), temp)) {
              if (strstr(linea, "[LOG")) {
                  log_num++;
              }
          }
          fclose(temp);
      }else{
        temp = fopen("log-servidor.txt","w");
        fclose(temp);
      }

      FILE *archivo = fopen("log-servidor.txt", "a");
      if (archivo == NULL) {
          perror("No se pudo abrir el archivo de log");
          exit(EXIT_FAILURE);
      }

      time_t hora_fin = time(NULL);
      fprintf(archivo, "[LOG %d]\n", log_num);
      fprintf(archivo, "Hora de inicio del servidor: %s", ctime(&hora_inicio));
      fprintf(archivo, "Hora de finalización del servidor: %s\n\n", ctime(&hora_fin));

      fclose(archivo);
      printf("\n[LOG] Señal SIGTERM recibida. Archivo de log %d generado.\n", log_num);
      keep_running=0;
}
