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

	log_info(logger, "EL ID recibido: %i", new->id);



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


	if (recv(socket_cliente, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL) == -1) {
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el tamanio del pokemon");
		free(catch);
		exit(-1);
	}


	if (recv(socket_cliente, catch->id , sizeof(uint32_t), MSG_WAITALL) == -1) { // lee id del mensaje ///
		close(socket_cliente);
		log_error(logger, "Hubo un problema recibiendo el ID del mensaje");
		free(catch);
		exit(-1);
	}

	log_info(logger, "EL ID recibido: %i", catch->id);

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

	log_info(logger, "EL ID recibido: %i", get->id);

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
