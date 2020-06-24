/*
 ============================================================================
 Name        : gameCard.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "utilsGameCard.h"

t_log* iniciar_loggerDEV(){
	//t_log* logger = log_create("gameBoy.log", "Game-watch-client", 1, LOG_LEVEL_INFO);
	return log_create("gameCardDEV.log", "Game-watch-client", 1, LOG_LEVEL_INFO);
}


t_log* iniciar_loggerGameCard(){
	//t_log* logger = log_create("gameBoy.log", "Game-watch-client", 1, LOG_LEVEL_INFO);
	return log_create("gameCard.log", "Game-watch-client", 1, LOG_LEVEL_INFO);
}

t_config* leer_config(){
		t_config* config;
		config = config_create("gameCard.config");
		return config;
}


void terminar_programa(int conexion);



int main(void) {

	int conexion;
	ipBroker = string_new();
	puertoBroker = string_new();


// ################################################# 	SETTER Y SARAZA 	####################################################

	loggerGameCard = iniciar_loggerGameCard();
	log_info(loggerGameCard, "logger iniciado.");

	loggerDev = iniciar_loggerDEV();
	log_info(loggerDev, "logger iniciado.");

	config = leer_config();
	log_info(loggerDev, "Config cargada.");

	ipBroker = config_get_string_value(config, "IP_BROKER");
	log_info(loggerDev, "La IP del Broker: %s", ipBroker);

	puertoBroker = config_get_string_value(config, "PUERTO_BROKER");
	log_info(loggerDev, "El Puerto es: %s", puertoBroker);

	tiempoReintentoConexion = atoi(config_get_string_value(config, "TIEMPO_DE_REINTENTO_CONEXION"));
	log_info(loggerDev, "El tiempo de reintento de conexion es: %i", tiempoReintentoConexion);

	tiempoReintentoOperacion = atoi(config_get_string_value(config, "TIEMPO_DE_REINTENTO_OPERACION"));
	log_info(loggerDev, "El tiempo de reintento de operacion es: %i", tiempoReintentoOperacion);


	tiempoRetardoOperacion = atoi(config_get_string_value(config, "TIEMPO_RETARDO_OPERACION"));
	log_info(loggerDev, "El tiempo de retardo de conexion es: %i", tiempoRetardoOperacion);

	puntoMontaje = config_get_string_value(config, "PUNTO_MONTAJE_TALLGRASS");
	log_info(loggerDev, "El punto de montaje es: %s", puntoMontaje);






	levantarTallGrass(puntoMontaje);


//################################################## 	CONEXION 	###############################################################################
		do{

			conexion = crear_conexion( ipBroker, puertoBroker);

			if(!conexion){
				log_error(loggerDev, "Error en conexion con BROKER, ip: %s puerto : %s, reintentando en %d segundo%s ", ipBroker, puertoBroker,tiempoReintentoConexion,tiempoReintentoConexion=1 ? "":"s");
				sleep(tiempoReintentoConexion);
			}
		}while(!conexion );		//reintentar cada tiempoReintentoConexion
		log_info(loggerDev, "conexion establecida con BROKER, ip: %s puerto : %s", ipBroker, puertoBroker);


		enviarMensaje(msaj_suscripcion,puertoBroker);//mandar dejame escuchar colas y atenderlas con espera activa bloqueante

		esperar_mensaje(puertoBroker); // choripasteado y adaptado de BKR


		//COPIO ATENDER DE BKR??






	}
















void terminar_programa(int conexion){
	log_destroy(loggerDev);
	log_destroy(loggerGameCard);
	config_destroy(config);
	liberar_conexion(conexion);
}
