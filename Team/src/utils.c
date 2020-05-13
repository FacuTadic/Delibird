/*
 * conexiones.c
 *
 *  Created on: 2 mar. 2019
 *      Author: utnso
 */


#include "utils.h"

/******************************** Conexión  *******************************/
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

/******************************** End Conexión  *******************************/
/******************************** Paquetes y Serializados  *******************************/
t_paquete* crearPaquete(t_buffer* buffer){
	t_paquete* paquete = malloc(sizeof(paquete));
	paquete->buffer = malloc(buffer->size + sizeof(buffer->size));
	memcpy (paquete->buffer,  buffer, buffer->size + sizeof(int));
	return paquete;
	// aca podemos liberar el buffer stream y buffer
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

void* serializar_get(t_paquete* paquete, int *bytes)
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


/******************************** END Paquetes y Serializados  *******************************/
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

void mensaje_get(char* mensaje, int socket_cliente, t_log* logger)
{
	int bytes;
	t_buffer* buffer = malloc(sizeof(t_buffer));
	buffer->size = strlen(mensaje)+1;
	buffer->stream = malloc(buffer->size);
	memcpy (buffer->stream, mensaje, buffer->size);

	t_paquete* paquete = crearPaquete(buffer);   // CREAR PAQUETE
	paquete->codigo_operacion = GET;

	log_info(logger, "la operacion a realizar es %i", paquete->codigo_operacion);
	void* flujo = serializar_get(paquete,&bytes);                   //  SERIALIZAR PAQUETE

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
			case GET:
				memcpy(mensaje, paquete->buffer->stream,paquete->buffer->size);
				free(paquete->buffer->stream);
				free(paquete->buffer);
				free(paquete);
				return mensaje;

			case MENSAJE:
				return "test MENSAJE CASE";
		}
	return "no hay codigo de operacion";
}

uint32_t recibir_get(int socket_cliente, t_log* logger)
{
	uint32_t id;
	if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1)
	{
		log_error(logger, "error: No se recibió el código de operación");
		return -1;
	}

	return id;
}

t_pokemon* recibir_appeared(int* socket_cliente, t_log* logger)
{

	uint32_t operacion;

	// cod op
	t_pokemon* pokemon = malloc(sizeof(pokemon));
	if (recv(*socket_cliente, &operacion ,sizeof(uint32_t),MSG_WAITALL) == -1){
		log_error(logger, "error: No se recibió el código de operación");
	}

	// size nombre
	if (recv(*socket_cliente, &pokemon->size, sizeof(uint32_t),MSG_WAITALL) == -1){
			log_error(logger, "error: No se recibió el tamaño del pokemon");
		}
	// nombre
	pokemon->nombre = malloc (sizeof(pokemon->nombre));
	if (recv(*socket_cliente, &pokemon->nombre, pokemon->size,MSG_WAITALL) == -1){
		log_error(logger, "error: No se recibió el nombre del pokemon");
	}

	// coord X
	if (recv(*socket_cliente, &pokemon->coordx, sizeof(uint32_t),MSG_WAITALL) == -1){
		log_error(logger, "error: No se pudo recibir la coordenada X");
	}

	// coord Y
	if (recv(*socket_cliente, &pokemon->coordY, sizeof(uint32_t),MSG_WAITALL) == -1){
		log_error(logger, "error: No se pudo recibir la coordenada Y");
	}

	return pokemon;
}


void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
