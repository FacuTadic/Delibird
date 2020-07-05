#include "protocol_team.h"

int mandar_suscripcion(int socket, uint32_t id_cola) {
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

	int status_send = send(socket, flujo, bytes, 0);
	if (status_send == -1) {
		close(socket);
		free(flujo);
		return -1;
	}

	free(flujo);
	return 1;
}

t_appeared* recibir_appeared(int socket_broker, uint32_t* size, t_log* logger) {
	t_appeared* appeared = malloc(sizeof(t_appeared));

	uint32_t tamanio_pokemon;

	uint32_t id_correlativo;

	log_info(logger, "Recibiendo tamanio total");

	int status_recv = recv(socket_broker, size, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		close(socket_broker);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		close(socket_broker);
		log_warning(logger, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(appeared);
		return NULL;
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	status_recv = recv(socket_broker, &id_correlativo, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		close(socket_broker);
		log_error(logger, "Hubo un problema recibiendo el id correlativo");
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		close(socket_broker);
		log_warning(logger, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(appeared);
		return NULL;
	}

	log_info(logger, "Id correlativo recibido: %i", id_correlativo);

	status_recv = recv(socket_broker, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		close(socket_broker);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		close(socket_broker);
		log_warning(logger, "El cliente con socket_broker %i acaba de cerrar la conexion", socket);
		free(appeared);
		return NULL;
	}

	appeared->pokemon = malloc(tamanio_pokemon);

	log_info(logger, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	status_recv = recv(socket_broker, appeared->pokemon, tamanio_pokemon, MSG_WAITALL);
	if (status_recv == -1) {
		close(socket_broker);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		close(socket_broker);
		log_warning(logger, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}

	log_info(logger, "Nombre del pokemon recibido: %s", appeared->pokemon);

	status_recv = recv(socket_broker, &(appeared->pos_X), sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		close(socket_broker);
		log_error(logger, "Hubo un problema recibiendo la posicion X");
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		close(socket_broker);
		log_warning(logger, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}

	log_info(logger, "Posicion X recibida: %i", appeared->pos_X);

	status_recv = recv(socket_broker, &(appeared->pos_Y), sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		close(socket_broker);
		log_error(logger, "Hubo un problema recibiendo la posicion Y");
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		close(socket_broker);
		log_warning(logger, "El cliente con socket_broker %i acaba de cerrar la conexion", socket);
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}

	log_info(logger, "Posicion Y recibida: %i", appeared->pos_Y);


	devolver_ack(socket_broker,logger);

	return appeared;
}

t_appeared* recibir_appeared_de_game_boy(int socket, uint32_t* size, t_log* logger) {

	t_appeared* appeared = malloc(sizeof(t_appeared));

	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	int status_recv = recv(socket, size, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		close(socket);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		close(socket);
		log_warning(logger, "El cliente con socket %i acaba de cerrar la conexion", socket);
		free(appeared);
		return NULL;
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	status_recv = recv(socket, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		close(socket);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		close(socket);
		log_warning(logger, "El cliente con socket %i acaba de cerrar la conexion", socket);
		free(appeared);
		return NULL;
	}

	appeared->pokemon = malloc(tamanio_pokemon);

	log_info(logger, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	status_recv = recv(socket, appeared->pokemon, tamanio_pokemon, MSG_WAITALL);
	if (status_recv == -1) {
		close(socket);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		close(socket);
		log_warning(logger, "El cliente con socket %i acaba de cerrar la conexion", socket);
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}

	log_info(logger, "Nombre del pokemon recibido: %s", appeared->pokemon);

	status_recv = recv(socket, &(appeared->pos_X), sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		close(socket);
		log_error(logger, "Hubo un problema recibiendo la posicion X");
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		close(socket);
		log_warning(logger, "El cliente con socket %i acaba de cerrar la conexion", socket);
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}

	log_info(logger, "Posicion X recibida: %i", appeared->pos_X);

	status_recv = recv(socket, &(appeared->pos_Y), sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		close(socket);
		log_error(logger, "Hubo un problema recibiendo la posicion Y");
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		close(socket);
		log_warning(logger, "El cliente con socket %i acaba de cerrar la conexion", socket);
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}

	log_info(logger, "Posicion Y recibida: %i", appeared->pos_Y);

	return appeared;
}

int devolver_ack(int socket_broker, t_log* logger) {
	uint32_t ack = 1;
	int status_send = send(socket_broker, (void *) &ack, sizeof(uint32_t), 0);
	if (status_send == -1) {
		log_error(logger, "Error: no se pudo enviar el ACK al socket %i", socket_broker);
		return -1;
	}
	log_info(logger, "Acknowledgement enviado al modulo Broker");
	return 1;
}
