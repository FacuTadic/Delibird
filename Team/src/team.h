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
#include "team_utils.h"
#include "protocol_team.h"

#endif

t_log* logger;
t_log* extense_logger;

t_config* config;

t_list* entrenadores; // t_entrenador

t_list* objetivo_global; // nombres de pokemon
t_list* pokemones_a_localizar; // nombres de pokemon


typedef struct {
	uint32_t index;
	uint32_t posX;
	uint32_t posY;
	int pokebolas;
	t_list* pokemones;
	t_list* pokemones_innecesarios;
	t_list* objetivos;
	t_list* objetivos_actuales;
} t_entrenador;




t_log* iniciar_logger(char* log_file);
t_log* iniciar_logger_sin_consola(char* log_file);
t_config* leer_config(void);
void inicializar_entrenadores(void);
void obtener_objetivo_global(void);
void obtener_pokemones_a_localizar(void);
