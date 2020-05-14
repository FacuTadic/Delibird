#include "broker.h"

void procesar_request(int cod_op, int cliente_fd) {
	int size;

	mensaje_queue* mensaje_a_guardar = malloc(sizeof(mensaje_queue));
	mensaje_a_guardar->id = generar_id();

	switch (cod_op) {
	case NEW: ;
		t_new* new_msg = recibir_new(cliente_fd, &size);

		mensaje_a_guardar->mensaje = (void*) new_msg;

		sem_wait(&gl_new_limite);
		pthread_mutex_lock(&gl_new_queue_lock);

		queue_push(gl_new_pokemon_queue, mensaje_a_guardar);

		pthread_mutex_unlock(&gl_new_queue_lock);
		sem_post(&gl_new_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id);

		close(cliente_fd);
		break;
	case APPEARED: ;
		t_appeared* appeared_msg = recibir_appeared(cliente_fd, &size);

		mensaje_a_guardar->mensaje = (void*) appeared_msg;

		sem_wait(&gl_appeared_limite);
		pthread_mutex_lock(&gl_appeared_queue_lock);

		queue_push(gl_appeared_pokemon_queue, mensaje_a_guardar);

		pthread_mutex_unlock(&gl_appeared_queue_lock);
		sem_post(&gl_appeared_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id);

		close(cliente_fd);
		break;
	case GET: ;
		t_get* get_msg = recibir_get(cliente_fd, &size);

		mensaje_a_guardar->mensaje = (void*) get_msg;

		sem_wait(&gl_get_limite);
		pthread_mutex_lock(&gl_get_queue_lock);

		queue_push(gl_get_pokemon_queue, mensaje_a_guardar);

		pthread_mutex_unlock(&gl_get_queue_lock);
		sem_post(&gl_get_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id);

		close(cliente_fd);
		break;
	case LOCALIZED: ;
		t_localized* localized_msg = recibir_localized(cliente_fd, &size);

		mensaje_a_guardar->mensaje = (void*) localized_msg;

		sem_wait(&gl_localized_limite);
		pthread_mutex_lock(&gl_localized_queue_lock);

		queue_push(gl_localized_pokemon_queue, mensaje_a_guardar);

		pthread_mutex_unlock(&gl_localized_queue_lock);
		sem_post(&gl_localized_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id);

		close(cliente_fd);
		break;
	case CATCH: ;
		t_catch* catch_msg = recibir_catch(cliente_fd, &size);

		mensaje_a_guardar->mensaje = (void*) catch_msg;

		sem_wait(&gl_catch_limite);
		pthread_mutex_lock(&gl_catch_queue_lock);

		queue_push(gl_catch_pokemon_queue, mensaje_a_guardar);

		pthread_mutex_unlock(&gl_catch_queue_lock);
		sem_post(&gl_catch_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id);

		close(cliente_fd);
		break;
	case CAUGHT: ;
		t_caught* caught_msg = recibir_caught(cliente_fd, &size);

		mensaje_a_guardar->mensaje = (void*) caught_msg;

		sem_wait(&gl_caught_limite);
		pthread_mutex_lock(&gl_caught_queue_lock);

		queue_push(gl_caught_pokemon_queue, mensaje_a_guardar);

		pthread_mutex_unlock(&gl_caught_queue_lock);
		sem_post(&gl_caught_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id);

		close(cliente_fd);
		break;
	case SUSCRIPCION: ;
		uint32_t id_cola = recibir_suscripcion(cliente_fd);

		switch(id_cola) {
		case NEW_ID: ;
			pthread_mutex_lock(&gl_new_list_lock);
			list_add(gl_new_suscriptores, (void *) &INFO_GAME_BOY);
			pthread_mutex_unlock(&gl_new_list_lock);
			break;
		case APPEARED_ID: ;
			pthread_mutex_lock(&gl_appeared_list_lock);
			list_add(gl_appeared_suscriptores, (void *) &INFO_GAME_BOY);
			pthread_mutex_unlock(&gl_appeared_list_lock);
			break;
		case CATCH_ID: ;
			pthread_mutex_lock(&gl_catch_list_lock);
			list_add(gl_catch_suscriptores, (void *) &INFO_GAME_BOY);
			pthread_mutex_unlock(&gl_catch_list_lock);
			break;
		case CAUGHT_ID: ;
			pthread_mutex_lock(&gl_caught_list_lock);
			list_add(gl_caught_suscriptores, (void *) &INFO_GAME_BOY);
			pthread_mutex_unlock(&gl_caught_list_lock);
			break;
		case GET_ID: ;
			pthread_mutex_lock(&gl_get_list_lock);
			list_add(gl_get_suscriptores, (void *) &INFO_GAME_BOY);
			pthread_mutex_unlock(&gl_get_list_lock);
			break;
		case LOCALIZED_ID: ;
			pthread_mutex_lock(&gl_localized_list_lock);
			list_add(gl_localized_suscriptores, (void *) &INFO_GAME_BOY);
			pthread_mutex_unlock(&gl_localized_list_lock);
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
	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1)
		cod_op = -1;
	procesar_request(cod_op, *socket);
}

void esperar_cliente(int socket_servidor) {
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_t thread;

	pthread_create(&thread, NULL, (void*) atender_cliente, &socket_cliente);
	pthread_detach(thread);
}

void* esperar_clientes(void* socket_servidor) {
	int socket = (int) socket_servidor;
	while(1) {
		esperar_cliente(socket);
	}
}

void* atender_new(void* args) {
	while (1) {
		sem_wait(&gl_new_mensajes);
		pthread_mutex_lock(&gl_new_queue_lock);

		mensaje_queue* primer_elemento = (mensaje_queue*) queue_peek(gl_new_pokemon_queue);

		pthread_mutex_unlock(&gl_new_queue_lock);

		atender_mensaje_new(primer_elemento);

		pthread_mutex_lock(&gl_new_queue_lock);

		mensaje_queue* elemento_a_eliminar = queue_pop(gl_new_pokemon_queue);

		pthread_mutex_unlock(&gl_new_queue_lock);
		sem_post(&gl_new_limite);

		t_new* mensaje_a_eliminar = (t_new*) elemento_a_eliminar->mensaje;

		free(mensaje_a_eliminar->pokemon);
		free(mensaje_a_eliminar);
		free(elemento_a_eliminar);
	}
}

void* atender_appeared(void* args) {
	while (1) {
		sem_wait(&gl_appeared_mensajes);
		pthread_mutex_lock(&gl_appeared_queue_lock);

		mensaje_queue* primer_elemento = (mensaje_queue*) queue_peek(gl_appeared_pokemon_queue);

		pthread_mutex_unlock(&gl_appeared_queue_lock);

		atender_mensaje_appeared(primer_elemento);

		pthread_mutex_lock(&gl_appeared_queue_lock);

		mensaje_queue* elemento_a_eliminar = (mensaje_queue*) queue_pop(gl_appeared_pokemon_queue);

		pthread_mutex_unlock(&gl_appeared_queue_lock);
		sem_post(&gl_appeared_limite);

		t_appeared* mensaje_a_eliminar = (t_appeared*) elemento_a_eliminar->mensaje;

		free(mensaje_a_eliminar->pokemon);
		free(mensaje_a_eliminar);
		free(elemento_a_eliminar);
	}
}

void* atender_catch(void* args) {
	while (1) {
		sem_wait(&gl_catch_mensajes);
		pthread_mutex_lock(&gl_catch_queue_lock);

		mensaje_queue* primer_elemento = (mensaje_queue*) queue_peek(gl_catch_pokemon_queue);

		pthread_mutex_unlock(&gl_catch_queue_lock);

		atender_mensaje_catch(primer_elemento);

		pthread_mutex_lock(&gl_catch_queue_lock);

		mensaje_queue* elemento_a_eliminar = (mensaje_queue*) queue_pop(gl_catch_pokemon_queue);

		pthread_mutex_unlock(&gl_catch_queue_lock);
		sem_post(&gl_catch_limite);

		t_catch* mensaje_a_eliminar = (t_catch*) elemento_a_eliminar->mensaje;

		free(mensaje_a_eliminar->pokemon);
		free(mensaje_a_eliminar);
		free(elemento_a_eliminar);
	}
}

void* atender_caught(void* args) {
	while (1) {
		sem_wait(&gl_caught_mensajes);
		pthread_mutex_lock(&gl_caught_queue_lock);

		mensaje_queue* primer_elemento = (mensaje_queue*) queue_peek(gl_caught_pokemon_queue);

		pthread_mutex_unlock(&gl_caught_queue_lock);

		atender_mensaje_caught(primer_elemento);

		pthread_mutex_lock(&gl_caught_queue_lock);

		mensaje_queue* elemento_a_eliminar = (mensaje_queue*) queue_pop(gl_caught_pokemon_queue);

		pthread_mutex_unlock(&gl_caught_queue_lock);
		sem_post(&gl_caught_limite);

		t_caught* mensaje_a_eliminar = (t_caught*) elemento_a_eliminar->mensaje;

		free(mensaje_a_eliminar);
		free(elemento_a_eliminar);
	}
}

void* atender_get(void* args) {
	while (1) {
		sem_wait(&gl_get_mensajes);
		pthread_mutex_lock(&gl_get_queue_lock);

		mensaje_queue* primer_elemento = (mensaje_queue*) queue_peek(gl_get_pokemon_queue);

		pthread_mutex_unlock(&gl_get_queue_lock);

		atender_mensaje_get(primer_elemento);

		pthread_mutex_lock(&gl_get_queue_lock);

		mensaje_queue* elemento_a_eliminar = (mensaje_queue*) queue_pop(gl_get_pokemon_queue);

		pthread_mutex_unlock(&gl_get_queue_lock);
		sem_post(&gl_get_limite);

		t_get* mensaje_a_eliminar = (t_get*) elemento_a_eliminar->mensaje;

		free(mensaje_a_eliminar->pokemon);
		free(mensaje_a_eliminar);
		free(elemento_a_eliminar);
	}
}

void* atender_localized(void* args) {
	while (1) {
		sem_wait(&gl_localized_mensajes);
		pthread_mutex_lock(&gl_localized_queue_lock);

		mensaje_queue* primer_elemento = (mensaje_queue*) queue_peek(gl_localized_pokemon_queue);

		pthread_mutex_unlock(&gl_localized_queue_lock);

		atender_mensaje_localized(primer_elemento);

		pthread_mutex_lock(&gl_localized_queue_lock);

		mensaje_queue* elemento_a_eliminar = (mensaje_queue*) queue_pop(gl_localized_pokemon_queue);

		pthread_mutex_unlock(&gl_localized_queue_lock);
		sem_post(&gl_localized_limite);

		t_localized* mensaje_a_eliminar = (t_localized*) elemento_a_eliminar->mensaje;

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

	// POR AHORA CAMINO FELIZ
	// COMO DIJO MACRI
	// DESDE AHORA POR LEY SOMOS TODOS FELICES

	// Elimino de la cola

	queue_pop(gl_new_pokemon_queue);
}

void atender_mensaje_appeared(mensaje_queue* mensaje) {
	pthread_mutex_lock(&gl_appeared_list_lock);
	t_list* suscriptores_para_enviar = list_duplicate(gl_appeared_suscriptores);
	pthread_mutex_unlock(&gl_appeared_list_lock);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		// ACA NO VA ATENDER APPEARED
		pthread_create(&(threads[i]), NULL, (void*) atender_appeared, NULL);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	// POR AHORA CAMINO FELIZ
	// COMO DIJO MACRI
	// DESDE AHORA POR LEY SOMOS TODOS FELICES

	// Elimino de la cola

	queue_pop(gl_appeared_pokemon_queue);
}

void atender_mensaje_catch(mensaje_queue* mensaje) {
	pthread_mutex_lock(&gl_catch_list_lock);
	t_list* suscriptores_para_enviar = list_duplicate(gl_catch_suscriptores);
	pthread_mutex_unlock(&gl_catch_list_lock);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		// ACA NO VA ATENDER APPEARED
		pthread_create(&(threads[i]), NULL, (void*) atender_appeared, NULL);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	// POR AHORA CAMINO FELIZ
	// COMO DIJO MACRI
	// DESDE AHORA POR LEY SOMOS TODOS FELICES

	// Elimino de la cola

	queue_pop(gl_catch_pokemon_queue);
}

void atender_mensaje_caught(mensaje_queue* mensaje) {
	pthread_mutex_lock(&gl_caught_list_lock);
	t_list* suscriptores_para_enviar = list_duplicate(gl_caught_suscriptores);
	pthread_mutex_unlock(&gl_caught_list_lock);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		// ACA NO VA ATENDER APPEARED
		pthread_create(&(threads[i]), NULL, (void*) atender_appeared, NULL);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	// POR AHORA CAMINO FELIZ
	// COMO DIJO MACRI
	// DESDE AHORA POR LEY SOMOS TODOS FELICES

	// Elimino de la cola

	queue_pop(gl_caught_pokemon_queue);
}

void atender_mensaje_get(mensaje_queue* mensaje) {
	pthread_mutex_lock(&gl_get_list_lock);
	t_list* suscriptores_para_enviar = list_duplicate(gl_get_suscriptores);
	pthread_mutex_unlock(&gl_get_list_lock);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		// ACA NO VA ATENDER APPEARED
		pthread_create(&(threads[i]), NULL, (void*) atender_appeared, NULL);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	// POR AHORA CAMINO FELIZ
	// COMO DIJO MACRI
	// DESDE AHORA POR LEY SOMOS TODOS FELICES

	// Elimino de la cola

	queue_pop(gl_get_pokemon_queue);
}

void atender_mensaje_localized(mensaje_queue* mensaje) {
	pthread_mutex_lock(&gl_localized_list_lock);
	t_list* suscriptores_para_enviar = list_duplicate(gl_localized_suscriptores);
	pthread_mutex_unlock(&gl_localized_list_lock);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		// ACA NO VA ATENDER APPEARED
		pthread_create(&(threads[i]), NULL, (void*) atender_appeared, NULL);
	}

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_join(threads[i], NULL);
	}

	// POR AHORA CAMINO FELIZ
	// COMO DIJO MACRI
	// DESDE AHORA POR LEY SOMOS TODOS FELICES

	// Elimino de la cola

	queue_pop(gl_localized_pokemon_queue);
}

void mandar_new(void* new_mandable) {

	mandable_struct* argumento_new = (mandable_struct*) new_mandable;



	// conectarnos
	// hablar con milton


	// serializar paquete
	// op_code [uint32_t] tamaño de todo lo que viene despues [uint32_t] gilada [extras]
	// gilada tiene primero id y id correlativo

	// enviar

	// esperar respuesta

	// cerrar conexion

	// liberar memoria

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

	inicializar_colas();

	inicializar_semaforos_colas();

	crea_generador_id();

	// leemos de config ip y puerto de cada modulo

	leer_info_modulos();

	// Levantamos el servidor en la ip y el puerto definidos en config

	int socket_servidor = iniciar_servidor(ip, puerto);

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
	gl_new_pokemon_queue = queue_create();
	gl_appeared_pokemon_queue = queue_create();
	gl_catch_pokemon_queue = queue_create();
	gl_caught_pokemon_queue = queue_create();
	gl_get_pokemon_queue = queue_create();
	gl_localized_pokemon_queue = queue_create();

	gl_new_suscriptores = list_create();
	gl_appeared_suscriptores = list_create();
	gl_catch_suscriptores = list_create();
	gl_caught_suscriptores = list_create();
	gl_get_suscriptores = list_create();
	gl_localized_suscriptores = list_create();

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

	log_info(extense_logger, "Colas iniciadas");
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
	INFO_TEAM.ip = config_get_string_value(config, "TEAM_IP");
	INFO_TEAM.puerto = config_get_string_value(config, "TEAM_PUERTO");
	INFO_GAME_BOY.ip = config_get_string_value(config, "GAME_BOY_IP");
	INFO_GAME_BOY.puerto = config_get_string_value(config, "GAME_BOY_PUERTO");
	INFO_GAME_CARD.ip = config_get_string_value(config, "GAME_CARD_IP");
	INFO_GAME_CARD.puerto = config_get_string_value(config, "GAME_CARD_PUERTO");
}

void inicializar_semaforos_colas(void) {
	int limite_cola_new;
	int limite_cola_appeared;
	int limite_cola_catch;
	int limite_cola_caught;
	int limite_cola_get;
	int limite_cola_localized;

	limite_cola_new = config_get_int_value(config, "LIMITE_COLA_NEW");
	limite_cola_appeared = config_get_int_value(config, "LIMITE_COLA_APPEARED");
	limite_cola_catch = config_get_int_value(config, "LIMITE_COLA_CATCH");
	limite_cola_caught = config_get_int_value(config, "LIMITE_COLA_CAUGHT");
	limite_cola_get = config_get_int_value(config, "LIMITE_COLA_GET");
	limite_cola_localized = config_get_int_value(config, "LIMITE_COLA_LOCALIZED");

	sem_init(&gl_new_limite, 0, limite_cola_new);
	sem_init(&gl_appeared_limite, 0, limite_cola_appeared);
	sem_init(&gl_catch_limite, 0, limite_cola_catch);
	sem_init(&gl_caught_limite, 0, limite_cola_caught);
	sem_init(&gl_get_limite, 0, limite_cola_get);
	sem_init(&gl_localized_limite, 0, limite_cola_localized);

	sem_init(&gl_new_mensajes, 0, 0);
	sem_init(&gl_appeared_mensajes, 0, 0);
	sem_init(&gl_catch_mensajes, 0, 0);
	sem_init(&gl_caught_mensajes, 0, 0);
	sem_init(&gl_get_mensajes, 0, 0);
	sem_init(&gl_localized_mensajes, 0, 0);

	pthread_mutex_init(&gl_new_queue_lock, NULL);
	pthread_mutex_init(&gl_appeared_queue_lock, NULL);
	pthread_mutex_init(&gl_catch_queue_lock, NULL);
	pthread_mutex_init(&gl_caught_queue_lock, NULL);
	pthread_mutex_init(&gl_get_queue_lock, NULL);
	pthread_mutex_init(&gl_localized_queue_lock, NULL);

	pthread_mutex_init(&gl_new_list_lock, NULL);
	pthread_mutex_init(&gl_appeared_list_lock, NULL);
	pthread_mutex_init(&gl_catch_list_lock, NULL);
	pthread_mutex_init(&gl_caught_list_lock, NULL);
	pthread_mutex_init(&gl_get_list_lock, NULL);
	pthread_mutex_init(&gl_localized_list_lock, NULL);
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
