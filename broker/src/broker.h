#ifndef BROKER_H_
#define BROKER_H_

#include "utilsBroker.h"
#include "commons/collections/queue.h"
#include "commons/log.h"
#include "commons/config.h"
#include "pthread.h"
#include "semaphore.h"
#include "broker_msj_id.h"

#endif /* BROKER_H_ */

t_log* logger;
t_log* extense_logger;

t_queue* NEW_POKEMON;
t_queue* APPEARED_POKEMON;
t_queue* CATCH_POKEMON;
t_queue* CAUGHT_POKEMON;
t_queue* GET_POKEMON;
t_queue* LOCALIZED_POKEMON;

t_list* new_suscriptores;
t_list* appeared_suscriptores;
t_list* catch_suscriptores;
t_list* caught_suscriptores;
t_list* get_suscriptores;
t_list* localized_suscriptores;

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

typedef struct {
	void* mensaje;
	uint32_t id;
} mensaje_queue;

t_log* iniciar_logger(char* log_file);
t_config* leer_config(void);
t_config* leer_config(void);
void inicializar_colas(void);
void terminar_programa(t_config* config);
