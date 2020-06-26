#include "protocol_team.h"

t_appeared_gb* recibir_appeared_de_game_boy(int socket, uint32_t* size, t_log* logger) {

	t_appeared_gb* appeared = malloc(sizeof(t_appeared_gb));

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
