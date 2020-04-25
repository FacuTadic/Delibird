/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"
#include <commons/string.h>

int main(void){
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/
	int conexion;
	char* ip;
	char* puerto;

	t_log* logger;
	t_config* config;

	logger = iniciar_logger();
	log_info(logger, "logger iniciado.");

	config = leer_config();
	log_info(logger, "config cargada.");

	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");
	log_info(logger, "La IP es: %s", ip);
	log_info(logger, "El Puerto es: %s", puerto);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	conexion = crear_conexion(ip, puerto);

	char* mensaje = "Hola";

	enviar_mensaje(mensaje, conexion,logger);

	char* mensajeRecibido = recibir_mensaje(conexion, logger);

	log_info(logger,"el mensaje recibido es: %s" ,mensajeRecibido);



	free(mensajeRecibido);
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
