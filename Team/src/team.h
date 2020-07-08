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

char* ip_broker;
char* puerto_broker;

int tiempo_deadlock;
uint32_t retardo_de_CPU;
int tiempo_reconexion;

int socket_escucha_game_boy;

int socket_escucha_appeared;
int socket_escucha_caught;
int socket_escucha_localized;

int estoy_conectado_al_broker; // 1 true 0 false

t_list* entrenadores; // t_entrenador

t_list* objetivo_global; // nombres de pokemon
t_list* pokemones_a_localizar; // nombres de pokemon

t_queue* cola_mensajes_recibidos;

pthread_mutex_t cola_mensajes_recibidos_mutex;

sem_t sem_cola_mensajes_nuevos;

typedef enum {
	ATRAPAR_POKEMON 		= 111,		// t_pokemon*
	INTERCAMBIAR_POKEMON 	= 112, 		// t_deadlock*
	NO_HACER_PINGO 			= 113, 		// NULL
} id_tarea;

typedef enum {
	ESTADO_NEW 				= 1111,
	ESTADO_READY 			= 1112,
	ESTADO_EXECUTING 		= 1113,
	ESTADO_BLOCKED 			= 1114,
	ESTADO_EXIT 			= 1115
} estado;

typedef enum {
	MENSAJE_APPEARED     = 2,
	MENSAJE_CAUGHT       = 4,
	MENSAJE_LOCALIZED    = 6,
	MENSAJE_DEADLOCK	 = 8,
} tipo_mensaje;

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
	tipo_mensaje tipo_mensaje;
	void* mensaje; // t_appeared, t_caught, t_localized o t_deadlock
} t_mensaje_recibido;

typedef struct {
	t_list* entrenadores; // de t_entrenador
	t_list* pokemones; // de pokemones char*
} t_deadlock;

typedef struct {
	char* nombre;
	uint32_t pos_X;
	uint32_t pos_Y;
} t_pokemon;

t_log* iniciar_logger(char* log_file);
t_log* iniciar_logger_sin_consola(char* log_file);
t_config* leer_config(void);
void terminar_programa();
void inicializar_entrenadores(void);
void inicializar_cola(void);
void obtener_objetivo_global(void);
void obtener_pokemones_a_localizar(void);
void laburar(void* entrenador);
void planificar(void);
void escuchar_appeared_de_broker(void);
void escuchar_caught_de_broker(void);
void escuchar_localized_de_broker(void);
void buscar_deadlocks();
void verificar_conexion();
void reconectar_al_broker();
t_deadlock* obtener_deadlock();
void corregir_deadlock(t_deadlock* deadlock);
void definir_primer_estado(t_entrenador* entrenador);
t_list* obtener_entrenadores_bloqueados();
void mandar_get();
void enviar_get_a_broker(char* nombre_pokemon);
void enviar_catch_a_broker(t_pokemon* pokemon);
void irA(uint32_t posX, uint32_t posY, t_entrenador* entrenador);
void intercambiar_pokemon_entre(t_entrenador* entrenador1, t_entrenador* entrenador2);
