#ifndef UTILSBROKER_H_
#define UTILSBROKER_H_

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

#endif

int iniciar_servidor(char* ip, char* puerto);
int crear_conexion(char *ip, char* puerto);
