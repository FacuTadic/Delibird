#include "broker.h"

void procesar_request(int cod_op, int cliente_fd) {
	int size;

	mensaje_queue* mensaje_a_guardar = malloc(sizeof(mensaje_queue));
	mensaje_a_guardar->id = generar_id();

	switch (cod_op) {
	case NEW: ;
		log_info(extense_logger, "Codigo de operacion recibido de %i corresponde a un NEW", cliente_fd);
		t_new* new_msg = recibir_new(cliente_fd, &size, extense_logger);

		mensaje_a_guardar->mensaje = (void*) new_msg;

		sem_wait(&gl_new_limite);
		pthread_mutex_lock(&gl_new_queue_lock);

		log_info(extense_logger, "Encolando el mensaje NEW");
		queue_push(gl_new_pokemon_queue, mensaje_a_guardar);
		log_info(extense_logger, "Mensaje NEW encolado");

		pthread_mutex_unlock(&gl_new_queue_lock);
		sem_post(&gl_new_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

		close(cliente_fd);
		break;
	case APPEARED: ;
		log_info(extense_logger, "Codigo de operacion recibido de %i corresponde a un APPEARED", cliente_fd);
		t_appeared* appeared_msg = recibir_appeared(cliente_fd, &size, extense_logger);

		mensaje_a_guardar->mensaje = (void*) appeared_msg;

		sem_wait(&gl_appeared_limite);
		pthread_mutex_lock(&gl_appeared_queue_lock);

		log_info(extense_logger, "Encolando el mensaje APPEARED");
		queue_push(gl_appeared_pokemon_queue, mensaje_a_guardar);
		log_info(extense_logger, "Mensaje APPEARED encolado");

		pthread_mutex_unlock(&gl_appeared_queue_lock);
		sem_post(&gl_appeared_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

		close(cliente_fd);
		break;
	case GET: ;
		log_info(extense_logger, "Codigo de operacion recibido de %i corresponde a un GET", cliente_fd);
		t_get* get_msg = recibir_get(cliente_fd, &size, extense_logger);

		mensaje_a_guardar->mensaje = (void*) get_msg;

		sem_wait(&gl_get_limite);
		pthread_mutex_lock(&gl_get_queue_lock);

		log_info(extense_logger, "Encolando el mensaje GET");
		queue_push(gl_get_pokemon_queue, mensaje_a_guardar);
		log_info(extense_logger, "Mensaje GET encolado");

		pthread_mutex_unlock(&gl_get_queue_lock);
		sem_post(&gl_get_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

		close(cliente_fd);
		break;
	case LOCALIZED: ;
		log_info(extense_logger, "Codigo de operacion recibido de %i corresponde a un LOCALIZED", cliente_fd);
		t_localized* localized_msg = recibir_localized(cliente_fd, &size, extense_logger);

		mensaje_a_guardar->mensaje = (void*) localized_msg;

		sem_wait(&gl_localized_limite);
		pthread_mutex_lock(&gl_localized_queue_lock);

		log_info(extense_logger, "Encolando el mensaje LOCALIZED");
		queue_push(gl_localized_pokemon_queue, mensaje_a_guardar);
		log_info(extense_logger, "Mensaje LOCALIZED encolado");

		pthread_mutex_unlock(&gl_localized_queue_lock);
		sem_post(&gl_localized_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

		close(cliente_fd);
		break;
	case CATCH: ;
		log_info(extense_logger, "Codigo de operacion recibido de %i corresponde a un CATCH", cliente_fd);
		t_catch* catch_msg = recibir_catch(cliente_fd, &size, extense_logger);

		mensaje_a_guardar->mensaje = (void*) catch_msg;

		sem_wait(&gl_catch_limite);
		pthread_mutex_lock(&gl_catch_queue_lock);

		log_info(extense_logger, "Encolando el mensaje CATCH");
		queue_push(gl_catch_pokemon_queue, mensaje_a_guardar);
		log_info(extense_logger, "Mensaje CATCH encolado");

		pthread_mutex_unlock(&gl_catch_queue_lock);
		sem_post(&gl_catch_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

		close(cliente_fd);
		break;
	case CAUGHT: ;
		log_info(extense_logger, "Codigo de operacion recibido de %i corresponde a un CAUGHT", cliente_fd);
		t_caught* caught_msg = recibir_caught(cliente_fd, &size, extense_logger);

		mensaje_a_guardar->mensaje = (void*) caught_msg;

		sem_wait(&gl_caught_limite);
		pthread_mutex_lock(&gl_caught_queue_lock);

		log_info(extense_logger, "Encolando el mensaje CAUGHT");
		queue_push(gl_caught_pokemon_queue, mensaje_a_guardar);
		log_info(extense_logger, "Mensaje CAUGHT encolado");

		pthread_mutex_unlock(&gl_caught_queue_lock);
		sem_post(&gl_caught_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);

		close(cliente_fd);
		break;
	case SUSCRIPCION: ;
		log_info(extense_logger, "Codigo de operacion recibido de %i corresponse a un SUSCRIPCION", cliente_fd);
		uint32_t id_cola = recibir_suscripcion(cliente_fd, &size, extense_logger);

		switch(id_cola) {
		case NEW_ID: ;
			log_info(extense_logger, "Id de cola recibido de %i corresponde a la cola de NEW", cliente_fd);
			log_info(extense_logger, "Agregando en la lista de suscriptores de la cola de NEW al modulo GAME BOY");
			pthread_mutex_lock(&gl_new_list_lock);
			list_add(gl_new_suscriptores, (void *) &INFO_GAME_BOY);
			pthread_mutex_unlock(&gl_new_list_lock);
			log_info(extense_logger, "Modulo GAME BOY agregado correctamente en la loca de NEW");
			break;
		case APPEARED_ID: ;
			log_info(extense_logger, "Id de cola recibido de %i corresponde a la cola de APPEARED", cliente_fd);
			log_info(extense_logger, "Agregando en la lista de suscriptores de la cola de APPEARED al modulo GAME BOY");
			pthread_mutex_lock(&gl_appeared_list_lock);
			list_add(gl_appeared_suscriptores, (void *) &INFO_GAME_BOY);
			pthread_mutex_unlock(&gl_appeared_list_lock);
			log_info(extense_logger, "Modulo GAME BOY agregado correctamente en la loca de APPEARED");
			break;
		case CATCH_ID: ;
			log_info(extense_logger, "Id de cola recibido de %i corresponde a la cola de CATCH", cliente_fd);
			log_info(extense_logger, "Agregando en la lista de suscriptores de la cola de CATCH al modulo GAME BOY");
			pthread_mutex_lock(&gl_catch_list_lock);
			list_add(gl_catch_suscriptores, (void *) &INFO_GAME_BOY);
			pthread_mutex_unlock(&gl_catch_list_lock);
			log_info(extense_logger, "Modulo GAME BOY agregado correctamente en la loca de CATCH");
			break;
		case CAUGHT_ID: ;
			log_info(extense_logger, "Id de cola recibido de %i corresponde a la cola de CAUGHT", cliente_fd);
			log_info(extense_logger, "Agregando en la lista de suscriptores de la cola de CAUGHT al modulo GAME BOY");
			pthread_mutex_lock(&gl_caught_list_lock);
			list_add(gl_caught_suscriptores, (void *) &INFO_GAME_BOY);
			pthread_mutex_unlock(&gl_caught_list_lock);
			log_info(extense_logger, "Modulo GAME BOY agregado correctamente en la loca de CAUGHT");
			break;
		case GET_ID: ;
			log_info(extense_logger, "Id de cola recibido de %i corresponde a la cola de GET", cliente_fd);
			log_info(extense_logger, "Agregando en la lista de suscriptores de la cola de GET al modulo GAME BOY");
			pthread_mutex_lock(&gl_get_list_lock);
			list_add(gl_get_suscriptores, (void *) &INFO_GAME_BOY);
			pthread_mutex_unlock(&gl_get_list_lock);
			log_info(extense_logger, "Modulo GAME BOY agregado correctamente en la loca de GET");
			break;
		case LOCALIZED_ID: ;
			log_info(extense_logger, "Id de cola recibido de %i corresponde a la cola de LOCALIZED", cliente_fd);
			log_info(extense_logger, "Agregando en la lista de suscriptores de la cola de LOCALIZED al modulo GAME BOY");
			pthread_mutex_lock(&gl_localized_list_lock);
			list_add(gl_localized_suscriptores, (void *) &INFO_GAME_BOY);
			pthread_mutex_unlock(&gl_localized_list_lock);
			log_info(extense_logger, "Modulo GAME BOY agregado correctamente en la loca de LOCALIZED");
			break;
		}

		close(cliente_fd);
		break;
	case 0:

		// va aca tambien?
		close(cliente_fd);

		pthread_exit(NULL);
	case -1:

		// va aca tambien?
		close(cliente_fd);

		pthread_exit(NULL);
	}
}

void atender_cliente(int* socket) {
	int cod_op;
	log_info(extense_logger, "Recibiendo codigo de operacion de socket %i", *socket);
	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1) {
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

	pthread_t thread;

	pthread_create(&thread, NULL, (void*) atender_cliente, &socket_cliente);
	pthread_detach(thread);
}

void* esperar_clientes(void* socket_servidor) {
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

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		mandable_struct* struct_para_enviar = malloc(sizeof(mandable_struct));
		struct_para_enviar->info = list_get(suscriptores_para_enviar, i);
		struct_para_enviar->mensaje_queue = mensaje;
		pthread_create(&(threads[i]), NULL, (void*) mandar_new, (void *) struct_para_enviar);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	free(threads);

	queue_pop(gl_new_pokemon_queue);
}

void atender_mensaje_appeared(mensaje_queue* mensaje) {
	pthread_mutex_lock(&gl_appeared_list_lock);
	t_list* suscriptores_para_enviar = list_duplicate(gl_appeared_suscriptores);
	pthread_mutex_unlock(&gl_appeared_list_lock);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		mandable_struct* struct_para_enviar = malloc(sizeof(mandable_struct));
		struct_para_enviar->info = list_get(suscriptores_para_enviar, i);
		struct_para_enviar->mensaje_queue = mensaje;
		pthread_create(&(threads[i]), NULL, (void*) mandar_appeared, (void *) struct_para_enviar);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	free(threads);

	queue_pop(gl_appeared_pokemon_queue);
}

void atender_mensaje_catch(mensaje_queue* mensaje) {
	pthread_mutex_lock(&gl_catch_list_lock);
	t_list* suscriptores_para_enviar = list_duplicate(gl_catch_suscriptores);
	pthread_mutex_unlock(&gl_catch_list_lock);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		mandable_struct* struct_para_enviar = malloc(sizeof(mandable_struct));
		struct_para_enviar->info = list_get(suscriptores_para_enviar, i);
		struct_para_enviar->mensaje_queue = mensaje;
		pthread_create(&(threads[i]), NULL, (void*) mandar_catch, (void *) struct_para_enviar);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	free(threads);

	queue_pop(gl_catch_pokemon_queue);
}

void atender_mensaje_caught(mensaje_queue* mensaje) {
	pthread_mutex_lock(&gl_caught_list_lock);
	t_list* suscriptores_para_enviar = list_duplicate(gl_caught_suscriptores);
	pthread_mutex_unlock(&gl_caught_list_lock);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		mandable_struct* struct_para_enviar = malloc(sizeof(mandable_struct));
		struct_para_enviar->info = list_get(suscriptores_para_enviar, i);
		struct_para_enviar->mensaje_queue = mensaje;
		pthread_create(&(threads[i]), NULL, (void*) mandar_caught, struct_para_enviar);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	free(threads);

	queue_pop(gl_caught_pokemon_queue);
}

void atender_mensaje_get(mensaje_queue* mensaje) {
	pthread_mutex_lock(&gl_get_list_lock);
	t_list* suscriptores_para_enviar = list_duplicate(gl_get_suscriptores);
	pthread_mutex_unlock(&gl_get_list_lock);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		mandable_struct* struct_para_enviar = malloc(sizeof(mandable_struct));
		struct_para_enviar->info = list_get(suscriptores_para_enviar, i);
		struct_para_enviar->mensaje_queue = mensaje;
		pthread_create(&(threads[i]), NULL, (void*) atender_get, struct_para_enviar);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	free(threads);

	queue_pop(gl_get_pokemon_queue);
}

void atender_mensaje_localized(mensaje_queue* mensaje) {
	pthread_mutex_lock(&gl_localized_list_lock);
	t_list* suscriptores_para_enviar = list_duplicate(gl_localized_suscriptores);
	pthread_mutex_unlock(&gl_localized_list_lock);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		mandable_struct* struct_para_enviar = malloc(sizeof(mandable_struct));
		struct_para_enviar->info = list_get(suscriptores_para_enviar, i);
		struct_para_enviar->mensaje_queue = mensaje;
		pthread_create(&(threads[i]), NULL, (void*) mandar_localized, struct_para_enviar);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	free(threads);

	queue_pop(gl_localized_pokemon_queue);
}

void mandar_new(void* new_mandable) {
	mandable_struct* argumento_new = (mandable_struct*) new_mandable;

	int socket_cliente = crear_conexion(argumento_new->info->ip, argumento_new->info->puerto);

	uint32_t bytes;
	void* flujo = serializar_new(argumento_new, &bytes);

	if (send(socket_cliente, flujo, bytes, 0) == -1){
		// ver que onda el log
		log_error(logger, "Error: No se pudo enviar el mensaje");
	}

	free(flujo);

//	uint32_t id;
//	if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1)
//	{
//		log_error(logger, "error: No se recibió el código de operación");
//	}

	// que onda esto? tiene que ir o no?
	close(socket_cliente);

	free(argumento_new);
}

void mandar_appeared(void* appeared_mandable) {
	mandable_struct* argumento_appeared = (mandable_struct*) appeared_mandable;

	int socket_cliente = crear_conexion(argumento_appeared->info->ip, argumento_appeared->info->puerto);

	uint32_t bytes;
	void* flujo = serializar_appeared(argumento_appeared, &bytes);

	if (send(socket_cliente, flujo, bytes, 0) == -1){
		// ver que onda el log
		log_error(logger, "Error: No se pudo enviar el mensaje");
	}

	free(flujo);

//	uint32_t id;
//	if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1)
//	{
//		log_error(logger, "error: No se recibió el código de operación");
//	}

	// que onda esto? tiene que ir o no?
	close(socket_cliente);

	free(argumento_appeared);
}

void mandar_catch(void* catch_mandable) {
	mandable_struct* argumento_catch = (mandable_struct*) catch_mandable;

	int socket_cliente = crear_conexion(argumento_catch->info->ip, argumento_catch->info->puerto);

	uint32_t bytes;
	void* flujo = serializar_catch(argumento_catch, &bytes);

	if (send(socket_cliente, flujo, bytes, 0) == -1){
		// ver que onda el log
		log_error(logger, "Error: No se pudo enviar el mensaje");
	}

	free(flujo);

//	uint32_t id;
//	if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1)
//	{
//		log_error(logger, "error: No se recibió el código de operación");
//	}

	// que onda esto? tiene que ir o no?
	close(socket_cliente);

	free(argumento_catch);
}

void mandar_caught(void* caught_mandable) {
	mandable_struct* argumento_caught = (mandable_struct*) caught_mandable;

	uint32_t bytes;
	void* flujo = serializar_caught(argumento_caught, &bytes);

	int socket_cliente = crear_conexion(argumento_caught->info->ip, argumento_caught->info->puerto);

	if (send(socket_cliente, flujo, bytes, 0) == -1){
		// ver que onda el log
		log_error(logger, "Error: No se pudo enviar el mensaje");
	}

	free(flujo);

//	uint32_t id;
//	if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1)
//	{
//		log_error(logger, "error: No se recibió el código de operación");
//	}

	// que onda esto? tiene que ir o no?
	close(socket_cliente);

	free(argumento_caught);
}

void mandar_get(void* get_mandable) {
	mandable_struct* argumento_get = (mandable_struct*) get_mandable;

	int socket_cliente = crear_conexion(argumento_get->info->ip, argumento_get->info->puerto);

	uint32_t bytes;
	void* flujo = serializar_get(argumento_get, &bytes);

	if (send(socket_cliente, flujo, bytes, 0) == -1){
		// ver que onda el log
		log_error(logger, "Error: No se pudo enviar el mensaje");
	}

	free(flujo);

//	uint32_t id;
//	if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1)
//	{
//		log_error(logger, "error: No se recibió el código de operación");
//	}

	// que onda esto? tiene que ir o no?
	close(socket_cliente);

	free(argumento_get);
}

void mandar_localized(void* localized_mandable) {
	mandable_struct* argumento_localized = (mandable_struct*) localized_mandable;

	int socket_cliente = crear_conexion(argumento_localized->info->ip, argumento_localized->info->puerto);

	uint32_t bytes;
	void* flujo = serializar_localized(argumento_localized, &bytes);

	if (send(socket_cliente, flujo, bytes, 0) == -1){
		// ver que onda el log
		log_error(logger, "Error: No se pudo enviar el mensaje");
	}

	free(flujo);

//	uint32_t id;
//	if (recv(socket_cliente,&id ,sizeof(uint32_t),MSG_WAITALL) == -1)
//	{
//		log_error(logger, "error: No se recibió el código de operación");
//	}

	// que onda esto? tiene que ir o no?
	close(socket_cliente);

	free(argumento_localized);
}

void* serializar_new(mandable_struct* new, uint32_t* bytes) {
	uint32_t tamanio_pokemon = (strlen(((t_new*) new->mensaje_queue->mensaje)->pokemon) + 1) * sizeof(char);

	*bytes = sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t) + tamanio_pokemon  + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t);

	uint32_t cod_op = 1;
	void* flujo = malloc(*bytes);
	int desplazamiento = 0;

	memcpy(flujo + desplazamiento, &cod_op, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, bytes, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(new->mensaje_queue->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &tamanio_pokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, ((t_new*) new->mensaje_queue->mensaje)->pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;
	memcpy(flujo + desplazamiento, &(((t_new*) new->mensaje_queue->mensaje)->pos_X), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(((t_new*) new->mensaje_queue->mensaje)->pos_Y), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(((t_new*) new->mensaje_queue->mensaje)->cantidad), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	return flujo;
}

void* serializar_appeared(mandable_struct* appeared, uint32_t* bytes) {
	uint32_t tamanio_pokemon = (strlen(((t_appeared*) appeared->mensaje_queue->mensaje)->pokemon) + 1) * sizeof(char);

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
	memcpy(flujo + desplazamiento, &(appeared->mensaje_queue->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(((t_appeared*) appeared->mensaje_queue->mensaje)->id_correlativo), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &tamanio_pokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, ((t_appeared*) appeared->mensaje_queue->mensaje)->pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;
	memcpy(flujo + desplazamiento, &(((t_appeared*) appeared->mensaje_queue->mensaje)->pos_X), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(((t_appeared*) appeared->mensaje_queue->mensaje)->pos_Y), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	return flujo;
}

void* serializar_catch(mandable_struct* catch, uint32_t* bytes) {
	uint32_t tamanio_pokemon = (strlen(((t_catch*) catch->mensaje_queue->mensaje)->pokemon) + 1) * sizeof(char);

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
	memcpy(flujo + desplazamiento, &(catch->mensaje_queue->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &tamanio_pokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, ((t_catch*) catch->mensaje_queue->mensaje)->pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;
	memcpy(flujo + desplazamiento, &(((t_catch*) catch->mensaje_queue->mensaje)->pos_X), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(((t_catch*) catch->mensaje_queue->mensaje)->pos_Y), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	return flujo;
}

void* serializar_caught(mandable_struct* caught, uint32_t* bytes) {
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
	memcpy(flujo + desplazamiento, &(caught->mensaje_queue->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(((t_caught*) caught->mensaje_queue->mensaje)->id_correlativo), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(((t_caught*) caught->mensaje_queue->mensaje)->flag_caught), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	return flujo;
}

void* serializar_get(mandable_struct* get, uint32_t* bytes) {
	uint32_t tamanio_pokemon = (strlen(((t_get*) get->mensaje_queue->mensaje)->pokemon) + 1) * sizeof(char);

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
	memcpy(flujo + desplazamiento, &(get->mensaje_queue->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &tamanio_pokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, ((t_get*) get->mensaje_queue->mensaje)->pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;

	return flujo;
}

void* serializar_localized(mandable_struct* localized, uint32_t* bytes) {
	uint32_t tamanio_pokemon = (strlen(((t_localized*) localized->mensaje_queue->mensaje)->pokemon) + 1) * sizeof(char);
	uint32_t elementos_lista = ((t_localized*) localized->mensaje_queue->mensaje)->l_coordenadas->elements_count;
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
	memcpy(flujo + desplazamiento, &(localized->mensaje_queue->id), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(((t_localized*) localized->mensaje_queue->mensaje)->id_correlativo), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &tamanio_pokemon, sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);
	memcpy(flujo + desplazamiento, ((t_localized*) localized->mensaje_queue->mensaje)->pokemon, tamanio_pokemon);
	desplazamiento += tamanio_pokemon;
	memcpy(flujo + desplazamiento, &(((t_localized*) localized->mensaje_queue->mensaje)->lugares), sizeof(uint32_t));
	desplazamiento += sizeof(uint32_t);

	for (int i = 0; i < elementos_lista; i++) {
		uint32_t* elemento_de_lista = (uint32_t*) list_get(((t_localized*) localized->mensaje_queue->mensaje)->l_coordenadas, i);
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

	// leemos de config ip y puerto de cada modulo

	leer_info_modulos();

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

	// AGREGAR SUSCRIPTORES

	log_info(extense_logger, "Llenando listas de suscriptores");

	list_add(gl_new_suscriptores, (void *) &INFO_GAME_CARD);
	log_info(extense_logger, "Lista de suscriptores de new");
	log_info(extense_logger, "GAME CARD ip: %s puerto: %s", INFO_GAME_CARD.ip, INFO_GAME_CARD.puerto);
	list_add(gl_appeared_suscriptores, (void *) &INFO_TEAM);
	log_info(extense_logger, "Lista de suscriptores de appeared");
	log_info(extense_logger, "TEAM ip: %s puerto: %s", INFO_TEAM.ip, INFO_TEAM.puerto);
	list_add(gl_catch_suscriptores, (void *) &INFO_GAME_CARD);
	log_info(extense_logger, "Lista de suscriptores de catch");
	log_info(extense_logger, "GAME CARD ip: %s puerto: %s", INFO_GAME_CARD.ip, INFO_GAME_CARD.puerto);
	list_add(gl_caught_suscriptores, (void *) &INFO_TEAM);
	log_info(extense_logger, "Lista de suscriptores de caught");
	log_info(extense_logger, "TEAM ip: %s puerto: %s", INFO_TEAM.ip, INFO_TEAM.puerto);
	list_add(gl_get_suscriptores, (void *) &INFO_GAME_CARD);
	log_info(extense_logger, "Lista de suscriptores de get");
	log_info(extense_logger, "GAME CARD ip: %s puerto: %s", INFO_GAME_CARD.ip, INFO_GAME_CARD.puerto);
	list_add(gl_localized_suscriptores, (void *) &INFO_TEAM);
	log_info(extense_logger, "Lista de suscriptores de localized");
	log_info(extense_logger, "TEAM ip: %s puerto: %s", INFO_TEAM.ip, INFO_TEAM.puerto);

	log_info(extense_logger, "Listas de suscriptores llenas");

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

void leer_info_modulos(void) {
	log_info(extense_logger, "Iniciando estructuras de modulos");

	INFO_TEAM.ip = config_get_string_value(config, "TEAM_IP");
	INFO_TEAM.puerto = config_get_string_value(config, "TEAM_PUERTO");
	INFO_GAME_BOY.ip = config_get_string_value(config, "GAME_BOY_IP");
	INFO_GAME_BOY.puerto = config_get_string_value(config, "GAME_BOY_PUERTO");
	INFO_GAME_CARD.ip = config_get_string_value(config, "GAME_CARD_IP");
	INFO_GAME_CARD.puerto = config_get_string_value(config, "GAME_CARD_PUERTO");

	log_info(extense_logger, "Estructuras de modulos iniciadas");
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
