#ifndef TEAM_H_
#define TEAM_H_

#include <commons/string.h>
#include <commons/collections/queue.h>
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "semaphore.h"
#include "team_utils.h"
#include "protocol_team.h"

#endif

t_log* logger;
t_log* extense_logger;

t_config* config;

int tiempo_deadlock;

int socket_escucha_game_boy;
int socket_escucha_appeared;
int socket_escucha_caught;
int socket_escucha_localized;

t_list* entrenadores; // t_entrenador

t_list* objetivo_global; // nombres de pokemon
t_list* pokemones_a_localizar; // nombres de pokemon

t_queue* cola_mensajes_recibidos;

pthread_mutex_t cola_mensajes_recibidos_mutex;

sem_t sem_cola_mensajes_nuevos;

// que se yo que va aca
typedef enum
{
	ATRAPAR_POKEMON 		= 111,
	INTERCAMBIAR_POKEMON 	= 112,
	NO_HACER_PINGO 			= 113 		// NULL
} id_tarea;

typedef enum
{
	ESTADO_NEW 				= 1111,
	ESTADO_READY 			= 1112,
	ESTADO_EXECUTING 		= 1113,
	ESTADO_BLOCKED 			= 1114,
	ESTADO_EXIT 			= 1115
} estado;

typedef struct {
	id_tarea id_tarea;
	void* parametros; // cada tipo de tarea tiene una estructura de parametros
} t_tarea;

typedef struct {
	uint32_t index;
	uint32_t posX;
	uint32_t posY;
	int pokebolas;
	t_list* pokemones;
	t_list* pokemones_innecesarios;
	t_list* objetivos;
	t_list* objetivos_actuales;
	estado estado;
	t_tarea* tarea_actual;
} t_entrenador;

typedef struct {
	op_code tipo_mensaje;
	void* mensaje; // t_appeared, t_caught o t_localized
} t_mensaje_recibido;




t_log* iniciar_logger(char* log_file);
t_log* iniciar_logger_sin_consola(char* log_file);
t_config* leer_config(void);
void terminar_programa();
void inicializar_entrenadores(void);
void inicializar_cola(void);
void obtener_objetivo_global(void);
void obtener_pokemones_a_localizar(void);
void laburar(void* entrenador);
void planificar (void);
void escuchar_appeared_de_broker(void);
void escuchar_caught_de_broker(void);
void escuchar_localized_de_broker(void);
void buscar_deadlocks();
