#ifndef TEAM_H_
#define TEAM_H_

#include <commons/string.h>
#include <commons/collections/queue.h>
#include <commons/collections/list.h>
#include <commons/config.h>
#include <commons/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include "semaphore.h"
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

// FIFO    1
// RR      2
// SJF-SD  3
// SJF-CD  4
int algoritmo_planificacion;

int quantum;

double alpha;
int estimacion_inicial;

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

t_queue* cola_pokemones;
t_queue* cola_caught;
t_queue* cola_deadlock;

pthread_mutex_t cola_pokemones_mutex;
pthread_mutex_t cola_caught_mutex;
pthread_mutex_t cola_deadlock_mutex;
pthread_mutex_t pokemones_llegados_mutex;
pthread_mutex_t catch_IDs_mutex;
pthread_mutex_t estoy_conectado_al_broker_mutex;
pthread_mutex_t planificacion_ready;
pthread_mutex_t planificacion_sjf;

sem_t sem_cola_pokemones;
sem_t sem_cola_caught;
sem_t sem_cola_deadlock;

sem_t sem_entrenadores_disponibles;

t_dictionary* pokemones_conocidos_que_no_se_intentan_atrapar;

t_dictionary* cantidad_de_pokemones_que_puedo_planificar;

int ciclos_CPU;
int cambios_contexto;
int cantidad_deadlocks;

pthread_mutex_t ciclos_CPU_mutex;
pthread_mutex_t cambios_contexto_mutex;
pthread_mutex_t cantidad_deadlocks_mutex;

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
	MENSAJE_POKEMON		 = 10
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
	int contador_ciclos_CPU;
	int contador_quantum;
	double estimacion;
	double real;
	double real_actual;
} t_entrenador;

//typedef struct {
//	tipo_mensaje tipo_mensaje;
//	void* mensaje; // t_pokemon, t_caught o t_deadlock
//} t_mensaje_recibido;

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

typedef struct {
	char* nombre;
	int cantidad;
} t_pokemon_planificables;

t_log* iniciar_logger(char* log_file);
t_log* iniciar_logger_sin_consola(char* log_file);
t_config* leer_config(void);
void terminar_programa();
void inicializar_entrenadores(void);
int cantidad_de_pokemon_entrenadores(char** pokemones);
void inicializar_cola(void);
void obtener_objetivo_global(void);
void obtener_pokemones_a_localizar(void);
void laburar_fifo(void* entrenador);
void laburar_rr(void* entrenador);
void laburar_sjf_sd(void* entrenador);
void laburar_sjf_cd(void* entrenador);
void planificar_pokemon(void);
void planificar_caught(void);
void planificar_deadlock(void);
void escuchar_appeared_de_broker(void);
void escuchar_caught_de_broker(void);
void escuchar_localized_de_broker(void);
void buscar_deadlocks(void);
void verificar_conexion(void);
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
t_deadlock* agregar_nivel_hasta_deadlock(t_nodo_arbol* raiz, int i, t_list* entrenadores_bloqueados);
t_list* obtener_nivel_de_arbol(t_nodo_arbol* raiz, int i);
int agregar_hijos(t_nodo_arbol* nodo, t_nodo_arbol* raiz, t_list* entrenadores_bloqueados);
t_list* entrenadores_que_necesitan_pokemon(char* pokemon, t_list* entrenadores_bloqueados);
t_deadlock* armar_deadlock(t_nodo_arbol* nodo_raiz);
void liberar_arbol(t_nodo_arbol* raiz);
int todavia_existe_deadlock(t_deadlock* mensaje_deadlock);
int entrenador_tiene_pokemon_innecesario(t_entrenador* entrenador, char* pokemon);
int pokemon_ya_fue_recibido(char* pokemon);
bool es_pokemon_global(char* nombre_pokemon);
t_list* entrenadores_que_pueden_ir_a_atrapar();
void agrego_pokemon_a_dictionary(t_pokemon* pokemon_a_agregar);
t_pokemon* generar_pokemon_de_appeared(t_appeared* mensaje_appeared);
t_list* generar_pokemones_de_localized(t_localized* mensaje_localized);
void obtener_cantidad_de_cada_pokemon_a_planificar();
int tengo_que_planificar_pokemon(t_pokemon* mensaje_pokemon);
t_entrenador* entrenador_mas_cercano(t_list* entrenadores_disponibles_para_ir_a_atrapar, int posXPokemon, int posYPokemon);
void contar_planificacion(t_pokemon* pokemon);
void liberar_planificacion(t_pokemon* pokemon);
int tengo_en_el_mapa(char* pokemon);
void borrar_pokemon_del_mapa(t_pokemon* pokemon);
t_pokemon* mejor_pokemon_para_reintentar(t_entrenador* entrenador, char* pokemon);
void cambiar_estado_de_entrenador(t_entrenador* entrenador, estado estado_nuevo, char* razon);
void cambiar_tarea_de_entrenador(t_entrenador* entrenador, t_tarea* tarea_nueva);
void mostrar_metricas();
int es_id_catch(uint32_t id);
int calcular_posicion_entrenador(int posXEntrenador, int posYEntrenador, int posXPokemon, int posYPokemon);
char* obtener_nombre_de_tarea(id_tarea id);
char* obtener_nombre_de_estado(estado id);
void consumir_tiempo_retardo(t_entrenador* entrenador);
double calcular_nueva_estimacion(t_entrenador* entrenador);
void definir_nueva_estimacion(t_entrenador* entrenador);
void mandar_a_siguente_entrenador_a_ejecutar_por_sjf(void);
void desalojar_si_es_necesario(t_entrenador* entrenador_planificado);
int cantidad_de_entrenadores_en_ready_o_exec(void);
