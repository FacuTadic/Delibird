#include "broker.h"

void procesar_request(int cod_op, int cliente_fd) {
	int size;

	mensaje_queue* mensaje_a_guardar = malloc(sizeof(mensaje_queue));
	mensaje_a_guardar->id = generar_id();

	switch (cod_op) {
	case NEW: ;
		t_new* new_msg = recibir_new(cliente_fd, &size);

		mensaje_a_guardar->mensaje = (void*) new_msg;

		sem_wait(&new_limite);
		pthread_mutex_lock(&new_lock);

		queue_push(NEW_POKEMON, mensaje_a_guardar);

		pthread_mutex_unlock(&new_lock);
		sem_post(&new_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id);

		close(cliente_fd);
		break;
	case APPEARED: ;
		t_appeared* appeared_msg = recibir_appeared(cliente_fd, &size);

		mensaje_a_guardar->mensaje = (void*) appeared_msg;

		sem_wait(&appeared_limite);
		pthread_mutex_lock(&appeared_lock);

		queue_push(APPEARED_POKEMON, mensaje_a_guardar);

		pthread_mutex_unlock(&appeared_lock);
		sem_post(&appeared_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id);

		close(cliente_fd);
		break;
	case GET: ;
		t_get* get_msg = recibir_get(cliente_fd, &size);

		mensaje_a_guardar->mensaje = (void*) get_msg;

		sem_wait(&get_limite);
		pthread_mutex_lock(&get_lock);

		queue_push(GET_POKEMON, mensaje_a_guardar);

		pthread_mutex_unlock(&get_lock);
		sem_post(&get_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id);

		close(cliente_fd);
		break;
	case LOCALIZED: ;
		t_localized* localized_msg = recibir_localized(cliente_fd, &size);

		mensaje_a_guardar->mensaje = (void*) localized_msg;

		sem_wait(&localized_limite);
		pthread_mutex_lock(&localized_lock);

		queue_push(LOCALIZED_POKEMON, mensaje_a_guardar);

		pthread_mutex_unlock(&localized_lock);
		sem_post(&localized_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id);

		close(cliente_fd);
		break;
	case CATCH: ;
		t_catch* catch_msg = recibir_catch(cliente_fd, &size);

		mensaje_a_guardar->mensaje = (void*) catch_msg;

		sem_wait(&catch_limite);
		pthread_mutex_lock(&catch_lock);

		queue_push(CATCH_POKEMON, mensaje_a_guardar);

		pthread_mutex_unlock(&catch_lock);
		sem_post(&catch_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id);

		close(cliente_fd);
		break;
	case CAUGHT: ;
		t_caught* caught_msg = recibir_caught(cliente_fd, &size);

		mensaje_a_guardar->mensaje = (void*) caught_msg;

		sem_wait(&caught_limite);
		pthread_mutex_lock(&caught_lock);

		queue_push(CAUGHT_POKEMON, mensaje_a_guardar);

		pthread_mutex_unlock(&caught_lock);
		sem_post(&caught_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id);

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
		// Hacer algo con el ultimo elemento de la cola

		// EL PRIMERO QUEDA COMO EJEMPLO

		sem_wait(&new_mensajes);
		pthread_mutex_lock(&new_lock);

		// leer elemento de la cola
		t_new* primer_elemento = (t_new*) queue_peek(NEW_POKEMON);

		pthread_mutex_unlock(&new_lock);

		atender_mensaje_new(primer_elemento);

		// laburar
		// 1. crear un hilo por cada modulo a enviar el elemento
		//      dentro del hilo
		//      a. conectar con el modulo correspondiente
		//      b. serializar el mensaje
		//      c. enviar
		//      d. esperar la respuesta
		//      e. marcar que el mensaje fue enviado al modulo
		//      f. matar conexion
		// 2. dentro del hilo atender_new chequear cuando termino todo
		//      maneras
		//      a. chequear que todos los modulos respondieron y que no queda enviarle nada a nadie
		//      b. chequear que ninguno de los tres hilos este levantado y corriendo, VER MANERAS


		// --------------------------------------------------------------------
		//                        -------------
		//                        -------------
		//                        -------------

		// una vez que este terminado

		pthread_mutex_lock(&new_lock);

		// eliminar de la cola y liberar memoria
		t_new* elemento_a_eliminar = queue_pop(NEW_POKEMON);

		pthread_mutex_unlock(&new_lock);
		sem_post(&new_limite);

		free(elemento_a_eliminar->pokemon);
		free(elemento_a_eliminar);
	}
}

void* atender_appeared(void* args) {
	while (1) {
		// Hacer algo con el ultimo elemento de la cola

		sem_wait(&appeared_mensajes);
		pthread_mutex_lock(&appeared_lock);

		// leer elemento de la cola
		t_appeared* primer_elemento = (t_appeared*) queue_peek(APPEARED_POKEMON);

		pthread_mutex_unlock(&appeared_lock);

		// laburar

		// una vez que este terminado

		pthread_mutex_lock(&appeared_lock);

		// eliminar de la cola
		t_appeared* elemento_a_eliminar = (t_appeared*) queue_pop(APPEARED_POKEMON);

		pthread_mutex_unlock(&appeared_lock);
		sem_post(&appeared_limite);

		free(elemento_a_eliminar->pokemon);
		free(elemento_a_eliminar);
	}
}

void* atender_catch(void* args) {
	while (1) {
		// Hacer algo con el ultimo elemento de la cola

		sem_wait(&catch_mensajes);
		pthread_mutex_lock(&catch_lock);

		// leer elemento de la cola
		t_catch* primer_elemento = (t_catch*) queue_peek(CATCH_POKEMON);

		pthread_mutex_unlock(&catch_lock);

		// laburar

		// una vez que este terminado

		pthread_mutex_lock(&catch_lock);

		// eliminar de la cola
		t_catch* elemento_a_eliminar = (t_catch*) queue_pop(CATCH_POKEMON);

		pthread_mutex_unlock(&catch_lock);
		sem_post(&catch_limite);

		free(elemento_a_eliminar->pokemon);
		free(elemento_a_eliminar);
	}
}

void* atender_caught(void* args) {
	while (1) {
		// Hacer algo con el ultimo elemento de la cola

		sem_wait(&caught_mensajes);
		pthread_mutex_lock(&caught_lock);

		// leer elemento de la cola
		t_caught* primer_elemento = (t_caught*) queue_peek(CAUGHT_POKEMON);

		pthread_mutex_unlock(&caught_lock);

		// laburar

		// una vez que este terminado

		pthread_mutex_lock(&caught_lock);

		// eliminar de la cola
		t_caught* elemento_a_eliminar = (t_caught*) queue_pop(CAUGHT_POKEMON);

		pthread_mutex_unlock(&caught_lock);
		sem_post(&caught_limite);

		free(elemento_a_eliminar);
	}
}

void* atender_get(void* args) {
	while (1) {
		// Hacer algo con el ultimo elemento de la cola

		sem_wait(&get_mensajes);
		pthread_mutex_lock(&get_lock);

		// leer elemento de la cola
		t_get* primer_elemento = (t_get*) queue_peek(GET_POKEMON);

		pthread_mutex_unlock(&get_lock);

		// laburar

		// una vez que este terminado

		pthread_mutex_lock(&get_lock);

		// eliminar de la cola
		t_get* elemento_a_eliminar = (t_get*) queue_pop(GET_POKEMON);

		pthread_mutex_unlock(&get_lock);
		sem_post(&get_limite);

		free(elemento_a_eliminar->pokemon);
		free(elemento_a_eliminar);
	}
}

void* atender_localized(void* args) {
	while (1) {
		// Hacer algo con el ultimo elemento de la cola

		sem_wait(&localized_mensajes);
		pthread_mutex_lock(&localized_lock);

		// leer elemento de la cola
		t_localized* primer_elemento = (t_localized*) queue_peek(LOCALIZED_POKEMON);

		pthread_mutex_unlock(&localized_lock);

		// laburar

		// una vez que este terminado

		pthread_mutex_lock(&localized_lock);

		// eliminar de la cola
		t_localized* elemento_a_eliminar = (t_localized*) queue_pop(LOCALIZED_POKEMON);

		pthread_mutex_unlock(&localized_lock);
		sem_post(&localized_limite);

		free(elemento_a_eliminar->pokemon);
		list_destroy(elemento_a_eliminar->l_coordenadas);
		free(elemento_a_eliminar->l_coordenadas);
		free(elemento_a_eliminar);
	}
}

void atender_mensaje_new(t_new* mensaje) {


	// crear una copia de esta lista;
	// NEW_STRUCT->suscriptores;

	t_list* suscriptores_para_enviar = list_duplicate(NEW_STRUCT.suscriptores);

	pthread_t* threads = malloc(sizeof(pthread_t) * suscriptores_para_enviar->elements_count);

	for (int i = 0; i < suscriptores_para_enviar->elements_count; i++) {
		pthread_create(&(threads[i]), NULL, (void*) atender_appeared, NULL);
		pthread_join(threads[i], NULL);
	}




	// crear un hilo para cada suscriptor para enviar los mensajes

	// chequear que ya volvieron todos
	// no se como se hace esto










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
	pthread_create(&hilo_escucha_de_clientes, NULL, (void *) esperar_clientes, (void*) socket_servidor);
	pthread_join(hilo_escucha_de_clientes, NULL);

	pthread_t hilo_atencion_de_new;
	pthread_t hilo_atencion_de_appeared;
	pthread_t hilo_atencion_de_catch;
	pthread_t hilo_atencion_de_caught;
	pthread_t hilo_atencion_de_get;
	pthread_t hilo_atencion_de_localized;

	pthread_create(&hilo_atencion_de_new, NULL, (void *) atender_new, NULL);
	pthread_join(hilo_atencion_de_new, NULL);
	pthread_create(&hilo_atencion_de_appeared, NULL, (void *) atender_appeared, NULL);
	pthread_join(hilo_atencion_de_appeared, NULL);
	pthread_create(&hilo_atencion_de_catch, NULL, (void *) atender_catch, NULL);
	pthread_join(hilo_atencion_de_catch, NULL);
	pthread_create(&hilo_atencion_de_caught, NULL, (void *) atender_caught, NULL);
	pthread_join(hilo_atencion_de_caught, NULL);
	pthread_create(&hilo_atencion_de_get, NULL, (void *) atender_get, NULL);
	pthread_join(hilo_atencion_de_get, NULL);
	pthread_create(&hilo_atencion_de_localized, NULL, (void *) atender_localized, NULL);
	pthread_join(hilo_atencion_de_localized, NULL);

	// Terminamos el programa

	terminar_programa();
	return EXIT_SUCCESS;
}

void inicializar_colas(void) {
	NEW_POKEMON = queue_create();
	APPEARED_POKEMON = queue_create();
	CATCH_POKEMON = queue_create();
	CAUGHT_POKEMON = queue_create();
	GET_POKEMON = queue_create();
	LOCALIZED_POKEMON = queue_create();

	new_suscriptores = list_create();
	appeared_suscriptores = list_create();
	catch_suscriptores = list_create();
	caught_suscriptores = list_create();
	get_suscriptores = list_create();
	localized_suscriptores = list_create();

	NEW_STRUCT.cola = NEW_POKEMON;
	APPEARED_STRUCT.cola = APPEARED_POKEMON;
	CATCH_STRUCT.cola = CATCH_POKEMON;
	CAUGHT_STRUCT.cola = CAUGHT_POKEMON;
	GET_STRUCT.cola = GET_POKEMON;
	LOCALIZED_STRUCT.cola = LOCALIZED_POKEMON;

	NEW_STRUCT.suscriptores = new_suscriptores;
	APPEARED_STRUCT.suscriptores = appeared_suscriptores;
	CATCH_STRUCT.suscriptores = catch_suscriptores;
	CAUGHT_STRUCT.suscriptores = caught_suscriptores;
	GET_STRUCT.suscriptores = get_suscriptores;
	LOCALIZED_STRUCT.suscriptores = localized_suscriptores;

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

	sem_init(&new_limite, 0, limite_cola_new);
	sem_init(&appeared_limite, 0, limite_cola_appeared);
	sem_init(&catch_limite, 0, limite_cola_catch);
	sem_init(&caught_limite, 0, limite_cola_caught);
	sem_init(&get_limite, 0, limite_cola_get);
	sem_init(&localized_limite, 0, limite_cola_localized);

	sem_init(&new_mensajes, 0, 0);
	sem_init(&appeared_mensajes, 0, 0);
	sem_init(&catch_mensajes, 0, 0);
	sem_init(&caught_mensajes, 0, 0);
	sem_init(&get_mensajes, 0, 0);
	sem_init(&localized_mensajes, 0, 0);

	pthread_mutex_init(&new_lock, NULL);
	pthread_mutex_init(&appeared_lock, NULL);
	pthread_mutex_init(&catch_lock, NULL);
	pthread_mutex_init(&caught_lock, NULL);
	pthread_mutex_init(&get_lock, NULL);
	pthread_mutex_init(&localized_lock, NULL);
}

void terminar_programa(void) {
	log_destroy(logger);
	log_destroy(extense_logger);
	config_destroy(config);
	queue_clean(NEW_POKEMON);
	queue_destroy(NEW_POKEMON);
	queue_clean(APPEARED_POKEMON);
	queue_destroy(APPEARED_POKEMON);
	queue_clean(CATCH_POKEMON);
	queue_destroy(CATCH_POKEMON);
	queue_clean(CAUGHT_POKEMON);
	queue_destroy(CAUGHT_POKEMON);
	queue_clean(GET_POKEMON);
	queue_destroy(GET_POKEMON);
	queue_clean(LOCALIZED_POKEMON);
	queue_destroy(LOCALIZED_POKEMON);
	list_clean(new_suscriptores);
	list_clean(appeared_suscriptores);
	list_clean(catch_suscriptores);
	list_clean(caught_suscriptores);
	list_clean(get_suscriptores);
	list_clean(localized_suscriptores);
	list_destroy(new_suscriptores);
	list_destroy(appeared_suscriptores);
	list_destroy(catch_suscriptores);
	list_destroy(caught_suscriptores);
	list_destroy(get_suscriptores);
	list_destroy(localized_suscriptores);
	sem_destroy(&new_limite);
	sem_destroy(&appeared_limite);
	sem_destroy(&catch_limite);
	sem_destroy(&caught_limite);
	sem_destroy(&get_limite);
	sem_destroy(&localized_limite);
	sem_destroy(&new_mensajes);
	sem_destroy(&appeared_mensajes);
	sem_destroy(&catch_mensajes);
	sem_destroy(&caught_mensajes);
	sem_destroy(&get_mensajes);
	sem_destroy(&localized_mensajes);
	pthread_mutex_destroy(&new_lock);
	pthread_mutex_destroy(&appeared_lock);
	pthread_mutex_destroy(&catch_lock);
	pthread_mutex_destroy(&caught_lock);
	pthread_mutex_destroy(&get_lock);
	pthread_mutex_destroy(&localized_lock);

	destruir_generador_id();

}
