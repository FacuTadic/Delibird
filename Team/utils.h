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

typedef enum
{
	MENSAJE = 1,
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
	uint32_t coordX,
			 coordY;
} t_coord;

typedef struct
{
	char* nombre;
	struct t_objetivo* sgte;
} t_objetivo;

typedef struct
{
	uint32_t id;
	t_coord coord;
	uint32_t capacidad;
	t_objetivo* objetivo;
	struct t_entrenador* sgte;
} t_entrenador;


int crear_conexion(char* ip, char* puerto);
void enviar_mensaje(char* mensaje, int socket_cliente, t_log* logger);
char* recibir_mensaje(int socket_cliente, t_log* logger);
void eliminar_paquete(t_paquete* paquete);
void liberar_conexion(int socket_cliente);

#endif /* UTILS_H_ */
