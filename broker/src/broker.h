
#ifndef BROKER_H_
#define BROKER_H_

#include "utilsBroker.h"
#include "commons/collections/queue.h"
#include "commons/log.h"
#include "commons/config.h"
#include "commons/process.h"
#include "pthread.h"
#include "semaphore.h"
#include "broker_ids.h"
#include "memoria_broker.h"
#include "signal.h"

#endif

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

t_list* gl_appeared_ids_respondidos;
t_list* gl_caught_ids_respondidos;
t_list* gl_localized_ids_respondidos;

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

pthread_mutex_t gl_appeared_ids_lock;
pthread_mutex_t gl_caught_ids_lock;
pthread_mutex_t gl_localized_ids_lock;

pthread_mutex_t mutex_utilizacion_memoria;

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
	void* mensaje; // se mapea a t_new, t_appeared, etc
	uint32_t id;
} mensaje_queue; // estructura que se mete en las colas

typedef struct {
	uint32_t id_cliente;
	int socket_cliente;
	int envio_ok; // 1 OK 0 MAL
	int ack; // 1 OK 0 MAL
} status_envio;

typedef struct {
	mensaje_queue* mensaje_queue;
	info_modulo* info;
	status_envio* status;
} mandable_struct; // estructura auxiliar para mandar a los hilos que mandan mensajes de cola

typedef struct {
	segmento_memoria* segmento;
	info_modulo* info_modulo;
	status_envio* status;
} mandable_memoria_struct; // estructura auxiliar para mandar a los hilos que mandan mensajes de memoria

typedef struct {
	uint32_t id;
} id_respondido;

t_log* iniciar_logger(char* log_file);
t_log* iniciar_logger_sin_consola(char* log_file);
t_config* leer_config(void);
void inicializar_memoria(void);
void inicializar_cola(void);
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
void* serializar_new(mensaje_queue* argumento_new, uint32_t* bytes);
void* serializar_appeared(mensaje_queue* argumento_appeared, uint32_t* bytes);
void* serializar_catch(mensaje_queue* argumento_catch, uint32_t* bytes);
void* serializar_caught(mensaje_queue* argumento_caught, uint32_t* bytes);
void* serializar_get(mensaje_queue* argumento_get, uint32_t* bytes);
void* serializar_localized(mensaje_queue* argumento_localized, uint32_t* bytes);
void* serializar_new_para_memoria(t_new* new, uint32_t* bytes);
void* serializar_appeared_para_memoria(t_appeared* appeared, uint32_t* bytes);
void* serializar_catch_para_memoria(t_catch* catch, uint32_t* bytes);
void* serializar_caught_para_memoria(t_caught* caught, uint32_t* bytes);
void* serializar_get_para_memoria(t_get* get, uint32_t* bytes);
void* serializar_localized_para_memoria(t_localized* localized, uint32_t* bytes);
void* serializar_new_desde_memoria(segmento_memoria* new, int* bytes);
void* serializar_appeared_desde_memoria(segmento_memoria* appeared, int* bytes);
void* serializar_catch_desde_memoria(segmento_memoria* catch, int* bytes);
void* serializar_caught_desde_memoria(segmento_memoria* caught, int* bytes);
void* serializar_get_desde_memoria(segmento_memoria* get, int* bytes);
void* serializar_localized_desde_memoria(segmento_memoria* localized, int* bytes);
void enviar_new_de_memoria(info_modulo* info_modulo);
void enviar_appeared_de_memoria(info_modulo* info_modulo);
void enviar_catch_de_memoria(info_modulo* info_modulo);
void enviar_caught_de_memoria(info_modulo* info_modulo);
void enviar_get_de_memoria(info_modulo* info_modulo);
void enviar_localized_de_memoria(info_modulo* info_modulo);
void enviar_mensaje_new_de_memoria(void* new_mandable_memoria);
void enviar_mensaje_appeared_de_memoria(void* new_mandable_memoria);
void enviar_mensaje_catch_de_memoria(void* new_mandable_memoria);
void enviar_mensaje_caught_de_memoria(void* new_mandable_memoria);
void enviar_mensaje_get_de_memoria(void* new_mandable_memoria);
void enviar_mensaje_localized_de_memoria(void* new_mandable_memoria);
void eliminar_suscriptor(pthread_mutex_t lock, t_list* lista_de_suscriptores, info_modulo* suscriptor);
int contiene_al_id_respondido(t_list* lista_de_ids_respondidos, uint32_t id_a_buscar);
