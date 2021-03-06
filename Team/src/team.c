#include "team.h"

void procesar_request_de_game_boy(int cod_op, int socket_game_boy) {
	log_info(extense_logger, "procesando request de Game boy");
	uint32_t size;
	if (cod_op == APPEARED) {
		log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un APPEARED", socket_game_boy);
		t_appeared* appeared_msg = recibir_appeared_de_game_boy(socket_game_boy, &size);
		if (appeared_msg != NULL) {
			log_info(extense_logger, "Appeared recibido de GAME BOY, pokemon %s en (%i,%i)", appeared_msg->pokemon, appeared_msg->pos_X, appeared_msg->pos_Y);
			log_info(logger, "Appeared recibido de GAME BOY, pokemon %s en (%i,%i)", appeared_msg->pokemon, appeared_msg->pos_X, appeared_msg->pos_Y);
			if (es_pokemon_global(appeared_msg->pokemon)) {
				t_pokemon* pokemon_a_agregar = generar_pokemon_de_appeared(appeared_msg);
				agrego_pokemon_a_dictionary(pokemon_a_agregar);
				if (pokemon_ya_fue_recibido(appeared_msg->pokemon) == 0) {
					log_info(extense_logger, "Pokemon %s llego por primera vez", appeared_msg->pokemon);
					char* pokemon_nuevo = string_duplicate(appeared_msg->pokemon);
					pthread_mutex_lock(&pokemones_llegados_mutex);
					list_add(pokemones_llegados, pokemon_nuevo);
					pthread_mutex_unlock(&pokemones_llegados_mutex);
				}
				pthread_mutex_lock(&cola_pokemones_mutex);
				queue_push(cola_pokemones, (void*) pokemon_a_agregar);
				pthread_mutex_unlock(&cola_pokemones_mutex);
				sem_post(&sem_cola_pokemones);
			} else {
				log_info(extense_logger, "El pokemon recibido %s no forma parte del objetivo global, se desestima el mensaje", appeared_msg->pokemon);
				free(appeared_msg->pokemon);
			}
			free(appeared_msg);
		}
	} else {
		log_info(extense_logger, "Mensaje recibido de Game boy a traves del socket %i no corresponde a un APPEARED", socket_game_boy);
	}
	log_info(extense_logger, "Mensaje de Game boy procesado");
	cerrar_conexion(socket_game_boy);
}

void atender_game_boy(int* socket_game_boy) {
	int socket_cliente = *socket_game_boy;
	uint32_t cod_op;
	log_info(extense_logger, "Recibiendo codigo de operacion de Game boy en socket %i", socket_cliente);
	int status_recv = recv(socket_cliente, &cod_op, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		cerrar_conexion(socket_cliente);
		log_error(extense_logger, "Hubo un problema recibiendo codigo de operacion de Game boy en socket %i", socket_cliente);
		pthread_exit(NULL);
	}
	if (status_recv == 0) {
		cerrar_conexion(socket_cliente);
		log_warning(extense_logger, "Game boy acaba de cerrar la conexion correspondiente al socket %i", socket_cliente);
		pthread_exit(NULL);
	}
	log_info(extense_logger, "Codigo de operacion de socket %i recibido: %i", socket_cliente, cod_op);
	procesar_request_de_game_boy(cod_op, socket_cliente);
	free(socket_game_boy);
}

void esperar_game_boy(int socket_escucha_game_boy) {
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_escucha_game_boy, (void*) &dir_cliente, &tam_direccion);
	log_info(extense_logger, "Socket %i de Game boy aceptado", socket_escucha_game_boy);

	if(socket_cliente != -1) {
		int* socket_final = malloc(sizeof(int));
		*socket_final = socket_cliente;

		pthread_t thread;

		pthread_create(&thread, NULL, (void*) atender_game_boy, socket_final);
		pthread_detach(thread);
	}
}

void escuchar_game_boy(void* socket_escucha_game_boy) {
	int socket = (int) socket_escucha_game_boy;
	log_info(extense_logger, "Esperando mensajes de game boy en socket %i", socket);
	while(1) {
		esperar_game_boy(socket);
	}
}

void escuchar_appeared_de_broker(void) {
	uint32_t id_cola = 12;

	log_info(extense_logger, "Escucho appeared de BROKER");

	int status_envio = mandar_suscripcion(socket_escucha_appeared, id_cola);

	if (status_envio == -1) {
		pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
		estoy_conectado_al_broker = 0;
		pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
		log_error(logger, "Hubo un problema enviando la suscripcion a la cola APPEARED del broker");
	}
	while (1) {
		sem_wait(&sem_conexion_appeared);
		if (estoy_conectado_al_broker == 1) {
			uint32_t size;
			t_appeared* appeared_msg = recibir_appeared(socket_escucha_appeared, &size);
			if (appeared_msg == NULL) {
				desconectar_broker();
			} else {
				log_info(extense_logger, "Appeared recibido de BROKER, pokemon %s en (%i,%i)", appeared_msg->pokemon, appeared_msg->pos_X, appeared_msg->pos_Y);
				log_info(logger, "Appeared recibido de BROKER, pokemon %s en (%i,%i)", appeared_msg->pokemon, appeared_msg->pos_X, appeared_msg->pos_Y);

				if(es_pokemon_global(appeared_msg->pokemon)) {
					log_info(extense_logger, "pokemon %s esta en el objetivo global", appeared_msg->pokemon);

					t_pokemon* pokemon_a_agregar = generar_pokemon_de_appeared(appeared_msg);

					agrego_pokemon_a_dictionary(pokemon_a_agregar);

					if (pokemon_ya_fue_recibido(appeared_msg->pokemon) == 0) {
						log_info(extense_logger, "Pokemon %s llego por primera vez", appeared_msg->pokemon);
						char* pokemon_nuevo = string_duplicate(appeared_msg->pokemon);
						pthread_mutex_lock(&pokemones_llegados_mutex);
						list_add(pokemones_llegados, pokemon_nuevo);
						pthread_mutex_unlock(&pokemones_llegados_mutex);
					}

					pthread_mutex_lock(&cola_pokemones_mutex);
					queue_push(cola_pokemones, (void*) pokemon_a_agregar);
					pthread_mutex_unlock(&cola_pokemones_mutex);
					sem_post(&sem_cola_pokemones);
				} else {
					log_info(extense_logger, "El pokemon recibido %s no forma parte del objetivo global, se desestima el mensaje", appeared_msg->pokemon);
					free(appeared_msg->pokemon);
				}
				free(appeared_msg);
			}
		}
		sem_post(&sem_conexion_appeared);
	}
}

void escuchar_caught_de_broker(void) {
	uint32_t id_cola = 14;

	log_info(extense_logger, "Escucho caught de BROKER");

	int status_envio = mandar_suscripcion(socket_escucha_caught, id_cola);

	if (status_envio == -1) {
		pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
		estoy_conectado_al_broker = 0;
		pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
		log_error(logger, "Hubo un problema enviando la suscripcion a la cola CAUGHT del broker");
	}
	while (1) {
		sem_wait(&sem_conexion_caught);
		if (estoy_conectado_al_broker == 1) {
			uint32_t size;
			t_caught* caught_msg = recibir_caught(socket_escucha_caught, &size);

			if (caught_msg == NULL) {
				desconectar_broker();
			} else {
				log_info(extense_logger, "Caught recibido de BROKER con id correlativo %i y flag %i", caught_msg->idCorrelativo, caught_msg->flag);
				log_info(logger, "Caught recibido de BROKER con id correlativo %i y flag %i", caught_msg->idCorrelativo, caught_msg->flag);
				if (es_id_catch(caught_msg->idCorrelativo) == 1) {
					pthread_mutex_lock(&cola_caught_mutex);
					queue_push(cola_caught, (void*) caught_msg);
					pthread_mutex_unlock(&cola_caught_mutex);
					sem_post(&sem_cola_caught);
				} else {
					free(caught_msg);
				}
			}
		}
		sem_post(&sem_conexion_caught);
	}
}

void escuchar_localized_de_broker(void) {
	uint32_t id_cola = 16;

	log_info(extense_logger, "Escucho localized de BROKER");

	int status_envio = mandar_suscripcion(socket_escucha_localized, id_cola);

	if (status_envio == -1) {
		pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
		estoy_conectado_al_broker = 0;
		pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
		log_error(logger, "Hubo un problema enviando la suscripcion a la cola LOCALIZED del broker");
	}
	while (1) {
		sem_wait(&sem_conexion_localized);
		if (estoy_conectado_al_broker == 1) {
			uint32_t size;
			t_localized* localized_msg = recibir_localized(socket_escucha_localized, &size);
			if (localized_msg == NULL) {
				desconectar_broker();
			} else {
				char* coordenadas_log = string_new();
				for (int k = 0; k < localized_msg->l_coordenadas->elements_count; k++) {
					uint32_t* coord = list_get(localized_msg->l_coordenadas, k);
					char* coord_str = string_itoa(*coord);
					string_append(&coordenadas_log, " ");
					string_append(&coordenadas_log, coord_str);
					free(coord_str);
				}

				log_info(extense_logger, "Localized recibido de BROKER, pokemon %s en coordenadas%s", localized_msg->pokemon, coordenadas_log);
				log_info(logger, "Localized recibido de BROKER, pokemon %s en coordenadas%s", localized_msg->pokemon, coordenadas_log);

				free(coordenadas_log);

				if (pokemon_ya_fue_recibido(localized_msg->pokemon) == 0) {
					log_info(extense_logger, "pokemon %s llego por primera vez",localized_msg->pokemon);
					char* pokemon_nuevo = string_duplicate(localized_msg->pokemon);
					pthread_mutex_lock(&pokemones_llegados_mutex);
					list_add(pokemones_llegados, (void*) pokemon_nuevo);
					pthread_mutex_unlock(&pokemones_llegados_mutex);
					t_list* pokemones = generar_pokemones_de_localized(localized_msg);

					for (int i = 0; i < pokemones->elements_count; i++) {
						pthread_mutex_lock(&cola_pokemones_mutex);
						queue_push(cola_pokemones, (void*) list_get(pokemones, i));
						pthread_mutex_unlock(&cola_pokemones_mutex);
						sem_post(&sem_cola_pokemones);
					}

					list_destroy(pokemones);
				}
				free(localized_msg->pokemon);
				for (int i = 0; i < localized_msg->l_coordenadas->elements_count; i++) {
					free(list_get(localized_msg->l_coordenadas, i));
				}
				list_destroy(localized_msg->l_coordenadas);
				free(localized_msg);
			}
		}
		sem_post(&sem_conexion_localized);
	}
}

int pokemon_ya_fue_recibido(char* pokemon) {
	int ya_fue_recibido = 0;
	pthread_mutex_lock(&pokemones_llegados_mutex);
	for (int i = 0; i < pokemones_llegados->elements_count; i++) {
		char* pokemon_lista = (char*) list_get(pokemones_llegados, i);
		ya_fue_recibido = string_equals_ignore_case(pokemon, pokemon_lista);
		if (ya_fue_recibido == 1) {
			i = pokemones_llegados->elements_count;
		}
	}
	pthread_mutex_unlock(&pokemones_llegados_mutex);
	return ya_fue_recibido;
}

void laburar_fifo(void* entrenador_param) {
	t_entrenador* entrenador = (t_entrenador*) entrenador_param;

	log_info(extense_logger, "Trabajo de entrenador %i comenzado", entrenador->index);

	while (entrenador->estado != ESTADO_EXIT) {

		sem_wait(&(entrenador->semaforo));
		switch(entrenador->tarea_actual->id_tarea) {
		case ATRAPAR_POKEMON: ;

		pthread_mutex_lock(&planificacion_ready);

		char* mensaje_log = string_new();
		string_append(&mensaje_log, "Ha comenzado a ejecutar la tarea de atrapar pokemon ");
		string_append(&mensaje_log, ((t_pokemon*) entrenador->tarea_actual->parametros)->nombre);

		cambiar_estado_de_entrenador(entrenador, ESTADO_EXECUTING, mensaje_log);
		t_pokemon* parametros_atrapado = (t_pokemon*) entrenador->tarea_actual->parametros;
		log_info(extense_logger, "Entrenador %i esta atrapando pokemon %s ubicado en (%i,%i)",entrenador->index, parametros_atrapado->nombre, parametros_atrapado->pos_X, parametros_atrapado->pos_Y);
		log_info(logger, "Entrenador %i esta atrapando pokemon %s ubicado en (%i,%i)",entrenador->index, parametros_atrapado->nombre, parametros_atrapado->pos_X, parametros_atrapado->pos_Y);
		free(mensaje_log);

		irA(parametros_atrapado->pos_X, parametros_atrapado->pos_Y, entrenador);

		cambiar_estado_de_entrenador(entrenador, ESTADO_BLOCKED, "Se ha enviado un catch");

		enviar_catch_a_broker(parametros_atrapado, entrenador);

		pthread_mutex_unlock(&planificacion_ready);

		break;

		case INTERCAMBIAR_POKEMON: ;

		pthread_mutex_lock(&planificacion_ready);

		log_info(extense_logger, "Entrenador %i entro por Intercambiar Pokemon", entrenador->index);

		cambiar_estado_de_entrenador(entrenador, ESTADO_EXECUTING, "Ha comenzado a ejecutar la tarea de intercambiar pokemon");
		log_info(extense_logger, "entrenador %i cambia su estado a EXECUTING",entrenador->index);
		t_deadlock* parametros_intercambio = (t_deadlock*) entrenador->tarea_actual->parametros;
		log_info(extense_logger, "Parametros del deadlock cargados");

		t_entrenador* otro_entrenador = (t_entrenador*) list_get(parametros_intercambio->entrenadores, 1);

		irA(otro_entrenador->posX, otro_entrenador->posY, entrenador);

		char* pokemon1 = (char*) list_get(parametros_intercambio->pokemones, 0);
		char* pokemon2 = (char*) list_get(parametros_intercambio->pokemones, 1);

		log_info(extense_logger, "Empezando el intercambio de pokemon %s y pokemon %s entre entrenador %i y entrenador %i", pokemon1, pokemon2, entrenador->index, otro_entrenador->index);
		intercambiar_pokemones(entrenador, otro_entrenador, pokemon1, pokemon2);
		log_info(extense_logger, "Intercambio realizado");

		if (parametros_intercambio->entrenadores->elements_count > 2) {
			list_remove(parametros_intercambio->entrenadores, 0); // se elimina al primer entrenador
			list_remove(parametros_intercambio->pokemones, 1); // se elimina al segundo pokemon al pedo (seria el pokemon que el primer entrenador necesita)

			pthread_mutex_lock(&cola_deadlock_mutex);
			queue_push(cola_deadlock, (void*) parametros_intercambio);
			pthread_mutex_unlock(&cola_deadlock_mutex);
			sem_post(&sem_cola_deadlock);
		} else {
			pthread_mutex_lock(&cantidad_deadlocks_mutex);
			cantidad_deadlocks++;
			pthread_mutex_unlock(&cantidad_deadlocks_mutex);
			if (otro_entrenador->pokemones_innecesarios->elements_count == 0) {
				cambiar_estado_de_entrenador(otro_entrenador, ESTADO_EXIT, "Ha terminado su objetivo");
				t_tarea* otra_tarea_pingo = malloc(sizeof(t_tarea));
				otra_tarea_pingo->id_tarea = NO_HACER_PINGO;
				cambiar_tarea_de_entrenador(otro_entrenador, otra_tarea_pingo);
				sem_post(&(otro_entrenador->semaforo));
			} else {
				t_tarea* otra_tarea_pingo = malloc(sizeof(t_tarea));
				otra_tarea_pingo->id_tarea = NO_HACER_PINGO;
				cambiar_tarea_de_entrenador(otro_entrenador, otra_tarea_pingo);
				list_destroy(parametros_intercambio->entrenadores);
				list_destroy(parametros_intercambio->pokemones);
				free(parametros_intercambio);
			}
		}

		if (entrenador->pokemones_innecesarios->elements_count == 0) {
			cambiar_estado_de_entrenador(entrenador, ESTADO_EXIT, "Ha terminado su objetivo");
			t_tarea* tarea_pingo = malloc(sizeof(t_tarea));
			tarea_pingo->id_tarea = NO_HACER_PINGO;
			cambiar_tarea_de_entrenador(entrenador, tarea_pingo);
			sem_post(&(entrenador->semaforo));
		} else {
			t_tarea* tarea_pingo = malloc(sizeof(t_tarea));
			tarea_pingo->id_tarea = NO_HACER_PINGO;
			cambiar_tarea_de_entrenador(entrenador, tarea_pingo);
			cambiar_estado_de_entrenador(entrenador, ESTADO_BLOCKED, "Se ha terminado de intercambiar pokemones y sigue sin cumplirse el objetivo");
			log_info(extense_logger, "entrenador %i cambia su estado a BLOQUEADO",entrenador->index);
		}

		pthread_mutex_unlock(&planificacion_ready);

		break;

		case NO_HACER_PINGO:
			break;
		default:
			log_info(extense_logger, "Entro por cualquier lado bro");
		}

	}

	log_info(extense_logger, "Entrenador %i ha terminado", entrenador->index);

	pthread_exit(NULL);
}

void laburar_rr(void* entrenador_param) {
	t_entrenador* entrenador = (t_entrenador*) entrenador_param;

	log_info(extense_logger, "Trabajo de entrenador %i comenzado", entrenador->index);

	while (entrenador->estado != ESTADO_EXIT) {

		sem_wait(&(entrenador->semaforo));
		switch(entrenador->tarea_actual->id_tarea) {
		case ATRAPAR_POKEMON: ;

		pthread_mutex_lock(&planificacion_ready);

		entrenador->contador_quantum = quantum;

		char* mensaje_log = string_new();
		string_append(&mensaje_log, "Ha comenzado a ejecutar la tarea de atrapar pokemon ");
		string_append(&mensaje_log, ((t_pokemon*) entrenador->tarea_actual->parametros)->nombre);

		cambiar_estado_de_entrenador(entrenador, ESTADO_EXECUTING, mensaje_log);
		t_pokemon* parametros_atrapado = (t_pokemon*) entrenador->tarea_actual->parametros;
		log_info(extense_logger, "Entrenador %i esta atrapando pokemon %s ubicado en (%i,%i)",entrenador->index, parametros_atrapado->nombre, parametros_atrapado->pos_X, parametros_atrapado->pos_Y);
		log_info(logger, "Entrenador %i esta atrapando pokemon %s ubicado en (%i,%i)",entrenador->index, parametros_atrapado->nombre, parametros_atrapado->pos_X, parametros_atrapado->pos_Y);
		free(mensaje_log);

		irA(parametros_atrapado->pos_X, parametros_atrapado->pos_Y, entrenador);

		cambiar_estado_de_entrenador(entrenador, ESTADO_BLOCKED, "Se ha enviado un catch");

		enviar_catch_a_broker(parametros_atrapado, entrenador);

		pthread_mutex_unlock(&planificacion_ready);

		entrenador->contador_quantum = quantum;

		break;

		case INTERCAMBIAR_POKEMON: ;

		pthread_mutex_lock(&planificacion_ready);

		entrenador->contador_quantum = quantum;

		log_info(extense_logger, "Entrenador %i entro por Intercambiar Pokemon", entrenador->index);

		cambiar_estado_de_entrenador(entrenador, ESTADO_EXECUTING, "Ha comenzado a ejecutar la tarea de intercambiar pokemon");
		log_info(extense_logger, "entrenador %i cambia su estado a EXECUTING",entrenador->index);
		t_deadlock* parametros_intercambio = (t_deadlock*) entrenador->tarea_actual->parametros;
		log_info(extense_logger, "Parametros del deadlock cargados");

		t_entrenador* otro_entrenador = (t_entrenador*) list_get(parametros_intercambio->entrenadores, 1);

		irA(otro_entrenador->posX, otro_entrenador->posY, entrenador);

		char* pokemon1 = (char*) list_get(parametros_intercambio->pokemones, 0);
		char* pokemon2 = (char*) list_get(parametros_intercambio->pokemones, 1);

		log_info(extense_logger, "Empezando el intercambio de pokemon %s y pokemon %s entre entrenador %i y entrenador %i", pokemon1, pokemon2, entrenador->index, otro_entrenador->index);
		intercambiar_pokemones(entrenador, otro_entrenador, pokemon1, pokemon2);
		log_info(extense_logger, "Intercambio realizado");

		if (parametros_intercambio->entrenadores->elements_count > 2) {
			list_remove(parametros_intercambio->entrenadores, 0); // se elimina al primer entrenador
			list_remove(parametros_intercambio->pokemones, 1); // se elimina al segundo pokemon al pedo (seria el pokemon que el primer entrenador necesita)

			pthread_mutex_lock(&cola_deadlock_mutex);
			queue_push(cola_deadlock, (void*) parametros_intercambio);
			pthread_mutex_unlock(&cola_deadlock_mutex);
			sem_post(&sem_cola_deadlock);
		} else {
			pthread_mutex_lock(&cantidad_deadlocks_mutex);
			cantidad_deadlocks++;
			pthread_mutex_unlock(&cantidad_deadlocks_mutex);
			if (otro_entrenador->pokemones_innecesarios->elements_count == 0) {
				cambiar_estado_de_entrenador(otro_entrenador, ESTADO_EXIT, "Ha terminado su objetivo");
				t_tarea* otra_tarea_pingo = malloc(sizeof(t_tarea));
				otra_tarea_pingo->id_tarea = NO_HACER_PINGO;
				cambiar_tarea_de_entrenador(otro_entrenador, otra_tarea_pingo);
				sem_post(&(otro_entrenador->semaforo));
			} else {
				t_tarea* otra_tarea_pingo = malloc(sizeof(t_tarea));
				otra_tarea_pingo->id_tarea = NO_HACER_PINGO;
				cambiar_tarea_de_entrenador(otro_entrenador, otra_tarea_pingo);
				list_destroy(parametros_intercambio->entrenadores);
				list_destroy(parametros_intercambio->pokemones);
				free(parametros_intercambio);
			}
		}

		if (entrenador->pokemones_innecesarios->elements_count == 0) {
			cambiar_estado_de_entrenador(entrenador, ESTADO_EXIT, "Ha terminado su objetivo");
			t_tarea* tarea_pingo = malloc(sizeof(t_tarea));
			tarea_pingo->id_tarea = NO_HACER_PINGO;
			cambiar_tarea_de_entrenador(entrenador, tarea_pingo);
			sem_post(&(entrenador->semaforo));
		} else {
			t_tarea* tarea_pingo = malloc(sizeof(t_tarea));
			tarea_pingo->id_tarea = NO_HACER_PINGO;
			cambiar_tarea_de_entrenador(entrenador, tarea_pingo);
			cambiar_estado_de_entrenador(entrenador, ESTADO_BLOCKED, "Se ha terminado de intercambiar pokemones y sigue sin cumplirse el objetivo");
			log_info(extense_logger, "entrenador %i cambia su estado a BLOQUEADO",entrenador->index);
		}

		pthread_mutex_unlock(&planificacion_ready);

		entrenador->contador_quantum = quantum;

		break;

		case NO_HACER_PINGO:
			break;
		default:
			log_info(extense_logger, "Entro por cualquier lado bro");
		}

	}

	log_info(extense_logger, "Entrenador %i ha terminado", entrenador->index);

	pthread_exit(NULL);
}

void laburar_sjf_sd(void* entrenador_param) {
	t_entrenador* entrenador = (t_entrenador*) entrenador_param;

	log_info(extense_logger, "Trabajo de entrenador %i comenzado", entrenador->index);

	while (entrenador->estado != ESTADO_EXIT) {

		sem_wait(&(entrenador->semaforo));
		switch(entrenador->tarea_actual->id_tarea) {
		case ATRAPAR_POKEMON: ;

		char* mensaje_log = string_new();
		string_append(&mensaje_log, "Ha comenzado a ejecutar la tarea de atrapar pokemon ");
		string_append(&mensaje_log, ((t_pokemon*) entrenador->tarea_actual->parametros)->nombre);

		cambiar_estado_de_entrenador(entrenador, ESTADO_EXECUTING, mensaje_log);
		t_pokemon* parametros_atrapado = (t_pokemon*) entrenador->tarea_actual->parametros;
		log_info(extense_logger, "Entrenador %i esta atrapando pokemon %s ubicado en (%i,%i)",entrenador->index, parametros_atrapado->nombre, parametros_atrapado->pos_X, parametros_atrapado->pos_Y);
		log_info(logger, "Entrenador %i esta atrapando pokemon %s ubicado en (%i,%i)",entrenador->index, parametros_atrapado->nombre, parametros_atrapado->pos_X, parametros_atrapado->pos_Y);
		free(mensaje_log);

		irA(parametros_atrapado->pos_X, parametros_atrapado->pos_Y, entrenador);

		cambiar_estado_de_entrenador(entrenador, ESTADO_BLOCKED, "Se ha enviado un catch");

		enviar_catch_a_broker(parametros_atrapado, entrenador);

		definir_nueva_estimacion(entrenador);

		mandar_a_siguente_entrenador_a_ejecutar_por_sjf();

		break;

		case INTERCAMBIAR_POKEMON: ;

		log_info(extense_logger, "Entrenador %i entro por Intercambiar Pokemon", entrenador->index);

		cambiar_estado_de_entrenador(entrenador, ESTADO_EXECUTING, "Ha comenzado a ejecutar la tarea de intercambiar pokemon");
		log_info(extense_logger, "entrenador %i cambia su estado a EXECUTING",entrenador->index);
		t_deadlock* parametros_intercambio = (t_deadlock*) entrenador->tarea_actual->parametros;
		log_info(extense_logger, "Parametros del deadlock cargados");

		t_entrenador* otro_entrenador = (t_entrenador*) list_get(parametros_intercambio->entrenadores, 1);

		irA(otro_entrenador->posX, otro_entrenador->posY, entrenador);

		char* pokemon1 = (char*) list_get(parametros_intercambio->pokemones, 0);
		char* pokemon2 = (char*) list_get(parametros_intercambio->pokemones, 1);

		log_info(extense_logger, "Empezando el intercambio de pokemon %s y pokemon %s entre entrenador %i y entrenador %i", pokemon1, pokemon2, entrenador->index, otro_entrenador->index);
		intercambiar_pokemones(entrenador, otro_entrenador, pokemon1, pokemon2);
		log_info(extense_logger, "Intercambio realizado");

		if (parametros_intercambio->entrenadores->elements_count > 2) {
			list_remove(parametros_intercambio->entrenadores, 0); // se elimina al primer entrenador
			list_remove(parametros_intercambio->pokemones, 1); // se elimina al segundo pokemon al pedo (seria el pokemon que el primer entrenador necesita)

			pthread_mutex_lock(&cola_deadlock_mutex);
			queue_push(cola_deadlock, (void*) parametros_intercambio);
			pthread_mutex_unlock(&cola_deadlock_mutex);
			sem_post(&sem_cola_deadlock);
		} else {
			pthread_mutex_lock(&cantidad_deadlocks_mutex);
			cantidad_deadlocks++;
			pthread_mutex_unlock(&cantidad_deadlocks_mutex);
			if (otro_entrenador->pokemones_innecesarios->elements_count == 0) {
				cambiar_estado_de_entrenador(otro_entrenador, ESTADO_EXIT, "Ha terminado su objetivo");
				t_tarea* otra_tarea_pingo = malloc(sizeof(t_tarea));
				otra_tarea_pingo->id_tarea = NO_HACER_PINGO;
				cambiar_tarea_de_entrenador(otro_entrenador, otra_tarea_pingo);
				sem_post(&(otro_entrenador->semaforo));
			} else {
				t_tarea* otra_tarea_pingo = malloc(sizeof(t_tarea));
				otra_tarea_pingo->id_tarea = NO_HACER_PINGO;
				cambiar_tarea_de_entrenador(otro_entrenador, otra_tarea_pingo);
				list_destroy(parametros_intercambio->entrenadores);
				list_destroy(parametros_intercambio->pokemones);
				free(parametros_intercambio);
			}
		}

		if (entrenador->pokemones_innecesarios->elements_count == 0) {
			cambiar_estado_de_entrenador(entrenador, ESTADO_EXIT, "Ha terminado su objetivo");
			t_tarea* tarea_pingo = malloc(sizeof(t_tarea));
			tarea_pingo->id_tarea = NO_HACER_PINGO;
			cambiar_tarea_de_entrenador(entrenador, tarea_pingo);
			sem_post(&(entrenador->semaforo));
		} else {
			t_tarea* tarea_pingo = malloc(sizeof(t_tarea));
			tarea_pingo->id_tarea = NO_HACER_PINGO;
			cambiar_tarea_de_entrenador(entrenador, tarea_pingo);
			cambiar_estado_de_entrenador(entrenador, ESTADO_BLOCKED, "Se ha terminado de intercambiar pokemones y sigue sin cumplirse el objetivo");
			log_info(extense_logger, "entrenador %i cambia su estado a BLOQUEADO",entrenador->index);
		}

		definir_nueva_estimacion(entrenador);

		mandar_a_siguente_entrenador_a_ejecutar_por_sjf();

		break;

		case NO_HACER_PINGO:
			break;
		default:
			log_info(extense_logger, "Entro por cualquier lado bro");
		}

	}

	log_info(extense_logger, "Entrenador %i ha terminado", entrenador->index);

	pthread_exit(NULL);
}

void laburar_sjf_cd(void* entrenador_param) {
	t_entrenador* entrenador = (t_entrenador*) entrenador_param;

	log_info(extense_logger, "Trabajo de entrenador %i comenzado", entrenador->index);

	while (entrenador->estado != ESTADO_EXIT) {

		sem_wait(&(entrenador->semaforo));
		switch(entrenador->tarea_actual->id_tarea) {
		case ATRAPAR_POKEMON: ;

		char* mensaje_log = string_new();
		string_append(&mensaje_log, "Ha comenzado a ejecutar la tarea de atrapar pokemon ");
		string_append(&mensaje_log, ((t_pokemon*) entrenador->tarea_actual->parametros)->nombre);

		cambiar_estado_de_entrenador(entrenador, ESTADO_EXECUTING, mensaje_log);
		t_pokemon* parametros_atrapado = (t_pokemon*) entrenador->tarea_actual->parametros;
		log_info(extense_logger, "Entrenador %i esta atrapando pokemon %s ubicado en (%i,%i)",entrenador->index, parametros_atrapado->nombre, parametros_atrapado->pos_X, parametros_atrapado->pos_Y);
		log_info(logger, "Entrenador %i esta atrapando pokemon %s ubicado en (%i,%i)",entrenador->index, parametros_atrapado->nombre, parametros_atrapado->pos_X, parametros_atrapado->pos_Y);
		free(mensaje_log);

		irA(parametros_atrapado->pos_X, parametros_atrapado->pos_Y, entrenador);

		cambiar_estado_de_entrenador(entrenador, ESTADO_BLOCKED, "Se ha enviado un catch");

		enviar_catch_a_broker(parametros_atrapado, entrenador);

		definir_nueva_estimacion(entrenador);

		mandar_a_siguente_entrenador_a_ejecutar_por_sjf();

		break;

		case INTERCAMBIAR_POKEMON: ;

		log_info(extense_logger, "Entrenador %i entro por Intercambiar Pokemon", entrenador->index);

		cambiar_estado_de_entrenador(entrenador, ESTADO_EXECUTING, "Ha comenzado a ejecutar la tarea de intercambiar pokemon");
		log_info(extense_logger, "entrenador %i cambia su estado a EXECUTING",entrenador->index);
		t_deadlock* parametros_intercambio = (t_deadlock*) entrenador->tarea_actual->parametros;
		log_info(extense_logger, "Parametros del deadlock cargados");

		t_entrenador* otro_entrenador = (t_entrenador*) list_get(parametros_intercambio->entrenadores, 1);

		irA(otro_entrenador->posX, otro_entrenador->posY, entrenador);

		char* pokemon1 = (char*) list_get(parametros_intercambio->pokemones, 0);
		char* pokemon2 = (char*) list_get(parametros_intercambio->pokemones, 1);

		log_info(extense_logger, "Empezando el intercambio de pokemon %s y pokemon %s entre entrenador %i y entrenador %i", pokemon1, pokemon2, entrenador->index, otro_entrenador->index);
		intercambiar_pokemones(entrenador, otro_entrenador, pokemon1, pokemon2);
		log_info(extense_logger, "Intercambio realizado");

		if (parametros_intercambio->entrenadores->elements_count > 2) {
			list_remove(parametros_intercambio->entrenadores, 0); // se elimina al primer entrenador
			list_remove(parametros_intercambio->pokemones, 1); // se elimina al segundo pokemon al pedo (seria el pokemon que el primer entrenador necesita)

			pthread_mutex_lock(&cola_deadlock_mutex);
			queue_push(cola_deadlock, (void*) parametros_intercambio);
			pthread_mutex_unlock(&cola_deadlock_mutex);
			sem_post(&sem_cola_deadlock);
		} else {
			pthread_mutex_lock(&cantidad_deadlocks_mutex);
			cantidad_deadlocks++;
			pthread_mutex_unlock(&cantidad_deadlocks_mutex);
			if (otro_entrenador->pokemones_innecesarios->elements_count == 0) {
				cambiar_estado_de_entrenador(otro_entrenador, ESTADO_EXIT, "Ha terminado su objetivo");
				t_tarea* otra_tarea_pingo = malloc(sizeof(t_tarea));
				otra_tarea_pingo->id_tarea = NO_HACER_PINGO;
				cambiar_tarea_de_entrenador(otro_entrenador, otra_tarea_pingo);
				sem_post(&(otro_entrenador->semaforo));
			} else {
				t_tarea* otra_tarea_pingo = malloc(sizeof(t_tarea));
				otra_tarea_pingo->id_tarea = NO_HACER_PINGO;
				cambiar_tarea_de_entrenador(otro_entrenador, otra_tarea_pingo);
				list_destroy(parametros_intercambio->entrenadores);
				list_destroy(parametros_intercambio->pokemones);
				free(parametros_intercambio);
			}
		}

		if (entrenador->pokemones_innecesarios->elements_count == 0) {
			cambiar_estado_de_entrenador(entrenador, ESTADO_EXIT, "Ha terminado su objetivo");
			t_tarea* tarea_pingo = malloc(sizeof(t_tarea));
			tarea_pingo->id_tarea = NO_HACER_PINGO;
			cambiar_tarea_de_entrenador(entrenador, tarea_pingo);
			sem_post(&(entrenador->semaforo));
		} else {
			t_tarea* tarea_pingo = malloc(sizeof(t_tarea));
			tarea_pingo->id_tarea = NO_HACER_PINGO;
			cambiar_tarea_de_entrenador(entrenador, tarea_pingo);
			cambiar_estado_de_entrenador(entrenador, ESTADO_BLOCKED, "Se ha terminado de intercambiar pokemones y sigue sin cumplirse el objetivo");
			log_info(extense_logger, "entrenador %i cambia su estado a BLOQUEADO",entrenador->index);
		}

		definir_nueva_estimacion(entrenador);

		mandar_a_siguente_entrenador_a_ejecutar_por_sjf();

		break;

		case NO_HACER_PINGO:
			break;
		default:
			log_info(extense_logger, "Entro por cualquier lado bro");
		}

	}

	log_info(extense_logger, "Entrenador %i ha terminado", entrenador->index);

	pthread_exit(NULL);
}

void planificar_pokemon(void) {
	log_info(extense_logger, "Hilo planificador de pokemones iniciado");

	while(1) {
		sem_wait(&sem_cola_pokemones);
		sem_wait(&sem_entrenadores_disponibles);

		pthread_mutex_lock(&cola_pokemones_mutex);
		t_pokemon* mensaje_pokemon = queue_pop(cola_pokemones);
		pthread_mutex_unlock(&cola_pokemones_mutex);

		log_info(extense_logger, "Planificando un nuevo pokemon");

		if (tengo_que_planificar_pokemon(mensaje_pokemon) == 1) {

			contar_planificacion(mensaje_pokemon);

			borrar_pokemon_del_mapa(mensaje_pokemon);

			t_list* entrenador_disponible = entrenadores_que_pueden_ir_a_atrapar();

			t_entrenador* entrenador_a_planificar = entrenador_mas_cercano(entrenador_disponible, mensaje_pokemon->pos_X, mensaje_pokemon->pos_Y);

			t_tarea* tarea_pokemon = malloc(sizeof(t_tarea));
			tarea_pokemon->id_tarea = ATRAPAR_POKEMON;
			tarea_pokemon->parametros = mensaje_pokemon;
			cambiar_tarea_de_entrenador(entrenador_a_planificar, tarea_pokemon);
			cambiar_estado_de_entrenador(entrenador_a_planificar, ESTADO_READY, "Entrenador planificado para ir a atrapar pokemon");
			entrenador_a_planificar->llegada = obtener_id_llegada();

			if (algoritmo_planificacion == 3) {
				pthread_mutex_lock(&planificacion_sjf);
				if (cantidad_de_entrenadores_en_ready_o_exec() == 1) {
					sem_post(&(entrenador_a_planificar->semaforo));
				}
				pthread_mutex_unlock(&planificacion_sjf);
			} else if (algoritmo_planificacion == 4) {
				pthread_mutex_lock(&planificacion_sjf);
				if (cantidad_de_entrenadores_en_ready_o_exec() == 1) {
					sem_post(&(entrenador_a_planificar->semaforo));
				} else {
					desalojar_si_es_necesario(entrenador_a_planificar);
				}
				pthread_mutex_unlock(&planificacion_sjf);
			} else {
				sem_post(&(entrenador_a_planificar->semaforo));
			}

			list_destroy(entrenador_disponible);

		} else {
			sem_post(&sem_entrenadores_disponibles);
		}
	}
}

void planificar_caught(void) {
	log_info(extense_logger, "Hilo planificador de mensajes caught iniciado");

	while(1) {
		sem_wait(&sem_cola_caught);
		pthread_mutex_lock(&cola_caught_mutex);
		t_caught* mensaje_caught = queue_pop(cola_caught);
		pthread_mutex_unlock(&cola_caught_mutex);

		log_info(extense_logger, "Planificando mensaje caught con id correlativo %i y flag de recibido %i", mensaje_caught->idCorrelativo, mensaje_caught->flag);

		t_catch_id* catch_id;
		int index_catch;

		pthread_mutex_lock(&catch_IDs_mutex);
		for (int i = 0; i < catch_IDs->elements_count; i++) {
			t_catch_id* catch_id_lista = list_get(catch_IDs, i);
			if (catch_id_lista->id_catch == mensaje_caught->idCorrelativo) {
				catch_id = catch_id_lista;
				index_catch = i;
				i = catch_IDs->elements_count;
			}
		}

		if (catch_id != NULL) {
			if (mensaje_caught->flag == 1) {
				adquirir_pokemon(catch_id->entrenador, catch_id->pokemon->nombre);
				if (catch_id->entrenador->pokebolas == 0) {
					if (list_size(catch_id->entrenador->objetivos_actuales) == 0) {
						cambiar_estado_de_entrenador(catch_id->entrenador, ESTADO_EXIT, "Ha terminado su objetivo");
						log_info(extense_logger, "no hago nada porque el entrenador %i cumplio sus objetivos y no tiene pokebolas",catch_id->entrenador->index );
						t_tarea* tarea_pingo = malloc(sizeof(t_tarea));
						tarea_pingo->id_tarea = NO_HACER_PINGO;
						cambiar_tarea_de_entrenador(catch_id->entrenador, tarea_pingo);
						sem_post(&(catch_id->entrenador->semaforo));
					} else {
						log_info(extense_logger, "no hago nada porque el entrenador tiene objetivos pero no pokebolas");
						t_tarea* tarea_pingo = malloc(sizeof(t_tarea));
						tarea_pingo->id_tarea = NO_HACER_PINGO;
						cambiar_tarea_de_entrenador(catch_id->entrenador, tarea_pingo);
					}
				} else {
					log_info(extense_logger, "Entrenador %i ha obtenido el pokemon %s", catch_id->entrenador->index, catch_id->pokemon->nombre);
					t_tarea* tarea_pingo = malloc(sizeof(t_tarea));
					tarea_pingo->id_tarea = NO_HACER_PINGO;
					cambiar_tarea_de_entrenador(catch_id->entrenador, tarea_pingo);
					sem_post(&sem_entrenadores_disponibles);
				}
			} else {
				if (tengo_en_el_mapa(catch_id->pokemon->nombre) == 1) {
					log_info(extense_logger, "entrenador %i va a atrapar pokemon %s",catch_id->entrenador->index,catch_id->pokemon->nombre);
					t_pokemon* pokemon_para_reintentar = mejor_pokemon_para_reintentar(catch_id->entrenador, catch_id->pokemon->nombre);
					t_tarea* tarea_reatrapar = malloc(sizeof(t_tarea));
					tarea_reatrapar->id_tarea = ATRAPAR_POKEMON;
					tarea_reatrapar->parametros = pokemon_para_reintentar;
					cambiar_tarea_de_entrenador(catch_id->entrenador, tarea_reatrapar);
					cambiar_estado_de_entrenador(catch_id->entrenador, ESTADO_READY, "Entrenador planificado para atrapar pokemon");
					sem_post(&(((t_entrenador*) catch_id->entrenador)->semaforo));
				} else {
					liberar_planificacion(catch_id->pokemon);
					sem_post(&sem_entrenadores_disponibles);
				}
			}
			list_remove(catch_IDs, index_catch);
			free(catch_id);
		} else {
			// ???
		}

		pthread_mutex_unlock(&catch_IDs_mutex);
		free(mensaje_caught);
	}
}

void planificar_deadlock(void) {
	log_info(extense_logger, "Hilo planificador de tratamiento de deadlocks iniciado");

	while(1) {
		sem_wait(&sem_cola_deadlock);

		pthread_mutex_lock(&cola_deadlock_mutex);
		t_deadlock* mensaje_deadlock = queue_pop(cola_deadlock);
		pthread_mutex_unlock(&cola_deadlock_mutex);

		log_info(extense_logger, "Planificando tratamiento de deadlock");

		if (todavia_existe_deadlock(mensaje_deadlock) == 1) {
			t_tarea* tarea_deadlock = malloc(sizeof(t_tarea));

			tarea_deadlock->id_tarea = INTERCAMBIAR_POKEMON;
			tarea_deadlock->parametros = (void*) mensaje_deadlock;

			t_entrenador* primer_entrenador = (t_entrenador*) list_get(mensaje_deadlock->entrenadores, 0);

			cambiar_tarea_de_entrenador(primer_entrenador, tarea_deadlock);
			cambiar_estado_de_entrenador(primer_entrenador, ESTADO_READY, "Entrenador planificado para intercambiar pokemones");
			primer_entrenador->llegada = obtener_id_llegada();

			if (algoritmo_planificacion == 3) {
				pthread_mutex_lock(&planificacion_sjf);
				if (cantidad_de_entrenadores_en_ready_o_exec() == 1) {
					sem_post(&(primer_entrenador->semaforo));
				}
				pthread_mutex_unlock(&planificacion_sjf);
			} else if (algoritmo_planificacion == 4) {
				pthread_mutex_lock(&planificacion_sjf);
				if (cantidad_de_entrenadores_en_ready_o_exec() == 1) {
					sem_post(&(primer_entrenador->semaforo));
				} else {
					desalojar_si_es_necesario(primer_entrenador);
				}
				pthread_mutex_unlock(&planificacion_sjf);
			} else {
				sem_post(&(primer_entrenador->semaforo));
			}
		} else {
			log_info(extense_logger, "Deadlock encontrado ya no aplica, desestimando para planificar");
			list_destroy(mensaje_deadlock->entrenadores);
			list_destroy(mensaje_deadlock->pokemones);
			free(mensaje_deadlock);
		}

	}
}

int todavia_existe_deadlock(t_deadlock* deadlock) {
	for (int i = 0; i < deadlock->entrenadores->elements_count; i++) {
		t_entrenador* entrenador_deadlock = list_get(deadlock->entrenadores, i);
		char* pokemon_deadlock = list_get(deadlock->pokemones, i);
		int hay_deadlock_en_entrenador = entrenador_tiene_pokemon_innecesario(entrenador_deadlock, pokemon_deadlock);
		if (hay_deadlock_en_entrenador == 0) {
			return 0;
		}
	}
	return 1;
}

int entrenador_tiene_pokemon_innecesario(t_entrenador* entrenador, char* pokemon) {
	for (int i = 0; i < entrenador->pokemones_innecesarios->elements_count; i++) {
		char* pokemon_lista = list_get(entrenador->pokemones_innecesarios, i);
		if (string_equals_ignore_case(pokemon_lista, pokemon) == 1) {
			return 1;
		}
	}
	return 0;
}

void buscar_deadlocks(void) {
	log_info(extense_logger, "Hilo de busqueda de deadlocks iniciado");
	while(1) {
		sleep(tiempo_deadlock);

		log_info(extense_logger, "Buscando deadlock...");
		log_info(logger, "Buscando deadlock...");
		t_deadlock* deadlock = obtener_deadlock();

		if (deadlock != NULL) {
			log_info(extense_logger, "Deadlock obtenido involucrando a %i entrenadores", deadlock->entrenadores->elements_count);
			log_info(logger, "Deadlock obtenido involucrando a %i entrenadores", deadlock->entrenadores->elements_count);

			pthread_mutex_lock(&cola_deadlock_mutex);
			queue_push(cola_deadlock, (void*) deadlock);
			pthread_mutex_unlock(&cola_deadlock_mutex);
			sem_post(&sem_cola_deadlock);
		} else {
			log_info(extense_logger, "No se han encontrado deadlocks");
			log_info(logger, "No se han encontrado deadlocks");
		}
	}
}

t_deadlock* obtener_deadlock() {
	t_list* entrenadores_bloqueados = obtener_entrenadores_bloqueados();
	for (int i = 0; i < entrenadores_bloqueados->elements_count; i++) {
		t_deadlock* deadlock = obtener_deadlock_por_arbol(entrenadores_bloqueados, i);
		if (deadlock != NULL) {
			list_destroy(entrenadores_bloqueados);
			return deadlock;
		}
	}
	list_destroy(entrenadores_bloqueados);
	return NULL;
}

t_deadlock* obtener_deadlock_por_arbol(t_list* entrenadores_bloqueados, int index) {
	t_nodo_arbol* raiz = malloc(sizeof(t_nodo_arbol));
	raiz->padre = NULL;
	raiz->entrenador = list_get(entrenadores_bloqueados, index);
	raiz->hijos = list_create();
	raiz->pokemon_faltante = NULL;

	log_info(extense_logger, "Cantidad de entrenadores bloqueados para buscar deadlocks: %i", entrenadores_bloqueados->elements_count);
	for (int i = 0; i < entrenadores_bloqueados->elements_count; i++) {
		t_deadlock* deadlock = agregar_nivel_hasta_deadlock(raiz, i, entrenadores_bloqueados);
		if (deadlock != NULL) {
			log_info(extense_logger, "Liberando arbol luego de encontrar deadlock");
			liberar_arbol(raiz);
			free(raiz);
			return deadlock;
		}
	}
	log_info(extense_logger, "Liberando arbol luego de no encontrar deadlock");
	liberar_arbol(raiz);
	free(raiz);
	return NULL;
}

t_deadlock* agregar_nivel_hasta_deadlock(t_nodo_arbol* raiz, int i, t_list* entrenadores_bloqueados) {
	t_list* nivel = obtener_nivel_de_arbol(raiz, i);
	t_deadlock* deadlock = NULL;
	for (int j = 0; j < nivel->elements_count; j++) {
		t_nodo_arbol* nodo = (t_nodo_arbol*) list_get(nivel, j);
		int ubicacion_nodo_raiz = agregar_hijos(nodo, raiz, entrenadores_bloqueados);
		if (ubicacion_nodo_raiz != -1) {
			t_nodo_arbol* nodo_raiz = (t_nodo_arbol*) list_get(nodo->hijos, ubicacion_nodo_raiz);
			deadlock = armar_deadlock(nodo_raiz);
			j = nivel->elements_count;
		}
	}
	list_destroy(nivel);
	return deadlock;
}

t_deadlock* armar_deadlock(t_nodo_arbol* nodo_raiz) {
	log_info(extense_logger, "Armando deadlock");
	t_deadlock* deadlock = malloc(sizeof(t_deadlock));
	deadlock->entrenadores = list_create();
	deadlock->pokemones = list_create();
	t_nodo_arbol* nodo_actual = nodo_raiz;
	while (nodo_actual->padre != NULL) {
		log_info(extense_logger, "Agregando entrenador %i al deadlock", nodo_actual->entrenador->index);
		list_add(deadlock->entrenadores, (void*) nodo_actual->entrenador);
		log_info(extense_logger, "Entrenador %i agregado al deadlock", nodo_actual->entrenador->index);
		nodo_actual = nodo_actual->padre;
	}
	nodo_actual = nodo_raiz;
	while (nodo_actual->pokemon_faltante != NULL) {
		log_info(extense_logger, "Agregando pokemon %s al deadlock", nodo_actual->pokemon_faltante);
		list_add(deadlock->pokemones, (void*) nodo_actual->pokemon_faltante);
		log_info(extense_logger, "Pokemon %s agregado al deadlock", nodo_actual->pokemon_faltante);
		nodo_actual = nodo_actual->padre;
	}

	char* ultimo_pokemon = list_remove(deadlock->pokemones, deadlock->pokemones->elements_count - 1);
	list_add_in_index(deadlock->pokemones, 0, ultimo_pokemon);

	return deadlock;
}

t_list* obtener_nivel_de_arbol(t_nodo_arbol* raiz, int index) {
	t_list* nivel = list_create();
	t_list* aux = list_create();
	list_add(nivel, raiz);
	for (int i = 0; i < index; i++) {
		for (int j = 0; j < nivel->elements_count; j++) {
			t_nodo_arbol* elemento_nivel = (t_nodo_arbol*) list_get(nivel, j);
			list_add_all(aux, elemento_nivel->hijos);
		}
		list_clean(nivel);
		list_add_all(nivel, aux);
		list_clean(aux);
	}
	list_destroy(aux);
	return nivel;
}

int agregar_hijos(t_nodo_arbol* nodo, t_nodo_arbol* raiz, t_list* entrenadores_bloqueados) {
	log_info(extense_logger, "Agregando hijos para entrenador %i", nodo->entrenador->index);
	int hijo_raiz = -1;
	for (int i = 0; i < nodo->entrenador->pokemones_innecesarios->elements_count; i++) {
		char* pokemon_innecesario_lista = (char*) list_get(nodo->entrenador->pokemones_innecesarios, i);
		t_list* entrenadores_que_necesitan = entrenadores_que_necesitan_pokemon(pokemon_innecesario_lista, entrenadores_bloqueados);
		for (int j = 0; j < entrenadores_que_necesitan->elements_count; j++) {
			t_entrenador* entrenador_hijo = (t_entrenador*) list_get(entrenadores_que_necesitan, j);
			t_nodo_arbol* nodo_hijo = malloc(sizeof(t_nodo_arbol));
			nodo_hijo->entrenador = entrenador_hijo;
			nodo_hijo->padre = nodo;
			nodo_hijo->pokemon_faltante = pokemon_innecesario_lista;
			nodo_hijo->hijos = list_create();
			list_add(nodo->hijos, nodo_hijo);
			if (nodo_hijo->entrenador->index == raiz->entrenador->index) {
				hijo_raiz = nodo->hijos->elements_count - 1;
				j = entrenadores_que_necesitan->elements_count;
				i = nodo->entrenador->pokemones_innecesarios->elements_count;
			}
		}
		list_destroy(entrenadores_que_necesitan);
	}
	return hijo_raiz;
}

void liberar_arbol(t_nodo_arbol* raiz) {
	if (raiz->hijos == NULL || raiz->hijos->elements_count == 0) {
		if (raiz->hijos != NULL) {
			list_destroy(raiz->hijos);
		}
	} else {
		for (int i = 0; i < raiz->hijos->elements_count; i++) {
			t_nodo_arbol* nodo_hijo = (t_nodo_arbol*) list_get(raiz->hijos, i);
			liberar_arbol(nodo_hijo);
			free(nodo_hijo);
		}
		list_destroy(raiz->hijos);
	}
}

t_list* entrenadores_que_necesitan_pokemon(char* pokemon, t_list* entrenadores_bloqueados) {
	t_list* entrenadores_que_necesitan_pokemon = list_create();
	for(int i = 0; i < entrenadores_bloqueados->elements_count; i++) {
		t_entrenador* entrenador_lista = (t_entrenador*) list_get(entrenadores_bloqueados, i);
		int ya_fue_agregado = 0;
		for(int j = 0; j < entrenador_lista->objetivos_actuales->elements_count; j++) {
			if (ya_fue_agregado == 0) {
				char* objetivo_lista = (char*) list_get(entrenador_lista->objetivos_actuales, j);
				int necesita = string_equals_ignore_case(pokemon, objetivo_lista);

				if (necesita == 1) {
					list_add(entrenadores_que_necesitan_pokemon, entrenador_lista);
					log_info(extense_logger, "Entrenador %i necesita el pokemon %s", entrenador_lista->index, objetivo_lista);
				}
			}
		}
	}
	return entrenadores_que_necesitan_pokemon;
}

t_list* obtener_entrenadores_bloqueados() {
	t_list* entrenadores_bloqueados = list_create();
	log_info(extense_logger, "Obtengo entrenadores bloqueados");
	for (int i = 0; i < entrenadores->elements_count; i++) {
		t_entrenador* entrenador_de_lista = (t_entrenador*) list_get(entrenadores, i);
		if ((entrenador_de_lista->estado == ESTADO_BLOCKED && entrenador_de_lista->pokebolas == 0) && entrenador_de_lista->tarea_actual->id_tarea == NO_HACER_PINGO) {
			list_add(entrenadores_bloqueados, entrenador_de_lista);
			log_info(extense_logger, "Entrenador %i agregado a la lista de bloqueados", i);
		}
	}
	log_info(extense_logger, "Entrenadores bloqueados obtenidos");
	return entrenadores_bloqueados;
}

t_entrenador* obtener_entrenador(int id) {
	for (int i = 0; i < entrenadores->elements_count; i++) {
		t_entrenador* entrenador_lista = (t_entrenador*) list_get(entrenadores, i);
		if (entrenador_lista->index == id) {
			return entrenador_lista;
		}
	}
	return NULL;
}

void verificar_conexion(void) {
	log_info(extense_logger, "Hilo de verificacion periodica de conexion con Broker iniciado");
	while(1) {
		sleep(tiempo_reconexion);
		log_info(extense_logger, "Iniciando verificacion de conexion con Broker...");
		log_info(logger, "Iniciando verificacion de conexion con Broker...");
		if (estoy_conectado_al_broker == 0) {
			reconectar_al_broker();
		} else {
			log_info(extense_logger, "Conexion con Broker en buen estado, verificando nuevamente en %i segundos", tiempo_reconexion);
			log_info(logger, "Conexion con Broker en buen estado, verificando nuevamente en %i segundos", tiempo_reconexion);
		}
	}
}

void reconectar_al_broker() {
	log_info(extense_logger, "Intentando reconectar con Broker");
	int conexion_al_broker = crear_conexion(ip_broker, puerto_broker);
	if (conexion_al_broker != -1) {
		socket_escucha_appeared = conexion_al_broker;
		log_info(extense_logger, "Socket de reconexion appeared: %i",socket_escucha_appeared);

		socket_escucha_caught = crear_conexion(ip_broker, puerto_broker);
		log_info(extense_logger, "Socket de reconexion appeared: %i",socket_escucha_caught);

		socket_escucha_localized = crear_conexion(ip_broker, puerto_broker);
		log_info(extense_logger, "Socket de reconexion appeared: %i",socket_escucha_localized);

		uint32_t id_cola_appeared = 12;
		int status_envio = mandar_suscripcion(socket_escucha_appeared, id_cola_appeared);
		if (status_envio == -1) {
			pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
			estoy_conectado_al_broker = 0;
			pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
			log_error(extense_logger, "Reconexion con Broker fallida, intentando nuevamente en %i segundos", tiempo_reconexion);
			log_error(logger, "Reconexion con Broker fallida, intentando nuevamente en %i segundos", tiempo_reconexion);
		} else {
			uint32_t id_cola_caught = 14;
			status_envio = mandar_suscripcion(socket_escucha_caught, id_cola_caught);
			if (status_envio == -1) {
				pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
				estoy_conectado_al_broker = 0;
				pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
				log_error(extense_logger, "Reconexion con Broker fallida, intentando nuevamente en %i segundos", tiempo_reconexion);
				log_error(logger, "Reconexion con Broker fallida, intentando nuevamente en %i segundos", tiempo_reconexion);
			} else {
				uint32_t id_cola_localized = 16;
				int status_envio = mandar_suscripcion(socket_escucha_localized, id_cola_localized);
				if (status_envio == -1) {
					pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
					estoy_conectado_al_broker = 0;
					pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
					log_error(extense_logger, "Reconexion con Broker fallida, intentando nuevamente en %i segundos", tiempo_reconexion);
					log_error(logger, "Reconexion con Broker fallida, intentando nuevamente en %i segundos", tiempo_reconexion);
				} else {
					log_info(logger, "conexion establecida con BROKER, ip: %s puerto : %s", ip_broker, puerto_broker);
					estoy_conectado_al_broker = 1;
					log_info(extense_logger, "Reconexion con Broker exitosa, verificando nuevamente en %i segundos", tiempo_reconexion);
					log_info(logger, "Reconexion con Broker exitosa, verificando nuevamente en %i segundos", tiempo_reconexion);
				}
			}
		}
	} else {
		log_error(extense_logger, "Reconexion con Broker fallida, intentando nuevamente en %i segundos", tiempo_reconexion);
		log_error(logger, "Reconexion con Broker fallida, intentando nuevamente en %i segundos", tiempo_reconexion);
	}

	if (estoy_conectado_al_broker_ultimo_chequeo == 0 && estoy_conectado_al_broker == 1) {
		sem_post(&sem_conexion_appeared);
		sem_post(&sem_conexion_caught);
		sem_post(&sem_conexion_localized);
	}
	estoy_conectado_al_broker_ultimo_chequeo = estoy_conectado_al_broker;
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
	extense_logger = iniciar_logger_sin_consola(extense_log_file);
	extense_logger_protocol = extense_logger;
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
	char* algoritmo_planificacion_config =  config_get_string_value(config, "ALGORITMO_PLANIFICACION");
	log_info(extense_logger, "EL algoritmo de planificacion es %s", algoritmo_planificacion_config);
	alpha = config_get_double_value(config, "ALPHA");
	estimacion_inicial = config_get_int_value(config, "ESTIMACION_INICIAL");

	if (string_equals_ignore_case(algoritmo_planificacion_config, "FIFO")) {
		algoritmo_planificacion = 1;
	} else if (string_equals_ignore_case(algoritmo_planificacion_config, "RR")) {
		algoritmo_planificacion = 2;
	} else if (string_equals_ignore_case(algoritmo_planificacion_config, "SJF-SD") || string_equals_ignore_case(algoritmo_planificacion_config, "SJFSD")) {
		algoritmo_planificacion = 3;
	} else if (string_equals_ignore_case(algoritmo_planificacion_config, "SJF-CD") || string_equals_ignore_case(algoritmo_planificacion_config, "SJFCD")) {
		algoritmo_planificacion = 4;
	}

	quantum = config_get_int_value(config, "QUANTUM");

	srand(time(NULL));

	generar_ID_Modulo();

	log_info(extense_logger, "El id del modulo es %i", id_modulo);

	catch_IDs = list_create();

	pokemones_llegados = list_create();

	pokemones_conocidos_que_no_se_intentan_atrapar = dictionary_create();

	id_llegada = 0;

	inicializar_entrenadores();

	obtener_objetivo_global();

	obtener_cantidad_de_cada_pokemon_a_planificar();

	ciclos_CPU = 0;
	cambios_contexto = 0;
	cantidad_deadlocks = 0;

	// crear cola de mensajes recibidos
	inicializar_cola();

	sem_init(&sem_conexion_appeared, 0, 1);
	sem_init(&sem_conexion_caught, 0, 1);
	sem_init(&sem_conexion_localized, 0, 1);

	pthread_mutex_init(&planificacion_ready, NULL);
	pthread_mutex_init(&cola_pokemones_mutex, NULL);
	pthread_mutex_init(&cola_caught_mutex, NULL);
	pthread_mutex_init(&cola_deadlock_mutex, NULL);
	pthread_mutex_init(&pokemones_llegados_mutex, NULL);
	pthread_mutex_init(&catch_IDs_mutex, NULL);
	pthread_mutex_init(&estoy_conectado_al_broker_mutex, NULL);
	pthread_mutex_init(&ciclos_CPU_mutex, NULL);
	pthread_mutex_init(&cambios_contexto_mutex, NULL);
	pthread_mutex_init(&cantidad_deadlocks_mutex, NULL);
	pthread_mutex_init(&planificacion_sjf, NULL);
	pthread_mutex_init(&llegada_mutex, NULL);
	pthread_mutex_init(&dictionary_intentan_atrapar_mutex, NULL);

	socket_escucha_game_boy = iniciar_escucha_game_boy(ip, puerto);
	pthread_t hilo_escucha_de_game_boy;
	pthread_create(&hilo_escucha_de_game_boy, NULL, (void*) escuchar_game_boy, (void*) socket_escucha_game_boy);
	pthread_detach(hilo_escucha_de_game_boy);

	// crear hilos planificadores y tirarles detach

	pthread_t hilo_planificador_pokemon;
	pthread_t hilo_planificador_caught;
	pthread_t hilo_planificador_deadlock;
	pthread_create(&hilo_planificador_pokemon, NULL, (void*) planificar_pokemon, NULL);
	pthread_create(&hilo_planificador_caught, NULL, (void*) planificar_caught, NULL);
	pthread_create(&hilo_planificador_deadlock, NULL, (void*) planificar_deadlock, NULL);
	pthread_detach(hilo_planificador_pokemon);
	pthread_detach(hilo_planificador_caught);
	pthread_detach(hilo_planificador_deadlock);

	// crear hilos que se conecten al broker y que cada uno escuche una cola del broker
	// cuando obtengan algo van a tener que mandar el mensaje a la cola del team correspondiente
	socket_escucha_appeared = crear_conexion(ip_broker, puerto_broker);

	if (socket_escucha_appeared != -1) {
		estoy_conectado_al_broker = 1;
		estoy_conectado_al_broker_ultimo_chequeo = 1;
	} else {
		estoy_conectado_al_broker = 0;
		estoy_conectado_al_broker_ultimo_chequeo = 0;
		sem_wait(&sem_conexion_appeared);
		sem_wait(&sem_conexion_caught);
		sem_wait(&sem_conexion_localized);
	}

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

	pthread_t* threads_entrenadores = malloc(sizeof(pthread_t) * entrenadores->elements_count);

	if (algoritmo_planificacion == 1) {
		for (int i = 0; i < entrenadores->elements_count; i++) {
			pthread_create(&(threads_entrenadores[i]), NULL, (void*) laburar_fifo, list_get(entrenadores, i));
		}
	} else if (algoritmo_planificacion == 2) {
		for (int i = 0; i < entrenadores->elements_count; i++) {
			pthread_create(&(threads_entrenadores[i]), NULL, (void*) laburar_rr, list_get(entrenadores, i));
		}
	} else if (algoritmo_planificacion == 3) {
		for (int i = 0; i < entrenadores->elements_count; i++) {
			pthread_create(&(threads_entrenadores[i]), NULL, (void*) laburar_sjf_sd, list_get(entrenadores, i));
		}
	} else if (algoritmo_planificacion == 4) {
		for (int i = 0; i < entrenadores->elements_count; i++) {
			pthread_create(&(threads_entrenadores[i]), NULL, (void*) laburar_sjf_cd, list_get(entrenadores, i));
		}
	}

	mandar_get();

	for (int i = 0; i < entrenadores->elements_count; i++) {
		pthread_join(threads_entrenadores[i], NULL);
	}

	free(threads_entrenadores);

	log_destroy(logger);
	logger = iniciar_logger(log_file);

	mostrar_metricas();
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
	log_info(extense_logger, "Inicializando colas");

	cola_pokemones = queue_create();
	cola_caught = queue_create();
	cola_deadlock = queue_create();
	pthread_mutex_init(&cola_pokemones_mutex, NULL);
	pthread_mutex_init(&cola_caught_mutex, NULL);
	pthread_mutex_init(&cola_deadlock_mutex, NULL);
	sem_init(&sem_cola_pokemones, 0, 0);
	sem_init(&sem_cola_caught, 0, 0);
	sem_init(&sem_cola_deadlock, 0, 0);

	t_list* entrenadores_libres = entrenadores_que_pueden_ir_a_atrapar();

	log_info(extense_logger, "Iniciando semaforo de entrenadores libres en %i", entrenadores_libres->elements_count);

	sem_init(&sem_entrenadores_disponibles, 0, entrenadores_libres->elements_count);

	list_destroy(entrenadores_libres);

	log_info(extense_logger, "Colas inicializadas");
}

void inicializar_entrenadores() {
	entrenadores = list_create();

	char** posiciones = config_get_array_value(config, "POSICIONES_ENTRENADORES");
	char** pokemones = config_get_array_value(config, "POKEMON_ENTRENADORES");
	char** objetivos = config_get_array_value(config, "OBJETIVOS_ENTRENADORES");

	int i = 0;

	int cant_pokemon_entrenadores = cantidad_de_pokemon_entrenadores(pokemones);

	while (posiciones[i] != NULL && objetivos[i] != NULL) {

		char* objetivo_entrenador = objetivos[i];
		log_info(extense_logger, "Objetivos para el entrenador %i: %s", i, objetivos[i]);
		char* posicion = posiciones[i];
		log_info(extense_logger, "Posiciones para el entrenador %i: %s", i, posiciones[i]);
		char* conjunto_pokemon;
		if (i < cant_pokemon_entrenadores) {
			conjunto_pokemon = pokemones[i];
		} else {
			conjunto_pokemon = "";
		}

		log_info(extense_logger, "Pokemones para el entrenador %i: %s", i, conjunto_pokemon);

		string_to_lower(objetivo_entrenador);
		string_to_lower(conjunto_pokemon);

		t_entrenador* entrenador = malloc(sizeof(t_entrenador));

		entrenador->index = i;

		t_list* objetivos = list_create();

		char* token_obj = strtok(objetivo_entrenador, "|");

		if (token_obj != NULL) {
			while(token_obj != NULL) {
				list_add(objetivos, token_obj);
				log_info(extense_logger, "Llego token objetivo %s para el entrenador %i", token_obj, i);
				token_obj = strtok(NULL, "|");
			}
		} else {
			log_info(extense_logger, "No existen objetivos para el entrenador %i", i);
		}

		entrenador->objetivos = objetivos;

		t_list* pokemones = list_create();

		char* token_pok = strtok(conjunto_pokemon, "|");

		if (token_pok != NULL) {
			while(token_pok != NULL) {
				list_add(pokemones, token_pok);
				log_info(extense_logger, "Llego token pokemon %s", token_pok);
				token_pok = strtok(NULL, "|");
			}
		} else {
			log_info(extense_logger, "No existen pokemones para el entrenador %i", i);
		}

		entrenador->pokemones = pokemones;

		char** posiciones = string_split(posicion, "|");

		entrenador->posX = (uint32_t) atoi(posiciones[0]);
		log_info(extense_logger, "Posicion X del entrenador %i: %i", i, entrenador->posX);
		entrenador->posY = (uint32_t) atoi(posiciones[1]);
		log_info(extense_logger, "Posicion Y del entrenador %i: %i", i, entrenador->posY);

		free(posiciones[0]);
		free(posiciones[1]);
		free(posiciones);

		t_list* objetivos_actuales = list_create();

		for (int p = 0; p < objetivos->elements_count; p++) {
			char* objetivo_lista = list_get(objetivos, p);
			char* objetivo_actual = string_duplicate(objetivo_lista);
			list_add(objetivos_actuales, objetivo_actual);
		}

		for (int k = 0; k < entrenador->pokemones->elements_count; k++) {
			char* pokemon_lista = list_get(entrenador->pokemones, k);
			for (int h = 0; h < objetivos_actuales->elements_count; h++) {
				char* objetivo_actual_lista = list_get(objetivos_actuales, h);
				if (string_equals_ignore_case(objetivo_actual_lista, pokemon_lista) == 1) {
					char* objetivo_removido = list_remove(objetivos_actuales, h);
					free(objetivo_removido);
					h = objetivos_actuales->elements_count;
				}
			}
		}

		entrenador->objetivos_actuales = objetivos_actuales;

		t_list* pokemones_innecesarios = list_create();

		for (int d = 0; d < pokemones->elements_count; d++) {
			char* pokemon_lista = list_get(pokemones, d);
			char* pokemon_innecesario_actual = string_duplicate(pokemon_lista);
			list_add(pokemones_innecesarios, pokemon_innecesario_actual);
		}

		for (int f = 0; f < entrenador->objetivos->elements_count; f++) {
			char* objetivo_lista = (char*) list_get(entrenador->objetivos, f);
			for (int c = 0; c < pokemones_innecesarios->elements_count; c++) {
				char* pokemon_innecesario_lista = (char*) list_get(pokemones_innecesarios, c);
				if (string_equals_ignore_case(pokemon_innecesario_lista, objetivo_lista) == 1) {
					char* pokemon_eliminado = list_remove(pokemones_innecesarios, c);
					free(pokemon_eliminado);
					c = pokemones_innecesarios->elements_count;
				}
			}
		}

		entrenador->pokemones_innecesarios = pokemones_innecesarios;

		entrenador->pokebolas = objetivos->elements_count - pokemones->elements_count;

		t_tarea* tarea = malloc(sizeof(t_tarea));

		tarea->id_tarea = NO_HACER_PINGO;

		entrenador->tarea_actual = tarea;

		definir_primer_estado(entrenador);

		sem_init(&(entrenador->semaforo), 0, 0);

		entrenador->contador_ciclos_CPU = 0;

		entrenador->estimacion = (double) estimacion_inicial;

		entrenador->real = (double) estimacion_inicial;

		entrenador->real_actual = 0;

		entrenador->llegada = 0;

		log_info(extense_logger, "Estado del entrenador %i: %i", entrenador->index, entrenador->estado);

		for (int l = 0; l < entrenador->objetivos->elements_count; l++) {
			log_info(extense_logger, "Objetivo del entrenador %i: %s", entrenador->index, (char*) list_get(entrenador->objetivos, l));
		}

		for (int y = 0; y < entrenador->objetivos_actuales->elements_count; y++) {
			log_info(extense_logger, "Objetivo actual del entrenador %i: %s", entrenador->index, (char*) list_get(entrenador->objetivos_actuales, y));
		}

		for (int w = 0; w < entrenador->pokemones->elements_count; w++) {
			log_info(extense_logger, "Pokemon del entrenador %i: %s", entrenador->index, (char*) list_get(entrenador->pokemones, w));
		}

		for (int q = 0; q < entrenador->pokemones_innecesarios->elements_count; q++) {
			log_info(extense_logger, "Pokemon innecesario del entrenador %i: %s", entrenador->index, (char*) list_get(entrenador->pokemones_innecesarios, q));
		}

		list_add(entrenadores, (void*) entrenador);

		free(objetivo_entrenador);
		free(posicion);
		if (string_equals_ignore_case(conjunto_pokemon, "") != 1) {
			free(conjunto_pokemon);
		}
		free(token_obj);
		free(token_pok);

		i++;
	}

	free(posiciones);
	free(pokemones);
	free(objetivos);
}

int cantidad_de_pokemon_entrenadores(char** pokemones) {
	int i = 0;
	while (pokemones[i] != NULL) {
		i++;
	}
	return i;
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
	log_info(extense_logger, "Obteniendo objetivo global");

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

	for (int i = 0; i < pokemones_innecesarios->elements_count; i++) {
		char* pokemon_innecesario_lista = (char*) list_get(pokemones_innecesarios, i);
		for (int j = 0; j < objetivo_global->elements_count; j++) {
			char* objetivo_lista = (char*) list_get(objetivo_global, j);
			if (string_equals_ignore_case(objetivo_lista, pokemon_innecesario_lista) == 1) {
				list_remove(objetivo_global, j);
				j = objetivo_global->elements_count;
			}
		}
	}

	for (int i = 0; i < objetivo_global->elements_count; i++) {
		log_info(extense_logger, "Objetivo global: %s", (char*) list_get(objetivo_global, i));
	}

	list_clean(pokemones_innecesarios);
	list_destroy(pokemones_innecesarios);

	log_info(extense_logger, "Objetivo global obtenido");
}

void obtener_cantidad_de_cada_pokemon_a_planificar() {
	log_info(extense_logger, "Obteniendo cantidad de cada pokemon a planificar");

	cantidad_de_pokemones_que_puedo_planificar = dictionary_create();
	for (int i = 0; i < objetivo_global->elements_count; i++) {
		char* objetivo_lista = list_get(objetivo_global, i);
		if (dictionary_has_key(cantidad_de_pokemones_que_puedo_planificar, objetivo_lista) == 1) {
			t_pokemon_planificables* cantidad = (t_pokemon_planificables*) dictionary_get(cantidad_de_pokemones_que_puedo_planificar, objetivo_lista);
			cantidad->cantidad++;
			log_info(extense_logger, "Cantidad a planificar de %s: %i", objetivo_lista, cantidad->cantidad);
		} else {
			t_pokemon_planificables* nuevo_pokemon = malloc(sizeof(t_pokemon_planificables));
			nuevo_pokemon->nombre = objetivo_lista;
			nuevo_pokemon->cantidad = 1;
			dictionary_put(cantidad_de_pokemones_que_puedo_planificar, objetivo_lista, nuevo_pokemon);
			log_info(extense_logger, "Cantidad a planificar de %s: %i", objetivo_lista, nuevo_pokemon->cantidad);
		}
	}

	log_info(extense_logger, "Cantidad de cada pokemon a planificar obtenida");
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
	log_info(extense_logger, "Mandando gets");

	obtener_pokemones_a_localizar();

	pthread_t* threads = malloc(sizeof(pthread_t) * pokemones_a_localizar->elements_count);

	for (int i= 0; i< pokemones_a_localizar->elements_count; i++) {
		pthread_create(&(threads[i]), NULL, (void*) enviar_get_a_broker, list_get(pokemones_a_localizar,i));
	}

	for (int i= 0; i< pokemones_a_localizar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	free(threads);

	log_info(extense_logger, "Gets mandados");
}

void enviar_get_a_broker(char* nombre_pokemon) {
	log_info(extense_logger, "Enviando get correspondiente al pokemon %s", nombre_pokemon);

	uint32_t tamanio_pokemon = strlen(nombre_pokemon) + 1;
	uint32_t bytes = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + tamanio_pokemon;
	uint32_t cod_op = 5;
	void* flujo = malloc(bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &tamanio_pokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, nombre_pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;

	int socket_broker = crear_conexion(ip_broker, puerto_broker);

	int status_send = send(socket_broker, flujo, bytes, 0);

	if (status_send == -1 || status_send == 0) {
		log_warning(extense_logger, "Error: No se pudo enviar el GET correspondiente al pokemon %s, iniciando el comportamiento default", nombre_pokemon);
		log_warning(logger, "Error: No se pudo enviar el GET correspondiente al pokemon %s, iniciando el comportamiento default", nombre_pokemon);
		pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
		estoy_conectado_al_broker = 0;
		pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
	} else {
		log_info(extense_logger, "Mensaje GET correspondiente al pokemon %s enviado correctamente al BROKER a traves del socket %i", nombre_pokemon, socket_broker);
		uint32_t id_get = recibir_ID_get(socket_broker);
		if (id_get == 0) {
			log_warning(extense_logger, "Hubo un problema al recibir el id correspondiente al mensaje GET con el pokemon %s", nombre_pokemon);
		} else {
			log_info(extense_logger, "Id del mensaje GET correspondiente al pokemon %s es %i", nombre_pokemon, id_get);
		}
	}

	cerrar_conexion(socket_broker);
	free(flujo);
}

void enviar_catch_a_broker(t_pokemon* pokemon, t_entrenador* entrenador) {
	log_info(extense_logger, "Entrenador %i enviando catch para el pokemon %s en (%i,%i)", entrenador->index, pokemon->nombre, pokemon->pos_X, pokemon->pos_Y);

	uint32_t tamanio_pokemon = strlen(pokemon->nombre)+ 1;
	uint32_t bytes = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + tamanio_pokemon + sizeof(uint32_t) + sizeof(uint32_t);
	uint32_t cod_op = 3;
	void* flujo = malloc(bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &tamanio_pokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, pokemon->nombre, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;
	memcpy(flujo + desplazamiento, &(pokemon->pos_X), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(pokemon->pos_Y), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	int socket_broker = crear_conexion(ip_broker, puerto_broker);

	entrenador->contador_ciclos_CPU++;
	pthread_mutex_lock(&ciclos_CPU_mutex);
	ciclos_CPU++;
	pthread_mutex_unlock(&ciclos_CPU_mutex);
	sleep(retardo_de_CPU);

	if (send(socket_broker, flujo, bytes, 0) == -1) {
		log_warning(extense_logger, "Error: No se pudo enviar el mensaje catch del entrenador %i para el pokemon %s en (%i,%i), obteniendo pokemon por comportamiento default", entrenador->index, pokemon->nombre, pokemon->pos_X, pokemon->pos_Y);
		log_warning(logger, "Error: No se pudo enviar el mensaje catch del entrenador %i para el pokemon %s en (%i,%i), obteniendo pokemon por comportamiento default", entrenador->index, pokemon->nombre, pokemon->pos_X, pokemon->pos_Y);
		adquirir_pokemon(entrenador, pokemon->nombre);
		if (entrenador->pokebolas == 0 && entrenador->objetivos_actuales->elements_count == 0) {
			cambiar_estado_de_entrenador(entrenador, ESTADO_EXIT, "Ha completado su objetivo");
			t_tarea* tarea_pingo = malloc(sizeof(t_tarea));
			tarea_pingo->id_tarea = NO_HACER_PINGO;
			cambiar_tarea_de_entrenador(entrenador, tarea_pingo);
			sem_post(&(entrenador->semaforo));
		} else if (entrenador->pokebolas == 0 && entrenador->objetivos_actuales->elements_count != 0) {
			t_tarea* tarea_pingo = malloc(sizeof(t_tarea));
			tarea_pingo->id_tarea = NO_HACER_PINGO;
			cambiar_tarea_de_entrenador(entrenador, tarea_pingo);
		} else {
			t_tarea* tarea_pingo = malloc(sizeof(t_tarea));
			tarea_pingo->id_tarea = NO_HACER_PINGO;
			cambiar_tarea_de_entrenador(entrenador, tarea_pingo);
			sem_post(&sem_entrenadores_disponibles);
		}
		pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
		estoy_conectado_al_broker = 0;
		pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
	} else {
		log_info(extense_logger, "El entrenador %i ha enviado el catch para el pokemon %s en (%i,%i) correctamente por el socket %i", entrenador->index, pokemon->nombre, pokemon->pos_X, pokemon->pos_Y, socket_broker);
		uint32_t id_catch = recibir_ID_Catch(socket_broker);
		if (id_catch == 0) {
			log_warning(extense_logger, "Error: Se pudo enviar el mensaje catch del entrenador %i para el pokemon %s en (%i,%i) pero no se ha podido obtener la respuesta del broker con el id, obteniendo pokemon por comportamiento default", entrenador->index, pokemon->nombre, pokemon->pos_X, pokemon->pos_Y);
			log_warning(logger, "Error: Se pudo enviar el mensaje catch del entrenador %i para el pokemon %s en (%i,%i) pero no se ha podido obtener la respuesta del broker con el id, obteniendo pokemon por comportamiento default", entrenador->index, pokemon->nombre, pokemon->pos_X, pokemon->pos_Y);
			adquirir_pokemon(entrenador, pokemon->nombre);
			if (entrenador->pokebolas == 0 && entrenador->objetivos_actuales->elements_count == 0) {
				cambiar_estado_de_entrenador(entrenador, ESTADO_EXIT, "Ha completado su objetivo");
				t_tarea* tarea_pingo = malloc(sizeof(t_tarea));
				tarea_pingo->id_tarea = NO_HACER_PINGO;
				cambiar_tarea_de_entrenador(entrenador, tarea_pingo);
				sem_post(&(entrenador->semaforo));
			} else if (entrenador->pokebolas == 0 && entrenador->objetivos_actuales->elements_count != 0) {
				t_tarea* tarea_pingo = malloc(sizeof(t_tarea));
				tarea_pingo->id_tarea = NO_HACER_PINGO;
				cambiar_tarea_de_entrenador(entrenador, tarea_pingo);
			} else {
				t_tarea* tarea_pingo = malloc(sizeof(t_tarea));
				tarea_pingo->id_tarea = NO_HACER_PINGO;
				cambiar_tarea_de_entrenador(entrenador, tarea_pingo);
				sem_post(&sem_entrenadores_disponibles);
			}
			pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
			estoy_conectado_al_broker = 0;
			pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
		} else {
			log_info(extense_logger, "Id del catch enviado por el entrenador %i para el pokemon %s en (%i,%i) recibido correctamente por el socket %i y es igual a %i", entrenador->index, pokemon->nombre, pokemon->pos_X, pokemon->pos_Y, socket_broker, id_catch);
			t_catch_id* catch_id = malloc(sizeof(t_catch_id));
			catch_id->id_catch = id_catch;
			catch_id->entrenador = entrenador;
			catch_id->pokemon = pokemon;
			pthread_mutex_lock(&catch_IDs_mutex);
			list_add(catch_IDs, (void*) catch_id);
			pthread_mutex_unlock(&catch_IDs_mutex);
		}
	}

	cerrar_conexion(socket_broker);
	free(flujo);
}

void irA(uint32_t posX, uint32_t posY, t_entrenador* entrenador) {
	log_info(extense_logger, "El entrenador %i se esta moviendo de (%i,%i) a (%i,%i)", entrenador->index, entrenador->posX, entrenador->posY, posX, posY);
	log_info(logger, "El entrenador %i se esta moviendo de (%i,%i) a (%i,%i)", entrenador->index, entrenador->posX, entrenador->posY, posX, posY);

	uint32_t entrenadorX = entrenador->posX;
	uint32_t entrenadorY = entrenador->posY;

	uint32_t coordX = abs(posX - entrenador->posX);
	uint32_t coordY = abs(posY - entrenador->posY);

	for (int y = 0; y < coordY; y++) {
		if (posY > entrenadorY) {
			entrenador->posY++;
			entrenador->contador_ciclos_CPU++;
			pthread_mutex_lock(&ciclos_CPU_mutex);
			ciclos_CPU++;
			pthread_mutex_unlock(&ciclos_CPU_mutex);
			log_info(extense_logger, "El entrenador %i se encuentra en (%i,%i)", entrenador->index, entrenador->posX, entrenador->posY);
			log_info(logger, "El entrenador %i se encuentra en (%i,%i)", entrenador->index, entrenador->posX, entrenador->posY);
			consumir_tiempo_retardo(entrenador);
		} else {
			entrenador->posY--;
			entrenador->contador_ciclos_CPU++;
			pthread_mutex_lock(&ciclos_CPU_mutex);
			ciclos_CPU++;
			pthread_mutex_unlock(&ciclos_CPU_mutex);
			log_info(extense_logger, "El entrenador %i se encuentra en (%i,%i)", entrenador->index, entrenador->posX, entrenador->posY);
			log_info(logger, "El entrenador %i se encuentra en (%i,%i)", entrenador->index, entrenador->posX, entrenador->posY);
			consumir_tiempo_retardo(entrenador);
		}
	}

	for (int x = 0; x < coordX; x++) {
		if (posX > entrenadorX) {
			entrenador->posX++;
			entrenador->contador_ciclos_CPU++;
			pthread_mutex_lock(&ciclos_CPU_mutex);
			ciclos_CPU++;
			pthread_mutex_unlock(&ciclos_CPU_mutex);
			log_info(extense_logger, "El entrenador %i se encuentra en (%i,%i)", entrenador->index, entrenador->posX, entrenador->posY);
			log_info(logger, "El entrenador %i se encuentra en (%i,%i)", entrenador->index, entrenador->posX, entrenador->posY);
			consumir_tiempo_retardo(entrenador);
		} else {
			entrenador->posX--;
			entrenador->contador_ciclos_CPU++;
			pthread_mutex_lock(&ciclos_CPU_mutex);
			ciclos_CPU++;
			pthread_mutex_unlock(&ciclos_CPU_mutex);
			log_info(extense_logger, "El entrenador %i se encuentra en (%i,%i)", entrenador->index, entrenador->posX, entrenador->posY);
			log_info(logger, "El entrenador %i se encuentra en (%i,%i)", entrenador->index, entrenador->posX, entrenador->posY);
			consumir_tiempo_retardo(entrenador);
		}
	}

	log_info(extense_logger, "El entrenador %i ha llegado a (%i,%i)", entrenador->index, entrenador->posX, entrenador->posY);
}

void consumir_tiempo_retardo(t_entrenador* entrenador) {
	sleep(retardo_de_CPU);
	if (algoritmo_planificacion == 2) {
		entrenador->contador_quantum--;
		if (entrenador->contador_quantum == 0) {
			entrenador->contador_quantum = quantum;
			pthread_mutex_unlock(&planificacion_ready);
			cambiar_estado_de_entrenador(entrenador, ESTADO_READY, "Quantum terminado");
			pthread_mutex_lock(&planificacion_ready);
			cambiar_estado_de_entrenador(entrenador, ESTADO_EXECUTING, "Ejecutando por quantum");
		}
	}
	if (algoritmo_planificacion == 3) {
		entrenador->real_actual++;
	} if (algoritmo_planificacion == 4) {
		entrenador->real_actual++;
		sem_post(&(entrenador->semaforo));
		sem_wait(&(entrenador->semaforo));
	}
}

void intercambiar_pokemones(t_entrenador* entrenador1, t_entrenador* entrenador2, char* pokemon1, char* pokemon2) {
	log_info(extense_logger, "Entrenadores %i y %i intercambiando pokemones %s y %s", entrenador1->index, entrenador2->index, pokemon1, pokemon2);
	log_info(logger, "Entrenadores %i y %i intercambiando pokemones %s y %s", entrenador1->index, entrenador2->index, pokemon1, pokemon2);

	for (int i = 0; i < 5; i++) {
		consumir_tiempo_retardo(entrenador1);
	}

	entrenador1->contador_ciclos_CPU = entrenador1->contador_ciclos_CPU + 5;
	pthread_mutex_lock(&ciclos_CPU_mutex);
	ciclos_CPU = ciclos_CPU + 5;
	pthread_mutex_unlock(&ciclos_CPU_mutex);

	eliminar_pokemon(entrenador1, pokemon1);
	eliminar_pokemon(entrenador2, pokemon2);

	adquirir_pokemon(entrenador1, pokemon2);
	adquirir_pokemon(entrenador2, pokemon1);

	log_info(extense_logger, "Entrenadores %i y %i han intercambiado pokemones %s y %s", entrenador1->index, entrenador2->index, pokemon1, pokemon2);
}

void eliminar_pokemon(t_entrenador* entrenador, char* pokemon) {
	log_info(extense_logger, "Entrenador %i eliminando pokemon %s", entrenador->index, pokemon);
	for (int i = 0; i < entrenador->pokemones->elements_count; i++) {
		char* pokemon_lista = (char*) list_get(entrenador->pokemones, i);
		if (string_equals_ignore_case(pokemon, pokemon_lista) == 1) {
			list_remove(entrenador->pokemones, i);
			i = entrenador->pokemones->elements_count;
		}
	}

	for (int i = 0; i < entrenador->pokemones_innecesarios->elements_count; i++) {
		char* pokemon_lista = (char*) list_get(entrenador->pokemones_innecesarios, i);
		if (string_equals_ignore_case(pokemon, pokemon_lista) == 1) {
			list_remove(entrenador->pokemones_innecesarios, i);
			i = entrenador->pokemones_innecesarios->elements_count;
		}
	}

	entrenador->pokebolas++;

	log_info(extense_logger, "El entrenador %i ha eliminado al pokemon %s", entrenador->index, pokemon);
}

void adquirir_pokemon(t_entrenador* entrenador, char* pokemon) {
	log_info(extense_logger, "Entrenador %i adquiriendo pokemon %s", entrenador->index, pokemon);

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

	entrenador->pokebolas--;

	log_info(extense_logger, "El entrenador %i ha adquirido al pokemon %s", entrenador->index, pokemon);
}

bool es_pokemon_global(char* nombre_pokemon){
	for (int i = 0; i < objetivo_global->elements_count; i++) {
		char* pokemon = (char*) list_get(objetivo_global, i);
		if (string_equals_ignore_case(pokemon, nombre_pokemon)) {
			return true;
		}
	}
	return false;
}

t_list* entrenadores_que_pueden_ir_a_atrapar() {
	t_list* entrenadores_disponibles = list_create();

	for (int i = 0; i < entrenadores->elements_count; i++) {
		t_entrenador* entrenador_lista = (t_entrenador*) list_get(entrenadores, i);
		if (entrenador_lista->estado == ESTADO_NEW ||
				(entrenador_lista->estado == ESTADO_BLOCKED && entrenador_lista->tarea_actual->id_tarea == NO_HACER_PINGO && entrenador_lista->pokebolas != 0)) {
			list_add(entrenadores_disponibles, entrenador_lista);
		}
	}
	if (list_is_empty(entrenadores_disponibles)) {
		log_warning(extense_logger, "No hay entrenadores disponibles para atrapar un pokemon");
	}

	return entrenadores_disponibles;
}

t_entrenador* entrenador_mas_cercano(t_list* entrenadores_disponibles_para_ir_a_atrapar, int posXPokemon, int posYPokemon) {
	t_entrenador* entrenador_seleccionado = list_get(entrenadores_disponibles_para_ir_a_atrapar, 0);
	uint32_t pos_base = calcular_posicion_entrenador(entrenador_seleccionado->posX, entrenador_seleccionado->posY, posXPokemon, posYPokemon);

	if(entrenadores_disponibles_para_ir_a_atrapar->elements_count > 1) {

		for (int i = 1; i < entrenadores_disponibles_para_ir_a_atrapar->elements_count; i++) {
			t_entrenador* entrenador_actual = list_get(entrenadores_disponibles_para_ir_a_atrapar, i);
			uint32_t pos_base_otro_entrenador = calcular_posicion_entrenador(entrenador_actual->posX, entrenador_actual->posY, posXPokemon, posYPokemon);

			if (pos_base_otro_entrenador < pos_base) {
				entrenador_seleccionado = entrenador_actual;
				pos_base = pos_base_otro_entrenador;
			}

		}
	}

	return entrenador_seleccionado;
}

void agrego_pokemon_a_dictionary(t_pokemon* pokemon_a_agregar) {
	pthread_mutex_lock(&dictionary_intentan_atrapar_mutex);
	log_info(extense_logger, "Agregando pokemon %s en (%i,%i) a los pokemones del mapa", pokemon_a_agregar->nombre, pokemon_a_agregar->pos_X, pokemon_a_agregar->pos_Y);
	if(dictionary_has_key(pokemones_conocidos_que_no_se_intentan_atrapar, pokemon_a_agregar->nombre)) {
		t_list* lista_pokemon = dictionary_get(pokemones_conocidos_que_no_se_intentan_atrapar,pokemon_a_agregar->nombre);
		list_add(lista_pokemon,pokemon_a_agregar);
	} else {
		t_list* lista_pokemon_nuevo = list_create();
		list_add(lista_pokemon_nuevo,pokemon_a_agregar);
		dictionary_put(pokemones_conocidos_que_no_se_intentan_atrapar, pokemon_a_agregar->nombre, lista_pokemon_nuevo);
	}
	log_info(extense_logger, "Pokemon %s en (%i,%i) agregado a los pokemones del mapa", pokemon_a_agregar->nombre, pokemon_a_agregar->pos_X, pokemon_a_agregar->pos_Y);
	pthread_mutex_unlock(&dictionary_intentan_atrapar_mutex);
}

t_pokemon* generar_pokemon_de_appeared(t_appeared* mensaje_appeared) {
	// No libero nombre de pokemon de appeared porque ya esta dentro de t_appeared

	t_pokemon* pokemon_a_retornar = malloc(sizeof(t_pokemon));

	pokemon_a_retornar->nombre = mensaje_appeared->pokemon;
	pokemon_a_retornar->pos_X = mensaje_appeared->pos_X;
	pokemon_a_retornar->pos_Y = mensaje_appeared->pos_Y;

	return pokemon_a_retornar;
}

t_list* generar_pokemones_de_localized(t_localized* mensaje_localized) {
	t_list* pokemones_a_retornizar = list_create();

	int j = 0;

	for (int i = 0; i < mensaje_localized->lugares; i++) {
		t_pokemon* pokemon = malloc(sizeof(t_pokemon));
		pokemon->nombre = malloc((strlen(mensaje_localized->pokemon) + 1) * sizeof(char));
		memcpy(pokemon->nombre, mensaje_localized->pokemon, (strlen(mensaje_localized->pokemon) + 1) * sizeof(char));
		pokemon->pos_X = *((uint32_t*) list_get(mensaje_localized->l_coordenadas, j));
		j++;
		pokemon->pos_Y = *((uint32_t*) list_get(mensaje_localized->l_coordenadas, j));
		j++;
		list_add(pokemones_a_retornizar, pokemon);
	}

	return pokemones_a_retornizar;
}

int tengo_que_planificar_pokemon(t_pokemon* pokemon) {
	t_pokemon_planificables* cantidad_disponible_para_atrapar = (t_pokemon_planificables*) dictionary_get(cantidad_de_pokemones_que_puedo_planificar, pokemon->nombre);
	if (cantidad_disponible_para_atrapar == NULL || cantidad_disponible_para_atrapar->cantidad < 1) {
		log_info(extense_logger, "El pokemon %s ubicado en (%i,%i) no se puede planificar", pokemon->nombre, pokemon->pos_X, pokemon->pos_Y);
		return 0;
	} else {
		log_info(extense_logger, "El pokemon %s ubicado en (%i,%i) se puede planificar", pokemon->nombre, pokemon->pos_X, pokemon->pos_Y);
		return 1;
	}
}

void contar_planificacion(t_pokemon* pokemon) {
	t_pokemon_planificables* cantidad_disponible_para_atrapar = (t_pokemon_planificables*) dictionary_get(cantidad_de_pokemones_que_puedo_planificar, pokemon->nombre);
	cantidad_disponible_para_atrapar->cantidad--;
	log_info(extense_logger, "Nueva cantidad de planificaciones disponibles para pokemon %s: %i", pokemon->nombre, cantidad_disponible_para_atrapar->cantidad);
}

int tengo_en_el_mapa(char* pokemon) {
	pthread_mutex_lock(&dictionary_intentan_atrapar_mutex);
	t_list* pokemones_en_el_mapa = (t_list*) dictionary_get(pokemones_conocidos_que_no_se_intentan_atrapar, pokemon);

	if (list_is_empty(pokemones_en_el_mapa) == 1) {
		return 1;
	} else {
		return 0;
	}

	pthread_mutex_unlock(&dictionary_intentan_atrapar_mutex);
}

void liberar_planificacion(t_pokemon* pokemon) {
	t_pokemon_planificables* cantidad_disponible_para_atrapar = (t_pokemon_planificables*) dictionary_get(cantidad_de_pokemones_que_puedo_planificar, pokemon->nombre);
	cantidad_disponible_para_atrapar->cantidad++;
	log_info(extense_logger, "Nueva cantidad de planificaciones disponibles para pokemon %s: %i", pokemon->nombre, cantidad_disponible_para_atrapar->cantidad);
}

void borrar_pokemon_del_mapa(t_pokemon* pokemon) {
	pthread_mutex_lock(&dictionary_intentan_atrapar_mutex);
	log_info(extense_logger, "Eliminando pokemon %s en (%i,%i) del mapa", pokemon->nombre, pokemon->pos_X, pokemon->pos_Y);
	t_list* pokemones = dictionary_get(pokemones_conocidos_que_no_se_intentan_atrapar, pokemon->nombre);
	if (pokemones != NULL) {
		for (int i = 0; i < pokemones->elements_count; i++) {
			t_pokemon* pokemon_lista = list_get(pokemones, i);
			if (pokemon_lista->pos_X == pokemon->pos_X && pokemon_lista->pos_Y == pokemon->pos_Y) {
				list_remove(pokemones, i);
				i = pokemones->elements_count;
			}
		}
	}
	pthread_mutex_unlock(&dictionary_intentan_atrapar_mutex);
}

t_pokemon* mejor_pokemon_para_reintentar(t_entrenador* entrenador, char* pokemon) {
	pthread_mutex_lock(&dictionary_intentan_atrapar_mutex);
	t_list* pokemones = dictionary_get(pokemones_conocidos_que_no_se_intentan_atrapar, pokemon);

	int distancia_minima = 500000;
	t_pokemon* pokemon_a_retornizar;
	for (int i = 0; i < pokemones->elements_count; i++) {
		t_pokemon* pokemon_lista = list_get(pokemones, i);
		int distancia = calcular_posicion_entrenador(entrenador->posX, entrenador->posY, pokemon_lista->pos_X, pokemon_lista->pos_Y);
		if (distancia < distancia_minima) {
			distancia_minima = distancia;
			pokemon_a_retornizar = pokemon_lista;
		}
	}
	pthread_mutex_unlock(&dictionary_intentan_atrapar_mutex);

	return pokemon_a_retornizar;
}

void cambiar_estado_de_entrenador(t_entrenador* entrenador, estado estado_nuevo, char* razon) {
	log_info(extense_logger, "Cambiando estado de entrenador %i de %s a %s", entrenador->index, obtener_nombre_de_estado(entrenador->estado), obtener_nombre_de_estado(estado_nuevo));
	if (entrenador->estado != estado_nuevo) {
		entrenador->estado = estado_nuevo;
		pthread_mutex_lock(&cambios_contexto_mutex);
		cambios_contexto++;
		pthread_mutex_unlock(&cambios_contexto_mutex);
	}
	log_info(extense_logger, "Estado de entrenador %i cambiado a %s. Razon del cambio: %s", entrenador->index, obtener_nombre_de_estado(estado_nuevo), razon);
	log_info(logger, "Estado de entrenador %i cambiado a %s. Razon del cambio: %s", entrenador->index, obtener_nombre_de_estado(estado_nuevo), razon);
}

void cambiar_tarea_de_entrenador(t_entrenador* entrenador, t_tarea* tarea_nueva) {
	log_info(extense_logger, "Cambiando tarea de entrenador %i de %s a %s", entrenador->index, obtener_nombre_de_tarea(entrenador->tarea_actual->id_tarea), obtener_nombre_de_tarea(tarea_nueva->id_tarea));
	switch(entrenador->tarea_actual->id_tarea) {
	case NO_HACER_PINGO:
		free(entrenador->tarea_actual);
		break;
	case ATRAPAR_POKEMON:
		free(entrenador->tarea_actual->parametros);
		free(entrenador->tarea_actual);
		break;
	case INTERCAMBIAR_POKEMON:
		free(entrenador->tarea_actual);
		break;
	}
	entrenador->tarea_actual = tarea_nueva;
	log_info(extense_logger, "Tarea de entrenador %i cambiada a %s", entrenador->index, obtener_nombre_de_tarea(tarea_nueva->id_tarea));
}

void mostrar_metricas() {
	log_info(extense_logger, "Proceso Team terminado. Cantidad de ciclos de CPU totales: %i", ciclos_CPU);
	log_info(logger, "Proceso Team terminado. Cantidad de ciclos de CPU totales: %i", ciclos_CPU);

	for (int i = 0; i < entrenadores->elements_count; i++) {
		t_entrenador* entrenador_lista = list_get(entrenadores, i);
		log_info(extense_logger, "Proceso Team terminado. Cantidad de ciclos de CPU del entrenador %i: %i", entrenador_lista->index, entrenador_lista->contador_ciclos_CPU);
		log_info(logger, "Cantidad de ciclos de CPU del entrenador %i: %i", entrenador_lista->index, entrenador_lista->contador_ciclos_CPU);
	}

	log_info(extense_logger, "Proceso Team terminado. Cantidad de cambios de contexto realizados: %i", cambios_contexto);
	log_info(logger, "Proceso Team terminado. Cantidad de cambios de contexto realizados: %i", cambios_contexto);
	log_info(extense_logger, "Proceso Team terminado. Cantidad de deadlocks encontrados y tratados: %i", cantidad_deadlocks);
	log_info(logger, "Proceso Team terminado. Cantidad de deadlocks encontrados y tratados: %i", cantidad_deadlocks);
}

int es_id_catch(uint32_t id) {
	int es_el_id_buscado(void* id_lista) {
		if (((t_catch_id*) id_lista)->id_catch == id) {
			return 1;
		} else {
			return 0;
		}
	}

	int es_id_de_catch;

	pthread_mutex_lock(&catch_IDs_mutex);
	// si el id del caught no esta en la lista de catch recibidos, ignora el mensaje
	if (list_find(catch_IDs, (void*) es_el_id_buscado) == NULL) {
		log_info(logger, "el ID correlativo %i de caught no corresponde a ningun catch, mensaje ignorado", id);
		es_id_de_catch = 0;
	} else {
		es_id_de_catch = 1;
	}
	pthread_mutex_unlock(&catch_IDs_mutex);

	return es_id_de_catch;
}

void definir_nueva_estimacion(t_entrenador* entrenador) {
	entrenador->real = entrenador->real_actual;
	entrenador->real_actual = 0;
	entrenador->estimacion = calcular_nueva_estimacion(entrenador);
}

double calcular_nueva_estimacion(t_entrenador* entrenador) {
	double estimacion_alpheada = entrenador->estimacion * alpha;
	double real_alpheado = entrenador->real * (1 - alpha);

	return estimacion_alpheada + real_alpheado;
}

int calcular_posicion_entrenador(int posXEntrenador, int posYEntrenador, int posXPokemon, int posYPokemon) {
	return abs(posXEntrenador - posXPokemon) + abs(posYEntrenador - posYPokemon);
}

char* obtener_nombre_de_tarea(id_tarea id) {
	switch(id) {
	case ATRAPAR_POKEMON:
		return "Atrapar pokemon";
	case INTERCAMBIAR_POKEMON:
		return "Intercambiar pokemon";
	case NO_HACER_PINGO:
		return "No hacer nada";
	default:
		return "";
	}
}

char* obtener_nombre_de_estado(estado id) {
	switch(id) {
	case ESTADO_NEW:
		return "Estado new";
	case ESTADO_READY:
		return "Estado ready";
	case ESTADO_EXECUTING:
		return "Estado executing";
	case ESTADO_BLOCKED:
		return "Estado blocked";
	case ESTADO_EXIT:
		return "Estado exit";
	default:
		return "";
	}
}

int cantidad_de_entrenadores_en_ready_o_exec(void) {
	int cantidad = 0;
	for (int i = 0; i < entrenadores->elements_count; i++) {
		t_entrenador* entrenador_lista = (t_entrenador*) list_get(entrenadores, i);
		if (entrenador_lista->estado == ESTADO_READY || entrenador_lista->estado == ESTADO_EXECUTING) {
			cantidad++;
		}
	}
	return cantidad;
}

void mandar_a_siguente_entrenador_a_ejecutar_por_sjf(void) {
	t_entrenador* entrenador_con_estimacion_mas_baja = NULL;
	double estimacion_mas_baja = 50000;

	for (int i = 0; i < entrenadores->elements_count; i++) {
		t_entrenador* entrenador_lista = (t_entrenador*) list_get(entrenadores, i);
		if (entrenador_lista->estado == ESTADO_READY && (entrenador_lista->estimacion < estimacion_mas_baja || (entrenador_lista->estimacion == estimacion_mas_baja && entrenador_lista->llegada < entrenador_con_estimacion_mas_baja->llegada))) {
			entrenador_con_estimacion_mas_baja = entrenador_lista;
			estimacion_mas_baja = entrenador_lista->estimacion;
		}
	}

	if (entrenador_con_estimacion_mas_baja != NULL) {
		sem_post(&entrenador_con_estimacion_mas_baja->semaforo);
		cambiar_estado_de_entrenador(entrenador_con_estimacion_mas_baja, ESTADO_EXECUTING, "Ejecutando tarea");
	}

}

void desalojar_si_es_necesario(t_entrenador* entrenador_planificado) {
	t_entrenador* entrenador_ejecutando = NULL;

	for (int i = 0; i < entrenadores->elements_count; i++) {
		t_entrenador* entrenador_lista = (t_entrenador*) list_get(entrenadores, i);
		if (entrenador_lista->estado == ESTADO_EXECUTING) {
			entrenador_ejecutando = entrenador_lista;
		}
	}

	if (entrenador_ejecutando != NULL) {
		if (entrenador_ejecutando->estimacion > entrenador_planificado->estimacion) {
			sem_wait(&(entrenador_ejecutando->semaforo));
			sem_post(&(entrenador_planificado->semaforo));
			cambiar_estado_de_entrenador(entrenador_ejecutando, ESTADO_BLOCKED, "Desalojado");
			cambiar_estado_de_entrenador(entrenador_planificado, ESTADO_EXECUTING, "Ejecutando tarea");
		}
	}
}

int obtener_id_llegada(void) {
	int aux;
	pthread_mutex_lock(&llegada_mutex);
	id_llegada++;
	aux = id_llegada;
	pthread_mutex_unlock(&llegada_mutex);
	return aux;
}

void generar_ID_Modulo() {
	id_modulo = rand();
	id_modulo_protocol = id_modulo;
}

void desconectar_broker() {
	pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
	if (estoy_conectado_al_broker == 1) {
		estoy_conectado_al_broker = 0;
		sem_wait(&sem_conexion_appeared);
		sem_wait(&sem_conexion_caught);
		sem_wait(&sem_conexion_localized);
	}
	pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
}

void terminar_programa() {
	free(ip_broker);
	free(puerto_broker);

	if (socket_escucha_game_boy != -1) {
		cerrar_conexion(socket_escucha_game_boy);
	}
	if (socket_escucha_appeared != -1) {
		cerrar_conexion(socket_escucha_appeared);
	}
	if (socket_escucha_caught != -1) {
		cerrar_conexion(socket_escucha_caught);
	}
	if (socket_escucha_localized != -1) {
		cerrar_conexion(socket_escucha_localized);
	}

	// liberar entrenadores?

	for (int i = 0; i < entrenadores->elements_count; i++) {
		t_entrenador* entrenador_lista = list_get(entrenadores, i);

		// ESTO TIRA INVALID FREE

		//		for (int k = 0; k < entrenador_lista->objetivos->elements_count; k++) {
		//			free(list_get(entrenador_lista->objetivos, k));
		//		}
		//		for (int k = 0; k < entrenador_lista->objetivos_actuales->elements_count; k++) {
		//			free(list_get(entrenador_lista->objetivos_actuales, k));
		//		}
		//		for (int k = 0; k < entrenador_lista->pokemones->elements_count; k++) {
		//			free(list_get(entrenador_lista->pokemones, k));
		//		}
		//		for (int k = 0; k < entrenador_lista->pokemones_innecesarios->elements_count; k++) {
		//			free(list_get(entrenador_lista->pokemones_innecesarios, k));
		//		}

		list_destroy(entrenador_lista->objetivos);
		list_destroy(entrenador_lista->objetivos_actuales);
		list_destroy(entrenador_lista->pokemones);
		list_destroy(entrenador_lista->pokemones_innecesarios);

		sem_destroy(&(entrenador_lista->semaforo));
		free(entrenador_lista->tarea_actual);
		free(entrenador_lista);
	}

	list_destroy(entrenadores);
	dictionary_destroy(pokemones_conocidos_que_no_se_intentan_atrapar);
	dictionary_destroy(cantidad_de_pokemones_que_puedo_planificar);

	sem_destroy(&sem_cola_pokemones);
	sem_destroy(&sem_cola_caught);
	sem_destroy(&sem_cola_deadlock);

	sem_destroy(&sem_conexion_appeared);
	sem_destroy(&sem_conexion_caught);
	sem_destroy(&sem_conexion_localized);

	pthread_mutex_destroy(&cola_pokemones_mutex);
	pthread_mutex_destroy(&cola_caught_mutex);
	pthread_mutex_destroy(&llegada_mutex);
	pthread_mutex_destroy(&cola_deadlock_mutex);
	pthread_mutex_destroy(&pokemones_llegados_mutex);
	pthread_mutex_destroy(&catch_IDs_mutex);
	pthread_mutex_destroy(&estoy_conectado_al_broker_mutex);
	pthread_mutex_destroy(&ciclos_CPU_mutex);
	pthread_mutex_destroy(&cambios_contexto_mutex);
	pthread_mutex_destroy(&cantidad_deadlocks_mutex);
	pthread_mutex_destroy(&planificacion_ready);
	pthread_mutex_destroy(&planificacion_sjf);
	pthread_mutex_destroy(&dictionary_intentan_atrapar_mutex);

	log_destroy(logger);
	log_destroy(extense_logger);

	queue_clean(cola_pokemones);

	queue_destroy(cola_pokemones);

	//config_destroy(config);

	for (int i = 0; i < objetivo_global->elements_count; i++) {
		free((char*) list_get(objetivo_global, i));
	}

	list_destroy(objetivo_global);

	list_destroy(pokemones_a_localizar);

	for (int i = 0; i < pokemones_llegados->elements_count; i++) {
		free((char*) list_get(pokemones_llegados, i));
	}

	list_destroy(pokemones_llegados);
}
