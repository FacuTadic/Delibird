#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<pthread.h>
#include "protocolBroker.h"

//#ifndef CONEXIONES_H_
#define CONEXIONES_H_

void* recibir_buffer(int*, int);

int iniciar_servidor(char* ip, char* puerto);
void* serializar_paquete(t_paquete* paquete, int bytes);
void devolver_mensaje(void* payload, int size, int socket_cliente);
