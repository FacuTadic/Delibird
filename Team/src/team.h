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
t_list* pokemones_llegados;

t_list* catch_IDs; // t_catch_id*

t_queue* cola_mensajes_recibidos;

pthread_mutex_t cola_mensajes_recibidos_mutex;

pthread_mutex_t planificacion_fifo;

sem_t sem_cola_mensajes_nuevos;

t_list* pokemones_conocidos_que_no_se_intentan_atrapar;

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
	int index;
	uint32_t posX;
	uint32_t posY;
	int pokebolas;
	t_list* pokemones;
	t_list* pokemones_innecesarios;
	t_list* objetivos;
	t_list* objetivos_actuales;
	estado estado;
	t_tarea* tarea_actual;
	sem_t semaforo;
} t_entrenador;

typedef struct {
	tipo_mensaje tipo_mensaje;
	void* mensaje; // t_appeared, t_caught, t_localized o t_deadlock
} t_mensaje_recibido;

typedef struct {
	t_list* entrenadores; // de t_entrenador
	t_list* pokemones; // de pokemones char*, son pokemones al pedo
} t_deadlock;

typedef struct {
	char* nombre;
	uint32_t pos_X;
	uint32_t pos_Y;
	int lo_estan_atrapando; // 1 true 0 false
} t_pokemon;

typedef struct nodo_arbol {
	t_entrenador* entrenador;
	struct nodo_arbol* padre;
	t_list* hijos; // nodo_arbol*
	char* pokemon_faltante;
} t_nodo_arbol;

typedef struct {
	uint32_t id_catch;
	t_entrenador* entrenador;
	t_pokemon* pokemon;
} t_catch_id;

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
void enviar_catch_a_broker(t_pokemon* pokemon, t_entrenador* entrenador);
void irA(uint32_t posX, uint32_t posY, t_entrenador* entrenador);
void intercambiar_pokemones(t_entrenador* entrenador1, t_entrenador* entrenador2, char* pokemon1, char* pokemon2);
void adquirir_pokemon(t_entrenador* entrenador, char* pokemon);
void eliminar_pokemon(t_entrenador* entrenador, char* pokemon);
t_deadlock* obtener_deadlock_por_arbol(t_list* entrenadores_bloqueados, int i);
t_entrenador* obtener_entrenador(int id);
t_deadlock* agregar_nivel_hasta_deadlock(t_nodo_arbol* raiz, int i);
t_list* obtener_nivel_de_arbol(t_nodo_arbol* raiz, int i);
int agregar_hijos(t_nodo_arbol* nodo, t_nodo_arbol* raiz);
t_list* entrenadores_que_necesitan_pokemon(char* pokemon);
t_deadlock* armar_deadlock(t_nodo_arbol* nodo_raiz);
void liberar_arbol(t_nodo_arbol* raiz);
int todavia_existe_deadlock(t_deadlock* mensaje_deadlock);
int pokemon_ya_fue_recibido(char* pokemon);
