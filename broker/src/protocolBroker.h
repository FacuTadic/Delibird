#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<pthread.h>

#ifndef PROTOCOLBROKER_H_
#define PROTOCOLBROKER_H_


///////////
// MENSAJES
///////////

typedef enum
{
	NEW          = 1,
    APPEARED     = 2,
    GET          = 3,
    LOCALIZED    = 4,
    CATCH        = 5,
    CAUGHT       = 6,
	SUSCRIPCION  = 7
} op_code;

typedef struct {
	int size;
	void* stream;
} t_buffer;

typedef struct {
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

typedef struct {
    char* pokemon;
    uint32_t pos_X, pos_Y, cantidad;
} t_new;

typedef struct {
	uint32_t id_correlativo;
    char* pokemon;
    uint32_t lugares;
    t_list* l_coordenadas;
} t_localized;

typedef struct {
    char* pokemon;
} t_get;

typedef struct {
	uint32_t id_correlativo;
	char* pokemon;
    uint32_t pos_X, pos_Y;
} t_appeared;

typedef struct {
    char* pokemon;
    uint32_t pos_X, pos_Y;
} t_catch;

typedef struct {
	uint32_t id_correlativo;
	uint32_t flag_caught;
} t_caught;


//////////
// MODULOS
//////////

typedef struct {
	char* ip;
	char* puerto;
	char* nombre;
} info_modulo;

info_modulo INFO_TEAM;
info_modulo INFO_GAME_BOY;
info_modulo INFO_GAME_CARD;

t_new* recibir_new(int socket_cliente, int* size, t_log* logger);
t_appeared* recibir_appeared(int socket_cliente, int* size, t_log* logger);
t_catch* recibir_catch(int socket_cliente, int* size, t_log* logger);
t_caught* recibir_caught(int socket_cliente, int* size, t_log* logger);
t_get* recibir_get(int socket_cliente, int* size, t_log* logger);
t_localized* recibir_localized(int socket_cliente, int* size, t_log* logger);
uint32_t recibir_suscripcion(int socket_cliente, int* size, t_log* logger);
void devolver_id(int socket_cliente, uint32_t id, t_log* logger);

#endif /* PROTOCOLBROKER_H_ */
