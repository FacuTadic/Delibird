/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"
#include <commons/string.h>
#include <stdio.h>
#include "utils.h"

int crearEntrenadores(char** objetivos,t_log* logger);
t_config* cargarConfigEntrenadores(){
	t_config* entrenadores;
	entrenadores = config_create("entrenadores.config");
	return entrenadores;

}

int main(void){
	int conexion;

	t_log* logger;
	t_config* config;
	t_config* entrenadores;

	logger = iniciar_logger();
	log_info(logger, "logger iniciado.");

	config = leer_config();
	log_info(logger, "config cargada.");

	char* ip 		= config_get_string_value(config, "IP");
	char* puerto 	= config_get_string_value(config, "PUERTO");
	log_info(logger, "La IP es: %s", ip);
	log_info(logger, "El Puerto es: %s", puerto);

	conexion = crear_conexion(ip, puerto);

	entrenadores = cargarConfigEntrenadores();
	int cantEntrenadores = config_keys_amount(entrenadores);
	char** objetivos = config_get_array_value(entrenadores, "OBJETIVOS_ENTRENADORES");
	//int i = crearEntrenadores(objetivos, logger);


	//log_info(logger, "encontre %d", cantEntrenadores);
	//log_info(logger, "hay %d objetivos", i);





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
