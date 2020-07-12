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

uint32_t id_modulo;

typedef enum
{
	NEW          = 1,
    APPEARED     = 2,
    CATCH        = 3,
	CAUGHT		 = 4,
	GET          = 5,
	LOCALIZED    = 6,
	SUSCRIPCION  = 7
} op_code;


typedef struct {
	uint32_t idCorrelativo;
	uint32_t flag;
} t_caught;

typedef struct {
	uint32_t id_correlativo;
    char* pokemon;
    uint32_t lugares;
    t_list* l_coordenadas;
} t_localized;

typedef struct {
	char* pokemon;
	uint32_t pos_X;
	uint32_t pos_Y;
} t_appeared;

int mandar_suscripcion(int socket, uint32_t id_cola);
t_appeared* recibir_appeared_de_game_boy(int socket, uint32_t* size, t_log* logger);
t_appeared* recibir_appeared(int socket, uint32_t* size, t_log* logger);
t_caught* recibir_caught(int socket_broker, uint32_t* size, t_log* logger);
t_localized* recibir_localized(int socket_broker, uint32_t* size, t_log* logger);
void generar_ID_Modulo();
uint32_t recibir_ID_Catch(int socket_broker,t_log* logger);
int devolver_ack(int socket_broker, t_log* logger);
