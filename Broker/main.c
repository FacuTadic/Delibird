#include "utilsBroker.h"
#include "commons/collections/queue.h"
#include "commons/log.h"
#include "commons/config.h"
#include "pthread.h"


t_queue* NEW_POKEMON;
t_queue* APPEARED_POKEMON;
t_queue* CATCH_POKEMON;
t_queue* CAUGHT_POKEMON;
t_queue* GET_POKEMON;
t_queue* LOCALIZED_POKEMON;

void* esperar_clientes(void* socket_servidor) {
	while(1) {
		esperar_cliente((int) socket_servidor);
	}
}

// Hay que ver si podemos hacer polimorfico esto
// Seguramente con un struct que contenga al t_queue*
// Si es asi hay que liberar bien la memoria al final

void* atender_new(void* args) {
	while (1) {
		// Hacer algo con el ultimo elemento de la cola
		//
	}
}

void* atender_appeared(void* args) {
	while (1) {
		// Hacer algo con el ultimo elemento de la cola
	}
}

void* atender_catch(void* args) {
	while (1) {
		// Hacer algo con el ultimo elemento de la cola
	}
}

void* atender_caught(void* args) {
	while (1) {
		// Hacer algo con el ultimo elemento de la cola
	}
}

void* atender_get(void* args) {
	while (1) {
		// Hacer algo con el ultimo elemento de la cola
	}
}

void* atender_localized(void* args) {
	while (1) {
		// Hacer algo con el ultimo elemento de la cola
	}
}

int main(void){
	char* ip;
	char* puerto;

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

}
