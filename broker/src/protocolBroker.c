#include "protocolBroker.h"

t_new* recibir_new(int socket_cliente, int* size) {

	// nombre, posicion y cantidad
	// 2 pikachu en (5,10)
	// 'pikachu' 5 10 2
	// en la realidad:
	// 7 pikachu 5 10 2

	t_new* new = malloc(sizeof(t_new));
	uint32_t tamanio_pokemon;

	recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL); // guardamos el tamanio del mensaje (incluye
															   // el tamanio del nombre del pokemon?)

	recv(socket_cliente, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL);
	recv(socket_cliente, new->pokemon, tamanio_pokemon, MSG_WAITALL);
	recv(socket_cliente, &(new->pos_X), sizeof(uint32_t), MSG_WAITALL);
	recv(socket_cliente, &(new->pos_Y), sizeof(uint32_t), MSG_WAITALL);
	recv(socket_cliente, &(new->cantidad), sizeof(uint32_t), MSG_WAITALL);

	return new;
}

t_appeared* recibir_appeared(int socket_cliente, int* size) {

	// debemos chequear que nos mandan datos correctos?
	// FALLAR O NO FALLAR? ESA ES LA CUESTION

	t_appeared* appeared = malloc(sizeof(t_appeared));

	uint32_t tamanio_pokemon;

	recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL);

	recv(socket_cliente, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL);
	recv(socket_cliente, appeared->pokemon, tamanio_pokemon, MSG_WAITALL);
	recv(socket_cliente, &(appeared->pos_X), sizeof(uint32_t), MSG_WAITALL);
	recv(socket_cliente, &(appeared->pos_Y), sizeof(uint32_t), MSG_WAITALL);

	return appeared;
}

t_catch* recibir_catch(int socket_cliente, int* size) {
	t_catch* catch = malloc(sizeof(t_catch));
	uint32_t tamanio_pokemon;

	recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL);

	recv(socket_cliente, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL);
	recv(socket_cliente, catch->pokemon, tamanio_pokemon, MSG_WAITALL);
	recv(socket_cliente, &(catch->pos_X), sizeof(uint32_t), MSG_WAITALL);
	recv(socket_cliente, &(catch->pos_Y), sizeof(uint32_t), MSG_WAITALL);

	return catch;
}

t_caught* recibir_caught(int socket_cliente, int* size) {
	t_caught* caught = malloc(sizeof(t_caught));

	recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL);

	recv(socket_cliente, &(caught->flag_caught), sizeof(uint32_t), MSG_WAITALL);

	return caught;
}

t_get* recibir_get(int socket_cliente, int* size) {
	t_get* get = malloc(sizeof(t_get));
	uint32_t tamanio_pokemon;

	recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL);

	recv(socket_cliente, &tamanio_pokemon, sizeof(uint32_t), MSG_WAITALL);
	recv(socket_cliente, get->pokemon, *size, MSG_WAITALL);
	return get;
}

t_localized* recibir_localized(int socket_cliente, int* size) {
	t_localized* localized = malloc(sizeof(t_localized));
	uint32_t tamanio;

	recv(socket_cliente, size, sizeof(uint32_t), MSG_WAITALL);

	recv(socket_cliente, &tamanio, sizeof(uint32_t), MSG_WAITALL);
	recv(socket_cliente, localized->pokemon,tamanio, MSG_WAITALL);
	recv(socket_cliente, &(localized->lugares), sizeof(uint32_t), MSG_WAITALL);

	localized->l_coordenadas = list_create();

	for (int i = 0; i < (localized->lugares)*2; i++) {
		uint32_t unidad_coord;

		recv(socket_cliente, &unidad_coord, sizeof(uint32_t), MSG_WAITALL);

		list_add(localized->l_coordenadas, &unidad_coord);
	}

	return localized;
}


void devolver_id(int socket_cliente, uint32_t id) {
	send(socket_cliente, (void *) id, sizeof(uint32_t), 0);
}
