#ifndef PROTOCOLGAMEBOY_H_
#define PROTOCOLGAMEBOY_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<pthread.h>




///////////
// MENSAJES
///////////

typedef struct {
    char* pokemon;
    uint32_t pos_X, pos_Y, cantidad;
    uint32_t id;
} t_newLlegada;

typedef struct {
	uint32_t id_correlativo;
    char* pokemon;
    uint32_t lugares;
    t_list* l_coordenadas;
    uint32_t id;
} t_localizedLlegada;

typedef struct {
    char* pokemon;
    uint32_t id;
} t_getLlegada;

typedef struct {
	uint32_t id_correlativo;
	char* pokemon;
    uint32_t pos_X, pos_Y;
    uint32_t id;
} t_appearedLlegada;

typedef struct {
    char* pokemon;
    uint32_t pos_X, pos_Y;
    uint32_t id;
} t_catchLlegada;

typedef struct {
	uint32_t id_correlativo;
	uint32_t flag_caught;
	uint32_t id;
} t_caughtLlegada;



//////////
// MODULOS
//////////

typedef struct {
	uint32_t id_cliente;
	int socket_cliente;
} info_modulo;

void recibir_new(int socket_cliente, uint32_t* size, t_log* logger);
void recibir_appeared(int socket_cliente, uint32_t* size, t_log* logger);
void recibir_catch(int socket_cliente, uint32_t* size, t_log* logger);
void recibir_caught(int socket_cliente, uint32_t* size, t_log* logger);
void recibir_get(int socket_cliente, uint32_t* size, t_log* logger);
void recibir_localized(int socket_cliente, uint32_t* size, t_log* logger);
void devolver_id(int socket_cliente, uint32_t id, t_log* logger);
#endif
