/*
 * conexiones.h
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */

#ifndef UTILS_H_
#define UTILS_H_

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

typedef enum
{
	MENSAJE = 1,
	GET = 2,
	APPEARED = 3,
	LOCALIZED = 4,
	CATCH = 5,
	CAUGHT = 6
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	int size;
	char* nombre;
} t_nombre;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

typedef struct
{
	uint32_t id;
	uint32_t coordx;
	uint32_t coordY;
	uint32_t capacidad;
	t_list*  objetivo;
	t_list* pokemones;
} t_entrenador;

typedef struct
{
	uint32_t size;
	char* nombre;
	uint32_t coordx;
	uint32_t coordY;
}t_pokemon;

typedef struct
{
	uint32_t coordx;
	uint32_t coordy;
	struct t_coord* sgte;
}t_coord;

typedef struct
{
	uint32_t sizeTotal;
	uint32_t size;
	char* nombre;
	uint32_t cantidad;
	t_coord* coords;
}t_localized;

typedef struct
{
	uint32_t sizeTotal;
	uint32_t size;
	char* nombre;
	uint32_t coordx;
	uint32_t coordY;
}t_appeared;



int crear_conexion(char* ip, char* puerto);
void enviar_mensaje(char* mensaje, int socket_cliente, t_log* logger);
char* recibir_mensaje(int socket_cliente, t_log* logger);
void eliminar_paquete(t_paquete* paquete);
void liberar_conexion(int socket_cliente);
void mainAppeared(int* conexion);
void mainCatch(int* conexion);
void mainLocalized(int* conexion);
void mainGet(t_list* objetivos);

#endif /* UTILS_H_ */
