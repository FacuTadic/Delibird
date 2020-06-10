#include "protocolBroker.h"

t_new* recibir_new(int socket_cliente, uint32_t* size, t_log* logger) {
	t_new* new = malloc(sizeof(t_new));
	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(new);
		return NULL;
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		free(new);
		return NULL;
	}

	new->pokemon = malloc(tamanio_pokemon);

	log_info(logger, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	if (recv(socket_cliente, new->pokemon, tamanio_pokemon, MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		free(new->pokemon);
		free(new);
		return NULL;
	}

	log_info(logger, "Nombre del pokemon recibido: %s", new->pokemon);

	if (recv(socket_cliente, &(new->pos_X), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion X");
		free(new->pokemon);
		free(new);
		return NULL;
	}

	log_info(logger, "Posicion X recibida: %i", new->pos_X);

	if (recv(socket_cliente, &(new->pos_Y), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion Y");
		free(new->pokemon);
		free(new);
		return NULL;
	}

	log_info(logger, "Posicion Y recibida: %i", new->pos_Y);

	if (recv(socket_cliente, &(new->cantidad), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la cantidad");
		free(new->pokemon);
		free(new);
		return NULL;
	}

	log_info(logger, "Cantidad recibida: %i", new->cantidad);

	return new;
}

t_appeared* recibir_appeared(int socket_cliente, uint32_t* size, t_log* logger) {
	t_appeared* appeared = malloc(sizeof(t_appeared));

	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(appeared);
		return NULL;
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, &(appeared->id_correlativo), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el id correlativo");
		free(appeared);
		return NULL;
	}

	log_info(logger, "Id correlativo recibido: %i", appeared->id_correlativo);

	if (recv(socket_cliente, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		free(appeared);
		return NULL;
	}

	appeared->pokemon = malloc(tamanio_pokemon);

	log_info(logger, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	if (recv(socket_cliente, appeared->pokemon, tamanio_pokemon, MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}

	log_info(logger, "Nombre del pokemon recibido: %s", appeared->pokemon);

	if (recv(socket_cliente, &(appeared->pos_X), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion X");
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}

	log_info(logger, "Posicion X recibida: %i", appeared->pos_X);

	if (recv(socket_cliente, &(appeared->pos_Y), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion Y");
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}

	log_info(logger, "Posicion Y recibida: %i", appeared->pos_Y);

	return appeared;
}

t_catch* recibir_catch(int socket_cliente, uint32_t* size, t_log* logger) {
	t_catch* catch = malloc(sizeof(t_catch));
	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(catch);
		return NULL;
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		free(catch);
		return NULL;
	}

	catch->pokemon = malloc(tamanio_pokemon);

	log_info(logger, "Tamanio del pokemon: %i", tamanio_pokemon);

	if (recv(socket_cliente, catch->pokemon, tamanio_pokemon, MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		free(catch->pokemon);
		free(catch);
		return NULL;
	}

	log_info(logger, "Nombre del pokemon recibido: %s", catch->pokemon);

	if (recv(socket_cliente, &(catch->pos_X), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion X");
		free(catch->pokemon);
		free(catch);
		return NULL;
	}

	log_info(logger, "Posicion X recibida: %i", catch->pos_X);

	if (recv(socket_cliente, &(catch->pos_Y), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la posicion Y");
		free(catch->pokemon);
		free(catch);
		return NULL;
	}

	log_info(logger, "Posicion Y recibida: %i", catch->pos_Y);

	return catch;
}

t_caught* recibir_caught(int socket_cliente, uint32_t* size, t_log* logger) {
	t_caught* caught = malloc(sizeof(t_caught));

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(caught);
		return NULL;
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, &(caught->id_correlativo), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el id correlativo");
		free(caught);
		return NULL;
	}

	log_info(logger, "Id correlativo recibido: %i", caught->id_correlativo);

	if (recv(socket_cliente, &(caught->flag_caught), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el flag de caught");
		free(caught);
		return NULL;
	}

	log_info(logger, "Flag de caught recibido: %i", caught->flag_caught);

	return caught;
}

t_get* recibir_get(int socket_cliente, uint32_t* size, t_log* logger) {
	t_get* get = malloc(sizeof(t_get));
	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(get);
		return NULL;
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, &tamanio_pokemon, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		free(get);
		return NULL;
	}

	get->pokemon = malloc(tamanio_pokemon);

	log_info(logger, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	if (recv(socket_cliente, get->pokemon, tamanio_pokemon, MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		free(get->pokemon);
		free(get);
		return NULL;
	}

	log_info(logger, "Nombre del pokemon recibido: %s", get->pokemon);

	return get;
}

t_localized* recibir_localized(int socket_cliente, uint32_t* size, t_log* logger) {
	t_localized* localized = malloc(sizeof(t_localized));
	uint32_t tamanio_pokemon;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(localized);
		return NULL;
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	if (recv(socket_cliente, &(localized->id_correlativo), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el id correlativo");
		free(localized);
		return NULL;
	}

	log_info(logger, "Id correlativo recibido: %i", localized->id_correlativo);

	if (recv(socket_cliente, &tamanio_pokemon, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		free(localized);
		return NULL;
	}

	localized->pokemon = malloc(tamanio_pokemon);

	log_info(logger, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	if (recv(socket_cliente, localized->pokemon,tamanio_pokemon, MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el nombre del pokemon");
		free(localized->pokemon);
		free(localized);
		return NULL;
	}

	log_info(logger, "Nombre del pokemon recibido: %s", localized->pokemon);

	if (recv(socket_cliente, &(localized->lugares), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo la cantidad de coordenadas");
		free(localized->pokemon);
		free(localized);
		return NULL;
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
			return NULL;
		}

		log_info(logger, "Unidad de coordenada recibida: %i", unidad_coord);

		list_add(localized->l_coordenadas, &unidad_coord);
	}

	return localized;
}

t_suscripcion* recibir_suscripcion(int socket_cliente, uint32_t* size, t_log* logger) {
	t_suscripcion* suscripcion = malloc(sizeof(t_suscripcion*));
	uint32_t tamanio_ip;
	uint32_t tamanio_puerto;

	log_info(logger, "Recibiendo tamanio total");

	if (recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio total");
		free(suscripcion);
		return NULL;
	}

	log_info(logger, "Tamanio total recibido: %i", *size);

	log_info(logger, "Recibiendo id de cola");

	if (recv(socket_cliente, &(suscripcion->id_cola), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el id de cola");
		free(suscripcion);
		return NULL;
	}

	log_info(logger, "Id de cola recibido: %i", suscripcion->id_cola);

	log_info(logger, "Recibiendo tamanio de ip");

	if (recv(socket_cliente, &tamanio_ip, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio de ip");
		free(suscripcion);
		return NULL;
	}

	log_info(logger, "Tamanio de ip recibido: %i", tamanio_ip);

	suscripcion->ip = malloc(tamanio_ip);

	log_info(logger, "Recibiendo ip");

	if (recv(socket_cliente, suscripcion->ip, tamanio_ip, MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el ip");
		free(suscripcion->ip);
		free(suscripcion);
		return NULL;
	}

	log_info(logger, "Ip recibido: %s", suscripcion->ip);

	log_info(logger, "Recibiendo tamanio de puerto");

	if (recv(socket_cliente, &tamanio_puerto, sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio de puerto");
		free(suscripcion->ip);
		free(suscripcion);
		return NULL;
	}

	log_info(logger, "Tamanio de puerto recibido: %i", tamanio_puerto);

	suscripcion->puerto = malloc(tamanio_puerto);

	log_info(logger, "Recibiendo puerto");

	if (recv(socket_cliente, suscripcion->puerto, tamanio_puerto, MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el puerto");
		free(suscripcion->ip);
		free(suscripcion->puerto);
		free(suscripcion);
		return NULL;
	}

	log_info(logger, "Puerto recibido: %s", suscripcion->puerto);

	return suscripcion;
}

void devolver_id(int socket_cliente, uint32_t id, t_log* logger) {
	log_info(logger, "Retornando id %i al socket %i", id, socket_cliente);

	if (send(socket_cliente, (void *) id, sizeof(uint32_t), 0) == -1) {
		log_error(logger, "Hubo un problema retornando el id %i al socket %i", id, socket_cliente);
	}
}
