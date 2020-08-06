#include "protocol_team.h"

int mandar_suscripcion(int socket_mandado, uint32_t id_cola) {
	uint32_t bytes = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);

	uint32_t cod_op = 7;
	void* flujo = malloc(bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &id_modulo_protocol, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &id_cola, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	int status_send = send(socket_mandado, flujo, bytes, 0);

	if (status_send == -1 || status_send == 0) {
		cerrar_conexion(socket_mandado);
		free(flujo);
		return -1;
	}

	free(flujo);
	return 1;
}

t_appeared* recibir_appeared(int socket_broker, uint32_t* size) {
	t_appeared* appeared = malloc(sizeof(t_appeared));
	uint32_t op_code;
	uint32_t tamanio_pokemon;
	uint32_t id;
	uint32_t id_correlativo;

	int status_recv = recv(socket_broker, &op_code, sizeof(uint32_t), MSG_WAITALL);
	log_info(extense_logger_protocol, "Recibiendo mensaje Appeared");
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el codigo de operacion");
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(appeared);
		return NULL;
	}

	if (op_code != 2) {
		log_error(extense_logger_protocol, "El codigo de operacion %i no corresponde a un mensaje Appeared", op_code);
		free(appeared);
		return NULL;
	}

	log_info(extense_logger_protocol, "Recibiendo tamanio total");

	status_recv = recv(socket_broker, size, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el tamanio total");
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(appeared);
		return NULL;
	}

	log_info(extense_logger_protocol, "Tamanio total recibido: %i", *size);

	status_recv = recv(socket_broker, &id, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el id");
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(appeared);
		return NULL;
	}

	log_info(extense_logger_protocol, "Id recibido: %i", id);

	status_recv = recv(socket_broker, &id_correlativo, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el id correlativo");
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(appeared);
		return NULL;
	}

	log_info(extense_logger_protocol, "Id correlativo recibido: %i", id_correlativo);

	status_recv = recv(socket_broker, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el tamanio del pokemon");
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket_broker %i acaba de cerrar la conexion", socket);
		free(appeared);
		return NULL;
	}

	appeared->pokemon = malloc(tamanio_pokemon);

	log_info(extense_logger_protocol, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	status_recv = recv(socket_broker, appeared->pokemon, tamanio_pokemon, MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el nombre del pokemon");
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}

	string_to_lower(appeared->pokemon);

	log_info(extense_logger_protocol, "Nombre del pokemon recibido: %s", appeared->pokemon);

	status_recv = recv(socket_broker, &(appeared->pos_X), sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo la posicion X");
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}

	log_info(extense_logger_protocol, "Posicion X recibida: %i", appeared->pos_X);

	status_recv = recv(socket_broker, &(appeared->pos_Y), sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo la posicion Y");
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket_broker %i acaba de cerrar la conexion", socket);
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}

	log_info(extense_logger_protocol, "Posicion Y recibida: %i", appeared->pos_Y);


	devolver_ack(socket_broker);

	return appeared;
}

t_caught* recibir_caught(int socket_broker, uint32_t* size) {
	t_caught* caught = malloc(sizeof(t_caught));
	uint32_t op_code;
	uint32_t id;

	int status_recv = recv(socket_broker, &op_code, sizeof(uint32_t), MSG_WAITALL);
	log_info(extense_logger_protocol, "Recibiendo mensaje Caught");
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el codigo de operacion");
		free(caught);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(caught);
		return NULL;
	}

	if (op_code != 4) {
		log_error(extense_logger_protocol, "El codigo de operacion %i no corresponde a un mensaje Caught", op_code);
		free(caught);
		return NULL;
	}

	log_info(extense_logger_protocol, "Recibiendo tamanio total");

	status_recv = recv(socket_broker, size, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el tamanio total");
		free(caught);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(caught);
		return NULL;
	}

	log_info(extense_logger_protocol, "Tamanio total recibido: %i", *size);

	status_recv = recv(socket_broker, &id, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el id");
		free(caught);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(caught);
		return NULL;
	}

	log_info(extense_logger_protocol, "Id recibido: %i", id);

	status_recv = recv(socket_broker, &(caught->idCorrelativo), sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el id correlativo");
		free(caught);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(caught);
		return NULL;
	}

	log_info(extense_logger_protocol, "Id correlativo recibido: %i", caught->idCorrelativo);

	status_recv = recv(socket_broker, &(caught->flag), sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el flag de atrapado");
		free(caught);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(caught);
		return NULL;
	}

	log_info(extense_logger_protocol, "Flag atrapado recibido: %i", caught->flag);

	devolver_ack(socket_broker);

	return caught;
}

t_localized* recibir_localized(int socket_broker, uint32_t* size) {
	t_localized* localized = malloc(sizeof(t_localized));
	uint32_t tamanio_pokemon;
	uint32_t op_code;
	uint32_t id;

	int status_recv = recv(socket_broker, &op_code, sizeof(uint32_t), MSG_WAITALL);
	log_info(extense_logger_protocol, "Recibiendo mensaje Localized");
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el codigo de operacion");
		free(localized);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(localized);
		return NULL;
	}

	if (op_code != 6) {
		log_error(extense_logger_protocol, "El codigo de operacion %i no corresponde a un mensaje Localized", op_code);
		free(localized);
		return NULL;
	}

	log_info(extense_logger_protocol, "Recibiendo tamanio total");

	status_recv = recv(socket_broker, size, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el tamanio total");
		free(localized);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(localized);
		return NULL;
	}

	log_info(extense_logger_protocol, "Tamanio total recibido: %i", *size);

	status_recv = recv(socket_broker, &id, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el id");
		free(localized);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(localized);
		return NULL;
	}

	log_info(extense_logger_protocol, "Id recibido: %i", id);

	status_recv = recv(socket_broker, &(localized->id_correlativo), sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el id correlativo");
		free(localized);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(localized);
		return NULL;
	}

	log_info(extense_logger_protocol, "Id correlativo recibido: %i", localized->id_correlativo);

	status_recv = recv(socket_broker, &tamanio_pokemon, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el tamanio del pokemon");
		free(localized);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(localized);
		return NULL;
	}

	localized->pokemon = malloc(tamanio_pokemon);

	log_info(extense_logger_protocol, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	status_recv = recv(socket_broker, localized->pokemon,tamanio_pokemon, MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el nombre del pokemon");
		free(localized->pokemon);
		free(localized);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(localized->pokemon);
		free(localized);
		return NULL;
	}

	string_to_lower(localized->pokemon);

	log_info(extense_logger_protocol, "Nombre del pokemon recibido: %s", localized->pokemon);

	status_recv = recv(socket_broker, &(localized->lugares), sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_broker);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo la cantidad de coordenadas");
		free(localized->pokemon);
		free(localized);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_broker);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		free(localized->pokemon);
		free(localized);
		return NULL;
	}

	log_info(extense_logger_protocol, "Cantidad de coordenadas recibida: %i", localized->lugares);

	localized->l_coordenadas = list_create();

	for (int i = 0; i < (localized->lugares)*2; i++) {
		uint32_t* unidad_coord = malloc(sizeof(uint32_t));

		status_recv = recv(socket_broker, unidad_coord, sizeof(uint32_t), MSG_WAITALL);
		if (status_recv == -1) {
			cerrar_conexion(socket_broker);
			log_error(extense_logger_protocol, "Hubo un problema recibiendo la unidad de coordenada");
			free(localized->pokemon);
			list_clean(localized->l_coordenadas);
			list_destroy(localized->l_coordenadas);
			free(localized);
			return NULL;
		}
		if (status_recv == 0) {
			cerrar_conexion(socket_broker);
			log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
			free(localized->pokemon);
			list_clean(localized->l_coordenadas);
			list_destroy(localized->l_coordenadas);
			free(localized);
			return NULL;
		}

		log_info(extense_logger_protocol, "Unidad de coordenada recibida: %i", *unidad_coord);

		list_add(localized->l_coordenadas, unidad_coord);
	}

	devolver_ack(socket_broker);

	return localized;
}

t_appeared* recibir_appeared_de_game_boy(int socket, uint32_t* size) {

	t_appeared* appeared = malloc(sizeof(t_appeared));

	uint32_t tamanio_pokemon;

	log_info(extense_logger_protocol, "Recibiendo tamanio total");

	int status_recv = recv(socket, size, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el tamanio total");
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket);
		free(appeared);
		return NULL;
	}

	log_info(extense_logger_protocol, "Tamanio total recibido: %i", *size);

	status_recv = recv(socket, &(tamanio_pokemon), sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el tamanio del pokemon");
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket);
		free(appeared);
		return NULL;
	}

	appeared->pokemon = malloc(tamanio_pokemon);

	log_info(extense_logger_protocol, "Tamanio del pokemon recibido: %i", tamanio_pokemon);

	status_recv = recv(socket, appeared->pokemon, tamanio_pokemon, MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el nombre del pokemon");
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket);
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}

	string_to_lower(appeared->pokemon);

	log_info(extense_logger_protocol, "Nombre del pokemon recibido: %s", appeared->pokemon);

	status_recv = recv(socket, &(appeared->pos_X), sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo la posicion X");
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket);
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}

	log_info(extense_logger_protocol, "Posicion X recibida: %i", appeared->pos_X);

	status_recv = recv(socket, &(appeared->pos_Y), sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket);
		log_error(extense_logger_protocol, "Hubo un problema recibiendo la posicion Y");
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}
	if (status_recv == 0) {
		cerrar_conexion(socket);
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket);
		free(appeared->pokemon);
		free(appeared);
		return NULL;
	}

	log_info(extense_logger_protocol, "Posicion Y recibida: %i", appeared->pos_Y);

	return appeared;
}

uint32_t recibir_ID_Catch(int socket_broker) {
	uint32_t id_Catch;

	log_info(extense_logger_protocol, "Recibiendo ID del CATCH enviado al Broker");

	int status_recv = recv(socket_broker, &id_Catch, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el id del catch");
		return 0;
	}
	if (status_recv == 0) {
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		return 0;
	}

	log_info(extense_logger_protocol, "El id recibido es: %i", id_Catch);

	return id_Catch;
}

uint32_t recibir_ID_get(int socket_broker) {
	uint32_t id_get;

	log_info(extense_logger_protocol, "Recibiendo ID del GET enviado por el Broker");

	int status_recv = recv(socket_broker, &id_get, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		log_error(extense_logger_protocol, "Hubo un problema recibiendo el id del get");
		return 0;
	}
	if (status_recv == 0) {
		log_warning(extense_logger_protocol, "El cliente con socket %i acaba de cerrar la conexion", socket_broker);
		return 0;
	}

	log_info(extense_logger_protocol, "El id recibido es: %i", id_get);

	return id_get;
}

int devolver_ack(int socket_broker) {
	uint32_t ack = 1;
	int status_send = send(socket_broker, &ack, sizeof(uint32_t), 0);
	if (status_send == -1) {
		log_error(extense_logger_protocol, "Error: no se pudo enviar el ACK al socket %i", socket_broker);
		return -1;
	}
	log_info(extense_logger_protocol, "Acknowledgement enviado al modulo Broker");
	return 1;
}
