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

typedef enum
{
	NEW          = 1,
    APPEARED     = 2,
    GET          = 3,
    LOCALIZED    = 4,
    CATCH        = 5,
    CAUGHT       = 6,
	MENSAJE 	 = 7
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
    char* pokemon;
    uint32_t lugares;
    t_list* l_coordenadas;
} t_localized;

typedef struct {
    char* pokemon;
} t_get;

typedef struct {
	char* pokemon;
    uint32_t pos_X, pos_Y;
} t_appeared;

typedef struct {
    char* pokemon;
    uint32_t pos_X, pos_Y;
} t_catch;

typedef struct {
	uint32_t flag_caught;
} t_caught;

t_new* recibir_new(int socket_cliente, int* size);
t_appeared* recibir_appeared(int socket_cliente, int* size);
t_catch* recibir_catch(int socket_cliente, int* size);
t_caught* recibir_caught(int socket_cliente, int* size);
t_get* recibir_get(int socket_cliente, int* size);
t_localized* recibir_localized(int socket_cliente, int* size);
void devolver_id(int socket_cliente, uint32_t id);

#endif /* PROTOCOLBROKER_H_ */
