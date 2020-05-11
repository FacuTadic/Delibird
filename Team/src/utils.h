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
	char* nombre;
	t_coord coord;
	uint capacidad;
	t_objetivo* objetivo;
	struct t_entrenador* sgte;
} t_entrenador;


int crear_conexion(char* ip, char* puerto);
void enviar_mensaje(char* mensaje, int socket_cliente, t_log* logger);
char* recibir_mensaje(int socket_cliente, t_log* logger);
void eliminar_paquete(t_paquete* paquete);
void liberar_conexion(int socket_cliente);


int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}

void* serializar_paquete(t_paquete* paquete, int *bytes)
{
	*bytes = sizeof(int) *2 + paquete->buffer->size;

	void * flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &(paquete->codigo_operacion), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(flujo + desplazamiento, &(paquete->buffer->size), sizeof(int));
	desplazamiento+= sizeof(int);
	memcpy(flujo + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return flujo;
}


//crearPaquete: Crea una estructura de tipo t_paquete y le enchufa el buffer dentro.

t_paquete* crearPaquete(t_buffer* buffer){
	t_paquete* paquete = malloc(sizeof(paquete));
	paquete->buffer = malloc(buffer->size + sizeof(buffer->size));
	memcpy (paquete->buffer,  buffer, buffer->size + sizeof(int));
	return paquete;
	// aca podemos liberar el buffer stream y buffer
}

/**********************************       ENVIAR MENSAJE       ***********************************************

 */

void enviar_mensaje(char* mensaje, int socket_cliente, t_log* logger)
{
	int bytes;
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = strlen(mensaje)+1;
	buffer->stream = malloc(buffer->size);
	memcpy (buffer->stream, mensaje, buffer->size);


	t_paquete* paquete = crearPaquete(buffer);   // CREAR PAQUETE
	paquete->codigo_operacion = MENSAJE;


	log_info(logger, "la operacion a realizar es %i", paquete->codigo_operacion);
	void* flujo = serializar_paquete(paquete,&bytes);                   //  SERIALIZAR PAQUETE

	//    ENVIAR MENSAJE
	if (send(socket_cliente, flujo, bytes, 0) == -1){
		log_error(logger, "Error: No se pudo enviar el mensaje");
	}
	free(flujo);
	free(buffer->stream);
	free(buffer);
	free(paquete->buffer);
	free(paquete);
}

/**********************************       RECIBIR MENSAJE       ***********************************************

 */
char* recibir_mensaje(int socket_cliente, t_log* logger)
{
	t_paquete* paquete = malloc(sizeof(paquete));
	if (recv(socket_cliente, &paquete->codigo_operacion ,sizeof(int),MSG_WAITALL) == -1){
		log_error(logger, "error: No se recibió el código de operación");
		return "Error: No se recibió el código de operación";
	}

	paquete->buffer = malloc (sizeof(paquete->buffer));
	if (recv(socket_cliente, &paquete->buffer->size, sizeof(int),MSG_WAITALL) == -1){
		log_error(logger, "error: No se recibió el tañaño del buffer");
		return "Error: No se recibió el tañaño del buffer";
	}


	paquete->buffer->stream = malloc(paquete->buffer->size);
	if (recv(socket_cliente, paquete->buffer->stream, paquete->buffer->size,MSG_WAITALL) == -1){
		log_error(logger, "error: No se pudo recibir el buffer");
		return "Error: No se pudo recibir el buffer";
	}

	char* mensaje = malloc(paquete->buffer->size);
	switch(paquete->codigo_operacion){
			case MENSAJE:
				memcpy(mensaje, paquete->buffer->stream,paquete->buffer->size);
				free(paquete->buffer->stream);
				free(paquete->buffer);
				free(paquete);
				return mensaje;
		}
				return "Me mandaron cualquier cosa"; // No entra por ningun codigo del enum.
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}


#endif /* UTILS_H_ */
