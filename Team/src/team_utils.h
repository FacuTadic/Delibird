#ifndef TEAM_UTILS_H_
#define TEAM_UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include <commons/collections/queue.h>
#include <commons/collections/list.h>

#endif

int crear_conexion(char* ip, char* puerto);
int iniciar_escucha_game_boy(char* ip, char* puerto);
