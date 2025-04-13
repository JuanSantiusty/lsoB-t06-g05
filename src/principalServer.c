#include "unixsocket.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char *argv[]) {
    if(!(argc == 2)){
      printf("Usar ./unix_server ruta");
      exit(-1);
    }
    create_unix_server(argv[1]);
    return 0;
}
