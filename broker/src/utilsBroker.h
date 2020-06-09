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

int iniciar_servidor(char* ip, char* puerto);
int crear_conexion(char *ip, char* puerto);
