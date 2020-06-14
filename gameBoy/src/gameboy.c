#include"utilsGameBoy.h"

t_log* iniciar_logger(){
	//t_log* logger = log_create("gameBoy.log", "Game-watch-client", 1, LOG_LEVEL_INFO);
	return log_create("gameBoyDEV.log", "Game-watch-client", 1, LOG_LEVEL_INFO);
}


t_log* iniciar_loggerGameBoy(){
	//t_log* logger = log_create("gameBoy.log", "Game-watch-client", 1, LOG_LEVEL_INFO);
	return log_create("gameBoy.log", "Game-watch-client", 1, LOG_LEVEL_INFO);
}

t_config* leer_config(){
		t_config* config;
		config = config_create("gameBoy.config");
		return config;
}


void terminar_programa(int conexion);


int main(int argc,char*argv[]){

	int conexion;
	char* ipNombre = string_new();
	char* puertoNombre = string_new();


	loggerDev = iniciar_logger();
	log_info(loggerDev, "logger DEV iniciado.");


	loggerGameBoy = iniciar_loggerGameBoy();
	log_info(loggerDev, "logger GameBoy iniciado.");

	config = leer_config();
	log_info(loggerDev, "Config cargada.");


	log_info(loggerDev, "EL modulo solicitado para comunicacion es: %s", argv[1]);

	string_append(&ipNombre, "IP_");
	string_append(&ipNombre,argv[1]);


	string_append(&puertoNombre, "PUERTO_");
	string_append(&puertoNombre,argv[1]);


	ip = config_get_string_value(config, ipNombre);
	log_info(loggerDev, "La IP de %s es %s", argv[1],ipNombre);


	puerto = config_get_string_value(config, puertoNombre);
	log_info(loggerDev, "El PUERTO de %s es %s", argv[1],ipNombre);



	ip_game_boy = config_get_string_value(config, "IP_GAMEBOY");
	puerto_game_boy = config_get_string_value(config, "PUERTO_GAMEBOY");

	idGameBoy = config_get_int_value(config,"ID_GAMEBOY");
	log_info(loggerDev, "La ID del modulo GameBoy es: %i", idGameBoy);
//###################################################### PROCESO #####################################################################################

	log_info(loggerGameBoy, "Iniciando conexion con el modulo %s", argv[1]);
	conexion = crear_conexion(ip, puerto);
	log_info(loggerGameBoy, "Conexion exitosa con %s IP: %s, PUERTO: %s", argv[1],ip,puerto);

	enviar_mensaje(argv, conexion);

	if (!strcmp(argv[2],"SUSCRIPTOR")) {
		log_info(loggerGameBoy, "Solicitando suscripcion a: %s", argv[2]);
		pthread_t hiloEscucha;
		int error = pthread_create(&hiloEscucha,NULL, (void *) recibirMensajesDeSuscripcion, &conexion);

		if(error != 0){
			log_error(loggerDev, "Hubo un problema al crear el hilo");
			return error;
		} else {
			pthread_detach(hiloEscucha);
		}

		int tiempoSuscripcion = atoi(argv[4]);
		sleep(tiempoSuscripcion);
	}
	terminar_programa(conexion);

}



void terminar_programa(int conexion){
	liberar_conexion(conexion);
	log_destroy(loggerDev);
	log_destroy(loggerGameBoy);
	config_destroy(config);
}
