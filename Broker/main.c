#include "utilsBroker.h"
#include "commons/collections/queue.h"
#include "commons/log.h"
#include "commons/config.h"
#include "pthread.h"
#include "semaphore.h"


t_queue* NEW_POKEMON;
t_queue* APPEARED_POKEMON;
t_queue* CATCH_POKEMON;
t_queue* CAUGHT_POKEMON;
t_queue* GET_POKEMON;
t_queue* LOCALIZED_POKEMON;

pthread_mutex_t new_lock;
pthread_mutex_t appeared_lock;
pthread_mutex_t catch_lock;
pthread_mutex_t caught_lock;
pthread_mutex_t get_lock;
pthread_mutex_t localized_lock;

sem_t new_limite;
sem_t appeared_limite;
sem_t catch_limite;
sem_t caught_limite;
sem_t get_limite;
sem_t localized_limite;

sem_t new_mensajes;
sem_t appeared_mensajes;
sem_t catch_mensajes;
sem_t caught_mensajes;
sem_t get_mensajes;
sem_t localized_mensajes;


void* esperar_clientes(void* socket_servidor) {
	int socket = (int) socket_servidor;
	while(1) {
		esperar_cliente(socket);
	}
}

void esperar_cliente(int socket_servidor)
{
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	pthread_create(&thread,NULL,(void*) atender_cliente, &socket_cliente);
	pthread_detach(thread);
}

void atender_cliente(int* socket)
{
	int cod_op;
	if(recv(*socket, &cod_op, sizeof(int), MSG_WAITALL) == -1)
		cod_op = -1;
	procesar_request(cod_op, *socket);
}

void procesar_request(int cod_op, int cliente_fd) {
	int size;

	//borrar cuando tengamos los tipos de cada case
	void* msg;

		switch (cod_op) {
		case 1:
			// NEW

			// tipo_de_dato_new msg = recibir_new(cliente_fd, &size);
			msg = recibir_new(cliente_fd, &size);


			sem_wait(&new_limite);
			pthread_mutex_lock(&new_lock);

			// insertar en la cola new

			pthread_mutex_unlock(&new_lock);
			sem_post(&new_mensajes);

			close(cliente_fd);
			// seguro en vez de msg sea una estructura mas compleja, cuidado con el free asi de una
			// tal vez ni se necesite hacer el free debido a que esa estructura se podria meter en la cola
			free(msg);
			break;
		case 2:
			// APPEARED

			// tipo_de_dato_appeared msg = recibir_new(cliente_fd, &size);
			msg = recibir_appeared(cliente_fd, &size);


			sem_wait(&appeared_limite);
			pthread_mutex_lock(&appeared_lock);

			// insertar en la cola appeared

			pthread_mutex_unlock(&appeared_lock);
			sem_post(&appeared_mensajes);

			close(cliente_fd);
			// seguro en vez de msg sea una estructura mas compleja, cuidado con el free asi de una
			// tal vez ni se necesite hacer el free debido a que esa estructura se podria meter en la cola
			free(msg);
			break;
		case 3:
			// GET

			// tipo_de_dato_get msg = recibir_new(cliente_fd, &size);
			msg = recibir_get(cliente_fd, &size);


			sem_wait(&get_limite);
			pthread_mutex_lock(&get_lock);

			// insertar en la cola get

			pthread_mutex_unlock(&get_lock);
			sem_post(&get_mensajes);

			close(cliente_fd);
			// seguro en vez de msg sea una estructura mas compleja, cuidado con el free asi de una
			// tal vez ni se necesite hacer el free debido a que esa estructura se podria meter en la cola
			free(msg);
			break;
		case 4:
			// LOCALIZED

			// tipo_de_dato_localized msg = recibir_new(cliente_fd, &size);
			msg = recibir_localized(cliente_fd, &size);


			sem_wait(&localized_limite);
			pthread_mutex_lock(&localized_lock);

			// insertar en la cola appeared

			pthread_mutex_unlock(&localized_lock);
			sem_post(&localized_mensajes);

			close(cliente_fd);
			// seguro en vez de msg sea una estructura mas compleja, cuidado con el free asi de una
			// tal vez ni se necesite hacer el free debido a que esa estructura se podria meter en la cola
			free(msg);
			break;
		case 5:
			// CATCH

			// tipo_de_dato_catch msg = recibir_new(cliente_fd, &size);
			msg = recibir_catch(cliente_fd, &size);


			sem_wait(&catch_limite);
			pthread_mutex_lock(&catch_lock);

			// insertar en la cola catch

			pthread_mutex_unlock(&catch_lock);
			sem_post(&catch_mensajes);

			close(cliente_fd);
			// seguro en vez de msg sea una estructura mas compleja, cuidado con el free asi de una
			// tal vez ni se necesite hacer el free debido a que esa estructura se podria meter en la cola
			free(msg);
			break;
		case 6:
			// CAUGHT

			// tipo_de_dato_caught msg = recibir_new(cliente_fd, &size);
			msg = recibir_caught(cliente_fd, &size);


			sem_wait(&caught_limite);
			pthread_mutex_lock(&caught_lock);

			// insertar en la cola caught

			pthread_mutex_unlock(&caught_lock);
			sem_post(&caught_mensajes);

			close(cliente_fd);
			// seguro en vez de msg sea una estructura mas compleja, cuidado con el free asi de una
			// tal vez ni se necesite hacer el free debido a que esa estructura se podria meter en la cola
			free(msg);
			break;
		case 7:
			// MENSAJE
			// se tiene que dejar ???

			msg = recibir_mensaje(cliente_fd, &size);

			devolver_mensaje(msg, size, cliente_fd);


			close(cliente_fd);
			// seguro en vez de msg sea una estructura mas compleja, cuidado con el free asi de una
			// tal vez ni se necesite hacer el free debido a que esa estructura se podria meter en la cola
			free(msg);
			break;
		case 0:
			pthread_exit(NULL);
		case -1:
			pthread_exit(NULL);
		}
}

void* recibir_mensaje(int socket_cliente, int* size)
{
	void * buffer;

	recv(socket_cliente, size, sizeof(int), MSG_WAITALL);
	buffer = malloc(*size);
	recv(socket_cliente, buffer, *size, MSG_WAITALL);

	return buffer;
}

void* recibir_new(int socket_cliente, int* size) {

}

void* recibir_appeared(int socket_cliente, int* size) {

}

void* recibir_catch(int socket_cliente, int* size) {

}

void* recibir_caught(int socket_cliente, int* size) {

}

void* recibir_get(int socket_cliente, int* size) {

}

void* recibir_localized(int socket_cliente, int* size) {

}

// Hay que ver si podemos hacer polimorfico esto
// Seguramente con un struct que contenga al t_queue*
// Si es asi hay que liberar bien la memoria al final

void* atender_new(void* args) {
	while (1) {
		// Hacer algo con el ultimo elemento de la cola

		sem_wait(&new_mensajes);
		pthread_mutex_lock(&new_lock);

		pthread_mutex_unlock(&new_lock);
		sem_post(&new_limite);

	}
}

void* atender_appeared(void* args) {
	while (1) {
		// Hacer algo con el ultimo elemento de la cola

		sem_wait(&appeared_mensajes);
		pthread_mutex_lock(&appeared_lock);

		pthread_mutex_unlock(&appeared_lock);
		sem_post(&appeared_limite);

	}
}

void* atender_catch(void* args) {
	while (1) {
		// Hacer algo con el ultimo elemento de la cola

		sem_wait(&catch_mensajes);
		pthread_mutex_lock(&catch_lock);

		pthread_mutex_unlock(&catch_lock);
		sem_post(&catch_limite);

	}
}

void* atender_caught(void* args) {
	while (1) {
		// Hacer algo con el ultimo elemento de la cola

		sem_wait(&caught_mensajes);
		pthread_mutex_lock(&caught_lock);

		pthread_mutex_unlock(&caught_lock);
		sem_post(&caught_limite);

	}
}

void* atender_get(void* args) {
	while (1) {
		// Hacer algo con el ultimo elemento de la cola

		sem_wait(&get_mensajes);
		pthread_mutex_lock(&get_lock);

		pthread_mutex_unlock(&get_lock);
		sem_post(&get_limite);

	}
}

void* atender_localized(void* args) {
	while (1) {
		// Hacer algo con el ultimo elemento de la cola

		sem_wait(&localized_mensajes);
		pthread_mutex_lock(&localized_lock);

		pthread_mutex_unlock(&localized_lock);
		sem_post(&localized_limite);

	}
}

int main(void){
	char* ip;
	char* puerto;
	int limite_cola_new;
	int limite_cola_appeared;
	int limite_cola_catch;
	int limite_cola_caught;
	int limite_cola_get;
	int limite_cola_localized;

	t_log* logger;
	t_config* config;

	// Iniciamos logger, variables levantadas de config y colas globales

	logger = iniciar_logger();
	log_info(logger, "logger iniciado.");

	config = leer_config();
	log_info(logger, "config cargada.");

	inicializar_colas();

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	log_info(logger, "La IP es: %s", ip);
	log_info(logger, "El Puerto es: %s", puerto);

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

	// Levantamos el servidor en la ip y el puerto definidos en config

	int socket_servidor = iniciar_servidor(ip, puerto);

	// Iniciamos un hilo para ir escuchando a los clientes
	// Tiene que estar vivo mientras que el broker este funcionando. Entonces corresponde detach?

	pthread_t* hilo_escucha_de_clientes;
	pthread_create(&hilo_escucha_de_clientes, NULL, (void *) esperar_clientes, socket_servidor);
	pthread_join(hilo_escucha_de_clientes, NULL);

	pthread_t* hilo_atencion_de_new;
	pthread_t* hilo_atencion_de_appeared;
	pthread_t* hilo_atencion_de_catch;
	pthread_t* hilo_atencion_de_caught;
	pthread_t* hilo_atencion_de_get;
	pthread_t* hilo_atencion_de_localized;

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

	terminar_programa(logger, config);
	return EXIT_SUCCESS;
}

void inicializar_colas(void) {
	NEW_POKEMON = queue_create();
	APPEARED_POKEMON = queue_create();
	CATCH_POKEMON = queue_create();
	CAUGHT_POKEMON = queue_create();
	GET_POKEMON = queue_create();
	LOCALIZED_POKEMON = queue_create();
}


t_log* iniciar_logger(void){
	t_log* logger = log_create("broker.log", "Game-watch-client", 1, LOG_LEVEL_INFO);
	return logger;
}

t_config* leer_config(){
		t_config* config;
		config = config_create("broker.config");
		return config;
}

void terminar_programa(t_log* logger, t_config* config){
	log_destroy(logger);
	config_destroy(config);
	queue_clean(&NEW_POKEMON);
	queue_destroy(&NEW_POKEMON);
	queue_clean(&APPEARED_POKEMON);
	queue_destroy(&APPEARED_POKEMON);
	queue_clean(&CATCH_POKEMON);
	queue_destroy(&CATCH_POKEMON);
	queue_clean(&CAUGHT_POKEMON);
	queue_destroy(&CAUGHT_POKEMON);
	queue_clean(&GET_POKEMON);
	queue_destroy(&GET_POKEMON);
	queue_clean(&LOCALIZED_POKEMON);
	queue_destroy(&LOCALIZED_POKEMON);
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
}
