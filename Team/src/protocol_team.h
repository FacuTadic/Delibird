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

typedef struct {
	char* pokemon;
	uint32_t pos_X;
	uint32_t pos_Y;
} t_appeared;

t_appeared* recibir_appeared_de_game_boy(int socket, uint32_t* size, t_log* logger);
