#ifndef PROTOCOL_TEAM_H_
#define PROTOCOL_TEAM_H_

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

typedef struct
{
	uint32_t sizeTotal;
	uint32_t size;
	char* nombre;
	uint32_t coordx;
	uint32_t coordY;
} t_appeared;

t_appeared* recibir_appeared_de_game_boy(int socket, uint32_t* size);
