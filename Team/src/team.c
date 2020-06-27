#include "team.h"

void procesar_request_de_game_boy(int cod_op, int socket) {
	uint32_t size;
	if (cod_op == APPEARED) {
		log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un APPEARED", socket);
		t_appeared* appeared_msg = recibir_appeared_de_game_boy(socket, &size, extense_logger);
		t_mensaje_recibido* mensaje = malloc(sizeof(t_mensaje_recibido));

		mensaje->tipo_mensaje = APPEARED;
		mensaje->mensaje = (void*) appeared_msg;

		pthread_mutex_lock(&cola_mensajes_recibidos_mutex);
		queue_push(cola_mensajes_recibidos, (void*) mensaje);
		pthread_mutex_unlock(&cola_mensajes_recibidos_mutex);
		sem_post(&sem_cola_mensajes_nuevos);
	} else {
		// log cualquiera pibe, no me mandaste un appeared
	}
	close(socket);
}

void atender_game_boy(int* socket) {
	uint32_t cod_op;
	log_info(extense_logger, "Recibiendo codigo de operacion de socket %i", *socket);
	int status_recv = recv(*socket, &cod_op, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		close(*socket);
		log_error(extense_logger, "Hubo un problema recibiendo codigo de operacion de socket %i", *socket);
		pthread_exit(NULL);
	}
	if (status_recv == 0) {
		close(*socket);
		log_warning(extense_logger, "Game boy acaba de cerrar la conexion correspondiente al socket %i", *socket);
		pthread_exit(NULL);
	}
	log_info(extense_logger, "Codigo de operacion de socket %i recibido: %i", *socket, cod_op);
	procesar_request_de_game_boy(cod_op, *socket);
}

void esperar_game_boy(int socket_escucha_game_boy) {
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_escucha_game_boy, (void*) &dir_cliente, &tam_direccion);

	pthread_t thread;

	pthread_create(&thread, NULL, (void*) atender_game_boy, &socket_cliente);
	pthread_detach(thread);
}

void escuchar_game_boy(void* socket_escucha_game_boy) {
	int socket = (int) socket_escucha_game_boy;
	log_info(extense_logger, "Esperando mensajes de game boy en socket %i", socket);
	while(1) {
		esperar_game_boy(socket);
	}
}

void laburar(void* entrenador_param) {
	t_entrenador* entrenador = (t_entrenador*) entrenador_param;

	while (entrenador->estado != ESTADO_EXIT) {

		// switch por tarea

	}

	pthread_exit(NULL);
}

void planificar() {
	// hacer cosas de planificador
}

int main(void) {
	char* ip;
	char* puerto;
	char* log_file;
	char* extense_log_file;

	// Iniciamos config, logger y variables esenciales levantadas de config

	config = leer_config();
	log_file = config_get_string_value(config, "LOG_FILE");
	extense_log_file = config_get_string_value(config, "EXTENSE_LOG_FILE");
	extense_logger = iniciar_logger(extense_log_file);
	log_info(extense_logger, "logger extenso iniciado");
	logger = iniciar_logger_sin_consola(log_file);
	log_info(extense_logger, "logger de la catedra iniciado");
	ip = config_get_string_value(config, "IP");
	log_info(extense_logger, "La IP es: %s", ip);
	puerto = config_get_string_value(config, "PUERTO");
	log_info(extense_logger, "El puerto es: %s", puerto);

	entrenadores = list_create();

	inicializar_entrenadores();

	obtener_objetivo_global();

	// crear cola de mensajes recibidos
	inicializar_cola();

	pthread_t* threads_entrenadores = malloc(sizeof(pthread_t) * entrenadores->elements_count);

	for (int i = 0; i < entrenadores->elements_count; i++) {
		pthread_create(&(threads_entrenadores[i]), NULL, (void *) laburar, list_get(entrenadores, i));
	}

	int socket_escucha_game_boy = iniciar_escucha_game_boy(ip, puerto);
	pthread_t hilo_escucha_de_game_boy;
	pthread_create(&hilo_escucha_de_game_boy, NULL, (void *) escuchar_game_boy, (void*) socket_escucha_game_boy);
	pthread_detach(hilo_escucha_de_game_boy);

	// crear hilo planificador y tirarle detach

	pthread_t hilo_planificador;
	pthread_create(&hilo_planificador, NULL, (void *) planificar, NULL);
	pthread_detach(hilo_planificador);

	// crear hilos que se conecten al broker y que cada uno escuche una cola del broker
	// cuando obtengan algo van a tener que mandar el mensaje a la cola del team correspondiente







	for (int i = 0; i < entrenadores->elements_count; i++) {
		pthread_join(threads_entrenadores[i], NULL);
	}

	terminar_programa(socket_escucha_game_boy);
	return EXIT_SUCCESS;
}

t_config* leer_config() {
	return config_create("team.config");
}

t_log* iniciar_logger(char* log_file) {
	return log_create(log_file, "Team", 1, LOG_LEVEL_INFO);
}

t_log* iniciar_logger_sin_consola(char* log_file) {
	return log_create(log_file, "Team", 0, LOG_LEVEL_INFO);
}

void inicializar_cola() {
	cola_mensajes_recibidos = queue_create();
	pthread_mutex_init(&cola_mensajes_recibidos_mutex, NULL);
	sem_init(&sem_cola_mensajes_nuevos, 0, 0);
}

void inicializar_entrenadores() {
	entrenadores = list_create();

	char** posiciones = config_get_array_value(config, "POSICIONES_ENTRENADORES");
	char** pokemones = config_get_array_value(config, "POKEMON_ENTRENADORES");
	char** objetivos = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");

	uint32_t i = 0;

	while (posiciones[i] != NULL && pokemones[i] != NULL && objetivos[i] != NULL) {

		char* objetivo_entrenador = objetivos[i];
		char* posicion = posiciones[i];
		char* conjunto_pokemon = pokemones[i];

		t_entrenador* entrenador = malloc(sizeof(t_entrenador));

		entrenador->index = i;

		t_list* objetivos = list_create();

		char* token_obj = strtok(objetivo_entrenador, "|");

		if (token_obj != NULL) {
			while(token_obj != NULL) {
				list_add(objetivos, token_obj);
				// log_info me llego este token
				token_obj = strtok(NULL, "|");
			}
		} else {
			// log info no hay nada
		}

		entrenador->objetivos = objetivos;

		t_list* pokemones = list_create();

		char* token_pok = strtok(conjunto_pokemon, "|");

		if (token_pok != NULL) {
			while(token_pok != NULL) {
				list_add(pokemones, token_pok);
				// log_info me llego este token
				token_pok = strtok(NULL, "|");
			}
		} else {
			// log info no hay nada
		}

		entrenador->pokemones = pokemones;

		char** posiciones = string_split(posicion, "|");

		entrenador->posX = (uint32_t) atoi(posiciones[0]);
		entrenador->posY = (uint32_t) atoi(posiciones[1]);


		t_list* objetivos_actuales = list_duplicate(objetivos);

		for (int k = 0; k < pokemones->elements_count; k++) {
			char* pokemon_de_lista = (char*) list_get(pokemones, k);
			int hay_que_eliminar_de_objetivo_actual = 0;
			int index_a_eliminar;
			int j = 0;
			char* objetivo_de_lista = (char*) list_get(objetivos_actuales, j);
			while (objetivo_de_lista != NULL && hay_que_eliminar_de_objetivo_actual == 0) {
				hay_que_eliminar_de_objetivo_actual = string_equals_ignore_case(objetivo_de_lista, pokemon_de_lista);
				if (hay_que_eliminar_de_objetivo_actual == 1) {
					index_a_eliminar = j;
				}
				j++;
				objetivo_de_lista = (char*) list_get(objetivos_actuales, j);
			}

			if (hay_que_eliminar_de_objetivo_actual == 1) {
				char* objetivo_actual_eliminado = (char*) list_remove(objetivos_actuales, index_a_eliminar);
				free(objetivo_actual_eliminado);
			}
		}

		entrenador->objetivos_actuales = objetivos_actuales;

		t_list* pokemones_innecesarios = list_duplicate(pokemones);

		for (int k = 0; k < objetivos->elements_count; k++) {
			char* objetivo_de_lista = (char*) list_get(pokemones, k);
			int hay_que_eliminar_de_pokemones_innecesarios = 0;
			int index_a_eliminar;
			int j = 0;
			char* pokemon_de_lista = (char*) list_get(pokemones_innecesarios, j);
			while (pokemon_de_lista != NULL && hay_que_eliminar_de_pokemones_innecesarios == 0) {
				hay_que_eliminar_de_pokemones_innecesarios = string_equals_ignore_case(objetivo_de_lista, pokemon_de_lista);
				if (hay_que_eliminar_de_pokemones_innecesarios == 1) {
					index_a_eliminar = j;
				}
				j++;
				pokemon_de_lista = (char*) list_get(pokemones_innecesarios, j);
			}

			if (hay_que_eliminar_de_pokemones_innecesarios == 1) {
				char* pokemon_innecesario_eliminado = (char*) list_remove(pokemones_innecesarios, index_a_eliminar);
				free(pokemon_innecesario_eliminado);
			}
		}

		entrenador->pokemones_innecesarios = pokemones_innecesarios;

		entrenador->pokebolas = objetivos->elements_count - pokemones->elements_count;

		entrenador->estado = ESTADO_NEW;

		t_tarea* tarea = malloc (sizeof(t_tarea));

		tarea->id_tarea = NO_HACER_PINGO;
		tarea->parametros = NULL;

		entrenador->tarea_actual = tarea;

		list_add(entrenadores, (void*) entrenador);

		free(objetivo_entrenador);
		free(posicion);
		free(conjunto_pokemon);
		free(posiciones);
		free(token_obj);
		free(token_pok);

		i++;
	}

	free(posiciones);
	free(pokemones);
	free(objetivos);
}

void obtener_objetivo_global() {
	objetivo_global = list_create();

	for(int i = 0; i < entrenadores->elements_count; i++) {
		t_entrenador* entrenador = (t_entrenador*) list_get(entrenadores, i);
		list_add_all(objetivo_global, entrenador->objetivos_actuales);
	}

	t_list* pokemones_innecesarios = list_create();

	for(int i = 0; i < entrenadores->elements_count; i++) {
		t_entrenador* entrenador = (t_entrenador*) list_get(entrenadores, i);
		list_add_all(pokemones_innecesarios, entrenador->pokemones_innecesarios);
	}

	for (int k = 0; k < pokemones_innecesarios->elements_count; k++) {
		char* pokemon_de_la_lista = (char*) list_get(pokemones_innecesarios, k);
		int hay_que_eliminar_de_objetivo_global = 0;
		int j = 0;
		char* objetivo = (char*) list_get(objetivo_global, j);
		while (objetivo != NULL && hay_que_eliminar_de_objetivo_global == 0) {
			hay_que_eliminar_de_objetivo_global = string_equals_ignore_case(objetivo, pokemon_de_la_lista);
			j++;
			objetivo = (char*) list_get(objetivo_global, j);
		}

		if (hay_que_eliminar_de_objetivo_global == 1) {
			j--;
			char* objetivo_global_eliminado = (char*) list_remove(objetivo_global, j);
			free(objetivo_global_eliminado);
		}
	}

	list_clean(pokemones_innecesarios);
	list_destroy(pokemones_innecesarios);
}

void obtener_pokemones_a_localizar() {
	pokemones_a_localizar = list_create();

	for (int i = 0; i < objetivo_global->elements_count; i++) {
		char* objetivo = (char*) list_get(objetivo_global, i);
		int hay_que_agregar = 1;
		for (int j = 0; j < pokemones_a_localizar->elements_count; j++) {
			char* pokemon = (char*) list_get(pokemones_a_localizar, j);
			int aux = string_equals_ignore_case(pokemon, objetivo);
			if (aux == 1) {
				hay_que_agregar = 0;
			}
		}
		if (hay_que_agregar == 1) {
			list_add(pokemones_a_localizar, objetivo);
		}
	}
}

void terminar_programa(int socket_game_boy) {
	close(socket_game_boy);

	// destroy stuff
}
































