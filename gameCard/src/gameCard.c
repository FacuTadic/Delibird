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


void terminar_programa();



int main(void) {

	ipBroker = string_new();
	puertoBroker = string_new();


	generar_ID_Modulo();

	// ################################################# 	SETTER Y SARAZA 	####################################################

	loggerGameCard = iniciar_loggerGameCard();
	log_info(loggerGameCard, "logger iniciado.");

	loggerDev = iniciar_loggerDEV();
	log_info(loggerDev, "logger iniciado.");

	config = leer_config();
	log_info(loggerDev, "Config cargada.");

	ip = config_get_string_value(config, "IP");
	log_info(loggerDev, "La IP es: %s", ip);

	puerto = config_get_string_value(config, "PUERTO");
	log_info(loggerDev, "El puerto es: %s", puerto);

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


	//SETEO DE VARIABLES PARA ARCHIVOSYDIRECTORIOS.H
	loggerDevArchDir=loggerDev;
	rutaBlocksArchDir=rutaBlocks;
	blockSizeArchDir=blockSize;

	//SETEO DE VARIABLES PARA CREACIONDEESTRUCTURASPARAENVIAR.H
	loggerDevEstructuras = loggerDev;

	//SETEO DE VARIABLES PARA PROTOCOLOGC.H
	id_modulo = id_moduloGC;
	loggerDevProtocolo = loggerDev;

	pthread_mutex_init(&estoy_conectado_al_broker_mutex, NULL);
	pthread_mutex_init(&estoy_leyendo_metadata_mutex,NULL);


	//################################################## 	CONEXION 	###############################################################################


	socketEscuchaGameBoy = iniciarEscuchaGameBoy(ip, puerto);
	pthread_t hilo_escucha_de_game_boy;
	pthread_create(&hilo_escucha_de_game_boy, NULL, (void*) escucharGameBoy, (void*) socketEscuchaGameBoy);
	pthread_detach(hilo_escucha_de_game_boy);


	socketEscuchaNew = crear_conexion(ipBroker, puertoBroker);
	socketEscuchaCatch = crear_conexion(ipBroker, puertoBroker);
	socketEscuchaGet = crear_conexion(ipBroker, puertoBroker);
	pthread_t hiloEscuchaNew;
	pthread_t hiloEscuchaCatch;
	pthread_t hiloEscuchaGet;
	pthread_create(&hiloEscuchaNew, NULL, (void*) escucharNewDeBroker, NULL);
	pthread_create(&hiloEscuchaCatch, NULL, (void*) escucharCatchDeBroker, NULL);
	pthread_create(&hiloEscuchaGet, NULL, (void*) escucharGetDeBroker, NULL);
	pthread_detach(hiloEscuchaNew);
	pthread_detach(hiloEscuchaCatch);
	pthread_detach(hiloEscuchaGet);


	pthread_t hiloReconexionBroker;
	pthread_create(&hiloReconexionBroker, NULL, (void*) verificarConexion, NULL);
	pthread_detach(hiloReconexionBroker);

	terminar_programa();
}


void terminar_programa(){
	log_destroy(loggerDev);
	log_destroy(loggerGameCard);
	config_destroy(config);
	liberar_conexion(socketEscuchaNew);
	liberar_conexion(socketEscuchaCatch);
	liberar_conexion(socketEscuchaGet);
	liberar_conexion(socketEscuchaGameBoy);
	pthread_mutex_destroy(&estoy_conectado_al_broker_mutex);
	free(rutaMetaData);
	free(rutaFiles);
	free(rutaBlocks);
	free(bitMap);
	free(magicNumber);
}
