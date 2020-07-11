#include "team.h"

void procesar_request_de_game_boy(int cod_op, int socket_game_boy) {
	uint32_t size;
	if (cod_op == APPEARED) {
		log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un APPEARED", socket_game_boy);
		t_appeared* appeared_msg = recibir_appeared_de_game_boy(socket_game_boy, &size, extense_logger);
		t_mensaje_recibido* mensaje = malloc(sizeof(t_mensaje_recibido));

		mensaje->tipo_mensaje = MENSAJE_APPEARED;
		mensaje->mensaje = (void*) appeared_msg;

		pthread_mutex_lock(&cola_mensajes_recibidos_mutex);
		queue_push(cola_mensajes_recibidos, (void*) mensaje);
		pthread_mutex_unlock(&cola_mensajes_recibidos_mutex);
		sem_post(&sem_cola_mensajes_nuevos);
	} else {
		log_info(extense_logger, "No se recibio nada bro");
		// log cualquiera pibe, no me mandaste un appeared
	}
	close(socket_game_boy);
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
	log_info(extense_logger, "Socket de game boy aceptado");

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

void escuchar_appeared_de_broker(void) {
	uint32_t id_cola = 11;
	int status_envio = mandar_suscripcion(socket_escucha_appeared, id_cola);

	if (status_envio == -1) {
		log_error(logger, "Hubo un problema enviando la suscripcion a la cola APPEARED del broker");
		// la cagamos ñeri
	} else {

		while (1) {
			uint32_t size;
			log_info(extense_logger, "Recibiendo el Appeared");
			t_appeared* appeared_msg = recibir_appeared(socket_escucha_appeared, &size, extense_logger);
			log_info(extense_logger, "Appeared recibido");
			t_mensaje_recibido* mensaje = malloc(sizeof(t_mensaje_recibido));

			mensaje->tipo_mensaje = MENSAJE_APPEARED;
			mensaje->mensaje = (void*) appeared_msg;

			pthread_mutex_lock(&cola_mensajes_recibidos_mutex);
			queue_push(cola_mensajes_recibidos, (void*) mensaje);
			pthread_mutex_unlock(&cola_mensajes_recibidos_mutex);
			sem_post(&sem_cola_mensajes_nuevos);
		}
	}

}

void escuchar_caught_de_broker(void) {
	uint32_t id_cola = 14;
	int status_envio = mandar_suscripcion(socket_escucha_caught, id_cola);

	if (status_envio == -1) {
		log_error(logger, "Hubo un problema enviando la suscripcion a la cola CAUGHT del broker");
		// la cagamos ñeri
	}else {

		while (1) {
			uint32_t size;
			log_info(extense_logger, "Recibiendo caught");
			t_caught* caught_msg = recibir_caught(socket_escucha_caught, &size, extense_logger);
			log_info(extense_logger, "Caught recibido");
			t_mensaje_recibido* mensaje = malloc(sizeof(t_mensaje_recibido));

			mensaje->tipo_mensaje = MENSAJE_CAUGHT;
			mensaje->mensaje = (void*) caught_msg;

			pthread_mutex_lock(&cola_mensajes_recibidos_mutex);
			queue_push(cola_mensajes_recibidos, (void*) mensaje);
			pthread_mutex_unlock(&cola_mensajes_recibidos_mutex);
			sem_post(&sem_cola_mensajes_nuevos);
		}
	}

}

void escuchar_localized_de_broker(void) {
	uint32_t id_cola = 16;
	int status_envio = mandar_suscripcion(socket_escucha_localized, id_cola);

	if (status_envio == -1) {
		log_error(logger, "Hubo un problema enviando la suscripcion a la cola LOCALIZED del broker");
		// la cagamos ñeri
	}else {

		while (1) {
			uint32_t size;
			log_info(extense_logger, "Recibiendo localized");
			t_caught* localized_msg = recibir_localized(socket_escucha_localized, &size, extense_logger);
			log_info(extense_logger, "Localized recibido");
			t_mensaje_recibido* mensaje = malloc(sizeof(t_mensaje_recibido));

			mensaje->tipo_mensaje = MENSAJE_LOCALIZED;
			mensaje->mensaje = (void*) localized_msg;

			pthread_mutex_lock(&cola_mensajes_recibidos_mutex);
			queue_push(cola_mensajes_recibidos, (void*) mensaje);
			pthread_mutex_unlock(&cola_mensajes_recibidos_mutex);
			sem_post(&sem_cola_mensajes_nuevos);
		}
	}

}

void laburar(void* entrenador_param) {
	t_entrenador* entrenador = (t_entrenador*) entrenador_param;

	while (entrenador->estado != ESTADO_EXIT) {

		sem_wait(entrenador->semaforo);

		log_info(extense_logger, "Se espera el laburo nro: %i",entrenador->tarea_actual->id_tarea);

		switch(entrenador->tarea_actual->id_tarea) {
			case ATRAPAR_POKEMON: ;

			log_info(extense_logger, "Entro por Atrapar Pokemon");

			t_pokemon* parametros_atrapado = (t_pokemon*) entrenador->tarea_actual->parametros;
			log_info(extense_logger, "Pokemon a atrapar: %s", parametros_atrapado->nombre);

			// ir al lugar en cuestion
			log_info(extense_logger, "Moviendo al entrenador a la posicion X: %i y posicion Y: %i",parametros_atrapado->pos_X,parametros_atrapado->pos_Y);
			irA(parametros_atrapado->pos_X, parametros_atrapado->pos_Y, entrenador);
			log_info(extense_logger, "Llego el entrenador");

			// mandar catch NO SE REINTENTA, SI FALLA EL ENVIO LO ATRAPE RE CHETO
			log_info(extense_logger, "Se envia el catch");
			enviar_catch_a_broker(parametros_atrapado->nombre);
			log_info(extense_logger, "Se envio el catch");

			// bloquearme esperando a que el planificador me desbloquee
			entrenador->estado = ESTADO_BLOCKED; // esto requiere aparte un bloqueo en serio con semaforos, hay que ver como hacemos esto
			log_info(extense_logger, "Entrenador bloqueado");

			// ver que onda que hago con el caught

			// cosas

			break;

			case INTERCAMBIAR_POKEMON: ;

			log_info(extense_logger, "Entro por Intercambiar Pokemon");

			t_deadlock* parametros_intercambio = (t_deadlock*) entrenador->tarea_actual->parametros;
			log_info(extense_logger, "Parametros del  deadlock cargados");

			// segundo entrenador de la lista de entrenadores
			// esto requiere que el planificador mande a intercambiar al primero de la lista
			t_entrenador* otro_entrenador = list_get(parametros_intercambio->entrenadores, 1);
			log_info(extense_logger, "Cargado el entrenador con el que intercambiar");

			// ir al lugar en cuestion
			log_info(extense_logger, "Moviendo al entrenador a la posicion X: %i y posicion Y: %i",parametros_atrapado->pos_X,parametros_atrapado->pos_Y);
			irA(otro_entrenador->posX, otro_entrenador->posY, entrenador);
			log_info(extense_logger, "Llego el entrenador");


			// intercambiar el pokemon correspondiente con el otro entrenador
			log_info(extense_logger, "Empezando el intercambio de pokemon");
			intercambiar_pokemones(entrenador, otro_entrenador, (char*) list_get(parametros_intercambio->pokemones, 0), (char*) list_get(parametros_intercambio->pokemones, 1));
			log_info(extense_logger, "Intercambio realizado");

			// obtener deadlock nuevo si existe y agregarlo a la cola de mensajes del planificador

			if (parametros_intercambio->entrenadores->elements_count > 2) {
				list_remove(parametros_intercambio->entrenadores, 0); // se elimina al primer entrenador
				list_remove(parametros_intercambio->pokemones, 1); // se elimina al segundo pokemon al pedo (seria el pokemon que el primer entrenador necesita)

				t_mensaje_recibido* mensaje_deadlock = malloc(sizeof(t_mensaje_recibido));

				mensaje_deadlock->tipo_mensaje = MENSAJE_DEADLOCK;
				mensaje_deadlock->mensaje = (void*) parametros_intercambio;

				pthread_mutex_lock(&cola_mensajes_recibidos_mutex);
				queue_push(cola_mensajes_recibidos, (void*) mensaje_deadlock);
				pthread_mutex_unlock(&cola_mensajes_recibidos_mutex);
				sem_post(&sem_cola_mensajes_nuevos);
			} else {
				list_destroy(parametros_intercambio->entrenadores);
				list_destroy(parametros_intercambio->pokemones);
				free(parametros_intercambio);
			}

			break;

		default:
			log_info(extense_logger, "Entro por cualquier lado bro");
		}

	}

	pthread_exit(NULL);
}

void planificar() {
	while(1) {
		sem_wait(&sem_cola_mensajes_nuevos);
		t_mensaje_recibido* mensaje_recibido = queue_pop(cola_mensajes_recibidos);

		log_info(extense_logger, "Se espera la planificacion nro: %i",mensaje_recibido->tipo_mensaje);
		switch(mensaje_recibido->tipo_mensaje) {

			case MENSAJE_APPEARED:

				log_info(extense_logger, "Entro por Mensaje Appeared");

				t_appeared* mensaje_appeared = (t_appeared*) mensaje_recibido->mensaje;

				// requiero atraparlo?

				// planifico entrenador para ir a atraparlo
					// obtengo entrenador que va a ir
					// le doy la t_tarea
					// lo desbloqueo

				//cosas
				break;

			case MENSAJE_CAUGHT:

				log_info(extense_logger, "Entro por Mensaje Caught");

				t_caught* mensaje_caught = (t_caught*) mensaje_recibido->mensaje;

				// verificar id correlativo

				// si es afirmativo
					// dar pokemon al entrenador
					// liberar para otras tareas
				// si es negativo
					// pa ke kieres saver eso jajaj salu2

				//cosas
				break;

			case MENSAJE_LOCALIZED:

				log_info(extense_logger, "Entro por Mensaje Localized");

				t_localized* mensaje_localized = (t_localized*) mensaje_recibido->mensaje;

				// ya recibi un appeared o localized de este pokemon?

				// requiero atraparlo?

				// planifico entrenador para ir a atraparlo
					// obtengo entrenador que va a ir
					// le doy la t_tarea
					// lo desbloqueo

				//cosas
				break;

			case MENSAJE_DEADLOCK:

				log_info(extense_logger, "Entro por Mensaje Deadlock");

				t_deadlock* mensaje_deadlock = (t_deadlock*) mensaje_recibido->mensaje;

				t_tarea* tarea_deadlock = malloc(sizeof(t_tarea));

				tarea_deadlock->id_tarea = INTERCAMBIAR_POKEMON;
				tarea_deadlock->parametros = (void*) mensaje_deadlock;

				((t_entrenador*) list_get(mensaje_deadlock->entrenadores, 0))->tarea_actual = tarea_deadlock;

				// mandar la estructura t_deadlock al primer entrenador de la lista

				sem_post(((t_entrenador*) list_get(mensaje_deadlock->entrenadores, 0))->semaforo);

				//cosas
				break;


			default:
				log_info(extense_logger, "Estas planificando cualquier cosa bro, que me mandas?");
		}

	}
}

void buscar_deadlocks() {

	while(1) {
		sleep(tiempo_deadlock);

		log_info(extense_logger, "Se esta obteniendo el deadlock");
		t_deadlock* deadlock = obtener_deadlock();
		log_info(extense_logger, "Se obtuvo el deadlock");

		if (deadlock != NULL) {
			t_mensaje_recibido* mensaje = malloc(sizeof(t_mensaje_recibido));
			mensaje->tipo_mensaje = MENSAJE_DEADLOCK;
			mensaje->mensaje = (void*) deadlock;

			pthread_mutex_lock(&cola_mensajes_recibidos_mutex);
			queue_push(cola_mensajes_recibidos, (void*) mensaje);
			pthread_mutex_unlock(&cola_mensajes_recibidos_mutex);
			sem_post(&sem_cola_mensajes_nuevos);
		}

	}
}

t_deadlock* obtener_deadlock() {
	t_list* entrenadores_bloqueados = obtener_entrenadores_bloqueados();

	// encontrar deadlock
	// papa caliente
	// te encontraron macheteandote
	// la piba te esta haciendo un boquete y te esta mordiendo, no sabes como decirle

	// armar estructura t_deadlock

	return NULL;
}

t_list* obtener_entrenadores_bloqueados() {
	t_list* entrenadores_bloqueados;

	for (int i = 0; i < entrenadores->elements_count; i++) {
		t_entrenador* entrenador_de_lista = list_get(entrenadores, entrenador_de_lista);
		if (entrenador_de_lista == ESTADO_BLOCKED && entrenador_de_lista->tarea_actual->id_tarea != ATRAPAR_POKEMON) {
			list_add(entrenadores_bloqueados, entrenador_de_lista);
			log_info(logger, "entrenador %i agregado a la lista de bloqueados", i);
		}
	}

	return entrenadores_bloqueados;
}

void verificar_conexion(char *ip, char* puerto) {
	while(1) {
		if (estoy_conectado_al_broker == 0) {
			log_error(logger, "Error en conexion con BROKER, ip: %s puerto: %s, reintentando en %i segundo%s", ip_broker, puerto_broker, tiempo_reconexion, tiempo_reconexion==1 ? "" : "s");
			reconectar_al_broker();
			sleep(tiempo_reconexion);
		}
	}
}

void reconectar_al_broker() {
	int conexion_al_broker = crear_conexion(ip_broker, puerto_broker);
	if (conexion_al_broker != -1) {
		int socket_escucha_appeared = conexion_al_broker;
		int socket_escucha_caught = crear_conexion(ip_broker, puerto_broker);
		int socket_escucha_localized = crear_conexion(ip_broker, puerto_broker);
		log_info(logger, "conexion establecida con BROKER, ip: %s puerto : %s", ip_broker, puerto_broker);
		estoy_conectado_al_broker = 1;
	}
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
	ip_broker = config_get_string_value(config, "IP_BROKER");
	log_info(extense_logger, "La IP del Broker es: %s", ip_broker);
	puerto_broker = config_get_string_value(config, "PUERTO_BROKER");
	log_info(extense_logger, "El puerto del Broker es: %s", puerto_broker);
	id_modulo = config_get_int_value(config, "ID_MODULO");
	log_info(extense_logger, "El Id de Team es: %i", id_modulo);
	tiempo_deadlock = config_get_int_value(config, "TIEMPO_DEADLOCK");
	log_info(extense_logger, "La busqueda de deadlocks se corre cada %i segundos", tiempo_deadlock);
	tiempo_reconexion = config_get_int_value(config, "TIEMPO_RECONEXION");
	log_info(extense_logger, "La conexion al Broker se verifica cada %i segundos", tiempo_reconexion);
	retardo_de_CPU = config_get_int_value(config, "RETARDO_CICLO_CPU");
	log_info(extense_logger, "EL tiempo de retardo de CPU es cada %i segundos", retardo_de_CPU);

	generara_ID_Modulo();

	entrenadores = list_create();

	inicializar_entrenadores();

	obtener_objetivo_global();

	// crear cola de mensajes recibidos
	inicializar_cola();

	pthread_t* threads_entrenadores = malloc(sizeof(pthread_t) * entrenadores->elements_count);

	for (int i = 0; i < entrenadores->elements_count; i++) {
		pthread_create(&(threads_entrenadores[i]), NULL, (void*) laburar, list_get(entrenadores, i));
	}

	socket_escucha_game_boy = iniciar_escucha_game_boy(ip, puerto);
	pthread_t hilo_escucha_de_game_boy;
	pthread_create(&hilo_escucha_de_game_boy, NULL, (void*) escuchar_game_boy, (void*) socket_escucha_game_boy);
	pthread_detach(hilo_escucha_de_game_boy);

	// crear hilo planificador y tirarle detach

	pthread_t hilo_planificador;
	pthread_create(&hilo_planificador, NULL, (void*) planificar, NULL);
	pthread_detach(hilo_planificador);

	// crear hilos que se conecten al broker y que cada uno escuche una cola del broker
	// cuando obtengan algo van a tener que mandar el mensaje a la cola del team correspondiente
	socket_escucha_appeared = crear_conexion(ip_broker, puerto_broker);
	socket_escucha_caught = crear_conexion(ip_broker, puerto_broker);
	socket_escucha_localized = crear_conexion(ip_broker, puerto_broker);
	pthread_t hilo_escucha_appeared;
	pthread_t hilo_escucha_caught;
	pthread_t hilo_escucha_localized;
	pthread_create(&hilo_escucha_appeared, NULL, (void*) escuchar_appeared_de_broker, NULL);
	pthread_create(&hilo_escucha_caught, NULL, (void*) escuchar_caught_de_broker, NULL);
	pthread_create(&hilo_escucha_localized, NULL, (void*) escuchar_localized_de_broker, NULL);
	pthread_detach(hilo_escucha_appeared);
	pthread_detach(hilo_escucha_caught);
	pthread_detach(hilo_escucha_localized);

	pthread_t hilo_reconexion_broker;
	pthread_create(&hilo_reconexion_broker, NULL, (void*) verificar_conexion, NULL);
	pthread_detach(hilo_reconexion_broker);

	// crear hilo que detecta deadlocks
	pthread_t hilo_deteccion_deadlock;
	pthread_create(&hilo_deteccion_deadlock, NULL, (void*) buscar_deadlocks, NULL);
	pthread_detach(hilo_deteccion_deadlock);

	mandar_get();

	for (int i = 0; i < entrenadores->elements_count; i++) {
		pthread_join(threads_entrenadores[i], NULL);
	}

	terminar_programa();
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

		t_tarea* tarea = malloc (sizeof(t_tarea));

		tarea->id_tarea = NO_HACER_PINGO;
		tarea->parametros = NULL;

		entrenador->tarea_actual = tarea;

		definir_primer_estado(entrenador);

		sem_init(entrenador->semaforo, 0, 0);

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

void definir_primer_estado(t_entrenador* entrenador) {
	if (entrenador->pokebolas == 0) {
		if (list_size(entrenador->pokemones_innecesarios) == 0) {
			entrenador->estado = ESTADO_EXIT;
		} else {
			entrenador->estado = ESTADO_BLOCKED;
		}

	} else {
		entrenador->estado = ESTADO_NEW;
	}
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

void mandar_get() {
	obtener_pokemones_a_localizar();

	pthread_t* threads = malloc(sizeof(pthread_t) * pokemones_a_localizar->elements_count);

	for(int i= 0; i< pokemones_a_localizar->elements_count; i++){
		pthread_create(&(threads[i]), NULL, (void*) enviar_get_a_broker, list_get(pokemones_a_localizar,i));
		pthread_detach(threads[i]);
	}
}

void enviar_get_a_broker(char* nombre_pokemon) {
	uint32_t tamanio_pokemon = strlen(nombre_pokemon) + 1;

	uint32_t bytes = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + tamanio_pokemon;

	uint32_t cod_op = 5;
	void* flujo = malloc(bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &tamanio_pokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, nombre_pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;

	int socket_broker = crear_conexion(ip_broker, puerto_broker);

	int status_send = send(socket_broker, flujo, bytes, 0);

	while (status_send == -1 || status_send == 0) {
		log_error(extense_logger, "Error: No se pudo enviar el GET correspondiente al pokemon %s", nombre_pokemon);
		estoy_conectado_al_broker = 0;
		sleep(tiempo_reconexion);
		status_send = send(socket_broker, flujo, bytes, 0);
	}

	log_info(extense_logger, "Mensaje GET con el pokemon %s enviado correctamente al BROKER a traves del socket %i", nombre_pokemon, puerto_broker);

	// hacer recv del id si es que nos interesa
	// medio como que no, o no?

	close(socket_broker);
	free(flujo);
}

void enviar_catch_a_broker(t_pokemon* pokemon){

	char* nombre_pokemon = pokemon->nombre;
	uint32_t posX = pokemon->pos_X;
	uint32_t posY = pokemon->pos_Y;

	uint32_t tamanio_pokemon = strlen(nombre_pokemon)+ 1;

	uint32_t bytes = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + tamanio_pokemon + sizeof(uint32_t) + sizeof(uint32_t);

	uint32_t cod_op = 3;
	void* flujo = malloc(bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &tamanio_pokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, nombre_pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;
	memcpy(flujo + desplazamiento, posX, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, posY, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	int socket_broker = crear_conexion(ip_broker, puerto_broker);

	if (send(socket_broker, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");

		// si falla entonces lo atrape, ya fue
		// o no???




	} else {
		log_info(extense_logger, "Mensaje Catch con el pokemon %s en la posicion (%i,%i)enviado correctamente al BROKER de socket %i", nombre_pokemon, posX, posY, socket_broker);


		// guardarse el id para cuando llega un caught
	}

	close(socket_broker);
	free(flujo);

}

void irA(uint32_t posX, uint32_t posY, t_entrenador* entrenador){
	//Movimientos por ciclos de CPU

	uint32_t entrenadorX = entrenador->posX;
	uint32_t entrenadorY = entrenador->posY;

	uint32_t coordX = abs(posX - entrenador->posX);
	uint32_t coordY = abs(posX - entrenador->posY);

	for (int x = 0; x<=coordX; x++) {
		for (int y = 0; y<= coordY; y++) {
			if (posY > entrenadorY) {
				entrenador->posY ++;
				sleep(retardo_de_CPU);
			} else {
				entrenador->posY --;
				sleep(retardo_de_CPU);
			}
		}

		if (posX > entrenadorX) {
			entrenador->posX ++;
			sleep(retardo_de_CPU);
		} else {
			entrenador->posX --;
			sleep(retardo_de_CPU);
		}

	}

	log_info(extense_logger, "EL pokemon esta en (%i,%i) y el entrenador esta en (%i,%i)", posX, posY, entrenador->posX, entrenador->posY);

}

void intercambiar_pokemones(t_entrenador* entrenador1, t_entrenador* entrenador2, char* pokemon1, char* pokemon2) {
	adquirir_pokemon(entrenador1, pokemon2);
	adquirir_pokemon(entrenador2, pokemon1);

	eliminar_pokemon(entrenador1, pokemon1);
	eliminar_pokemon(entrenador2, pokemon2);
}

void eliminar_pokemon(t_entrenador* entrenador, char* pokemon) {
	for (int i = 0; i < entrenador->pokemones->elements_count; i++) {
		char* pokemon_lista = (char*) list_get(entrenador->pokemones, i);
		int es_igual = string_equals_ignore_case(pokemon, pokemon_lista);
		if (es_igual == 1) {
			list_remove(entrenador->pokemones, i);
			i = entrenador->pokemones->elements_count;
		}
	}

	for (int i = 0; i < entrenador->pokemones_innecesarios->elements_count; i++) {
		char* pokemon_lista = (char*) list_get(entrenador->pokemones_innecesarios, i);
		int es_igual = string_equals_ignore_case(pokemon, pokemon_lista);
		if (es_igual == 1) {
			list_remove(entrenador->pokemones_innecesarios, i);
			i = entrenador->pokemones_innecesarios->elements_count;
		}
	}
}

void adquirir_pokemon(t_entrenador* entrenador, char* pokemon) {
	list_add(entrenador->pokemones, (void*) pokemon);

	int presente_en_objetivo_actual = 0;

	for (int i = 0; i < entrenador->objetivos_actuales->elements_count; i++) {
		char* objetivo_actual_lista = (char*) list_get(entrenador->objetivos_actuales, i);
		int es_igual = string_equals_ignore_case(pokemon, objetivo_actual_lista);
		if (es_igual == 1) {
			list_remove(entrenador->objetivos_actuales, i);
			i = entrenador->objetivos_actuales->elements_count;
			presente_en_objetivo_actual = 1;
		}
	}

	if (presente_en_objetivo_actual == 0) {
		list_add(entrenador->pokemones_innecesarios, pokemon);
	}
}

void terminar_programa() {
	close(socket_escucha_game_boy);
	close(socket_escucha_appeared);
	close(socket_escucha_caught);
	close(socket_escucha_localized);

	// destroy stuff
}






















