#include "broker.h"

void procesar_request(int cod_op, int cliente_fd) {
	uint32_t size;

	mensaje_queue* mensaje_a_guardar = malloc(sizeof(mensaje_queue));
	log_info(extense_logger, "Generando id para el mensaje del socket %i", cliente_fd);
	mensaje_a_guardar->id = generar_id();
	log_info(extense_logger, "El id generado es %i", mensaje_a_guardar->id);

	switch (cod_op) {
	case NEW: ;
		log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un NEW", cliente_fd);
		t_new* new_msg = recibir_new(cliente_fd, &size, extense_logger);

		if (NULL == new_msg) {
			free(mensaje_a_guardar);
			pthread_exit(NULL);
		} else {
			mensaje_a_guardar->mensaje = (void*) new_msg;

			sem_wait(&gl_new_limite);

			log_info(extense_logger, "Encolando el mensaje NEW con id %i", mensaje_a_guardar->id);
			pthread_mutex_lock(&gl_new_queue_lock);
			queue_push(gl_new_pokemon_queue, mensaje_a_guardar);
			pthread_mutex_unlock(&gl_new_queue_lock);
			log_info(extense_logger, "Mensaje NEW con id %i encolado", mensaje_a_guardar->id);
			log_info(logger, "Mensaje NEW con id %i encolado", mensaje_a_guardar->id);

			sem_post(&gl_new_mensajes);

			devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

			close(cliente_fd);
		}
		break;
	case APPEARED: ;
		log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un APPEARED", cliente_fd);
		t_appeared* appeared_msg = recibir_appeared(cliente_fd, &size, extense_logger);

		if (NULL == appeared_msg) {
			free(mensaje_a_guardar);
			pthread_exit(NULL);
		} else {
			mensaje_a_guardar->mensaje = (void*) appeared_msg;

			sem_wait(&gl_appeared_limite);

			log_info(extense_logger, "Encolando el mensaje APPEARED con id %i", mensaje_a_guardar->id);
			pthread_mutex_lock(&gl_appeared_queue_lock);
			queue_push(gl_appeared_pokemon_queue, mensaje_a_guardar);
			pthread_mutex_unlock(&gl_appeared_queue_lock);
			log_info(extense_logger, "Mensaje APPEARED con id %i encolado", mensaje_a_guardar->id);
			log_info(logger, "Mensaje APPEARED con id %i encolado", mensaje_a_guardar->id);

			sem_post(&gl_appeared_mensajes);

			devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

			close(cliente_fd);
		}
		break;
	case GET: ;
		log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un GET", cliente_fd);
		t_get* get_msg = recibir_get(cliente_fd, &size, extense_logger);

		if (NULL == get_msg) {
			free(mensaje_a_guardar);
			pthread_exit(NULL);
		} else {
			mensaje_a_guardar->mensaje = (void*) get_msg;

			sem_wait(&gl_get_limite);

			log_info(extense_logger, "Encolando el mensaje GET con id %i", mensaje_a_guardar->id);
			pthread_mutex_lock(&gl_get_queue_lock);
			queue_push(gl_get_pokemon_queue, mensaje_a_guardar);
			pthread_mutex_unlock(&gl_get_queue_lock);
			log_info(extense_logger, "Mensaje GET con id %i encolado", mensaje_a_guardar->id);
			log_info(logger, "Mensaje GET con id %i encolado", mensaje_a_guardar->id);

			sem_post(&gl_get_mensajes);

			devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

			close(cliente_fd);
		}
		break;
	case LOCALIZED: ;
		log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un LOCALIZED", cliente_fd);
		t_localized* localized_msg = recibir_localized(cliente_fd, &size, extense_logger);

		if (NULL == localized_msg) {
			free(mensaje_a_guardar);
			pthread_exit(NULL);
		} else {
			mensaje_a_guardar->mensaje = (void*) localized_msg;

			sem_wait(&gl_localized_limite);

			log_info(extense_logger, "Encolando el mensaje LOCALIZED con id %i", mensaje_a_guardar->id);
			pthread_mutex_lock(&gl_localized_queue_lock);
			queue_push(gl_localized_pokemon_queue, mensaje_a_guardar);
			pthread_mutex_unlock(&gl_localized_queue_lock);
			log_info(extense_logger, "Mensaje LOCALIZED con id %i encolado", mensaje_a_guardar->id);
			log_info(logger, "Mensaje LOCALIZED con id %i encolado", mensaje_a_guardar->id);

			sem_post(&gl_localized_mensajes);

			devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

			close(cliente_fd);
		}
		break;
	case CATCH: ;
		log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un CATCH", cliente_fd);
		t_catch* catch_msg = recibir_catch(cliente_fd, &size, extense_logger);

		if (NULL == catch_msg) {
			free(mensaje_a_guardar);
			pthread_exit(NULL);
		} else {
			mensaje_a_guardar->mensaje = (void*) catch_msg;

			sem_wait(&gl_catch_limite);

			log_info(extense_logger, "Encolando el mensaje CATCH con id %i", mensaje_a_guardar->id);
			pthread_mutex_lock(&gl_catch_queue_lock);
			queue_push(gl_catch_pokemon_queue, mensaje_a_guardar);
			pthread_mutex_unlock(&gl_catch_queue_lock);
			log_info(extense_logger, "Mensaje CATCH con id %i encolado", mensaje_a_guardar->id);
			log_info(logger, "Mensaje CATCH con id %i encolado", mensaje_a_guardar->id);

			sem_post(&gl_catch_mensajes);

			devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

			close(cliente_fd);
		}
		break;
	case CAUGHT: ;
		log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un CAUGHT", cliente_fd);
		t_caught* caught_msg = recibir_caught(cliente_fd, &size, extense_logger);

		if (NULL == caught_msg) {
			free(mensaje_a_guardar);
			pthread_exit(NULL);
		} else {
			mensaje_a_guardar->mensaje = (void*) caught_msg;

			sem_wait(&gl_caught_limite);

			log_info(extense_logger, "Encolando el mensaje CAUGHT con id %i", mensaje_a_guardar->id);
			pthread_mutex_lock(&gl_caught_queue_lock);
			queue_push(gl_caught_pokemon_queue, mensaje_a_guardar);
			pthread_mutex_unlock(&gl_caught_queue_lock);
			log_info(extense_logger, "Mensaje CAUGHT con id %i encolado", mensaje_a_guardar->id);
			log_info(logger, "Mensaje CAUGHT con id %i encolado", mensaje_a_guardar->id);

			sem_post(&gl_caught_mensajes);

			devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

			close(cliente_fd);
		}
		break;
	case SUSCRIPCION: ;
		log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponse a un SUSCRIPCION", cliente_fd);
		t_suscripcion* suscripcion = recibir_suscripcion(cliente_fd, &size, extense_logger);

		if (NULL == suscripcion) {
			free(mensaje_a_guardar);
			pthread_exit(NULL);
		}

		info_modulo* info_modulo = malloc(sizeof(info_modulo));
		info_modulo->ip = suscripcion->ip;
		info_modulo->puerto = suscripcion->puerto;

		switch(suscripcion->id_cola) {
		case NEW_ID: ;
			log_info(extense_logger, "Id de cola recibido del socket cliente %i corresponde a la cola de NEW", cliente_fd);

			pthread_mutex_lock(&gl_new_list_lock);
			list_add(gl_new_suscriptores, (void *) &info_modulo);
			pthread_mutex_unlock(&gl_new_list_lock);
			enviar_new_de_memoria(info_modulo);
			log_info(extense_logger, "Modulo con ip %s y puerto %s agregado correctamente en la cola de NEW", suscripcion->ip, suscripcion->puerto);
			log_info(logger, "Modulo con ip %s y puerto %s agregado correctamente en la cola de NEW", suscripcion->ip, suscripcion->puerto);
			break;
		case APPEARED_ID: ;
			log_info(extense_logger, "Id de cola recibido del socket cliente %i corresponde a la cola de APPEARED", cliente_fd);
			pthread_mutex_lock(&gl_appeared_list_lock);
			list_add(gl_appeared_suscriptores, (void *) &info_modulo);
			pthread_mutex_unlock(&gl_appeared_list_lock);
			enviar_appeared_de_memoria(info_modulo);
			log_info(extense_logger, "Modulo con ip %s y puerto %s agregado correctamente en la cola de APPEARED", suscripcion->ip, suscripcion->puerto);
			log_info(logger, "Modulo con ip %s y puerto %s agregado correctamente en la cola de APPEARED", suscripcion->ip, suscripcion->puerto);
			break;
		case CATCH_ID: ;
			log_info(extense_logger, "Id de cola recibido del socket cliente %i corresponde a la cola de CATCH", cliente_fd);
			pthread_mutex_lock(&gl_catch_list_lock);
			list_add(gl_catch_suscriptores, (void *) &info_modulo);
			pthread_mutex_unlock(&gl_catch_list_lock);
			enviar_catch_de_memoria(info_modulo);
			log_info(extense_logger, "Modulo con ip %s y puerto %s agregado correctamente en la cola de CATCH", suscripcion->ip, suscripcion->puerto);
			log_info(logger, "Modulo con ip %s y puerto %s agregado correctamente en la cola de CATCH", suscripcion->ip, suscripcion->puerto);
			break;
		case CAUGHT_ID: ;
			log_info(extense_logger, "Id de cola recibido del socket cliente %i corresponde a la cola de CAUGHT", cliente_fd);
			pthread_mutex_lock(&gl_caught_list_lock);
			list_add(gl_caught_suscriptores, (void *) &info_modulo);
			pthread_mutex_unlock(&gl_caught_list_lock);
			enviar_caught_de_memoria(info_modulo);
			log_info(extense_logger, "Modulo con ip %s y puerto %s agregado correctamente en la cola de CAUGHT", suscripcion->ip, suscripcion->puerto);
			log_info(logger, "Modulo con ip %s y puerto %s agregado correctamente en la cola de CAUGHT", suscripcion->ip, suscripcion->puerto);
			break;
		case GET_ID: ;
			log_info(extense_logger, "Id de cola recibido del socket cliente %i corresponde a la cola de GET", cliente_fd);
			pthread_mutex_lock(&gl_get_list_lock);
			list_add(gl_get_suscriptores, (void *) &info_modulo);
			pthread_mutex_unlock(&gl_get_list_lock);
			enviar_get_de_memoria(info_modulo);
			log_info(extense_logger, "Modulo con ip %s y puerto %s agregado correctamente en la cola de GET", suscripcion->ip, suscripcion->puerto);
			log_info(logger, "Modulo con ip %s y puerto %s agregado correctamente en la cola de GET", suscripcion->ip, suscripcion->puerto);
			break;
		case LOCALIZED_ID: ;
			log_info(extense_logger, "Id de cola recibido del socket cliente %i corresponde a la cola de LOCALIZED", cliente_fd);
			pthread_mutex_lock(&gl_localized_list_lock);
			list_add(gl_localized_suscriptores, (void *) &info_modulo);
			pthread_mutex_unlock(&gl_localized_list_lock);
			enviar_localized_de_memoria(info_modulo);
			log_info(extense_logger, "Modulo con ip %s y puerto %s agregado correctamente en la cola de LOCALIZED", suscripcion->ip, suscripcion->puerto);
			log_info(logger, "Modulo con ip %s y puerto %s agregado correctamente en la cola de LOCALIZED", suscripcion->ip, suscripcion->puerto);
			break;
		}

		free(suscripcion);

		close(cliente_fd);
		break;
	default:
		log_warning(extense_logger, "El codigo de operacion %i no corresponde a ninguna operacion conocida", cod_op);
		close(cliente_fd);
		pthread_exit(NULL);
	}
}

void atender_cliente(int* socket) {
	int cod_op;
	log_info(extense_logger, "Recibiendo codigo de operacion de socket %i", *socket);
	if(recv(*socket, &cod_op, sizeof(uint32_t), MSG_WAITALL) == -1) {
		cod_op = -1;
		log_error(extense_logger, "Hubo un problema recibiendo codigo de operacion de socket %i", *socket);
	}
	log_info(extense_logger, "Codigo de operacion de socket %i recibido: %i", *socket, cod_op);
	procesar_request(cod_op, *socket);
}

void esperar_cliente(int socket_servidor) {
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	log_info(extense_logger, "Socket cliente %i aceptado", socket_cliente);
	log_info(logger, "Nueva conexion de un proceso con socket cliente %i", socket_cliente);

	pthread_t thread;

	pthread_create(&thread, NULL, (void*) atender_cliente, &socket_cliente);
	pthread_detach(thread);
}

void esperar_clientes(void* socket_servidor) {
	int socket = (int) socket_servidor;
	log_info(extense_logger, "Esperando clientes en socket %i", socket);
	while(1) {
		esperar_cliente(socket);
	}
}

void* atender_new(void* args) {
	log_info(extense_logger, "Atendiendo cola NEW");
	while (1) {
		sem_wait(&gl_new_mensajes);
		pthread_mutex_lock(&gl_new_queue_lock);

		mensaje_queue* primer_elemento = (mensaje_queue*) queue_peek(gl_new_pokemon_queue);

		pthread_mutex_unlock(&gl_new_queue_lock);

		log_info(extense_logger, "Atendiendo mensaje NEW con id %i", primer_elemento->id);

		atender_mensaje_new(primer_elemento);

		log_info(extense_logger, "Eliminando mensaje NEW con id %i", primer_elemento->id);

		pthread_mutex_lock(&gl_new_queue_lock);

		mensaje_queue* elemento_a_eliminar = queue_pop(gl_new_pokemon_queue);

		pthread_mutex_unlock(&gl_new_queue_lock);
		sem_post(&gl_new_limite);

		t_new* mensaje_a_eliminar = (t_new*) elemento_a_eliminar->mensaje;

		log_info(extense_logger, "Eliminado mensaje NEW con id %i", primer_elemento->id);

		free(mensaje_a_eliminar->pokemon);
		free(mensaje_a_eliminar);
		free(elemento_a_eliminar);
	}
}

void* atender_appeared(void* args) {
	log_info(extense_logger, "Atendiendo cola APPEARED");
	while (1) {
		sem_wait(&gl_appeared_mensajes);
		pthread_mutex_lock(&gl_appeared_queue_lock);

		mensaje_queue* primer_elemento = (mensaje_queue*) queue_peek(gl_appeared_pokemon_queue);

		pthread_mutex_unlock(&gl_appeared_queue_lock);

		log_info(extense_logger, "Atendiendo mensaje APPEARED con id %i", primer_elemento->id);

		atender_mensaje_appeared(primer_elemento);

		log_info(extense_logger, "Eliminando mensaje APPEARED con id %i", primer_elemento->id);

		pthread_mutex_lock(&gl_appeared_queue_lock);

		mensaje_queue* elemento_a_eliminar = (mensaje_queue*) queue_pop(gl_appeared_pokemon_queue);

		pthread_mutex_unlock(&gl_appeared_queue_lock);
		sem_post(&gl_appeared_limite);

		t_appeared* mensaje_a_eliminar = (t_appeared*) elemento_a_eliminar->mensaje;

		log_info(extense_logger, "Eliminado mensaje APPEARED con id %i", primer_elemento->id);

		free(mensaje_a_eliminar->pokemon);
		free(mensaje_a_eliminar);
		free(elemento_a_eliminar);
	}
}

void* atender_catch(void* args) {
	log_info(extense_logger, "Atendiendo cola CATCH");
	while (1) {
		sem_wait(&gl_catch_mensajes);
		pthread_mutex_lock(&gl_catch_queue_lock);

		mensaje_queue* primer_elemento = (mensaje_queue*) queue_peek(gl_catch_pokemon_queue);

		pthread_mutex_unlock(&gl_catch_queue_lock);

		log_info(extense_logger, "Atendiendo mensaje CATCH con id %i", primer_elemento->id);

		atender_mensaje_catch(primer_elemento);

		log_info(extense_logger, "Eliminando mensaje CATCH con id %i", primer_elemento->id);

		pthread_mutex_lock(&gl_catch_queue_lock);

		mensaje_queue* elemento_a_eliminar = (mensaje_queue*) queue_pop(gl_catch_pokemon_queue);

		pthread_mutex_unlock(&gl_catch_queue_lock);
		sem_post(&gl_catch_limite);

		t_catch* mensaje_a_eliminar = (t_catch*) elemento_a_eliminar->mensaje;

		log_info(extense_logger, "Eliminado mensaje CATCH con id %i", primer_elemento->id);

		free(mensaje_a_eliminar->pokemon);
		free(mensaje_a_eliminar);
		free(elemento_a_eliminar);
	}
}

void* atender_caught(void* args) {
	log_info(extense_logger, "Atendiendo cola CAUGHT");
	while (1) {
		sem_wait(&gl_caught_mensajes);
		pthread_mutex_lock(&gl_caught_queue_lock);

		mensaje_queue* primer_elemento = (mensaje_queue*) queue_peek(gl_caught_pokemon_queue);

		pthread_mutex_unlock(&gl_caught_queue_lock);

		log_info(extense_logger, "Atendiendo mensaje CAUGHT con id %i", primer_elemento->id);

		atender_mensaje_caught(primer_elemento);

		log_info(extense_logger, "Eliminando mensaje CAUGHT con id %i", primer_elemento->id);

		pthread_mutex_lock(&gl_caught_queue_lock);

		mensaje_queue* elemento_a_eliminar = (mensaje_queue*) queue_pop(gl_caught_pokemon_queue);

		pthread_mutex_unlock(&gl_caught_queue_lock);
		sem_post(&gl_caught_limite);

		t_caught* mensaje_a_eliminar = (t_caught*) elemento_a_eliminar->mensaje;

		log_info(extense_logger, "Eliminado mensaje CAUGHT con id %i", primer_elemento->id);

		free(mensaje_a_eliminar);
		free(elemento_a_eliminar);
	}
}

void* atender_get(void* args) {
	log_info(extense_logger, "Atendiendo cola GET");
	while (1) {
		sem_wait(&gl_get_mensajes);
		pthread_mutex_lock(&gl_get_queue_lock);

		mensaje_queue* primer_elemento = (mensaje_queue*) queue_peek(gl_get_pokemon_queue);

		pthread_mutex_unlock(&gl_get_queue_lock);

		log_info(extense_logger, "Atendiendo mensaje GET con id %i", primer_elemento->id);

		atender_mensaje_get(primer_elemento);

		log_info(extense_logger, "Eliminando mensaje GET con id %i", primer_elemento->id);

		pthread_mutex_lock(&gl_get_queue_lock);

		mensaje_queue* elemento_a_eliminar = (mensaje_queue*) queue_pop(gl_get_pokemon_queue);

		pthread_mutex_unlock(&gl_get_queue_lock);
		sem_post(&gl_get_limite);

		t_get* mensaje_a_eliminar = (t_get*) elemento_a_eliminar->mensaje;

		log_info(extense_logger, "Eliminado mensaje GET con id %i", primer_elemento->id);

		free(mensaje_a_eliminar->pokemon);
		free(mensaje_a_eliminar);
		free(elemento_a_eliminar);
	}
}

void* atender_localized(void* args) {
	log_info(extense_logger, "Atendiendo cola LOCALIZED");
	while (1) {
		sem_wait(&gl_localized_mensajes);
		pthread_mutex_lock(&gl_localized_queue_lock);

		mensaje_queue* primer_elemento = (mensaje_queue*) queue_peek(gl_localized_pokemon_queue);

		pthread_mutex_unlock(&gl_localized_queue_lock);

		log_info(extense_logger, "Atendiendo mensaje LOCALIZED con id %i", primer_elemento->id);

		atender_mensaje_localized(primer_elemento);

		log_info(extense_logger, "Eliminando mensaje LOCALIZED con id %i", primer_elemento->id);

		pthread_mutex_lock(&gl_localized_queue_lock);

		mensaje_queue* elemento_a_eliminar = (mensaje_queue*) queue_pop(gl_localized_pokemon_queue);

		pthread_mutex_unlock(&gl_localized_queue_lock);
		sem_post(&gl_localized_limite);

		t_localized* mensaje_a_eliminar = (t_localized*) elemento_a_eliminar->mensaje;

		log_info(extense_logger, "Eliminado mensaje LOCALIZED con id %i", primer_elemento->id);

		free(mensaje_a_eliminar->pokemon);
		list_destroy(mensaje_a_eliminar->l_coordenadas);
		free(mensaje_a_eliminar->l_coordenadas);
		free(mensaje_a_eliminar);
		free(elemento_a_eliminar);
	}
}

void atender_mensaje_new(mensaje_queue* mensaje) {
	pthread_mutex_lock(&gl_new_list_lock);
	t_list* suscriptores_para_enviar = list_duplicate(gl_new_suscriptores);
	pthread_mutex_unlock(&gl_new_list_lock);

	data_tabla* registro = malloc(sizeof(data_tabla));

	registro->id = mensaje->id;
	registro->tipo = NEW;

	uint32_t bytes;

	void* mensaje_memoria = serializar_new(mensaje, &bytes);

	registro->limit = bytes;

	guardar_mensaje_en_memoria(registro, mensaje_memoria, logger);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	mandable_struct* struct_para_enviar = malloc(sizeof(mandable_struct) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		struct_para_enviar[i].info = list_get(suscriptores_para_enviar, i);
		struct_para_enviar[i].mensaje_queue = mensaje;
		pthread_create(&(threads[i]), NULL, (void*) mandar_new, (void *) &struct_para_enviar[i]);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	t_list* mandados = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->envio_ok == 1) {
			list_add(mandados, list_get(suscriptores_para_enviar, i));
		}
	}

	t_list* acks = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->ack == 1) {
			list_add(acks, list_get(suscriptores_para_enviar, i));
		}
	}

	guardar_info_envios(mensaje->id, mandados, acks);

	free(threads);
	free(struct_para_enviar);
	list_clean(suscriptores_para_enviar);
	list_destroy(suscriptores_para_enviar);
}

void atender_mensaje_appeared(mensaje_queue* mensaje) {
	pthread_mutex_lock(&gl_appeared_list_lock);
	t_list* suscriptores_para_enviar = list_duplicate(gl_appeared_suscriptores);
	pthread_mutex_unlock(&gl_appeared_list_lock);

	data_tabla* registro = malloc(sizeof(data_tabla));

	registro->id = mensaje->id;
	registro->id_correlativo = ((t_localized*) mensaje->mensaje)->id_correlativo;
	registro->tipo = APPEARED;

	uint32_t bytes;

	void* mensaje_memoria = serializar_appeared(mensaje, &bytes);

	registro->limit = bytes;

	guardar_mensaje_en_memoria(registro, mensaje_memoria, logger);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	mandable_struct* struct_para_enviar = malloc(sizeof(mandable_struct) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		struct_para_enviar[i].info = list_get(suscriptores_para_enviar, i);
		struct_para_enviar[i].mensaje_queue = mensaje;
		pthread_create(&(threads[i]), NULL, (void*) mandar_appeared, (void *) &struct_para_enviar[i]);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	t_list* mandados = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->envio_ok == 1) {
			list_add(mandados, list_get(suscriptores_para_enviar, i));
		}
	}

	t_list* acks = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->ack == 1) {
			list_add(acks, list_get(suscriptores_para_enviar, i));
		}
	}

	guardar_info_envios(mensaje->id, mandados, acks);

	free(threads);
	free(struct_para_enviar);
	list_clean(suscriptores_para_enviar);
	list_destroy(suscriptores_para_enviar);
}

void atender_mensaje_catch(mensaje_queue* mensaje) {
	pthread_mutex_lock(&gl_catch_list_lock);
	t_list* suscriptores_para_enviar = list_duplicate(gl_catch_suscriptores);
	pthread_mutex_unlock(&gl_catch_list_lock);

	data_tabla* registro = malloc(sizeof(data_tabla));

	registro->id = mensaje->id;
	registro->tipo = CATCH;

	uint32_t bytes;

	void* mensaje_memoria = serializar_catch(mensaje, &bytes);

	registro->limit = bytes;

	guardar_mensaje_en_memoria(registro, mensaje_memoria, logger);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	mandable_struct* struct_para_enviar = malloc(sizeof(mandable_struct) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		struct_para_enviar[i].info = list_get(suscriptores_para_enviar, i);
		struct_para_enviar[i].mensaje_queue = mensaje;
		pthread_create(&(threads[i]), NULL, (void*) mandar_catch, (void *) &struct_para_enviar[i]);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	t_list* mandados = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->envio_ok == 1) {
			list_add(mandados, list_get(suscriptores_para_enviar, i));
		}
	}

	t_list* acks = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->ack == 1) {
			list_add(acks, list_get(suscriptores_para_enviar, i));
		}
	}

	guardar_info_envios(mensaje->id, mandados, acks);

	free(threads);
	free(struct_para_enviar);
	list_clean(suscriptores_para_enviar);
	list_destroy(suscriptores_para_enviar);
}

void atender_mensaje_caught(mensaje_queue* mensaje) {
	pthread_mutex_lock(&gl_caught_list_lock);
	t_list* suscriptores_para_enviar = list_duplicate(gl_caught_suscriptores);
	pthread_mutex_unlock(&gl_caught_list_lock);

	data_tabla* registro = malloc(sizeof(data_tabla));

	registro->id = mensaje->id;
	registro->id_correlativo = ((t_localized*) mensaje->mensaje)->id_correlativo;
	registro->tipo = CAUGHT;

	uint32_t bytes;

	void* mensaje_memoria = serializar_caught(mensaje, &bytes);

	registro->limit = bytes;

	guardar_mensaje_en_memoria(registro, mensaje_memoria, logger);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	mandable_struct* struct_para_enviar = malloc(sizeof(mandable_struct) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		struct_para_enviar[i].info = list_get(suscriptores_para_enviar, i);
		struct_para_enviar[i].mensaje_queue = mensaje;
		pthread_create(&(threads[i]), NULL, (void*) mandar_caught, &struct_para_enviar[i]);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	t_list* mandados = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->envio_ok == 1) {
			list_add(mandados, list_get(suscriptores_para_enviar, i));
		}
	}

	t_list* acks = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->ack == 1) {
			list_add(acks, list_get(suscriptores_para_enviar, i));
		}
	}

	guardar_info_envios(mensaje->id, mandados, acks);

	free(threads);
	free(struct_para_enviar);
	list_clean(suscriptores_para_enviar);
	list_destroy(suscriptores_para_enviar);
}

void atender_mensaje_get(mensaje_queue* mensaje) {
	pthread_mutex_lock(&gl_get_list_lock);
	t_list* suscriptores_para_enviar = list_duplicate(gl_get_suscriptores);
	pthread_mutex_unlock(&gl_get_list_lock);

	data_tabla* registro = malloc(sizeof(data_tabla));

	registro->id = mensaje->id;
	registro->tipo = GET;

	uint32_t bytes;

	void* mensaje_memoria = serializar_get(mensaje, &bytes);

	registro->limit = bytes;

	guardar_mensaje_en_memoria(registro, mensaje_memoria, logger);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	mandable_struct* struct_para_enviar = malloc(sizeof(mandable_struct) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		struct_para_enviar[i].info = list_get(suscriptores_para_enviar, i);
		struct_para_enviar[i].mensaje_queue = mensaje;
		pthread_create(&(threads[i]), NULL, (void*) atender_get, &struct_para_enviar[i]);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	t_list* mandados = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->envio_ok == 1) {
			list_add(mandados, list_get(suscriptores_para_enviar, i));
		}
	}

	t_list* acks = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->ack == 1) {
			list_add(acks, list_get(suscriptores_para_enviar, i));
		}
	}

	guardar_info_envios(mensaje->id, mandados, acks);

	free(threads);
	free(struct_para_enviar);
	list_clean(suscriptores_para_enviar);
	list_destroy(suscriptores_para_enviar);
}

void atender_mensaje_localized(mensaje_queue* mensaje) {
	pthread_mutex_lock(&gl_localized_list_lock);
	t_list* suscriptores_para_enviar = list_duplicate(gl_localized_suscriptores);
	pthread_mutex_unlock(&gl_localized_list_lock);

	data_tabla* registro = malloc(sizeof(data_tabla));

	registro->id = mensaje->id;
	registro->id_correlativo = ((t_localized*) mensaje->mensaje)->id_correlativo;
	registro->tipo = LOCALIZED;

	uint32_t bytes;

	void* mensaje_memoria = serializar_localized(mensaje, &bytes);

	registro->limit = bytes;

	guardar_mensaje_en_memoria(registro, mensaje_memoria, logger);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	mandable_struct* struct_para_enviar = malloc(sizeof(mandable_struct) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		struct_para_enviar[i].info = list_get(suscriptores_para_enviar, i);
		struct_para_enviar[i].mensaje_queue = mensaje;
		pthread_create(&(threads[i]), NULL, (void*) mandar_localized, &struct_para_enviar[i]);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	t_list* mandados = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->envio_ok == 1) {
			list_add(mandados, list_get(suscriptores_para_enviar, i));
		}
	}

	t_list* acks = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->ack == 1) {
			list_add(acks, list_get(suscriptores_para_enviar, i));
		}
	}

	guardar_info_envios(mensaje->id, mandados, acks);

	free(threads);
	free(struct_para_enviar);
	list_clean(suscriptores_para_enviar);
	list_destroy(suscriptores_para_enviar);
}

void mandar_new(void* new_mandable) {
	mandable_struct* argumento_new = (mandable_struct*) new_mandable;

	status_envio* status = malloc(sizeof(status_envio));

	status->ip = argumento_new->info->ip;
	status->puerto = argumento_new->info->puerto;

	log_info(extense_logger, "Preparandose para enviar mensaje NEW con id %i al ip %s puerto %s", argumento_new->mensaje_queue->id, argumento_new->info->ip, argumento_new->info->puerto);

	uint32_t bytes;
	void* flujo = serializar_new(argumento_new->mensaje_queue, &bytes);

	int socket_cliente = crear_conexion(argumento_new->info->ip, argumento_new->info->puerto);

	log_info(extense_logger, "Conexion creada para el ip %s puerto %s con socket %i", argumento_new->info->ip, argumento_new->info->puerto, socket_cliente);
	log_info(extense_logger, "Enviando mensaje NEW con id %i al ip %s puerto %s", argumento_new->mensaje_queue->id, argumento_new->info->ip, argumento_new->info->puerto);

	if (send(socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje NEW con id %i al ip %s puerto %s enviado correctamente", argumento_new->mensaje_queue->id, argumento_new->info->ip, argumento_new->info->puerto);
		log_info(logger, "Mensaje NEW con id %i al ip %s puerto %s enviado correctamente", argumento_new->mensaje_queue->id, argumento_new->info->ip, argumento_new->info->puerto);
		uint32_t id;
		if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de ip %s puerto %s", argumento_new->info->ip, argumento_new->info->puerto);
			status->ack = 0;
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de ip %s puerto %s recibido", argumento_new->info->ip, argumento_new->info->puerto);
			log_info(logger, "ACK de ip %s puerto %s recibido", argumento_new->info->ip, argumento_new->info->puerto);
		}
	}

	close(socket_cliente);
	free(flujo);

	argumento_new->status = status;
}

void mandar_appeared(void* appeared_mandable) {
	mandable_struct* argumento_appeared = (mandable_struct*) appeared_mandable;

	status_envio* status = malloc(sizeof(status_envio));

	status->ip = argumento_appeared->info->ip;
	status->puerto = argumento_appeared->info->puerto;

	log_info(extense_logger, "Preparandose para enviar mensaje APPEARED con id %i al ip %s puerto %s", argumento_appeared->mensaje_queue->id, argumento_appeared->info->ip, argumento_appeared->info->puerto);

	uint32_t bytes;
	void* flujo = serializar_appeared(argumento_appeared->mensaje_queue, &bytes);

	int socket_cliente = crear_conexion(argumento_appeared->info->ip, argumento_appeared->info->puerto);

	log_info(extense_logger, "Conexion creada para el ip %s puerto %s con socket %i", argumento_appeared->info->ip, argumento_appeared->info->puerto, socket_cliente);
	log_info(extense_logger, "Enviando mensaje APPEARED con id %i al ip %s puerto %s", argumento_appeared->mensaje_queue->id, argumento_appeared->info->ip, argumento_appeared->info->puerto);

	if (send(socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje APPEARED con id %i al ip %s puerto %s enviado correctamente", argumento_appeared->mensaje_queue->id, argumento_appeared->info->ip, argumento_appeared->info->puerto);
		log_info(logger, "Mensaje APPEARED con id %i al ip %s puerto %s enviado correctamente", argumento_appeared->mensaje_queue->id, argumento_appeared->info->ip, argumento_appeared->info->puerto);
		uint32_t id;
		if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de ip %s puerto %s", argumento_appeared->info->ip, argumento_appeared->info->puerto);
			status->ack = 0;
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de ip %s puerto %s recibido", argumento_appeared->info->ip, argumento_appeared->info->puerto);
			log_info(logger, "ACK de ip %s puerto %s recibido", argumento_appeared->info->ip, argumento_appeared->info->puerto);
		}
	}

	close(socket_cliente);
	free(flujo);

	argumento_appeared->status = status;
}

void mandar_catch(void* catch_mandable) {
	mandable_struct* argumento_catch = (mandable_struct*) catch_mandable;

	status_envio* status = malloc(sizeof(status_envio));

	status->ip = argumento_catch->info->ip;
	status->puerto = argumento_catch->info->puerto;

	log_info(extense_logger, "Preparandose para enviar mensaje CATCH con id %i al ip %s puerto %s", argumento_catch->mensaje_queue->id, argumento_catch->info->ip, argumento_catch->info->puerto);

	int socket_cliente = crear_conexion(argumento_catch->info->ip, argumento_catch->info->puerto);

	uint32_t bytes;
	void* flujo = serializar_catch(argumento_catch->mensaje_queue, &bytes);

	log_info(extense_logger, "Conexion creada para el ip %s puerto %s con socket %i", argumento_catch->info->ip, argumento_catch->info->puerto, socket_cliente);
	log_info(extense_logger, "Enviando mensaje CATCH con id %i al ip %s puerto %s", argumento_catch->mensaje_queue->id, argumento_catch->info->ip, argumento_catch->info->puerto);

	if (send(socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 1;
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje CATCH con id %i al ip %s puerto %s enviado correctamente", argumento_catch->mensaje_queue->id, argumento_catch->info->ip, argumento_catch->info->puerto);
		log_info(logger, "Mensaje CATCH con id %i al ip %s puerto %s enviado correctamente", argumento_catch->mensaje_queue->id, argumento_catch->info->ip, argumento_catch->info->puerto);
		uint32_t id;
		if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de ip %s puerto %s", argumento_catch->info->ip, argumento_catch->info->puerto);
			status->ack = 0;
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de ip %s puerto %s recibido", argumento_catch->info->ip, argumento_catch->info->puerto);
			log_info(logger, "ACK de ip %s puerto %s recibido", argumento_catch->info->ip, argumento_catch->info->puerto);
		}
	}

	close(socket_cliente);
	free(flujo);

	argumento_catch->status = status;
}

void mandar_caught(void* caught_mandable) {
	mandable_struct* argumento_caught = (mandable_struct*) caught_mandable;

	status_envio* status = malloc(sizeof(status_envio));

	status->ip = argumento_caught->info->ip;
	status->puerto = argumento_caught->info->puerto;

	log_info(extense_logger, "Preparandose para enviar mensaje CAUGHT con id %i al ip %s puerto %s", argumento_caught->mensaje_queue->id, argumento_caught->info->ip, argumento_caught->info->puerto);

	uint32_t bytes;
	void* flujo = serializar_caught(argumento_caught->mensaje_queue, &bytes);

	int socket_cliente = crear_conexion(argumento_caught->info->ip, argumento_caught->info->puerto);

	log_info(extense_logger, "Conexion creada para el ip %s puerto %s con socket %i", argumento_caught->info->ip, argumento_caught->info->puerto, socket_cliente);
	log_info(extense_logger, "Enviando mensaje CAUGHT con id %i al ip %s puerto %s", argumento_caught->mensaje_queue->id, argumento_caught->info->ip, argumento_caught->info->puerto);

	if (send(socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje CAUGHT con id %i al ip %s puerto %s enviado correctamente", argumento_caught->mensaje_queue->id, argumento_caught->info->ip, argumento_caught->info->puerto);
		log_info(logger, "Mensaje CAUGHT con id %i al ip %s puerto %s enviado correctamente", argumento_caught->mensaje_queue->id, argumento_caught->info->ip, argumento_caught->info->puerto);
		uint32_t id;
		if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de ip %s puerto %s", argumento_caught->info->ip, argumento_caught->info->puerto);
			status->ack = 0;
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de ip %s puerto %s recibido", argumento_caught->info->ip, argumento_caught->info->puerto);
			log_info(logger, "ACK de ip %s puerto %s recibido", argumento_caught->info->ip, argumento_caught->info->puerto);
		}
	}

	close(socket_cliente);
	free(flujo);

	argumento_caught->status = status;
}

void mandar_get(void* get_mandable) {
	mandable_struct* argumento_get = (mandable_struct*) get_mandable;

	status_envio* status = malloc(sizeof(status_envio));

	status->ip = argumento_get->info->ip;
	status->puerto = argumento_get->info->puerto;

	log_info(extense_logger, "Preparandose para enviar mensaje GET con id %i al ip %s puerto %s", argumento_get->mensaje_queue->id, argumento_get->info->ip, argumento_get->info->puerto);

	uint32_t bytes;
	void* flujo = serializar_get(argumento_get->mensaje_queue, &bytes);

	int socket_cliente = crear_conexion(argumento_get->info->ip, argumento_get->info->puerto);

	log_info(extense_logger, "Conexion creada para el ip %s puerto %s con socket %i", argumento_get->info->ip, argumento_get->info->puerto, socket_cliente);
	log_info(extense_logger, "Enviando mensaje GET con id %i al ip %s puerto %s", argumento_get->mensaje_queue->id, argumento_get->info->ip, argumento_get->info->puerto);

	if (send(socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje GET con id %i al ip %s puerto %s enviado correctamente", argumento_get->mensaje_queue->id, argumento_get->info->ip, argumento_get->info->puerto);
		log_info(logger, "Mensaje GET con id %i al ip %s puerto %s enviado correctamente", argumento_get->mensaje_queue->id, argumento_get->info->ip, argumento_get->info->puerto);
		uint32_t id;
		if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de ip %s puerto %s", argumento_get->info->ip, argumento_get->info->puerto);
			status->ack = 0;
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de ip %s puerto %s recibido", argumento_get->info->ip, argumento_get->info->puerto);
			log_info(logger, "ACK de ip %s puerto %s recibido", argumento_get->info->ip, argumento_get->info->puerto);
		}
	}

	close(socket_cliente);
	free(flujo);

	argumento_get->status = status;
}

void mandar_localized(void* localized_mandable) {
	mandable_struct* argumento_localized = (mandable_struct*) localized_mandable;

	status_envio* status = malloc(sizeof(status_envio));

	status->ip = argumento_localized->info->ip;
	status->puerto = argumento_localized->info->puerto;

	log_info(extense_logger, "Preparandose para enviar mensaje LOCALIZED con id %i al ip %s puerto %s", argumento_localized->mensaje_queue->id, argumento_localized->info->ip, argumento_localized->info->puerto);

	uint32_t bytes;
	void* flujo = serializar_localized(argumento_localized->mensaje_queue, &bytes);

	int socket_cliente = crear_conexion(argumento_localized->info->ip, argumento_localized->info->puerto);

	log_info(extense_logger, "Conexion creada para el ip %s puerto %s con socket %i", argumento_localized->info->ip, argumento_localized->info->puerto, socket_cliente);
	log_info(extense_logger, "Enviando mensaje LOCALIZED con id %i al ip %s puerto %s", argumento_localized->mensaje_queue->id, argumento_localized->info->ip, argumento_localized->info->puerto);

	if (send(socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje LOCALIZED con id %i al ip %s puerto %s enviado correctamente", argumento_localized->mensaje_queue->id, argumento_localized->info->ip, argumento_localized->info->puerto);
		log_info(logger, "Mensaje LOCALIZED con id %i al ip %s puerto %s enviado correctamente", argumento_localized->mensaje_queue->id, argumento_localized->info->ip, argumento_localized->info->puerto);
		uint32_t id;
		if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de ip %s puerto %s", argumento_localized->info->ip, argumento_localized->info->puerto);
			status->ack = 0;
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de ip %s puerto %s recibido", argumento_localized->info->ip, argumento_localized->info->puerto);
			log_info(logger, "ACK de ip %s puerto %s recibido", argumento_localized->info->ip, argumento_localized->info->puerto);
		}
	}

	close(socket_cliente);
	free(flujo);

	argumento_localized->status = status;
}

void enviar_new_de_memoria(info_modulo* info_modulo) {
	t_list* segmentos = obtener_segmentos_new();

	mandable_memoria_struct* struct_para_enviar = malloc(sizeof(mandable_struct) * segmentos->elements_count);

	pthread_t* threads = malloc(sizeof(pthread_t) * segmentos->elements_count);

	for (int i = 0; i < segmentos->elements_count; i++) {
		struct_para_enviar[i].info_modulo = info_modulo;
		struct_para_enviar[i].segmento = list_get(segmentos, i);
		pthread_create(&(threads[i]), NULL, (void*) enviar_mensaje_new_de_memoria, &struct_para_enviar[i]);
	}

	for (int i = 0; i < segmentos->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	free(threads);
	free(struct_para_enviar);
}

void enviar_appeared_de_memoria(info_modulo* info_modulo) {
	t_list* segmentos = obtener_segmentos_appeared();

	mandable_memoria_struct* struct_para_enviar = malloc(sizeof(mandable_struct) * segmentos->elements_count);

	pthread_t* threads = malloc(sizeof(pthread_t) * segmentos->elements_count);

	for (int i = 0; i < segmentos->elements_count; i++) {
		struct_para_enviar[i].info_modulo = info_modulo;
		struct_para_enviar[i].segmento = list_get(segmentos, i);
		pthread_create(&(threads[i]), NULL, (void*) enviar_mensaje_appeared_de_memoria, &struct_para_enviar[i]);
	}

	for (int i = 0; i < segmentos->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	free(threads);
	free(struct_para_enviar);
}

void enviar_catch_de_memoria(info_modulo* info_modulo) {
	t_list* segmentos = obtener_segmentos_catch();

	mandable_memoria_struct* struct_para_enviar = malloc(sizeof(mandable_struct) * segmentos->elements_count);

	pthread_t* threads = malloc(sizeof(pthread_t) * segmentos->elements_count);

	for (int i = 0; i < segmentos->elements_count; i++) {
		struct_para_enviar[i].info_modulo = info_modulo;
		struct_para_enviar[i].segmento = list_get(segmentos, i);
		pthread_create(&(threads[i]), NULL, (void*) enviar_mensaje_catch_de_memoria, &struct_para_enviar[i]);
	}

	for (int i = 0; i < segmentos->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	free(threads);
	free(struct_para_enviar);
}

void enviar_caught_de_memoria(info_modulo* info_modulo) {
	t_list* segmentos = obtener_segmentos_caught();

	mandable_memoria_struct* struct_para_enviar = malloc(sizeof(mandable_struct) * segmentos->elements_count);

	pthread_t* threads = malloc(sizeof(pthread_t) * segmentos->elements_count);

	for (int i = 0; i < segmentos->elements_count; i++) {
		struct_para_enviar[i].info_modulo = info_modulo;
		struct_para_enviar[i].segmento = list_get(segmentos, i);
		pthread_create(&(threads[i]), NULL, (void*) enviar_mensaje_caught_de_memoria, &struct_para_enviar[i]);
	}

	for (int i = 0; i < segmentos->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	free(threads);
	free(struct_para_enviar);
}

void enviar_get_de_memoria(info_modulo* info_modulo) {
	t_list* segmentos = obtener_segmentos_get();

	mandable_memoria_struct* struct_para_enviar = malloc(sizeof(mandable_struct) * segmentos->elements_count);

	pthread_t* threads = malloc(sizeof(pthread_t) * segmentos->elements_count);

	for (int i = 0; i < segmentos->elements_count; i++) {
		struct_para_enviar[i].info_modulo = info_modulo;
		struct_para_enviar[i].segmento = list_get(segmentos, i);
		pthread_create(&(threads[i]), NULL, (void*) enviar_mensaje_get_de_memoria, &struct_para_enviar[i]);
	}

	for (int i = 0; i < segmentos->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	free(threads);
	free(struct_para_enviar);
}

void enviar_localized_de_memoria(info_modulo* info_modulo) {
	t_list* segmentos = obtener_segmentos_localized();

	mandable_memoria_struct* struct_para_enviar = malloc(sizeof(mandable_struct) * segmentos->elements_count);

	pthread_t* threads = malloc(sizeof(pthread_t) * segmentos->elements_count);

	for (int i = 0; i < segmentos->elements_count; i++) {
		struct_para_enviar[i].info_modulo = info_modulo;
		struct_para_enviar[i].segmento = list_get(segmentos, i);
		pthread_create(&(threads[i]), NULL, (void*) enviar_mensaje_localized_de_memoria, &struct_para_enviar[i]);
	}

	for (int i = 0; i < segmentos->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	free(threads);
	free(struct_para_enviar);
}

void enviar_mensaje_new_de_memoria(void* new_mandable_memoria) {
	mandable_memoria_struct* argumento = (mandable_memoria_struct*) new_mandable_memoria;

	status_envio* status = malloc(sizeof(status_envio));

	log_info(extense_logger, "Preparandose para enviar mensaje NEW de memoria con id %i al ip %s puerto %s", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);

	int socket_cliente = crear_conexion(argumento->info_modulo->ip, argumento->info_modulo->puerto);

	log_info(extense_logger, "Conexion creada para el ip %s puerto %s con socket %i", argumento->info_modulo->ip, argumento->info_modulo->puerto, socket_cliente);
	log_info(extense_logger, "Enviando mensaje NEW de memoria con id %i al ip %s puerto %s", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);

	if (send(socket_cliente, argumento->segmento->mensaje, argumento->segmento->registro->limit, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje NEW de memoria con id %i al ip %s puerto %s enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);
		log_info(logger, "Mensaje NEW de memoria con id %i al ip %s puerto %s enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);
		uint32_t id;
		if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de ip %s puerto %s", argumento->info_modulo->ip, argumento->info_modulo->puerto);
			status->ack = 0;
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de ip %s puerto %s recibido", argumento->info_modulo->ip, argumento->info_modulo->puerto);
			log_info(logger, "ACK de ip %s puerto %s recibido", argumento->info_modulo->ip, argumento->info_modulo->puerto);
		}
	}

	close(socket_cliente);

	if (1 == status->envio_ok) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->envios, argumento->info_modulo);
		pthread_mutex_unlock(&mutex_memoria);
	}
	if (1 == status->ack) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->acknowledgements, argumento->info_modulo);
		pthread_mutex_unlock(&mutex_memoria);
	}

	free(status);
}

void enviar_mensaje_appeared_de_memoria(void* appeared_mandable_memoria) {
	mandable_memoria_struct* argumento = (mandable_memoria_struct*) appeared_mandable_memoria;

	status_envio* status = malloc(sizeof(status_envio));

	log_info(extense_logger, "Preparandose para enviar mensaje APPEARED de memoria con id %i al ip %s puerto %s", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);

	int socket_cliente = crear_conexion(argumento->info_modulo->ip, argumento->info_modulo->puerto);

	log_info(extense_logger, "Conexion creada para el ip %s puerto %s con socket %i", argumento->info_modulo->ip, argumento->info_modulo->puerto, socket_cliente);
	log_info(extense_logger, "Enviando mensaje APPEARED de memoria con id %i al ip %s puerto %s", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);

	if (send(socket_cliente, argumento->segmento->mensaje, argumento->segmento->registro->limit, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje APPEARED de memoria con id %i al ip %s puerto %s enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);
		log_info(logger, "Mensaje APPEARED de memoria con id %i al ip %s puerto %s enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);
		uint32_t id;
		if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de ip %s puerto %s", argumento->info_modulo->ip, argumento->info_modulo->puerto);
			status->ack = 0;
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de ip %s puerto %s recibido", argumento->info_modulo->ip, argumento->info_modulo->puerto);
			log_info(logger, "ACK de ip %s puerto %s recibido", argumento->info_modulo->ip, argumento->info_modulo->puerto);
		}
	}

	close(socket_cliente);

	if (1 == status->envio_ok) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->envios, argumento->info_modulo);
		pthread_mutex_unlock(&mutex_memoria);
	}
	if (1 == status->ack) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->acknowledgements, argumento->info_modulo);
		pthread_mutex_unlock(&mutex_memoria);
	}

	free(status);
}

void enviar_mensaje_catch_de_memoria(void* catch_mandable_memoria) {
	mandable_memoria_struct* argumento = (mandable_memoria_struct*) catch_mandable_memoria;

	status_envio* status = malloc(sizeof(status_envio));

	log_info(extense_logger, "Preparandose para enviar mensaje CATCH de memoria con id %i al ip %s puerto %s", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);

	int socket_cliente = crear_conexion(argumento->info_modulo->ip, argumento->info_modulo->puerto);

	log_info(extense_logger, "Conexion creada para el ip %s puerto %s con socket %i", argumento->info_modulo->ip, argumento->info_modulo->puerto, socket_cliente);
	log_info(extense_logger, "Enviando mensaje CATCH de memoria con id %i al ip %s puerto %s", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);

	if (send(socket_cliente, argumento->segmento->mensaje, argumento->segmento->registro->limit, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje CATCH de memoria con id %i al ip %s puerto %s enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);
		log_info(logger, "Mensaje CATCH de memoria con id %i al ip %s puerto %s enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);
		uint32_t id;
		if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de ip %s puerto %s", argumento->info_modulo->ip, argumento->info_modulo->puerto);
			status->ack = 0;
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de ip %s puerto %s recibido", argumento->info_modulo->ip, argumento->info_modulo->puerto);
			log_info(logger, "ACK de ip %s puerto %s recibido", argumento->info_modulo->ip, argumento->info_modulo->puerto);
		}
	}

	close(socket_cliente);

	if (1 == status->envio_ok) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->envios, argumento->info_modulo);
		pthread_mutex_unlock(&mutex_memoria);
	}
	if (1 == status->ack) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->acknowledgements, argumento->info_modulo);
		pthread_mutex_unlock(&mutex_memoria);
	}

	free(status);
}

void enviar_mensaje_caught_de_memoria(void* caught_mandable_memoria) {
	mandable_memoria_struct* argumento = (mandable_memoria_struct*) caught_mandable_memoria;

	status_envio* status = malloc(sizeof(status_envio));

	log_info(extense_logger, "Preparandose para enviar mensaje CAUGHT de memoria con id %i al ip %s puerto %s", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);

	int socket_cliente = crear_conexion(argumento->info_modulo->ip, argumento->info_modulo->puerto);

	log_info(extense_logger, "Conexion creada para el ip %s puerto %s con socket %i", argumento->info_modulo->ip, argumento->info_modulo->puerto, socket_cliente);
	log_info(extense_logger, "Enviando mensaje CAUGHT de memoria con id %i al ip %s puerto %s", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);

	if (send(socket_cliente, argumento->segmento->mensaje, argumento->segmento->registro->limit, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje CAUGHT de memoria con id %i al ip %s puerto %s enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);
		log_info(logger, "Mensaje CAUGHT de memoria con id %i al ip %s puerto %s enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);
		uint32_t id;
		if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de ip %s puerto %s", argumento->info_modulo->ip, argumento->info_modulo->puerto);
			status->ack = 0;
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de ip %s puerto %s recibido", argumento->info_modulo->ip, argumento->info_modulo->puerto);
			log_info(logger, "ACK de ip %s puerto %s recibido", argumento->info_modulo->ip, argumento->info_modulo->puerto);
		}
	}

	close(socket_cliente);

	if (1 == status->envio_ok) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->envios, argumento->info_modulo);
		pthread_mutex_unlock(&mutex_memoria);
	}
	if (1 == status->ack) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->acknowledgements, argumento->info_modulo);
		pthread_mutex_unlock(&mutex_memoria);
	}

	free(status);
}

void enviar_mensaje_get_de_memoria(void* get_mandable_memoria) {
	mandable_memoria_struct* argumento = (mandable_memoria_struct*) get_mandable_memoria;

	status_envio* status = malloc(sizeof(status_envio));

	log_info(extense_logger, "Preparandose para enviar mensaje GET de memoria con id %i al ip %s puerto %s", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);

	int socket_cliente = crear_conexion(argumento->info_modulo->ip, argumento->info_modulo->puerto);

	log_info(extense_logger, "Conexion creada para el ip %s puerto %s con socket %i", argumento->info_modulo->ip, argumento->info_modulo->puerto, socket_cliente);
	log_info(extense_logger, "Enviando mensaje GET de memoria con id %i al ip %s puerto %s", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);

	if (send(socket_cliente, argumento->segmento->mensaje, argumento->segmento->registro->limit, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje GET de memoria con id %i al ip %s puerto %s enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);
		log_info(logger, "Mensaje GET de memoria con id %i al ip %s puerto %s enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);
		uint32_t id;
		if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de ip %s puerto %s", argumento->info_modulo->ip, argumento->info_modulo->puerto);
			status->ack = 0;
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de ip %s puerto %s recibido", argumento->info_modulo->ip, argumento->info_modulo->puerto);
			log_info(logger, "ACK de ip %s puerto %s recibido", argumento->info_modulo->ip, argumento->info_modulo->puerto);
		}
	}

	close(socket_cliente);

	if (1 == status->envio_ok) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->envios, argumento->info_modulo);
		pthread_mutex_unlock(&mutex_memoria);
	}
	if (1 == status->ack) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->acknowledgements, argumento->info_modulo);
		pthread_mutex_unlock(&mutex_memoria);
	}

	free(status);
}

void enviar_mensaje_localized_de_memoria(void* localized_mandable_memoria) {
	mandable_memoria_struct* argumento = (mandable_memoria_struct*) localized_mandable_memoria;

	status_envio* status = malloc(sizeof(status_envio));

	log_info(extense_logger, "Preparandose para enviar mensaje LOCALIZED de memoria con id %i al ip %s puerto %s", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);

	int socket_cliente = crear_conexion(argumento->info_modulo->ip, argumento->info_modulo->puerto);

	log_info(extense_logger, "Conexion creada para el ip %s puerto %s con socket %i", argumento->info_modulo->ip, argumento->info_modulo->puerto, socket_cliente);
	log_info(extense_logger, "Enviando mensaje LOCALIZED de memoria con id %i al ip %s puerto %s", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);

	if (send(socket_cliente, argumento->segmento->mensaje, argumento->segmento->registro->limit, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje LOCALIZED de memoria con id %i al ip %s puerto %s enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);
		log_info(logger, "Mensaje LOCALIZED de memoria con id %i al ip %s puerto %s enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->ip, argumento->info_modulo->puerto);
		uint32_t id;
		if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de ip %s puerto %s", argumento->info_modulo->ip, argumento->info_modulo->puerto);
			status->ack = 0;
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de ip %s puerto %s recibido", argumento->info_modulo->ip, argumento->info_modulo->puerto);
			log_info(logger, "ACK de ip %s puerto %s recibido", argumento->info_modulo->ip, argumento->info_modulo->puerto);
		}
	}

	close(socket_cliente);

	if (1 == status->envio_ok) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->envios, argumento->info_modulo);
		pthread_mutex_unlock(&mutex_memoria);
	}
	if (1 == status->ack) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->acknowledgements, argumento->info_modulo);
		pthread_mutex_unlock(&mutex_memoria);
	}

	free(status);
}

void* serializar_new(mensaje_queue* new, uint32_t* bytes) {
	uint32_t tamanio_pokemon = (strlen(((t_new*) new->mensaje)->pokemon) + 1) * sizeof(char);

	*bytes = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + tamanio_pokemon  + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);

	uint32_t cod_op = 1;
	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(new->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &tamanio_pokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, ((t_new*) new->mensaje)->pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;
	memcpy(flujo + desplazamiento, &(((t_new*) new->mensaje)->pos_X), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(((t_new*) new->mensaje)->pos_Y), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(((t_new*) new->mensaje)->cantidad), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	return flujo;
}

void* serializar_appeared(mensaje_queue* appeared, uint32_t* bytes) {
	uint32_t tamanio_pokemon = (strlen(((t_appeared*) appeared->mensaje)->pokemon) + 1) * sizeof(char);

	*bytes = sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t) + tamanio_pokemon +
			sizeof(uint32_t) + sizeof(uint32_t);

	uint32_t cod_op = 2;
	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(appeared->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(((t_appeared*) appeared->mensaje)->id_correlativo), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &tamanio_pokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, ((t_appeared*) appeared->mensaje)->pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;
	memcpy(flujo + desplazamiento, &(((t_appeared*) appeared->mensaje)->pos_X), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(((t_appeared*) appeared->mensaje)->pos_Y), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	return flujo;
}

void* serializar_catch(mensaje_queue* catch, uint32_t* bytes) {
	uint32_t tamanio_pokemon = (strlen(((t_catch*) catch->mensaje)->pokemon) + 1) * sizeof(char);

	*bytes = sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t) + sizeof(uint32_t) +
			tamanio_pokemon + sizeof(uint32_t) +
			sizeof(uint32_t);

	uint32_t cod_op = 5;
	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(catch->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &tamanio_pokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, ((t_catch*) catch->mensaje)->pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;
	memcpy(flujo + desplazamiento, &(((t_catch*) catch->mensaje)->pos_X), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(((t_catch*) catch->mensaje)->pos_Y), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	return flujo;
}

void* serializar_caught(mensaje_queue* caught, uint32_t* bytes) {
	*bytes = sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t);

	uint32_t cod_op = 6;
	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(caught->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(((t_caught*) caught->mensaje)->id_correlativo), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(((t_caught*) caught->mensaje)->flag_caught), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	return flujo;
}

void* serializar_get(mensaje_queue* get, uint32_t* bytes) {
	uint32_t tamanio_pokemon = (strlen(((t_get*) get->mensaje)->pokemon) + 1) * sizeof(char);

	*bytes = sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t) + sizeof(uint32_t) +
			tamanio_pokemon;

	uint32_t cod_op = 3;
	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(get->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &tamanio_pokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, ((t_get*) get->mensaje)->pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;

	return flujo;
}

void* serializar_localized(mensaje_queue* localized, uint32_t* bytes) {
	uint32_t tamanio_pokemon = (strlen(((t_localized*) localized->mensaje)->pokemon) + 1) * sizeof(char);
	uint32_t elementos_lista = ((t_localized*) localized->mensaje)->l_coordenadas->elements_count;
	uint32_t tamanio_lista = elementos_lista * sizeof(uint32_t);

	*bytes = sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t) + tamanio_pokemon +
			sizeof(uint32_t) + tamanio_lista;

	uint32_t cod_op = 4;
	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(localized->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(((t_localized*) localized->mensaje)->id_correlativo), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &tamanio_pokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, ((t_localized*) localized->mensaje)->pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;
	memcpy(flujo + desplazamiento, &(((t_localized*) localized->mensaje)->lugares), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	for (int i = 0; i < elementos_lista; i++) {
		uint32_t* elemento_de_lista = (uint32_t*) list_get(((t_localized*) localized->mensaje)->l_coordenadas, i);
		memcpy(flujo + desplazamiento, elemento_de_lista, sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
	}

	return flujo;
}

int main(void) {
	char* ip;
	char* puerto;
	char* log_file;
	char* extense_log_file;

	// Iniciamos config, logger, variables levantadas de config y colas globales

	config = leer_config();
	log_file = config_get_string_value(config, "LOG_FILE");
	extense_log_file = config_get_string_value(config, "EXTENSE_LOG_FILE");
	extense_logger = iniciar_logger(extense_log_file);
	log_info(extense_logger, "logger extenso iniciado");
	logger = iniciar_logger(log_file);
	log_info(extense_logger, "logger de la catedra iniciado");
	ip = config_get_string_value(config, "IP");
	log_info(extense_logger, "La IP es: %s", ip);
	puerto = config_get_string_value(config, "PUERTO");
	log_info(extense_logger, "El puerto es: %s", puerto);

	inicializar_memoria();

	inicializar_colas();

	inicializar_semaforos_colas();

	log_info(extense_logger, "Creando generador id");

	crea_generador_id();

	log_info(extense_logger, "Generador id creado");

	// Levantamos el servidor en la ip y el puerto definidos en config

	log_info(extense_logger, "Levantando servidor");

	int socket_servidor = iniciar_servidor(ip, puerto);

	log_info(extense_logger, "Servidor levantado");

	// Iniciamos un hilo para ir escuchando a los clientes
	// Tiene que estar vivo mientras que el broker este funcionando. Entonces corresponde detach?


	pthread_t hilo_escucha_de_clientes;

	pthread_t hilo_atencion_de_new;
	pthread_t hilo_atencion_de_appeared;
	pthread_t hilo_atencion_de_catch;
	pthread_t hilo_atencion_de_caught;
	pthread_t hilo_atencion_de_get;
	pthread_t hilo_atencion_de_localized;

	pthread_create(&hilo_escucha_de_clientes, NULL, (void *) esperar_clientes, (void*) socket_servidor);

	pthread_create(&hilo_atencion_de_new, NULL, (void *) atender_new, NULL);
	pthread_create(&hilo_atencion_de_appeared, NULL, (void *) atender_appeared, NULL);
	pthread_create(&hilo_atencion_de_catch, NULL, (void *) atender_catch, NULL);
	pthread_create(&hilo_atencion_de_caught, NULL, (void *) atender_caught, NULL);
	pthread_create(&hilo_atencion_de_get, NULL, (void *) atender_get, NULL);
	pthread_create(&hilo_atencion_de_localized, NULL, (void *) atender_localized, NULL);

	pthread_join(hilo_escucha_de_clientes, NULL);

	pthread_join(hilo_atencion_de_new, NULL);
	pthread_join(hilo_atencion_de_appeared, NULL);
	pthread_join(hilo_atencion_de_catch, NULL);
	pthread_join(hilo_atencion_de_caught, NULL);
	pthread_join(hilo_atencion_de_get, NULL);
	pthread_join(hilo_atencion_de_localized, NULL);

	// Terminamos el programa

	terminar_programa();
	return EXIT_SUCCESS;
}

void inicializar_memoria() {
	int tamanio_memoria = config_get_int_value(config, "TAMANIO_MEMORIA");
	tamanio_minimo_particion = config_get_int_value(config, "TAMANIO_MINIMO_PARTICION");
	char* algoritmo_busqueda = config_get_string_value(config, "ALGORITMO_PARTICION_LIBRE");

	char* ff = "FF";

	int resultado = strcasecmp(ff, algoritmo_busqueda);

	if (0 == resultado) {
		algoritmo_particion_libre = 1;
	} else {
		algoritmo_particion_libre = 2;
	}

	memoria = malloc(tamanio_memoria);
	tabla_segmentos = dictionary_create();
	particiones_libres = list_create();

	pthread_mutex_init(&mutex_memoria, NULL);

	particion_libre* memoria_completa = malloc(sizeof(particion_libre));
	memoria_completa->desde = memoria;
	memoria_completa->hasta = memoria + tamanio_memoria;
	list_add(particiones_libres, memoria_completa);

}

void inicializar_colas(void) {
	log_info(extense_logger, "Iniciando colas");

	gl_new_pokemon_queue = queue_create();
	gl_appeared_pokemon_queue = queue_create();
	gl_catch_pokemon_queue = queue_create();
	gl_caught_pokemon_queue = queue_create();
	gl_get_pokemon_queue = queue_create();
	gl_localized_pokemon_queue = queue_create();

	log_info(extense_logger, "Colas iniciadas");

	log_info(extense_logger, "Iniciando listas de suscriptores");

	gl_new_suscriptores = list_create();
	gl_appeared_suscriptores = list_create();
	gl_catch_suscriptores = list_create();
	gl_caught_suscriptores = list_create();
	gl_get_suscriptores = list_create();
	gl_localized_suscriptores = list_create();

	log_info(extense_logger, "Listas de suscriptores iniciadas");

	log_info(extense_logger, "Iniciando estructuras de colas");

	NEW_STRUCT.cola = gl_new_pokemon_queue;
	APPEARED_STRUCT.cola = gl_appeared_pokemon_queue;
	CATCH_STRUCT.cola = gl_catch_pokemon_queue;
	CAUGHT_STRUCT.cola = gl_caught_pokemon_queue;
	GET_STRUCT.cola = gl_get_pokemon_queue;
	LOCALIZED_STRUCT.cola = gl_localized_pokemon_queue;

	NEW_STRUCT.suscriptores = gl_new_suscriptores;
	APPEARED_STRUCT.suscriptores = gl_appeared_suscriptores;
	CATCH_STRUCT.suscriptores = gl_catch_suscriptores;
	CAUGHT_STRUCT.suscriptores = gl_caught_suscriptores;
	GET_STRUCT.suscriptores = gl_get_suscriptores;
	LOCALIZED_STRUCT.suscriptores = gl_localized_suscriptores;

	NEW_STRUCT.id = NEW_ID;
	APPEARED_STRUCT.id = APPEARED_ID;
	CATCH_STRUCT.id = CATCH_ID;
	CAUGHT_STRUCT.id = CAUGHT_ID;
	GET_STRUCT.id = GET_ID;
	LOCALIZED_STRUCT.id = LOCALIZED_ID;

	log_info(extense_logger, "Estructuras de colas iniciadas");
}

t_log* iniciar_logger(char* log_file) {
	return log_create(log_file, "Broker", 1, LOG_LEVEL_INFO);
}

t_config* leer_config() {
	t_config* config;
	config = config_create("broker.config");
	return config;
}

void inicializar_semaforos_colas(void) {
	int limite_cola_new;
	int limite_cola_appeared;
	int limite_cola_catch;
	int limite_cola_caught;
	int limite_cola_get;
	int limite_cola_localized;

	log_info(extense_logger, "Iniciando limites de colas");

	limite_cola_new = config_get_int_value(config, "LIMITE_COLA_NEW");
	limite_cola_appeared = config_get_int_value(config, "LIMITE_COLA_APPEARED");
	limite_cola_catch = config_get_int_value(config, "LIMITE_COLA_CATCH");
	limite_cola_caught = config_get_int_value(config, "LIMITE_COLA_CAUGHT");
	limite_cola_get = config_get_int_value(config, "LIMITE_COLA_GET");
	limite_cola_localized = config_get_int_value(config, "LIMITE_COLA_LOCALIZED");

	log_info(extense_logger, "Limites de colas iniciadas");

	log_info(extense_logger, "Iniciando semaforos de limite");

	sem_init(&gl_new_limite, 0, limite_cola_new);
	sem_init(&gl_appeared_limite, 0, limite_cola_appeared);
	sem_init(&gl_catch_limite, 0, limite_cola_catch);
	sem_init(&gl_caught_limite, 0, limite_cola_caught);
	sem_init(&gl_get_limite, 0, limite_cola_get);
	sem_init(&gl_localized_limite, 0, limite_cola_localized);

	log_info(extense_logger, "Semaforos de limite iniciados");

	log_info(extense_logger, "Iniciando semaforos de mensajes");

	sem_init(&gl_new_mensajes, 0, 0);
	sem_init(&gl_appeared_mensajes, 0, 0);
	sem_init(&gl_catch_mensajes, 0, 0);
	sem_init(&gl_caught_mensajes, 0, 0);
	sem_init(&gl_get_mensajes, 0, 0);
	sem_init(&gl_localized_mensajes, 0, 0);

	log_info(extense_logger, "Semaforos de mensajes iniciados");

	log_info(extense_logger, "Iniciando semaforos de colas");

	pthread_mutex_init(&gl_new_queue_lock, NULL);
	pthread_mutex_init(&gl_appeared_queue_lock, NULL);
	pthread_mutex_init(&gl_catch_queue_lock, NULL);
	pthread_mutex_init(&gl_caught_queue_lock, NULL);
	pthread_mutex_init(&gl_get_queue_lock, NULL);
	pthread_mutex_init(&gl_localized_queue_lock, NULL);

	log_info(extense_logger, "Semaforos de colas iniciados");

	log_info(extense_logger, "Iniciando semaforos de listas");

	pthread_mutex_init(&gl_new_list_lock, NULL);
	pthread_mutex_init(&gl_appeared_list_lock, NULL);
	pthread_mutex_init(&gl_catch_list_lock, NULL);
	pthread_mutex_init(&gl_caught_list_lock, NULL);
	pthread_mutex_init(&gl_get_list_lock, NULL);
	pthread_mutex_init(&gl_localized_list_lock, NULL);

	log_info(extense_logger, "Semaforos de listas iniciados");
}

void terminar_programa(void) {
	log_destroy(logger);
	log_destroy(extense_logger);
	config_destroy(config);
	queue_clean(gl_new_pokemon_queue);
	queue_destroy(gl_new_pokemon_queue);
	queue_clean(gl_appeared_pokemon_queue);
	queue_destroy(gl_appeared_pokemon_queue);
	queue_clean(gl_catch_pokemon_queue);
	queue_destroy(gl_catch_pokemon_queue);
	queue_clean(gl_caught_pokemon_queue);
	queue_destroy(gl_caught_pokemon_queue);
	queue_clean(gl_get_pokemon_queue);
	queue_destroy(gl_get_pokemon_queue);
	queue_clean(gl_localized_pokemon_queue);
	queue_destroy(gl_localized_pokemon_queue);
	list_clean(gl_new_suscriptores);
	list_clean(gl_appeared_suscriptores);
	list_clean(gl_catch_suscriptores);
	list_clean(gl_caught_suscriptores);
	list_clean(gl_get_suscriptores);
	list_clean(gl_localized_suscriptores);
	list_destroy(gl_new_suscriptores);
	list_destroy(gl_appeared_suscriptores);
	list_destroy(gl_catch_suscriptores);
	list_destroy(gl_caught_suscriptores);
	list_destroy(gl_get_suscriptores);
	list_destroy(gl_localized_suscriptores);
	sem_destroy(&gl_new_limite);
	sem_destroy(&gl_appeared_limite);
	sem_destroy(&gl_catch_limite);
	sem_destroy(&gl_caught_limite);
	sem_destroy(&gl_get_limite);
	sem_destroy(&gl_localized_limite);
	sem_destroy(&gl_new_mensajes);
	sem_destroy(&gl_appeared_mensajes);
	sem_destroy(&gl_catch_mensajes);
	sem_destroy(&gl_caught_mensajes);
	sem_destroy(&gl_get_mensajes);
	sem_destroy(&gl_localized_mensajes);
	pthread_mutex_destroy(&gl_new_queue_lock);
	pthread_mutex_destroy(&gl_appeared_queue_lock);
	pthread_mutex_destroy(&gl_catch_queue_lock);
	pthread_mutex_destroy(&gl_caught_queue_lock);
	pthread_mutex_destroy(&gl_get_queue_lock);
	pthread_mutex_destroy(&gl_localized_queue_lock);
	pthread_mutex_destroy(&gl_new_list_lock);
	pthread_mutex_destroy(&gl_appeared_list_lock);
	pthread_mutex_destroy(&gl_catch_list_lock);
	pthread_mutex_destroy(&gl_caught_list_lock);
	pthread_mutex_destroy(&gl_get_list_lock);
	pthread_mutex_destroy(&gl_localized_list_lock);

	destruir_generador_id();
}
