#ifndef BROKER_H_
#define BROKER_H_

#include "utilsBroker.h"
#include "commons/collections/queue.h"
#include "commons/log.h"
#include "commons/config.h"
#include "commons/process.h"
#include "pthread.h"
#include "semaphore.h"
#include "broker_msj_id.h"

#endif /* BROKER_H_ */

t_log* logger;
t_log* extense_logger;

t_config* config;

t_queue* gl_new_pokemon_queue;
t_queue* gl_appeared_pokemon_queue;
t_queue* gl_catch_pokemon_queue;
t_queue* gl_caught_pokemon_queue;
t_queue* gl_get_pokemon_queue;
t_queue* gl_localized_pokemon_queue;

t_list* gl_new_suscriptores;
t_list* gl_appeared_suscriptores;
t_list* gl_catch_suscriptores;
t_list* gl_caught_suscriptores;
t_list* gl_get_suscriptores;
t_list* gl_localized_suscriptores;

pthread_mutex_t gl_new_queue_lock;
pthread_mutex_t gl_appeared_queue_lock;
pthread_mutex_t gl_catch_queue_lock;
pthread_mutex_t gl_caught_queue_lock;
pthread_mutex_t gl_get_queue_lock;
pthread_mutex_t gl_localized_queue_lock;

pthread_mutex_t gl_new_list_lock;
pthread_mutex_t gl_appeared_list_lock;
pthread_mutex_t gl_catch_list_lock;
pthread_mutex_t gl_caught_list_lock;
pthread_mutex_t gl_get_list_lock;
pthread_mutex_t gl_localized_list_lock;

sem_t gl_new_limite;
sem_t gl_appeared_limite;
sem_t gl_catch_limite;
sem_t gl_caught_limite;
sem_t gl_get_limite;
sem_t gl_localized_limite;

sem_t gl_new_mensajes;
sem_t gl_appeared_mensajes;
sem_t gl_catch_mensajes;
sem_t gl_caught_mensajes;
sem_t gl_get_mensajes;
sem_t gl_localized_mensajes;

typedef enum
{
	NEW_ID          = 11,
    APPEARED_ID     = 12,
	CATCH_ID        = 13,
	CAUGHT_ID       = 14,
	GET_ID          = 15,
    LOCALIZED_ID    = 16
} id_cola;

typedef struct {
	t_queue* cola;
	t_list* suscriptores;
	id_cola id;
} cola_struct;

cola_struct NEW_STRUCT;
cola_struct APPEARED_STRUCT;
cola_struct CATCH_STRUCT;
cola_struct CAUGHT_STRUCT;
cola_struct GET_STRUCT;
cola_struct LOCALIZED_STRUCT;

typedef struct {
	void* mensaje;
	uint32_t id;
} mensaje_queue;

typedef struct {
	mensaje_queue* mensaje_queue;
	info_modulo* info;
} mandable_struct;

t_log* iniciar_logger(char* log_file);
t_config* leer_config(void);
t_config* leer_config(void);
void leer_info_modulos(void);
void inicializar_colas(void);
void inicializar_semaforos_colas(void);
void terminar_programa(void);
void atender_mensaje_new(mensaje_queue*);
void atender_mensaje_appeared(mensaje_queue*);
void atender_mensaje_catch(mensaje_queue*);
void atender_mensaje_caught(mensaje_queue*);
void atender_mensaje_get(mensaje_queue*);
void atender_mensaje_localized(mensaje_queue*);
void mandar_new(void* new_mandable);
void mandar_appeared(void* new_mandable);
void mandar_catch(void* new_mandable);
void mandar_caught(void* new_mandable);
void mandar_get(void* new_mandable);
void mandar_localized(void* new_mandable);
void* serializar_new(mandable_struct* argumento_new, uint32_t* bytes);
void* serializar_appeared(mandable_struct* argumento_appeared, uint32_t* bytes);
void* serializar_catch(mandable_struct* argumento_catch, uint32_t* bytes);
void* serializar_caught(mandable_struct* argumento_caught, uint32_t* bytes);
void* serializar_get(mandable_struct* argumento_get, uint32_t* bytes);
void* serializar_localized(mandable_struct* argumento_localized, uint32_t* bytes);
