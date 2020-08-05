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

	if (new_msg == NULL) {
		free(mensaje_a_guardar);
		pthread_exit(NULL);
	} else {
		mensaje_a_guardar->mensaje = (void*) new_msg;

		devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

		sem_wait(&gl_new_limite);

		log_info(extense_logger, "Encolando el mensaje NEW con id %i", mensaje_a_guardar->id);
		pthread_mutex_lock(&gl_new_queue_lock);
		queue_push(gl_new_pokemon_queue, mensaje_a_guardar);
		pthread_mutex_unlock(&gl_new_queue_lock);
		log_info(extense_logger, "Mensaje NEW con id %i encolado", mensaje_a_guardar->id);
		log_info(logger, "Mensaje NEW con id %i encolado", mensaje_a_guardar->id);

		sem_post(&gl_new_mensajes);
	}
	break;
	case APPEARED: ;
	log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un APPEARED", cliente_fd);
	t_appeared* appeared_msg = recibir_appeared(cliente_fd, &size, extense_logger);

	if (appeared_msg == NULL) {
		free(mensaje_a_guardar);
		pthread_exit(NULL);
	} else {
		pthread_mutex_lock(&gl_appeared_ids_lock);
		int ya_fue_respondido = contiene_al_id_respondido(gl_appeared_ids_respondidos, appeared_msg->id_correlativo);
		pthread_mutex_unlock(&gl_appeared_ids_lock);

		if (ya_fue_respondido == 0) {
			mensaje_a_guardar->mensaje = (void*) appeared_msg;
			devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

			sem_wait(&gl_appeared_limite);

			log_info(extense_logger, "Encolando el mensaje APPEARED con id %i", mensaje_a_guardar->id);
			uint32_t id_respondido = appeared_msg->id_correlativo;
			pthread_mutex_lock(&gl_appeared_ids_lock);
			list_add(gl_appeared_ids_respondidos, (void*) &id_respondido);
			pthread_mutex_unlock(&gl_appeared_ids_lock);
			pthread_mutex_lock(&gl_appeared_queue_lock);
			queue_push(gl_appeared_pokemon_queue, mensaje_a_guardar);
			pthread_mutex_unlock(&gl_appeared_queue_lock);
			log_info(extense_logger, "Mensaje APPEARED con id %i encolado", mensaje_a_guardar->id);
			log_info(logger, "Mensaje APPEARED con id %i encolado", mensaje_a_guardar->id);

			sem_post(&gl_appeared_mensajes);
		} else {
			log_info(extense_logger, "El mensaje con id %i ya obtuvo una respuesta, desestimando mensaje APPEARED con el mismo id correlativo", appeared_msg->id_correlativo);
		}
	}
	break;
	case CATCH: ;
	log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un CATCH", cliente_fd);
	t_catch* catch_msg = recibir_catch(cliente_fd, &size, extense_logger);

	if (catch_msg == NULL) {
		free(mensaje_a_guardar);
		pthread_exit(NULL);
	} else {
		mensaje_a_guardar->mensaje = (void*) catch_msg;
		devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

		sem_wait(&gl_catch_limite);

		log_info(extense_logger, "Encolando el mensaje CATCH con id %i", mensaje_a_guardar->id);
		pthread_mutex_lock(&gl_catch_queue_lock);
		queue_push(gl_catch_pokemon_queue, mensaje_a_guardar);
		pthread_mutex_unlock(&gl_catch_queue_lock);
		log_info(extense_logger, "Mensaje CATCH con id %i encolado", mensaje_a_guardar->id);
		log_info(logger, "Mensaje CATCH con id %i encolado", mensaje_a_guardar->id);

		sem_post(&gl_catch_mensajes);
	}
	break;
	case CAUGHT: ;
	log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un CAUGHT", cliente_fd);
	t_caught* caught_msg = recibir_caught(cliente_fd, &size, extense_logger);

	if (caught_msg == NULL) {
		free(mensaje_a_guardar);
		pthread_exit(NULL);
	} else {
		pthread_mutex_lock(&gl_caught_ids_lock);
		int ya_fue_respondido = contiene_al_id_respondido(gl_caught_ids_respondidos, caught_msg->id_correlativo);
		pthread_mutex_unlock(&gl_caught_ids_lock);
		if (ya_fue_respondido == 0) {
			mensaje_a_guardar->mensaje = (void*) caught_msg;

			devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

			sem_wait(&gl_caught_limite);

			log_info(extense_logger, "Encolando el mensaje CAUGHT con id %i", mensaje_a_guardar->id);
			uint32_t id_respondido = caught_msg->id_correlativo;
			pthread_mutex_lock(&gl_caught_ids_lock);
			list_add(gl_caught_ids_respondidos, (void*) &id_respondido);
			pthread_mutex_unlock(&gl_caught_ids_lock);
			pthread_mutex_lock(&gl_caught_queue_lock);
			queue_push(gl_caught_pokemon_queue, mensaje_a_guardar);
			pthread_mutex_unlock(&gl_caught_queue_lock);
			log_info(extense_logger, "Mensaje CAUGHT con id %i encolado", mensaje_a_guardar->id);
			log_info(logger, "Mensaje CAUGHT con id %i encolado", mensaje_a_guardar->id);

			sem_post(&gl_caught_mensajes);
		} else {
			log_info(extense_logger, "El mensaje con id %i ya obtuvo una respuesta, desestimando mensaje CAUGHT con el mismo id correlativo", caught_msg->id_correlativo);
		}
	}
	break;
	case GET: ;
	log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un GET", cliente_fd);
	t_get* get_msg = recibir_get(cliente_fd, &size, extense_logger);

	if (get_msg == NULL) {
		free(mensaje_a_guardar);
		pthread_exit(NULL);
	} else {
		mensaje_a_guardar->mensaje = (void*) get_msg;

		devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

		sem_wait(&gl_get_limite);

		log_info(extense_logger, "Encolando el mensaje GET con id %i", mensaje_a_guardar->id);
		pthread_mutex_lock(&gl_get_queue_lock);
		queue_push(gl_get_pokemon_queue, mensaje_a_guardar);
		pthread_mutex_unlock(&gl_get_queue_lock);
		log_info(extense_logger, "Mensaje GET con id %i encolado", mensaje_a_guardar->id);
		log_info(logger, "Mensaje GET con id %i encolado", mensaje_a_guardar->id);

		sem_post(&gl_get_mensajes);
	}
	break;
	case LOCALIZED: ;
	log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un LOCALIZED", cliente_fd);
	t_localized* localized_msg = recibir_localized(cliente_fd, &size, extense_logger);

	if (localized_msg == NULL) {
		free(mensaje_a_guardar);
		pthread_exit(NULL);
	} else {
		pthread_mutex_lock(&gl_localized_ids_lock);
		int ya_fue_respondido = contiene_al_id_respondido(gl_localized_ids_respondidos, localized_msg->id_correlativo);
		pthread_mutex_unlock(&gl_localized_ids_lock);

		if (ya_fue_respondido == 0) {
			mensaje_a_guardar->mensaje = (void*) localized_msg;

			devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

			sem_wait(&gl_localized_limite);

			log_info(extense_logger, "Encolando el mensaje LOCALIZED con id %i", mensaje_a_guardar->id);
			uint32_t id_respondido = localized_msg->id_correlativo;
			pthread_mutex_lock(&gl_localized_ids_lock);
			list_add(gl_localized_ids_respondidos, (void*) &id_respondido);
			pthread_mutex_unlock(&gl_localized_ids_lock);
			pthread_mutex_lock(&gl_localized_queue_lock);
			queue_push(gl_localized_pokemon_queue, mensaje_a_guardar);
			pthread_mutex_unlock(&gl_localized_queue_lock);
			log_info(extense_logger, "Mensaje LOCALIZED con id %i encolado", mensaje_a_guardar->id);
			log_info(logger, "Mensaje LOCALIZED con id %i encolado", mensaje_a_guardar->id);

			sem_post(&gl_localized_mensajes);
		} else {
			log_info(extense_logger, "El mensaje con id %i ya obtuvo una respuesta, desestimando mensaje LOCALIZED con el mismo id correlativo", localized_msg->id_correlativo);
		}
	}
	break;
	case SUSCRIPCION: ;
	log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un SUSCRIPCION", cliente_fd);
	t_suscripcion* suscripcion = recibir_suscripcion(cliente_fd, &size, extense_logger);

	if (suscripcion == NULL) {
		free(mensaje_a_guardar);
		pthread_exit(NULL);
	} else {

		info_modulo* info_modulo = malloc(sizeof(info_modulo));
		info_modulo->id_cliente = suscripcion->id_cliente;
		info_modulo->socket_cliente = cliente_fd;

		switch(suscripcion->id_cola) {
		case NEW_ID: ;
		log_info(extense_logger, "Id de cola recibido del socket cliente %i corresponde a la cola de NEW", cliente_fd);

		pthread_mutex_lock(&gl_new_list_lock);
		list_add(gl_new_suscriptores, (void *) info_modulo);
		pthread_mutex_unlock(&gl_new_list_lock);
		enviar_new_de_memoria(info_modulo);
		log_info(extense_logger, "Modulo con id %i agregado correctamente en la cola de NEW", suscripcion->id_cliente);
		log_info(logger, "Modulo con id %i agregado correctamente en la cola de NEW", suscripcion->id_cliente);
		break;
		case APPEARED_ID: ;
		log_info(extense_logger, "Id de cola recibido del socket cliente %i corresponde a la cola de APPEARED", cliente_fd);
		pthread_mutex_lock(&gl_appeared_list_lock);
		list_add(gl_appeared_suscriptores, (void *) info_modulo);
		pthread_mutex_unlock(&gl_appeared_list_lock);
		enviar_appeared_de_memoria(info_modulo);
		log_info(extense_logger, "Modulo con id %i agregado correctamente en la cola de APPEARED", suscripcion->id_cliente);
		log_info(logger, "Modulo con id %i agregado correctamente en la cola de APPEARED", suscripcion->id_cliente);
		break;
		case CATCH_ID: ;
		log_info(extense_logger, "Id de cola recibido del socket cliente %i corresponde a la cola de CATCH", cliente_fd);
		pthread_mutex_lock(&gl_catch_list_lock);
		list_add(gl_catch_suscriptores, (void *) info_modulo);
		pthread_mutex_unlock(&gl_catch_list_lock);
		enviar_catch_de_memoria(info_modulo);
		log_info(extense_logger, "Modulo con id %i agregado correctamente en la cola de CATCH", suscripcion->id_cliente);
		log_info(logger, "Modulo con id %i agregado correctamente en la cola de CATCH", suscripcion->id_cliente);
		break;
		case CAUGHT_ID: ;
		log_info(extense_logger, "Id de cola recibido del socket cliente %i corresponde a la cola de CAUGHT", cliente_fd);
		pthread_mutex_lock(&gl_caught_list_lock);
		list_add(gl_caught_suscriptores, (void *) info_modulo);
		pthread_mutex_unlock(&gl_caught_list_lock);
		enviar_caught_de_memoria(info_modulo);
		log_info(extense_logger, "Modulo con id %i agregado correctamente en la cola de CAUGHT", suscripcion->id_cliente);
		log_info(logger, "Modulo con id %i agregado correctamente en la cola de CAUGHT", suscripcion->id_cliente);
		break;
		case GET_ID: ;
		log_info(extense_logger, "Id de cola recibido del socket cliente %i corresponde a la cola de GET", cliente_fd);
		pthread_mutex_lock(&gl_get_list_lock);
		list_add(gl_get_suscriptores, (void *) info_modulo);
		pthread_mutex_unlock(&gl_get_list_lock);
		enviar_get_de_memoria(info_modulo);
		log_info(extense_logger, "Modulo con id %i agregado correctamente en la cola de GET", suscripcion->id_cliente);
		log_info(logger, "Modulo con id %i agregado correctamente en la cola de GET", suscripcion->id_cliente);
		break;
		case LOCALIZED_ID: ;
		log_info(extense_logger, "Id de cola recibido del socket cliente %i corresponde a la cola de LOCALIZED", cliente_fd);
		pthread_mutex_lock(&gl_localized_list_lock);
		list_add(gl_localized_suscriptores, (void *) info_modulo);
		pthread_mutex_unlock(&gl_localized_list_lock);
		enviar_localized_de_memoria(info_modulo);
		log_info(extense_logger, "Modulo con id %i agregado correctamente en la cola de LOCALIZED", suscripcion->id_cliente);
		log_info(logger, "Modulo con id %i agregado correctamente en la cola de LOCALIZED", suscripcion->id_cliente);
		break;
		}
	}
	free(suscripcion);
	free(mensaje_a_guardar);
	break;
	}
}

void atender_cliente(int* socket) {
	int socket_cliente = *socket;
	uint32_t cod_op;
	log_info(extense_logger, "Recibiendo codigo de operacion de socket %i", socket_cliente);
	int status_recv = recv(socket_cliente, &cod_op, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		close(socket_cliente);
		log_error(extense_logger, "Hubo un problema recibiendo codigo de operacion de socket %i", socket_cliente);
		pthread_exit(NULL);
	}
	if (status_recv == 0) {
		close(socket_cliente);
		log_warning(extense_logger, "El cliente acaba de cerrar la conexion correspondiente al socket %i", socket_cliente);
		pthread_exit(NULL);
	}
	log_info(extense_logger, "Codigo de operacion de socket %i recibido: %i", socket_cliente, cod_op);
	procesar_request(cod_op, socket_cliente);
}

void esperar_cliente(int socket_servidor) {
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	log_info(extense_logger, "Socket cliente %i aceptado", socket_cliente);
	log_info(logger, "Nueva conexion de un proceso con socket cliente %i", socket_cliente);

	if (socket_cliente != -1) {
		int* socket_final = malloc(sizeof(int));
		*socket_final = socket_cliente;

		pthread_t thread;

		pthread_create(&thread, NULL, (void*) atender_cliente, socket_final);
		pthread_detach(thread);
	}
}

void esperar_clientes(void* socket_servidor) {
	int socket = (int) socket_servidor;
	log_info(extense_logger, "Esperando clientes en socket %i", socket);
	while(1) {
		esperar_cliente(socket);
	}
}

int contiene_al_id_respondido(t_list* lista_de_ids_respondidos, uint32_t id_a_buscar) {
	for (int i = 0; i < lista_de_ids_respondidos->elements_count; i++) {
		uint32_t* id = (uint32_t*) list_get(lista_de_ids_respondidos, i);
		if (id_a_buscar == *id) {
			return 1;
		}
	}
	return 0;
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
		log_info(extense_logger, "A");
		free(mensaje_a_eliminar);
		log_info(extense_logger, "B");
		free(elemento_a_eliminar);
		log_info(extense_logger, "C");
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
	registro->id_temporal = generar_id_temporal();
	registro->id_modificacion = generar_id_modificacion();
	registro->tamanio_nombre_pokemon = (strlen(((t_new*) mensaje->mensaje)->pokemon) + 1) * sizeof(char);
	registro->tamanio_lista_coordenadas = 0;

	uint32_t bytes;

	void* mensaje_memoria = serializar_new_para_memoria((t_new*) mensaje->mensaje, &bytes);

	registro->limit = bytes;

	guardar_mensaje_en_memoria(registro, mensaje_memoria);

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
			info_modulo* enviado = list_get(suscriptores_para_enviar, i);
			list_add(mandados, &(enviado->id_cliente));
		}
	}

	t_list* acks = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->ack == 1) {
			info_modulo* enviado = list_get(suscriptores_para_enviar, i);
			list_add(acks, &(enviado->id_cliente));
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
	registro->id_temporal = generar_id_temporal();
	registro->id_modificacion = generar_id_modificacion();
	registro->tamanio_nombre_pokemon = (strlen(((t_appeared*) mensaje->mensaje)->pokemon) + 1) * sizeof(char);
	registro->tamanio_lista_coordenadas = 0;

	uint32_t bytes;

	void* mensaje_memoria = serializar_appeared_para_memoria((t_appeared*) mensaje->mensaje, &bytes);

	registro->limit = bytes;

	guardar_mensaje_en_memoria(registro, mensaje_memoria);

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
			info_modulo* enviado = list_get(suscriptores_para_enviar, i);
			list_add(mandados, &(enviado->id_cliente));
		}
	}

	t_list* acks = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->ack == 1) {
			info_modulo* enviado = list_get(suscriptores_para_enviar, i);
			list_add(acks, &(enviado->id_cliente));
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
	registro->id_temporal = generar_id_temporal();
	registro->id_modificacion = generar_id_modificacion();
	registro->tamanio_nombre_pokemon = (strlen(((t_catch*) mensaje->mensaje)->pokemon) + 1) * sizeof(char);
	registro->tamanio_lista_coordenadas = 0;

	uint32_t bytes;

	void* mensaje_memoria = serializar_catch_para_memoria((t_catch*) mensaje->mensaje, &bytes);

	registro->limit = bytes;

	guardar_mensaje_en_memoria(registro, mensaje_memoria);

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
			info_modulo* enviado = list_get(suscriptores_para_enviar, i);
			list_add(mandados, &(enviado->id_cliente));
		}
	}

	t_list* acks = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->ack == 1) {
			info_modulo* enviado = list_get(suscriptores_para_enviar, i);
			list_add(acks, &(enviado->id_cliente));
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
	registro->id_temporal = generar_id_temporal();
	registro->id_modificacion = generar_id_modificacion();
	registro->tamanio_nombre_pokemon = 0;
	registro->tamanio_lista_coordenadas = 0;

	uint32_t bytes;

	void* mensaje_memoria = serializar_caught_para_memoria((t_caught*) mensaje->mensaje, &bytes);

	registro->limit = bytes;

	guardar_mensaje_en_memoria(registro, mensaje_memoria);

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
			info_modulo* enviado = list_get(suscriptores_para_enviar, i);
			list_add(mandados, &(enviado->id_cliente));
		}
	}

	t_list* acks = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->ack == 1) {
			info_modulo* enviado = list_get(suscriptores_para_enviar, i);
			list_add(acks, &(enviado->id_cliente));
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
	registro->id_temporal = generar_id_temporal();
	registro->id_modificacion = generar_id_modificacion();
	registro->tamanio_nombre_pokemon = (strlen(((t_get*) mensaje->mensaje)->pokemon) + 1) * sizeof(char);
	registro->tamanio_lista_coordenadas = 0;

	uint32_t bytes;

	void* mensaje_memoria = serializar_get_para_memoria((t_get*) mensaje->mensaje, &bytes);

	registro->limit = bytes;

	guardar_mensaje_en_memoria(registro, mensaje_memoria);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	mandable_struct* struct_para_enviar = malloc(sizeof(mandable_struct) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		struct_para_enviar[i].info = list_get(suscriptores_para_enviar, i);
		struct_para_enviar[i].mensaje_queue = mensaje;
		pthread_create(&(threads[i]), NULL, (void*) mandar_get, &struct_para_enviar[i]);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	t_list* mandados = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->envio_ok == 1) {
			info_modulo* enviado = list_get(suscriptores_para_enviar, i);
			list_add(mandados, &(enviado->id_cliente));
		}
	}

	t_list* acks = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->ack == 1) {
			info_modulo* enviado = list_get(suscriptores_para_enviar, i);
			list_add(acks, &(enviado->id_cliente));
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
	registro->id_temporal = generar_id_temporal();
	registro->id_modificacion = generar_id_modificacion();
	registro->tamanio_nombre_pokemon = (strlen(((t_localized*) mensaje->mensaje)->pokemon) + 1) * sizeof(char);
	registro->tamanio_lista_coordenadas = ((t_localized*) mensaje->mensaje)->lugares;

	uint32_t bytes;

	void* mensaje_memoria = serializar_localized_para_memoria((t_localized*) mensaje->mensaje, &bytes);

	registro->limit = bytes;

	guardar_mensaje_en_memoria(registro, mensaje_memoria);

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
			info_modulo* enviado = list_get(suscriptores_para_enviar, i);
			list_add(mandados, &(enviado->id_cliente));
		}
	}

	t_list* acks = list_create();

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		if (struct_para_enviar[i].status->ack == 1) {
			info_modulo* enviado = list_get(suscriptores_para_enviar, i);
			list_add(acks, &(enviado->id_cliente));
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

	status->id_cliente = argumento_new->info->id_cliente;
	status->socket_cliente = argumento_new->info->socket_cliente;

	log_info(extense_logger, "Preparandose para enviar mensaje NEW con id %i al cliente %i socket %i", argumento_new->mensaje_queue->id, argumento_new->info->id_cliente, argumento_new->info->socket_cliente);

	uint32_t bytes;
	void* flujo = serializar_new(argumento_new->mensaje_queue, &bytes);

	log_info(extense_logger, "Enviando mensaje NEW con id %i al cliente %i socket %i", argumento_new->mensaje_queue->id, argumento_new->info->id_cliente, argumento_new->info->socket_cliente);

	if (send(argumento_new->info->socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
		close(argumento_new->info->socket_cliente);
		eliminar_suscriptor(gl_new_list_lock, gl_new_suscriptores, argumento_new->info);
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje NEW con id %i al cliente %i socket %i enviado correctamente", argumento_new->mensaje_queue->id, argumento_new->info->id_cliente, argumento_new->info->socket_cliente);
		log_info(logger, "Mensaje NEW con id %i al cliente %i socket %i enviado correctamente", argumento_new->mensaje_queue->id, argumento_new->info->id_cliente, argumento_new->info->socket_cliente);
		uint32_t id;
		int status_recv = recv(argumento_new->info->socket_cliente, &id, sizeof(uint32_t), MSG_WAITALL);
		if (status_recv == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de cliente %i socket %i", argumento_new->info->id_cliente, argumento_new->info->socket_cliente);
			status->ack = 0;
			//close(argumento_new->info->socket_cliente);
			//eliminar_suscriptor(gl_new_list_lock, gl_new_suscriptores, argumento_new->info);
		} else if (status_recv == 0) {
			log_warning(extense_logger, "El cliente %i acaba de cerrar la conexion correspondiente al socket %i", argumento_new->info->id_cliente, argumento_new->info->socket_cliente);
			status->ack = 0;
			close(argumento_new->info->socket_cliente);
			eliminar_suscriptor(gl_new_list_lock, gl_new_suscriptores, argumento_new->info);
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de cliente %i socket %i recibido", argumento_new->info->id_cliente, argumento_new->info->socket_cliente);
			log_info(logger, "ACK de cliente %i socket %i recibido", argumento_new->info->id_cliente, argumento_new->info->socket_cliente);
		}
	}

	free(flujo);

	argumento_new->status = status;
}

void mandar_appeared(void* appeared_mandable) {
	mandable_struct* argumento_appeared = (mandable_struct*) appeared_mandable;

	status_envio* status = malloc(sizeof(status_envio));

	status->id_cliente = argumento_appeared->info->id_cliente;
	status->socket_cliente = argumento_appeared->info->socket_cliente;

	log_info(extense_logger, "Preparandose para enviar mensaje APPEARED con id %i al cliente %i socket %i", argumento_appeared->mensaje_queue->id, argumento_appeared->info->id_cliente, argumento_appeared->info->socket_cliente);

	uint32_t bytes;
	void* flujo = serializar_appeared(argumento_appeared->mensaje_queue, &bytes);

	log_info(extense_logger, "Enviando mensaje APPEARED con id %i al cliente %i socket %i", argumento_appeared->mensaje_queue->id, argumento_appeared->info->id_cliente, argumento_appeared->info->socket_cliente);

	if (send(argumento_appeared->info->socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
		close(argumento_appeared->info->socket_cliente);
		eliminar_suscriptor(gl_appeared_list_lock, gl_appeared_suscriptores, argumento_appeared->info);
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje APPEARED con id %i al cliente %i socket %i enviado correctamente", argumento_appeared->mensaje_queue->id, argumento_appeared->info->id_cliente, argumento_appeared->info->socket_cliente);
		log_info(logger, "Mensaje APPEARED con id %i al cliente %i socket %i enviado correctamente", argumento_appeared->mensaje_queue->id, argumento_appeared->info->id_cliente, argumento_appeared->info->socket_cliente);
		uint32_t id;
		int status_recv = recv(argumento_appeared->info->socket_cliente,&id ,sizeof(uint32_t), MSG_WAITALL);
		if (status_recv == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de cliente %i socket %i", argumento_appeared->info->id_cliente, argumento_appeared->info->socket_cliente);
			status->ack = 0;
			//close(argumento_appeared->info->socket_cliente);
			//eliminar_suscriptor(gl_appeared_list_lock, gl_appeared_suscriptores, argumento_appeared->info);
		} else if (status_recv == 0) {
			log_error(extense_logger, "El cliente %i acaba de cerrar la conexion correspondiente al socket %i", argumento_appeared->info->id_cliente, argumento_appeared->info->socket_cliente);
			status->ack = 0;
			close(argumento_appeared->info->socket_cliente);
			eliminar_suscriptor(gl_appeared_list_lock, gl_appeared_suscriptores, argumento_appeared->info);
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de cliente %i socket %i recibido", argumento_appeared->info->id_cliente, argumento_appeared->info->socket_cliente);
			log_info(logger, "ACK de cliente %i socket %i recibido", argumento_appeared->info->id_cliente, argumento_appeared->info->socket_cliente);
		}
	}

	free(flujo);

	argumento_appeared->status = status;
}

void mandar_catch(void* catch_mandable) {
	mandable_struct* argumento_catch = (mandable_struct*) catch_mandable;

	status_envio* status = malloc(sizeof(status_envio));

	status->id_cliente = argumento_catch->info->id_cliente;
	status->socket_cliente = argumento_catch->info->socket_cliente;

	log_info(extense_logger, "Preparandose para enviar mensaje CATCH con id %i al cliente %i socket %i", argumento_catch->mensaje_queue->id, argumento_catch->info->id_cliente, argumento_catch->info->socket_cliente);

	uint32_t bytes;
	void* flujo = serializar_catch(argumento_catch->mensaje_queue, &bytes);

	log_info(extense_logger, "Enviando mensaje CATCH con id %i al cliente %i socket %i", argumento_catch->mensaje_queue->id, argumento_catch->info->id_cliente, argumento_catch->info->socket_cliente);

	if (send(argumento_catch->info->socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
		close(argumento_catch->info->socket_cliente);
		eliminar_suscriptor(gl_catch_list_lock, gl_catch_suscriptores, argumento_catch->info);
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje CATCH con id %i al cliente %i socket %i enviado correctamente", argumento_catch->mensaje_queue->id, argumento_catch->info->id_cliente, argumento_catch->info->socket_cliente);
		log_info(logger, "Mensaje CATCH con id %i al cliente %i socket %i enviado correctamente", argumento_catch->mensaje_queue->id, argumento_catch->info->id_cliente, argumento_catch->info->socket_cliente);
		uint32_t id;
		int status_recv = recv(argumento_catch->info->socket_cliente, &id, sizeof(uint32_t), MSG_WAITALL);
		if (status_recv == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de cliente %i socket %i", argumento_catch->info->id_cliente, argumento_catch->info->socket_cliente);
			status->ack = 0;
			//close(argumento_catch->info->socket_cliente);
			//eliminar_suscriptor(gl_catch_list_lock, gl_catch_suscriptores, argumento_catch->info);
		} else if (status_recv == 0) {
			log_warning(extense_logger, "El cliente %i acaba de cerrar la conexion correspondiente al socket %i", argumento_catch->info->id_cliente, argumento_catch->info->socket_cliente);
			status->ack = 0;
			close(argumento_catch->info->socket_cliente);
			eliminar_suscriptor(gl_catch_list_lock, gl_catch_suscriptores, argumento_catch->info);
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de cliente %i socket %i recibido", argumento_catch->info->id_cliente, argumento_catch->info->socket_cliente);
			log_info(logger, "ACK de cliente %i socket %i recibido", argumento_catch->info->id_cliente, argumento_catch->info->socket_cliente);
		}
	}

	free(flujo);

	argumento_catch->status = status;
}

void mandar_caught(void* caught_mandable) {
	mandable_struct* argumento_caught = (mandable_struct*) caught_mandable;

	status_envio* status = malloc(sizeof(status_envio));

	status->id_cliente = argumento_caught->info->id_cliente;
	status->socket_cliente = argumento_caught->info->socket_cliente;

	log_info(extense_logger, "Preparandose para enviar mensaje CAUGHT con id %i al cliente %i socket %i", argumento_caught->mensaje_queue->id, argumento_caught->info->id_cliente, argumento_caught->info->socket_cliente);

	uint32_t bytes;
	void* flujo = serializar_caught(argumento_caught->mensaje_queue, &bytes);

	log_info(extense_logger, "Enviando mensaje CAUGHT con id %i al cliente %i socket %i", argumento_caught->mensaje_queue->id, argumento_caught->info->id_cliente, argumento_caught->info->socket_cliente);

	if (send(argumento_caught->info->socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
		close(argumento_caught->info->socket_cliente);
		eliminar_suscriptor(gl_caught_list_lock, gl_caught_suscriptores, argumento_caught->info);
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje CAUGHT con id %i al cliente %i socket %i enviado correctamente", argumento_caught->mensaje_queue->id, argumento_caught->info->id_cliente, argumento_caught->info->socket_cliente);
		log_info(logger, "Mensaje CAUGHT con id %i al cliente %i socket %i enviado correctamente", argumento_caught->mensaje_queue->id, argumento_caught->info->id_cliente, argumento_caught->info->socket_cliente);
		uint32_t id;
		int status_recv = recv(argumento_caught->info->socket_cliente, &id, sizeof(uint32_t), MSG_WAITALL);
		if (status_recv == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de cliente %i socket %i", argumento_caught->info->id_cliente, argumento_caught->info->socket_cliente);
			status->ack = 0;
			//close(argumento_caught->info->socket_cliente);
			//eliminar_suscriptor(gl_caught_list_lock, gl_caught_suscriptores, argumento_caught->info);
		} else if (status_recv == 0) {
			log_warning(extense_logger, "El cliente %i acaba de cerrar la conexion correspondiente al socket %i", argumento_caught->info->id_cliente, argumento_caught->info->socket_cliente);
			status->ack = 0;
			close(argumento_caught->info->socket_cliente);
			eliminar_suscriptor(gl_catch_list_lock, gl_catch_suscriptores, argumento_caught->info);
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de cliente %i socket %i recibido", argumento_caught->info->id_cliente, argumento_caught->info->socket_cliente);
			log_info(logger, "ACK de cliente %i socket %i recibido", argumento_caught->info->id_cliente, argumento_caught->info->socket_cliente);
		}
	}

	free(flujo);

	argumento_caught->status = status;
}

void mandar_get(void* get_mandable) {
	mandable_struct* argumento_get = (mandable_struct*) get_mandable;

	status_envio* status = malloc(sizeof(status_envio));

	status->id_cliente = argumento_get->info->id_cliente;
	status->socket_cliente = argumento_get->info->socket_cliente;

	log_info(extense_logger, "Preparandose para enviar mensaje GET con id %i al cliente %i socket %i", argumento_get->mensaje_queue->id, argumento_get->info->id_cliente, argumento_get->info->socket_cliente);

	uint32_t bytes;
	void* flujo = serializar_get(argumento_get->mensaje_queue, &bytes);

	log_info(extense_logger, "Enviando mensaje GET con id %i al cliente %i socket %i", argumento_get->mensaje_queue->id, argumento_get->info->id_cliente, argumento_get->info->socket_cliente);

	if (send(argumento_get->info->socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
		close(argumento_get->info->socket_cliente);
		eliminar_suscriptor(gl_get_list_lock, gl_get_suscriptores, argumento_get->info);
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje GET con id %i al cliente %i socket %i enviado correctamente", argumento_get->mensaje_queue->id, argumento_get->info->id_cliente, argumento_get->info->socket_cliente);
		log_info(logger, "Mensaje GET con id %i al cliente %i socket %i enviado correctamente", argumento_get->mensaje_queue->id, argumento_get->info->id_cliente, argumento_get->info->socket_cliente);
		uint32_t id;
		int status_recv = recv(argumento_get->info->socket_cliente, &id, sizeof(uint32_t), MSG_WAITALL);
		if (status_recv == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de cliente %i socket %i", argumento_get->info->id_cliente, argumento_get->info->socket_cliente);
			status->ack = 0;
			//close(argumento_get->info->socket_cliente);
			//eliminar_suscriptor(gl_get_list_lock, gl_get_suscriptores, argumento_get->info);
		} else if (status_recv == 0) {
			log_warning(extense_logger, "El cliente %i acaba de cerrar la conexion correspondiente al socket %i", argumento_get->info->id_cliente, argumento_get->info->socket_cliente);
			status->ack = 0;
			close(argumento_get->info->socket_cliente);
			eliminar_suscriptor(gl_get_list_lock, gl_get_suscriptores, argumento_get->info);
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de cliente %i socket %i recibido", argumento_get->info->id_cliente, argumento_get->info->socket_cliente);
			log_info(logger, "ACK de cliente %i socket %i recibido", argumento_get->info->id_cliente, argumento_get->info->socket_cliente);
		}
	}

	free(flujo);

	argumento_get->status = status;
}

void mandar_localized(void* localized_mandable) {
	mandable_struct* argumento_localized = (mandable_struct*) localized_mandable;

	status_envio* status = malloc(sizeof(status_envio));

	status->id_cliente = argumento_localized->info->id_cliente;
	status->socket_cliente = argumento_localized->info->socket_cliente;

	log_info(extense_logger, "Preparandose para enviar mensaje LOCALIZED con id %i al cliente %i socket %i", argumento_localized->mensaje_queue->id, argumento_localized->info->id_cliente, argumento_localized->info->socket_cliente);

	uint32_t bytes;
	void* flujo = serializar_localized(argumento_localized->mensaje_queue, &bytes);

	log_info(extense_logger, "Enviando mensaje LOCALIZED con cliente %i socket %i puerto %s", argumento_localized->mensaje_queue->id, argumento_localized->info->id_cliente, argumento_localized->info->socket_cliente);

	if (send(argumento_localized->info->socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
		close(argumento_localized->info->socket_cliente);
		eliminar_suscriptor(gl_localized_list_lock, gl_localized_suscriptores, argumento_localized->info);
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje LOCALIZED con id %i al cliente %i socket %i enviado correctamente", argumento_localized->mensaje_queue->id, argumento_localized->info->id_cliente, argumento_localized->info->socket_cliente);
		log_info(logger, "Mensaje LOCALIZED con id %i al cliente %i socket %i enviado correctamente", argumento_localized->mensaje_queue->id, argumento_localized->info->id_cliente, argumento_localized->info->socket_cliente);
		uint32_t id;
		int status_recv = recv(argumento_localized->info->socket_cliente, &id, sizeof(uint32_t), MSG_WAITALL);
		if (status_recv == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de cliente %i socket %i", argumento_localized->info->id_cliente, argumento_localized->info->socket_cliente);
			status->ack = 0;
			//close(argumento_localized->info->socket_cliente);
			//eliminar_suscriptor(gl_localized_list_lock, gl_localized_suscriptores, argumento_localized->info);
		} else if (status_recv == 0) {
			log_warning(extense_logger, "El cliente %i acaba de cerrar la conexion correspondiente al socket %i", argumento_localized->info->id_cliente, argumento_localized->info->socket_cliente);
			status->ack = 0;
			close(argumento_localized->info->socket_cliente);
			eliminar_suscriptor(gl_localized_list_lock, gl_localized_suscriptores, argumento_localized->info);
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de cliente %i socket %i recibido", argumento_localized->info->id_cliente, argumento_localized->info->socket_cliente);
			log_info(logger, "ACK de cliente %i socket %i recibido", argumento_localized->info->id_cliente, argumento_localized->info->socket_cliente);
		}
	}

	free(flujo);

	argumento_localized->status = status;
}

void enviar_new_de_memoria(info_modulo* info_modulo) {
	t_list* segmentos = obtener_segmentos_new(info_modulo->id_cliente);

	if (segmentos->elements_count == 0) {
		log_info(extense_logger, "No existen segmentos NEW a enviar al cliente %i socket %i", info_modulo->id_cliente, info_modulo->socket_cliente);
	} else {
		log_info(extense_logger, "Segmentos NEW a enviar al cliente %i socket %i: %i", info_modulo->id_cliente, info_modulo->socket_cliente, segmentos->elements_count);

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

	for (int i = 0; i < segmentos->elements_count; i++) {
		segmento_memoria* seg = list_get(segmentos, i);
		free(seg);
	}

	list_destroy(segmentos);
}

void enviar_appeared_de_memoria(info_modulo* info_modulo) {
	t_list* segmentos = obtener_segmentos_appeared(info_modulo->id_cliente);

	if (segmentos->elements_count == 0) {
		log_info(extense_logger, "No existen segmentos APPEARED a enviar al cliente %i socket %i", info_modulo->id_cliente, info_modulo->socket_cliente);
	} else {
		log_info(extense_logger, "Segmentos APPEARED a enviar al cliente %i socket %i: %i", info_modulo->id_cliente, info_modulo->socket_cliente, segmentos->elements_count);
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

	for (int i = 0; i < segmentos->elements_count; i++) {
		segmento_memoria* seg = list_get(segmentos, i);
		free(seg);
	}

	list_destroy(segmentos);
}

void enviar_catch_de_memoria(info_modulo* info_modulo) {
	t_list* segmentos = obtener_segmentos_catch(info_modulo->id_cliente);

	if (segmentos->elements_count == 0) {
		log_info(extense_logger, "No existen segmentos CATCH a enviar al cliente %i socket %i", info_modulo->id_cliente, info_modulo->socket_cliente);
	} else {
		log_info(extense_logger, "Segmentos CATCH a enviar al cliente %i socket %i: %i", info_modulo->id_cliente, info_modulo->socket_cliente, segmentos->elements_count);
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

	for (int i = 0; i < segmentos->elements_count; i++) {
		segmento_memoria* seg = list_get(segmentos, i);
		free(seg);
	}

	list_destroy(segmentos);
}

void enviar_caught_de_memoria(info_modulo* info_modulo) {
	t_list* segmentos = obtener_segmentos_caught(info_modulo->id_cliente);

	if (segmentos->elements_count == 0) {
		log_info(extense_logger, "No existen segmentos CAUGHT a enviar al cliente %i socket %i", info_modulo->id_cliente, info_modulo->socket_cliente);
	} else {
		log_info(extense_logger, "Segmentos CAUGHT a enviar al cliente %i socket %i: %i", info_modulo->id_cliente, info_modulo->socket_cliente, segmentos->elements_count);
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

	for (int i = 0; i < segmentos->elements_count; i++) {
		segmento_memoria* seg = list_get(segmentos, i);
		free(seg);
	}

	list_destroy(segmentos);
}

void enviar_get_de_memoria(info_modulo* info_modulo) {
	t_list* segmentos = obtener_segmentos_get(info_modulo->id_cliente);

	if (segmentos->elements_count == 0) {
		log_info(extense_logger, "No existen segmentos GET a enviar al cliente %i socket %i", info_modulo->id_cliente, info_modulo->socket_cliente);
	} else {
		log_info(extense_logger, "Segmentos GET a enviar al cliente %i socket %i: %i", info_modulo->id_cliente, info_modulo->socket_cliente, segmentos->elements_count);
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

	for (int i = 0; i < segmentos->elements_count; i++) {
		segmento_memoria* seg = list_get(segmentos, i);
		free(seg);
	}

	list_destroy(segmentos);
}

void enviar_localized_de_memoria(info_modulo* info_modulo) {
	t_list* segmentos = obtener_segmentos_localized(info_modulo->id_cliente);

	if (segmentos->elements_count == 0) {
		log_info(extense_logger, "No existen segmentos LOCALIZED a enviar al cliente %i socket %i", info_modulo->id_cliente, info_modulo->socket_cliente);
	} else {
		log_info(extense_logger, "Segmentos LOCALIZED a enviar al cliente %i socket %i: %i", info_modulo->id_cliente, info_modulo->socket_cliente, segmentos->elements_count);
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

	for (int i = 0; i < segmentos->elements_count; i++) {
		segmento_memoria* seg = list_get(segmentos, i);
		free(seg);
	}

	list_destroy(segmentos);
}

void enviar_mensaje_new_de_memoria(void* new_mandable_memoria) {
	mandable_memoria_struct* argumento = (mandable_memoria_struct*) new_mandable_memoria;

	status_envio* status = malloc(sizeof(status_envio));

	int bytes;
	void* flujo = serializar_new_desde_memoria(argumento->segmento, &bytes);

	log_info(extense_logger, "Enviando mensaje NEW de memoria con id %i al cliente %i socket %i", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);

	if (send(argumento->info_modulo->socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
		close(argumento->info_modulo->socket_cliente);
		eliminar_suscriptor(gl_new_list_lock, gl_new_suscriptores, argumento->info_modulo);
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje NEW de memoria con id %i al cliente %i socket %i enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		log_info(logger, "Mensaje NEW de memoria con id %i al cliente %i socket %i enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		uint32_t id;
		int status_recv = recv(argumento->info_modulo->socket_cliente, &id, sizeof(uint32_t), MSG_WAITALL);
		if (status_recv == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de cliente %i socket %i", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			status->ack = 0;
			//close(argumento->info_modulo->socket_cliente);
			//eliminar_suscriptor(gl_new_list_lock, gl_new_suscriptores, argumento->info_modulo);
		} else if (status_recv == 0) {
			log_warning(extense_logger, "El cliente %i acaba de cerrar la conexion correspondiente al socket %i", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			status->ack = 0;
			close(argumento->info_modulo->socket_cliente);
			eliminar_suscriptor(gl_new_list_lock, gl_new_suscriptores, argumento->info_modulo);
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de cliente %i socket %i recibido", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			log_info(logger, "ACK de cliente %i socket %i recibido", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		}
	}

	if (1 == status->envio_ok) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->envios, &(argumento->info_modulo->id_cliente));
		argumento->segmento->registro->id_modificacion = generar_id_modificacion();
		pthread_mutex_unlock(&mutex_memoria);
	}
	if (1 == status->ack) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->acknowledgements, &(argumento->info_modulo->id_cliente));
		argumento->segmento->registro->id_modificacion = generar_id_modificacion();
		pthread_mutex_unlock(&mutex_memoria);
	}

	free(status);
}

void enviar_mensaje_appeared_de_memoria(void* appeared_mandable_memoria) {
	mandable_memoria_struct* argumento = (mandable_memoria_struct*) appeared_mandable_memoria;

	status_envio* status = malloc(sizeof(status_envio));

	int bytes;
	void* flujo = serializar_appeared_desde_memoria(argumento->segmento, &bytes);

	log_info(extense_logger, "Enviando mensaje APPEARED de memoria con id %i al cliente %i socket %i", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);

	if (send(argumento->info_modulo->socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
		close(argumento->info_modulo->socket_cliente);
		eliminar_suscriptor(gl_appeared_list_lock, gl_appeared_suscriptores, argumento->info_modulo);
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje APPEARED de memoria con id %i al cliente %i socket %i enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		log_info(logger, "Mensaje APPEARED de memoria con id %i al cliente %i socket %i enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		uint32_t id;
		int status_recv = recv(argumento->info_modulo->socket_cliente, &id, sizeof(uint32_t), MSG_WAITALL);
		if (status_recv == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de cliente %i socket %i", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			status->ack = 0;
			//close(argumento->info_modulo->socket_cliente);
			//eliminar_suscriptor(gl_appeared_list_lock, gl_appeared_suscriptores, argumento->info_modulo);
		} else if (status_recv == 0) {
			log_warning(extense_logger, "El cliente %i acaba de cerrar la conexion correspondiente al socket %i", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			status->ack = 0;
			close(argumento->info_modulo->socket_cliente);
			eliminar_suscriptor(gl_appeared_list_lock, gl_appeared_suscriptores, argumento->info_modulo);
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de cliente %i socket %i recibido", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			log_info(logger, "ACK de cliente %i socket %i recibido", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		}
	}

	if (1 == status->envio_ok) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->envios, &(argumento->info_modulo->id_cliente));
		argumento->segmento->registro->id_modificacion = generar_id_modificacion();
		pthread_mutex_unlock(&mutex_memoria);
	}
	if (1 == status->ack) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->acknowledgements, &(argumento->info_modulo->id_cliente));
		argumento->segmento->registro->id_modificacion = generar_id_modificacion();
		pthread_mutex_unlock(&mutex_memoria);
	}

	free(status);
}

void enviar_mensaje_catch_de_memoria(void* catch_mandable_memoria) {
	mandable_memoria_struct* argumento = (mandable_memoria_struct*) catch_mandable_memoria;

	status_envio* status = malloc(sizeof(status_envio));

	int bytes;
	void* flujo = serializar_catch_desde_memoria(argumento->segmento, &bytes);

	log_info(extense_logger, "Enviando mensaje CATCH de memoria con id %i al cliente %i socket %i", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);

	if (send(argumento->info_modulo->socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
		close(argumento->info_modulo->socket_cliente);
		eliminar_suscriptor(gl_catch_list_lock, gl_catch_suscriptores, argumento->info_modulo);
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje CATCH de memoria con id %i al cliente %i socket %i enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		log_info(logger, "Mensaje CATCH de memoria con id %i al cliente %i socket %i enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		uint32_t id;
		int status_recv = recv(argumento->info_modulo->socket_cliente, &id, sizeof(uint32_t), MSG_WAITALL);
		if (status_recv == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de cliente %i socket %i", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			status->ack = 0;
			//close(argumento->info_modulo->socket_cliente);
			//eliminar_suscriptor(gl_catch_list_lock, gl_catch_suscriptores, argumento->info_modulo);
		} else if (status_recv == 0) {
			log_warning(extense_logger, "El cliente %i acaba de cerrar la conexion correspondiente al socket %i", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			status->ack = 0;
			close(argumento->info_modulo->socket_cliente);
			eliminar_suscriptor(gl_catch_list_lock, gl_catch_suscriptores, argumento->info_modulo);
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de cliente %i socket %i recibido", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			log_info(logger, "ACK de cliente %i socket %i recibido", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		}
	}

	if (1 == status->envio_ok) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->envios, &(argumento->info_modulo->id_cliente));
		argumento->segmento->registro->id_modificacion = generar_id_modificacion();
		pthread_mutex_unlock(&mutex_memoria);
	}
	if (1 == status->ack) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->acknowledgements, &(argumento->info_modulo->id_cliente));
		argumento->segmento->registro->id_modificacion = generar_id_modificacion();
		pthread_mutex_unlock(&mutex_memoria);
	}

	free(status);
}

void enviar_mensaje_caught_de_memoria(void* caught_mandable_memoria) {
	mandable_memoria_struct* argumento = (mandable_memoria_struct*) caught_mandable_memoria;

	status_envio* status = malloc(sizeof(status_envio));

	int bytes;
	void* flujo = serializar_caught_desde_memoria(argumento->segmento, &bytes);

	log_info(extense_logger, "Enviando mensaje CAUGHT %i de memoria con cliente %i socket %i", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);

	if (send(argumento->info_modulo->socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
		close(argumento->info_modulo->socket_cliente);
		eliminar_suscriptor(gl_caught_list_lock, gl_caught_suscriptores, argumento->info_modulo);
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje CAUGHT de memoria con id %i al cliente %i socket %i enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		log_info(logger, "Mensaje CAUGHT de memoria con id %i al cliente %i socket %i enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		uint32_t id;
		int status_recv = recv(argumento->info_modulo->socket_cliente, &id, sizeof(uint32_t), MSG_WAITALL);
		if (status_recv == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de cliente %i socket %i", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			status->ack = 0;
			//close(argumento->info_modulo->socket_cliente);
			//eliminar_suscriptor(gl_caught_list_lock, gl_caught_suscriptores, argumento->info_modulo);
		} else if (status_recv == 0) {
			log_warning(extense_logger, "El cliente %i acaba de cerrar la conexion correspondiente al socket %i", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			status->ack = 0;
			close(argumento->info_modulo->socket_cliente);
			eliminar_suscriptor(gl_caught_list_lock, gl_caught_suscriptores, argumento->info_modulo);
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de cliente %i socket %i recibido", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			log_info(logger, "ACK de cliente %i socket %i recibido", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		}
	}

	if (1 == status->envio_ok) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->envios, &(argumento->info_modulo->id_cliente));
		argumento->segmento->registro->id_modificacion = generar_id_modificacion();
		pthread_mutex_unlock(&mutex_memoria);
	}
	if (1 == status->ack) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->acknowledgements, &(argumento->info_modulo->id_cliente));
		argumento->segmento->registro->id_modificacion = generar_id_modificacion();
		pthread_mutex_unlock(&mutex_memoria);
	}

	free(status);
}

void enviar_mensaje_get_de_memoria(void* get_mandable_memoria) {
	mandable_memoria_struct* argumento = (mandable_memoria_struct*) get_mandable_memoria;

	status_envio* status = malloc(sizeof(status_envio));

	int bytes;
	void* flujo = serializar_get_desde_memoria(argumento->segmento, &bytes);

	log_info(extense_logger, "Enviando mensaje GET de memoria con id %i al cliente %i socket %i", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);

	if (send(argumento->info_modulo->socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		status->envio_ok = 0;
		status->ack = 0;
		close(argumento->info_modulo->socket_cliente);
		eliminar_suscriptor(gl_get_list_lock, gl_get_suscriptores, argumento->info_modulo);
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje GET de memoria con id %i al cliente %i socket %i enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		log_info(logger, "Mensaje GET de memoria con id %i al cliente %i socket %i enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		uint32_t id;
		int status_recv = recv(argumento->info_modulo->socket_cliente, &id, sizeof(uint32_t), MSG_WAITALL);
		if (status_recv == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de cliente %i socket %i", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			status->ack = 0;
			//close(argumento->info_modulo->socket_cliente);
			//eliminar_suscriptor(gl_get_list_lock, gl_get_suscriptores, argumento->info_modulo);
		} else if (status_recv == 0) {
			log_warning(extense_logger, "El cliente %i acaba de cerrar la conexion correspondiente al socket %i", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			status->ack = 0;
			close(argumento->info_modulo->socket_cliente);
			eliminar_suscriptor(gl_get_list_lock, gl_get_suscriptores, argumento->info_modulo);
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de cliente %i socket %i recibido", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			log_info(logger, "ACK de cliente %i socket %i recibido", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		}
	}

	if (1 == status->envio_ok) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->envios, &(argumento->info_modulo->id_cliente));
		argumento->segmento->registro->id_modificacion = generar_id_modificacion();
		pthread_mutex_unlock(&mutex_memoria);
	}
	if (1 == status->ack) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->acknowledgements, &(argumento->info_modulo->id_cliente));
		argumento->segmento->registro->id_modificacion = generar_id_modificacion();
		pthread_mutex_unlock(&mutex_memoria);
	}

	free(status);
}

void enviar_mensaje_localized_de_memoria(void* localized_mandable_memoria) {
	mandable_memoria_struct* argumento = (mandable_memoria_struct*) localized_mandable_memoria;

	status_envio* status = malloc(sizeof(status_envio));

	int bytes;
	void* flujo = serializar_localized_desde_memoria(argumento->segmento, &bytes);

	log_info(extense_logger, "Enviando mensaje LOCALIZED de memoria con id %i al cliente %i socket %i", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);

	if (send(argumento->info_modulo->socket_cliente, flujo, bytes, 0) == -1) {
		log_error(extense_logger, "Error: No se pudo enviar el mensaje");
		close(argumento->info_modulo->socket_cliente);
		status->envio_ok = 0;
		status->ack = 0;
		eliminar_suscriptor(gl_localized_list_lock, gl_localized_suscriptores, argumento->info_modulo);
	} else {
		status->envio_ok = 1;
		log_info(extense_logger, "Mensaje LOCALIZED de memoria con id %i al cliente %i socket %i enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		log_info(logger, "Mensaje LOCALIZED de memoria con id %i al cliente %i socket %i enviado correctamente", argumento->segmento->registro->id, argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		uint32_t id;
		int status_recv = recv(argumento->info_modulo->socket_cliente, &id, sizeof(uint32_t), MSG_WAITALL);
		if (status_recv == -1) {
			log_error(extense_logger, "Error: No se recibió el ACK de cliente %i socket %i", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			status->ack = 0;
			//close(argumento->info_modulo->socket_cliente);
			//eliminar_suscriptor(gl_localized_list_lock, gl_localized_suscriptores, argumento->info_modulo);
		} else if (status_recv == 0) {
			log_warning(extense_logger, "El cliente %i acaba de cerrar la conexion correspondiente al socket %i", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			close(argumento->info_modulo->socket_cliente);
			status->ack = 0;
			eliminar_suscriptor(gl_localized_list_lock, gl_localized_suscriptores, argumento->info_modulo);
		} else {
			status->ack = 1;
			log_info(extense_logger, "ACK de cliente %i socket %i recibido", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
			log_info(logger, "ACK de cliente %i socket %i recibido", argumento->info_modulo->id_cliente, argumento->info_modulo->socket_cliente);
		}
	}

	if (1 == status->envio_ok) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->envios, &(argumento->info_modulo->id_cliente));
		argumento->segmento->registro->id_modificacion = generar_id_modificacion();
		pthread_mutex_unlock(&mutex_memoria);
	}
	if (1 == status->ack) {
		pthread_mutex_lock(&mutex_memoria);
		list_add(argumento->segmento->registro->acknowledgements, &(argumento->info_modulo->id_cliente));
		argumento->segmento->registro->id_modificacion = generar_id_modificacion();
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

	uint32_t cod_op = 3;
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

	uint32_t cod_op = 4;
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

	uint32_t cod_op = 5;
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
	log_info(extense_logger, "A");
	uint32_t tamanio_pokemon = (strlen(((t_localized*) localized->mensaje)->pokemon) + 1) * sizeof(char);
	log_info(extense_logger, "B");
	uint32_t elementos_lista = ((t_localized*) localized->mensaje)->l_coordenadas->elements_count;
	log_info(extense_logger, "C");
	uint32_t tamanio_lista = elementos_lista * sizeof(uint32_t);

	*bytes = sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t) + tamanio_pokemon +
			sizeof(uint32_t) + tamanio_lista;

	log_info(extense_logger, "D");

	uint32_t cod_op = 6;
	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	log_info(extense_logger, "E");

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

	log_info(extense_logger, "F");

	for (int i = 0; i < elementos_lista; i++) {
		uint32_t* elemento_de_lista = (uint32_t*) list_get(((t_localized*) localized->mensaje)->l_coordenadas, i);
		memcpy(flujo + desplazamiento, elemento_de_lista, sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
	}

	log_info(extense_logger, "G");

	return flujo;
}

void* serializar_new_para_memoria(t_new* new, uint32_t* bytes) {
	int tamanio_pokemon = (strlen(new->pokemon) + 1) * sizeof(char);

	*bytes = tamanio_pokemon + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);

	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, new->pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;
	memcpy(flujo + desplazamiento, &(new->pos_X), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(new->pos_Y), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(new->cantidad), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	return flujo;
}

void* serializar_appeared_para_memoria(t_appeared* appeared, uint32_t* bytes) {
	int tamanio_pokemon = (strlen(appeared->pokemon) + 1) * sizeof(char);

	*bytes = tamanio_pokemon + sizeof(uint32_t) + sizeof(uint32_t);

	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, appeared->pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;
	memcpy(flujo + desplazamiento, &(appeared->pos_X), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(appeared->pos_Y), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	return flujo;
}

void* serializar_catch_para_memoria(t_catch* catch, uint32_t* bytes) {
	int tamanio_pokemon = (strlen(catch->pokemon) + 1) * sizeof(char);

	*bytes = tamanio_pokemon + sizeof(uint32_t) + sizeof(uint32_t);

	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, catch->pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;
	memcpy(flujo + desplazamiento, &(catch->pos_X), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(catch->pos_Y), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	return flujo;
}

void* serializar_caught_para_memoria(t_caught* caught, uint32_t* bytes) {

	*bytes = sizeof(uint32_t);

	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &(caught->flag_caught), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	return flujo;
}

void* serializar_get_para_memoria(t_get* get, uint32_t* bytes) {
	int tamanio_pokemon = (strlen(get->pokemon) + 1) * sizeof(char);

	*bytes = tamanio_pokemon;

	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, get->pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;

	return flujo;
}

void* serializar_localized_para_memoria(t_localized* localized, uint32_t* bytes) {
	int tamanio_pokemon = (strlen(localized->pokemon) + 1) * sizeof(char);

	*bytes = tamanio_pokemon + sizeof(uint32_t)
						+ (localized->lugares * 2 * sizeof(uint32_t));

	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, localized->pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;
	memcpy(flujo + desplazamiento, &(localized->lugares), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);


	for (int i = 0; i < localized->l_coordenadas->elements_count; i++) {
		uint32_t* elemento_de_lista = (uint32_t*) list_get(localized->l_coordenadas, i);
		memcpy(flujo + desplazamiento, &elemento_de_lista, sizeof(uint32_t));
		desplazamiento += sizeof(uint32_t);
	}

	return flujo;
}

void* serializar_new_desde_memoria(segmento_memoria* new, int* bytes) {
	*bytes = sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t) + sizeof(uint32_t) +
			new->registro->tamanio_nombre_pokemon + sizeof(uint32_t) +
			sizeof(uint32_t) + sizeof(uint32_t);

	uint32_t cod_op = 1;
	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	log_info(extense_logger, "Serializando new de memoria, codigo de operacion: %i", cod_op);
	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	log_info(extense_logger, "Serializando new de memoria, bytes: %i", *bytes);
	memcpy(flujo + desplazamiento, bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	log_info(extense_logger, "Serializando new de memoria, id: %i", new->registro->id);
	memcpy(flujo + desplazamiento, &(new->registro->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	log_info(extense_logger, "Serializando new de memoria, tamanio del pokemon: %i", new->registro->tamanio_nombre_pokemon);
	memcpy(flujo + desplazamiento, &(new->registro->tamanio_nombre_pokemon), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, new->mensaje, new->registro->limit);
	desplazamiento += new->registro->limit;

	return flujo;
}

void* serializar_appeared_desde_memoria(segmento_memoria* appeared, int* bytes) {
	*bytes = sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t) + sizeof(uint32_t) +
			appeared->registro->tamanio_nombre_pokemon + sizeof(uint32_t) +
			sizeof(uint32_t) + sizeof(uint32_t);

	uint32_t cod_op = 2;
	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(appeared->registro->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(appeared->registro->id_correlativo), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(appeared->registro->tamanio_nombre_pokemon), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, appeared->mensaje, appeared->registro->limit);
	desplazamiento += appeared->registro->limit;

	return flujo;
}

void* serializar_catch_desde_memoria(segmento_memoria* catch, int* bytes) {
	*bytes = sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t) + sizeof(uint32_t) +
			catch->registro->tamanio_nombre_pokemon +
			sizeof(uint32_t) + sizeof(uint32_t);

	uint32_t cod_op = 3;
	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(catch->registro->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(catch->registro->tamanio_nombre_pokemon), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, catch->mensaje, catch->registro->limit);
	desplazamiento += catch->registro->limit;

	return flujo;
}

void* serializar_caught_desde_memoria(segmento_memoria* caught, int* bytes) {

	*bytes = sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t);

	uint32_t cod_op = 4;
	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(caught->registro->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(caught->registro->id_correlativo), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, caught->mensaje, caught->registro->limit);
	desplazamiento += caught->registro->limit;

	return flujo;
}

void* serializar_get_desde_memoria(segmento_memoria* get, int* bytes) {
	*bytes = sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t) + sizeof(uint32_t) +
			get->registro->tamanio_nombre_pokemon;

	uint32_t cod_op = 5;
	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(get->registro->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(get->registro->tamanio_nombre_pokemon), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, get->mensaje, get->registro->limit);
	desplazamiento += get->registro->limit;

	return flujo;
}

void* serializar_localized_desde_memoria(segmento_memoria* localized, int* bytes) {
	uint32_t tamanio_lista = localized->registro->tamanio_lista_coordenadas * sizeof(uint32_t);

	*bytes = sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t) + sizeof(uint32_t) +
			sizeof(uint32_t) + localized->registro->tamanio_nombre_pokemon +
			sizeof(uint32_t) + tamanio_lista;

	uint32_t cod_op = 6;
	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(localized->registro->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(localized->registro->id_correlativo), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(localized->registro->tamanio_nombre_pokemon), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, localized->mensaje, localized->registro->limit);
	desplazamiento += localized->registro->limit;

	return flujo;
}

void eliminar_suscriptor(pthread_mutex_t lock, t_list* lista_de_suscriptores, info_modulo* suscriptor) {
	pthread_mutex_lock(&lock);

	int posicion = -1;

	for (int i = 0; i < lista_de_suscriptores->elements_count; i++) {
		info_modulo* info = list_get(lista_de_suscriptores, i);
		if (info->id_cliente == suscriptor->id_cliente) {
			posicion = i;
		}
	}

	if (posicion == -1) {
		log_info(extense_logger, "Suscriptor con id %i y socket %i no fue encontrado en la lista de suscriptores intentando ser eliminado, tal vez ya fue eliminado", suscriptor->id_cliente, suscriptor->socket_cliente);
	} else {
		log_info(extense_logger, "Eliminando suscriptor con id %i y socket %i", suscriptor->id_cliente, suscriptor->socket_cliente);
		info_modulo* a_remover = list_remove(lista_de_suscriptores, posicion);
		free(a_remover);
	}

	pthread_mutex_unlock(&lock);
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
	logger = iniciar_logger_sin_consola(log_file);
	log_info(extense_logger, "logger de la catedra iniciado");
	ip = config_get_string_value(config, "IP");
	log_info(extense_logger, "La IP es: %s", ip);
	puerto = config_get_string_value(config, "PUERTO");
	log_info(extense_logger, "El puerto es: %s", puerto);

	inicializar_memoria();

	// me abro a la señal para hacer el dump de la cache

	signal(SIGUSR1, dump_cache);

	inicializar_cola();

	inicializar_semaforos_colas();

	log_info(extense_logger, "Creando generador id");

	crea_generador_id();

	log_info(extense_logger, "Generador id creado");

	// Levantamos el servidor en la ip y el puerto definidos en config

	log_info(extense_logger, "Levantando servidor");

	int socket_servidor = iniciar_servidor(ip, puerto);

	log_info(extense_logger, "Servidor levantado");

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

	terminar_programa();
	return EXIT_SUCCESS;
}

void inicializar_memoria() {
	tamanio_memoria = config_get_int_value(config, "TAMANO_MEMORIA");

	log_info(extense_logger, "El tamanio de la memoria es %i", tamanio_memoria);

	frecuencia_compactacion = config_get_int_value(config, "FRECUENCIA_COMPACTACION");

	cantidad_eliminacion = 0;

	log_info(extense_logger, "La frecuencia de la compactacion es %i", frecuencia_compactacion);

	tamanio_minimo_particion = config_get_int_value(config, "TAMANO_MINIMO_PARTICION");

	log_info(extense_logger, "El tamanio minimo de la particion es %i", tamanio_minimo_particion);

	char* algoritmo_busqueda = config_get_string_value(config, "ALGORITMO_PARTICION_LIBRE");

	log_info(extense_logger, "El algoritmo de busqueda de particiones libres es %s", algoritmo_busqueda);

	char* algoritmo_reemplazo = config_get_string_value(config, "ALGORITMO_REEMPLAZO");

	log_info(extense_logger, "El algoritmo de reemplazo de particiones es %s", algoritmo_reemplazo);

	logger_memoria = logger;
	extense_logger_memoria = extense_logger;

	char* ff = "FF";

	int resultado_busqueda = strcasecmp(ff, algoritmo_busqueda);

	if (0 == resultado_busqueda) {
		algoritmo_particion_libre = 1; // FF First fit
	} else {
		algoritmo_particion_libre = 2; // BF Best fit
	}

	char* fifo = "FIFO";

	int resultado_reemplazo = strcasecmp(fifo, algoritmo_reemplazo);

	if (0 == resultado_reemplazo) {
		algoritmo_reemplazo_particiones = 1; // FIFO First in first out
	} else {
		algoritmo_reemplazo_particiones = 2; // LRU Least recently used
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

void inicializar_cola(void) {
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

	log_info(extense_logger, "Iniciando listas de ids respondidos");

	gl_appeared_ids_respondidos = list_create();
	gl_caught_ids_respondidos = list_create();
	gl_localized_ids_respondidos = list_create();

	log_info(extense_logger, "Listas de ids respondidos iniciadas");

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

t_log* iniciar_logger_sin_consola(char* log_file) {
	return log_create(log_file, "Broker", 0, LOG_LEVEL_INFO);
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

	log_info(extense_logger, "Iniciando semaforos de ids respondidos");

	pthread_mutex_init(&gl_appeared_ids_lock, NULL);
	pthread_mutex_init(&gl_caught_ids_lock, NULL);
	pthread_mutex_init(&gl_localized_ids_lock, NULL);

	log_info(extense_logger, "Semaforos de ids respondidos iniciados");
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
	list_clean(gl_appeared_ids_respondidos);
	list_clean(gl_caught_ids_respondidos);
	list_clean(gl_localized_ids_respondidos);
	list_destroy(gl_appeared_ids_respondidos);
	list_destroy(gl_caught_ids_respondidos);
	list_destroy(gl_localized_ids_respondidos);
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
	pthread_mutex_destroy(&gl_appeared_ids_lock);
	pthread_mutex_destroy(&gl_caught_ids_lock);
	pthread_mutex_destroy(&gl_localized_ids_lock);

	destruir_generador_id();
}
