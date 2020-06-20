#include "protocoloGameCard.h"
#include "utilsGameCard.h"

void recibir_new(int socket_cliente, uint32_t* size, t_log* logger) {
	t_newLlegada* new = malloc(sizeof(t_newLlegada));
	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(new);
		exit(-1);
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		free(new);
		exit(-1);
	}

	if (recv(socket_cliente, new->id , sizeof(uint32_t), MSG_WAITALL) == -1) {
			close(socket_cliente);
			log_error(logger, "Hubo un problema recibiendo el ID del mensaje");
			free(new);
			exit(-1);
	}

	new->pokemon = malloc(tamanio_pokemon);

	log_info(logger, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	if (recv(socket_cliente, new->pokemon, tamanio_pokemon, MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		free(new->pokemon);
		free(new);
		exit(-1);
	}

	log_info(logger, "Nombre del pokemon recibido: %s", new->pokemon);

	if (recv(socket_cliente, &(new->pos_X), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion X");
		free(new->pokemon);
		free(new);
		exit(-1);
	}

	log_info(logger, "Posicion X recibida: %i", new->pos_X);

	if (recv(socket_cliente, &(new->pos_Y), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion Y");
		free(new->pokemon);
		free(new);
		exit(-1);
	}

	log_info(logger, "Posicion Y recibida: %i", new->pos_Y);

	if (recv(socket_cliente, &(new->cantidad), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la cantidad");
		free(new->pokemon);
		free(new);
		exit(-1);
	}

	log_info(logger, "Cantidad recibida: %i", new->cantidad);

}

void recibir_appeared(int socket_cliente, uint32_t* size, t_log* logger) {
	t_appearedLlegada* appeared = malloc(sizeof(t_appearedLlegada));

	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(appeared);
		exit(-1);
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, appeared->id , sizeof(uint32_t), MSG_WAITALL) == -1) { // lee id del mensaje ///
				close(socket_cliente);
				log_error(logger, "Hubo un problema recibiendo el ID del mensaje");
				free(appeared);
				exit(-1);
	}
	if (recv(socket_cliente, &(appeared->id_correlativo), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el id correlativo");
		free(appeared);
		exit(-1);
	}

	log_info(logger, "Id correlativo recibido: %i", appeared->id_correlativo);

	if (recv(socket_cliente, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		free(appeared);
		exit(-1);
	}

	appeared->pokemon = malloc(tamanio_pokemon);

	log_info(logger, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	if (recv(socket_cliente, appeared->pokemon, tamanio_pokemon, MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		free(appeared->pokemon);
		free(appeared);
		exit(-1);
	}

	log_info(logger, "Nombre del pokemon recibido: %s", appeared->pokemon);

	if (recv(socket_cliente, &(appeared->pos_X), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion X");
		free(appeared->pokemon);
		free(appeared);
		exit(-1);
	}

	log_info(logger, "Posicion X recibida: %i", appeared->pos_X);

	if (recv(socket_cliente, &(appeared->pos_Y), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion Y");
		free(appeared->pokemon);
		free(appeared);
		exit(-1);
	}

	log_info(logger, "Posicion Y recibida: %i", appeared->pos_Y);

}

void recibir_catch(int socket_cliente, uint32_t* size, t_log* logger) {
	t_catchLlegada* catch = malloc(sizeof(t_catchLlegada));
	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(catch);
		exit(-1);
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, catch->id , sizeof(uint32_t), MSG_WAITALL) == -1) { // lee id del mensaje ///
					close(socket_cliente);
					log_error(logger, "Hubo un problema recibiendo el ID del mensaje");
					free(catch);
					exit(-1);
	}
	if (recv(socket_cliente, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		free(catch);
		exit(-1);
	}

	catch->pokemon = malloc(tamanio_pokemon);

	log_info(logger, "Tamanio del pokemon: %i", tamanio_pokemon);

	if (recv(socket_cliente, catch->pokemon, tamanio_pokemon, MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		free(catch->pokemon);
		free(catch);
		exit(-1);
	}

	log_info(logger, "Nombre del pokemon recibido: %s", catch->pokemon);

	if (recv(socket_cliente, &(catch->pos_X), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion X");
		free(catch->pokemon);
		free(catch);
		exit(-1);
	}

	log_info(logger, "Posicion X recibida: %i", catch->pos_X);

	if (recv(socket_cliente, &(catch->pos_Y), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion Y");
		free(catch->pokemon);
		free(catch);
		exit(-1);
	}

	log_info(logger, "Posicion Y recibida: %i", catch->pos_Y);

}

void recibir_caught(int socket_cliente, uint32_t* size, t_log* logger) {
	t_caughtLlegada* caught = malloc(sizeof(t_caughtLlegada));

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(caught);
		exit(-1);
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, caught->id , sizeof(uint32_t), MSG_WAITALL) == -1) { // lee id del mensaje ///
					close(socket_cliente);
					log_error(logger, "Hubo un problema recibiendo el ID del mensaje");
					free(caught);
					exit(-1);
	}
	if (recv(socket_cliente, &(caught->id_correlativo), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el id correlativo");
		free(caught);
		exit(-1);
	}

	log_info(logger, "Id correlativo recibido: %i", caught->id_correlativo);

	if (recv(socket_cliente, &(caught->flag_caught), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el flag de caught");
		free(caught);
		exit(-1);
	}

	log_info(logger, "Flag de caught recibido: %i", caught->flag_caught);
}

void recibir_get(int socket_cliente, uint32_t* size, t_log* logger) {
	t_getLlegada* get = malloc(sizeof(t_getLlegada));
	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(get);
		exit(-1);
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, get->id , sizeof(uint32_t), MSG_WAITALL) == -1) { // lee id del mensaje ///
					close(socket_cliente);
					log_error(logger, "Hubo un problema recibiendo el ID del mensaje");
					free(get);
					exit(-1);
	}

	if (recv(socket_cliente, &tamanio_pokemon, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		free(get);
		exit(-1);
	}

	get->pokemon = malloc(tamanio_pokemon);

	log_info(logger, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	if (recv(socket_cliente, get->pokemon, tamanio_pokemon, MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		free(get->pokemon);
		free(get);
		exit(-1);
	}

	log_info(logger, "Nombre del pokemon recibido: %s", get->pokemon);
}

void recibir_localized(int socket_cliente, uint32_t* size, t_log* logger) {
	t_localizedLlegada* localized = malloc(sizeof(t_localizedLlegada));
	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(localized);
		exit(-1);
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, localized->id , sizeof(uint32_t), MSG_WAITALL) == -1) { // lee id del mensaje ///
					close(socket_cliente);
					log_error(logger, "Hubo un problema recibiendo el ID del mensaje");
					free(localized);
					exit(-1);
	}

	if (recv(socket_cliente, &(localized->id_correlativo), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el id correlativo");
		free(localized);
		exit(-1);
	}

	log_info(logger, "Id correlativo recibido: %i", localized->id_correlativo);

	if (recv(socket_cliente, &tamanio_pokemon, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		free(localized);
		exit(-1);
	}

	localized->pokemon = malloc(tamanio_pokemon);

	log_info(logger, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	if (recv(socket_cliente, localized->pokemon,tamanio_pokemon, MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		free(localized->pokemon);
		free(localized);
		exit(-1);
	}

	log_info(logger, "Nombre del pokemon recibido: %s", localized->pokemon);

	if (recv(socket_cliente, &(localized->lugares), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la cantidad de coordenadas");
		free(localized->pokemon);
		free(localized);
		exit(-1);
	}

	log_info(logger, "Cantidad de coordenadas recibida: %i", localized->lugares);

	localized->l_coordenadas = list_create();

	for (int i = 0; i < (localized->lugares)*2; i++) {
		uint32_t unidad_coord;

		if (recv(socket_cliente, &unidad_coord, sizeof(uint32_t), MSG_WAITALL) == -1) {
			close(socket_cliente);
			log_error(logger, "Hubo un problema recibiendo la unidad de coordenada");
			free(localized->pokemon);
			list_clean(localized->l_coordenadas);
			list_destroy(localized->l_coordenadas);
			free(localized);
			exit(-1);
		}

		log_info(logger, "Unidad de coordenada recibida: %i", unidad_coord);

		list_add(localized->l_coordenadas, &unidad_coord);
	}

}
