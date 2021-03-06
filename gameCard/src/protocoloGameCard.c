#include "protocoloGameCard.h"
#include "creacionDeEstructurasParaEnvio.h"

//####################################################	ENVIAR MENSAJES	#########################################################################

int mandar_suscripcion(int socket_mandado, uint32_t id_cola) {
	uint32_t bytes = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);

	uint32_t cod_op = 7;
	void* flujo = malloc(bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &id_modulo, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &id_cola, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	int status_send = send(socket_mandado, flujo, bytes, 0);

	if (status_send == -1 || status_send == 0) {
		liberar_conexion(socket_mandado);
		free(flujo);
		return -1;
	}

	free(flujo);
	return 1;
}


void enviar_appeared(int socketCliente, t_appeared* appearedAEnviar){

	t_buffer* bufferAppeared = crearBufferAppeared(appearedAEnviar);
	uint32_t bytes;

	log_info(loggerDevProtocolo, "Creando paquete");
	t_paquete* paquete = crearPaquete(bufferAppeared);   // CREAR PAQUETE

	paquete->codigo_operacion = APPEARED;
	log_info(loggerDevProtocolo, "CODIGO DE OPERACION: %i", paquete->codigo_operacion);

	log_info(loggerDevProtocolo, "Paquete Creado");
	log_info(loggerDevProtocolo, "la operacion a realizar es %i", paquete->codigo_operacion);

	log_info(loggerDevProtocolo, "Serializando...");
	void* flujo = serializar_paquete(paquete,&bytes);                   //  SERIALIZAR PAQUETE
	log_info(loggerDevProtocolo, "Serializacion completa");


	log_info(loggerDevProtocolo, "El peso total es: %i",paquete->buffer->size);

	//    ENVIAR MENSAJE
	if(send(socketCliente, flujo, bytes, 0) == -1){
		log_error(loggerDevProtocolo, "Error: No se pudo enviar el mensaje");
	}

	close(socketCliente);

	free(flujo);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);


}


void enviar_caught(int socketCliente, t_caught* caughtAEnviar){
	t_buffer* bufferCaught = crearBufferCaught(caughtAEnviar);
	uint32_t bytes;

	log_info(loggerDevProtocolo, "Creando paquete");
	t_paquete* paquete = crearPaquete(bufferCaught);   // CREAR PAQUETE

	paquete->codigo_operacion = CAUGHT;
	log_info(loggerDevProtocolo, "CODIGO DE OPERACION: %i", paquete->codigo_operacion);

	log_info(loggerDevProtocolo, "Paquete Creado");
	log_info(loggerDevProtocolo, "la operacion a realizar es %i", paquete->codigo_operacion);

	log_info(loggerDevProtocolo, "Serializando...");
	void* flujo = serializar_paquete(paquete,&bytes);                   //  SERIALIZAR PAQUETE
	log_info(loggerDevProtocolo, "Serializacion completa");


	log_info(loggerDevProtocolo, "El peso total es: %i",paquete->buffer->size);

	//    ENVIAR MENSAJE
	if(send(socketCliente, flujo, bytes, 0) == -1){
		log_error(loggerDevProtocolo, "Error: No se pudo enviar el mensaje");
	}

	close(socketCliente);

	free(flujo);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);
}


void enviar_localized(int socketCliente, t_localized* localizedAEnviar){
	t_buffer* bufferLocalized = crearBufferLocalized(localizedAEnviar);
	uint32_t bytes;

	log_info(loggerDevProtocolo, "Creando paquete");
	t_paquete* paquete = crearPaquete(bufferLocalized);   // CREAR PAQUETE

	paquete->codigo_operacion = LOCALIZED;
	log_info(loggerDevProtocolo, "CODIGO DE OPERACION: %i", paquete->codigo_operacion);

	log_info(loggerDevProtocolo, "Paquete Creado");
	log_info(loggerDevProtocolo, "la operacion a realizar es %i", paquete->codigo_operacion);

	log_info(loggerDevProtocolo, "Serializando...");
	void* flujo = serializar_paquete(paquete,&bytes);                   //  SERIALIZAR PAQUETE
	log_info(loggerDevProtocolo, "Serializacion completa");


	log_info(loggerDevProtocolo, "El peso total es: %i",paquete->buffer->size);

	//    ENVIAR MENSAJE
	if(send(socketCliente, flujo, bytes, 0) == -1){
		log_error(loggerDevProtocolo, "Error: No se pudo enviar el mensaje");
	}

	close(socketCliente);

	free(flujo);
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);}


//#######################################################	RECIBIR MENSAJE	#################################################################


t_newLlegada* recibir_new(int socket_cliente, uint32_t* size, t_log* logger) {
	t_newLlegada* new = malloc(sizeof(t_newLlegada));
	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		//liberar_conexion(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(new);
		exit(-1);
	}

	log_info(logger, "Tamanio total recibido: %i", *size);


	if (recv(socket_cliente, &new->id , sizeof(uint32_t), MSG_WAITALL) == -1) {
		//liberar_conexion(socket_cliente);
			log_error(logger, "Hubo un problema recibiendo el ID del mensaje");
			free(new);
			exit(-1);
	}

	log_info(logger, "EL ID recibido: %i", new->id);

	if (recv(socket_cliente, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL) == -1) {
		//liberar_conexion(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		free(new);
		exit(-1);
	}


	new->pokemon = malloc(tamanio_pokemon);

	log_info(logger, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	if (recv(socket_cliente, new->pokemon, tamanio_pokemon, MSG_WAITALL) == -1) {
		//liberar_conexion(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		free(new->pokemon);
		free(new);
		exit(-1);
	}

	log_info(logger, "Nombre del pokemon recibido: %s", new->pokemon);

	if (recv(socket_cliente, &(new->pos_X), sizeof(uint32_t), MSG_WAITALL) == -1) {
		//liberar_conexion(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion X");
		free(new->pokemon);
		free(new);
		exit(-1);
	}

	log_info(logger, "Posicion X recibida: %i", new->pos_X);

	if (recv(socket_cliente, &(new->pos_Y), sizeof(uint32_t), MSG_WAITALL) == -1) {
		//liberar_conexion(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion Y");
		free(new->pokemon);
		free(new);
		exit(-1);
	}

	log_info(logger, "Posicion Y recibida: %i", new->pos_Y);

	if (recv(socket_cliente, &(new->cantidad), sizeof(uint32_t), MSG_WAITALL) == -1) {
		//liberar_conexion(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la cantidad");
		free(new->pokemon);
		free(new);
		exit(-1);
	}
	log_info(logger, "Cantidad recibida: %i", new->cantidad);

	return new;
}

t_catchLlegada* recibir_catch(int socket_cliente, uint32_t* size, t_log* logger) {
	t_catchLlegada* catch = malloc(sizeof(t_catchLlegada));
	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		liberar_conexion(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(catch);
		exit(-1);
	}

	log_info(logger, "Tamanio total recibido: %i", *size);


	if (recv(socket_cliente, &catch->id , sizeof(uint32_t), MSG_WAITALL) == -1) { // lee id del mensaje ///
		liberar_conexion(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el ID del mensaje");
		free(catch);
		exit(-1);
	}

	log_info(logger, "EL ID recibido: %i", catch->id);

	if (recv(socket_cliente, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL) == -1) {
		liberar_conexion(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		free(catch);
		exit(-1);
	}

	catch->pokemon = malloc(tamanio_pokemon);

	log_info(logger, "Tamanio del pokemon: %i", tamanio_pokemon);

	if (recv(socket_cliente, catch->pokemon, tamanio_pokemon, MSG_WAITALL) == -1) {
		liberar_conexion(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		free(catch->pokemon);
		free(catch);
		exit(-1);
	}

	log_info(logger, "Nombre del pokemon recibido: %s", catch->pokemon);

	if (recv(socket_cliente, &(catch->pos_X), sizeof(uint32_t), MSG_WAITALL) == -1) {
		liberar_conexion(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion X");
		free(catch->pokemon);
		free(catch);
		exit(-1);
	}

	log_info(logger, "Posicion X recibida: %i", catch->pos_X);

	if (recv(socket_cliente, &(catch->pos_Y), sizeof(uint32_t), MSG_WAITALL) == -1) {
		liberar_conexion(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion Y");
		free(catch->pokemon);
		free(catch);
		exit(-1);
	}

	log_info(logger, "Posicion Y recibida: %i", catch->pos_Y);

	return catch;
}

t_getLlegada* recibir_get(int socket_cliente, uint32_t* size, t_log* logger) {
	t_getLlegada* get = malloc(sizeof(t_getLlegada));
	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		liberar_conexion(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(get);
		exit(-1);
	}

	log_info(logger, "Tamanio total recibido: %i", *size);


	if (recv(socket_cliente, &get->id , sizeof(uint32_t), MSG_WAITALL) == -1) { // lee id del mensaje ///
		liberar_conexion(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el ID del mensaje");
		free(get);
		exit(-1);
	}

	log_info(logger, "EL ID recibido: %i", get->id);

	if (recv(socket_cliente, &tamanio_pokemon, sizeof(uint32_t), MSG_WAITALL) == -1) {
		liberar_conexion(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		free(get);
		exit(-1);
	}


	get->pokemon = malloc(tamanio_pokemon);

	log_info(logger, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	if (recv(socket_cliente, get->pokemon, tamanio_pokemon, MSG_WAITALL) == -1) {
		liberar_conexion(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		free(get->pokemon);
		free(get);
		exit(-1);
	}

	log_info(logger, "Nombre del pokemon recibido: %s", get->pokemon);

	return get;
}



void liberar_conexion(int socket_cliente){
	if (socket_cliente != -1) {
		close(socket_cliente);
	}
}


int devolver_ack(int socket_broker) {
	uint32_t ack = 1;
	int status_send = send(socket_broker, (void *) &ack, sizeof(uint32_t), 0);
	if (status_send == -1) {
		log_error(loggerDevProtocolo, "Error: no se pudo enviar el ACK al socket %i", socket_broker);
		return -1;
	}
	log_info(loggerDevProtocolo, "Acknowledgement enviado al modulo Broker");
	return 1;
}
