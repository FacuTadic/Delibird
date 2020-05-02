/*
 ============================================================================
 Name        : team.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "tp0.h"
#include <commons/string.h>
#include <commons/collections/queue.h>
#include <commons/collections/list.h>

t_config* cargarConfigEntrenadores(){
	t_config* entrenadores;
	entrenadores = config_create("entrenadores.config");
	return entrenadores;
}

int main(void){
	int conexion;
	int cantEntrenadores;
	t_log* logger;
	t_config* config;
	t_config* entrenadoresCFG;
	t_list* entrenadores;
	t_list* objetivosTeam;

	logger = iniciar_logger();
	config = leer_config();

	char* ip 		= config_get_string_value(config, "IP");
	char* puerto 	= config_get_string_value(config, "PUERTO");

	conexion = crear_conexion(ip, puerto);

	entrenadores = list_create();
	entrenadoresCFG = cargarConfigEntrenadores(); // Cargamos la config
	cantEntrenadores = config_keys_amount(entrenadoresCFG); // obtenemos la cardinalidad
	char** objetivos = config_get_array_value(entrenadoresCFG, "OBJETIVOS_ENTRENADORES"); // obtenemos los objetivos
	t_entrenador* trainer;
	list_add(entrenadores, trainer);
	log_info(logger, "La cantidad de entrenadores a cargar es de: %d", cantEntrenadores);
	log_info(logger, "lista creada");



//		funciones del TP
//		conexion = crear_conexion(ip, puerto);
//		enviar_mensaje("Pikachu", conexion,logger); // enviar_get ("pokemon", conexion);
//		char* mensajeRecibido = recibir_mensaje(conexion, logger); // el ID
//		log_info(logger,"el mensaje recibido es: %s" ,mensajeRecibido);
//		free (mensajeRecibido);
//		liberar_conexion(conexion);
	terminar_programa(conexion, logger, config);
}

void terminar_programa(int conexion, t_log* logger, t_config* config){
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);

}

t_log* iniciar_logger(void){
	t_log* logger = log_create("tp0.log", "Game-watch-client", 1, LOG_LEVEL_INFO);
	return logger;
}

t_config* leer_config(){
		t_config* config;
		config = config_create("tp0.config");
		return config;
}
