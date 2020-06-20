#ifndef PROTOCOLOGAMECARD_H_
#define PROTOCOLOGAMECARD_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<pthread.h>


typedef struct {
    char* pokemon;
    uint32_t pos_X, pos_Y, cantidad;
    uint32_t id;
} t_newLlegada;



typedef struct {
    char* pokemon;
    uint32_t id;
} t_getLlegada;


typedef struct {
    char* pokemon;
    uint32_t pos_X, pos_Y;
    uint32_t id;
} t_catchLlegada;


typedef struct {
	uint32_t id_cliente;
	int socket_cliente;
} info_modulo;

void recibir_new(int socket_cliente, uint32_t* size, t_log* logger);
void recibir_catch(int socket_cliente, uint32_t* size, t_log* logger);
void recibir_get(int socket_cliente, uint32_t* size, t_log* logger);
void devolver_id(int socket_cliente, uint32_t id, t_log* logger);


#endif /* PROTOCOLOGAMECARD_H_ */
