#include "protocolBroker.h"

t_new* recibir_new(int socket_cliente, int* size, t_log* logger) {
	t_new* new = malloc(sizeof(t_new));
	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		pthread_exit(NULL);
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		pthread_exit(NULL);
	}

	log_info(logger, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	if (recv(socket_cliente, new->pokemon, tamanio_pokemon, MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		pthread_exit(NULL);
	}

	log_info(logger, "Nombre del pokemon recibido: %s", new->pokemon);

	if (recv(socket_cliente, &(new->pos_X), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion X");
		pthread_exit(NULL);
	}

	log_info(logger, "Posicion X recibida: %i", new->pos_X);

	if (recv(socket_cliente, &(new->pos_Y), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion Y");
		pthread_exit(NULL);
	}

	log_info(logger, "Posicion Y recibida: %i", new->pos_Y);

	if (recv(socket_cliente, &(new->cantidad), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la cantidad");
		pthread_exit(NULL);
	}

	log_info(logger, "Cantidad recibida: %i", new->cantidad);

	return new;
}

t_appeared* recibir_appeared(int socket_cliente, int* size, t_log* logger) {
	t_appeared* appeared = malloc(sizeof(t_appeared));

	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		pthread_exit(NULL);
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, &(appeared->id_correlativo), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el id correlativo");
		pthread_exit(NULL);
	}

	log_info(logger, "Id correlativo recibido: %i", appeared->id_correlativo);

	if (recv(socket_cliente, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		pthread_exit(NULL);
	}

	log_info(logger, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	if (recv(socket_cliente, appeared->pokemon, tamanio_pokemon, MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		pthread_exit(NULL);
	}

	log_info(logger, "Nombre del pokemon recibido: %s", appeared->pokemon);

	if (recv(socket_cliente, &(appeared->pos_X), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion X");
		pthread_exit(NULL);
	}

	log_info(logger, "Posicion X recibida: %i", appeared->pos_X);

	if (recv(socket_cliente, &(appeared->pos_Y), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion Y");
		pthread_exit(NULL);
	}

	log_info(logger, "Posicion Y recibida: %i", appeared->pos_Y);

	return appeared;
}

t_catch* recibir_catch(int socket_cliente, int* size, t_log* logger) {
	t_catch* catch = malloc(sizeof(t_catch));
	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		pthread_exit(NULL);
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		pthread_exit(NULL);
	}

	log_info(logger, "Tamanio del pokemon: %i", tamanio_pokemon);

	if (recv(socket_cliente, catch->pokemon, tamanio_pokemon, MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		pthread_exit(NULL);
	}

	log_info(logger, "Nombre del pokemon recibido: %s", catch->pokemon);

	if (recv(socket_cliente, &(catch->pos_X), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion X");
		pthread_exit(NULL);
	}

	log_info(logger, "Posicion X recibida: %i", catch->pos_X);

	if (recv(socket_cliente, &(catch->pos_Y), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion Y");
		pthread_exit(NULL);
	}

	log_info(logger, "Posicion Y recibida: %i", catch->pos_Y);

	return catch;
}

t_caught* recibir_caught(int socket_cliente, int* size, t_log* logger) {
	t_caught* caught = malloc(sizeof(t_caught));

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		pthread_exit(NULL);
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, &(caught->id_correlativo), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el id correlativo");
		pthread_exit(NULL);
	}

	log_info(logger, "Id correlativo recibido: %i", caught->id_correlativo);

	if (recv(socket_cliente, &(caught->flag_caught), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el flag de caught");
		pthread_exit(NULL);
	}

	log_info(logger, "Flag de caught recibido: %i", caught->flag_caught);

	return caught;
}

t_get* recibir_get(int socket_cliente, int* size, t_log* logger) {
	t_get* get = malloc(sizeof(t_get));
	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		pthread_exit(NULL);
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, &tamanio_pokemon, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		pthread_exit(NULL);
	}

	log_info(logger, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	if (recv(socket_cliente, get->pokemon, *size, MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		pthread_exit(NULL);
	}

	log_info(logger, "Nombre del pokemon recibido: %i", get->pokemon);

	return get;
}

t_localized* recibir_localized(int socket_cliente, int* size, t_log* logger) {
	t_localized* localized = malloc(sizeof(t_localized));
	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		pthread_exit(NULL);
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, &(localized->id_correlativo), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el id correlativo");
		pthread_exit(NULL);
	}

	log_info(logger, "Id correlativo recibido: %i", localized->id_correlativo);

	if (recv(socket_cliente, &tamanio_pokemon, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		pthread_exit(NULL);
	}

	log_info(logger, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	if (recv(socket_cliente, localized->pokemon,tamanio_pokemon, MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		pthread_exit(NULL);
	}

	log_info(logger, "Nombre del pokemon recibido: %s", localized->pokemon);

	if (recv(socket_cliente, &(localized->lugares), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la cantidad de coordenadas");
		pthread_exit(NULL);
	}

	log_info(logger, "Cantidad de coordenadas recibida: %i", localized->lugares);

	localized->l_coordenadas = list_create();

	for (int i = 0; i < (localized->lugares)*2; i++) {
		uint32_t unidad_coord;

		if (recv(socket_cliente, &unidad_coord, sizeof(uint32_t), MSG_WAITALL) == -1) {
			close(socket_cliente);
			log_error(logger, "Hubo un problema recibiendo la unidad de coordenada");
			pthread_exit(NULL);
		}

		log_info(logger, "Unidad de coordenada recibida: %i", unidad_coord);

		list_add(localized->l_coordenadas, &unidad_coord);
	}

	return localized;
}

uint32_t recibir_suscripcion(int socket_cliente, t_log* logger) {
	uint32_t id_cola;

	log_info(logger, "Recibiendo id de cola");

	if (recv(socket_cliente, &id_cola, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el id de cola");
		pthread_exit(NULL);
	}

	log_info(logger, "Id de cola recibido: %i", id_cola);

	return id_cola;
}

void devolver_id(int socket_cliente, uint32_t id, t_log* logger) {
	log_info(logger, "Retornando id %i", id);

	if (send(socket_cliente, (void *) id, sizeof(uint32_t), 0) == -1) {
		log_error(logger, "Hubo un problema retornando el id %i", id);
	}
}
