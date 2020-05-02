#include"utilsGameBoy.h"


t_log* iniciar_logger(){
	//t_log* logger = log_create("gameBoy.log", "Game-watch-client", 1, LOG_LEVEL_INFO);
	return log_create("gameBoy.log", "Game-watch-client", 1, LOG_LEVEL_INFO);
}


t_config* leer_config(){
		t_config* config;
		config = config_create("gameBoy.config");
		return config;
}


char* modulos[5]={"BROKER","TEAM","GAMECARD","SUSCRIPTOR","SERVER"};


int main(int argc,char*argv[]){

	int conexion;
	char* ipNombre = string_new();
	char* puertoNombre = string_new();
	char* ip;
	char* puerto;


	t_log* logger;
	t_config* config;

	logger = iniciar_logger();
	log_info(logger, "logger iniciado.");

	config = leer_config();
	log_info(logger, "config cargada.");


	printf("%s",argv[1]);



	string_append(&ipNombre, "IP_");
	string_append(&ipNombre,argv[1]);

	log_info(logger, "EL modulo array es: %s", argv[1]);
	log_info(logger, "La IP es: %s", ipNombre);


	string_append(&puertoNombre, "PUERTO_");
	string_append(&puertoNombre,argv[1]);

	log_info(logger, "EL modulo array es: %s", argv[1]);
	log_info(logger, "La IP es: %s", ipNombre);



	ip = config_get_string_value(config, ipNombre);
	puerto = config_get_string_value(config, puertoNombre);
	log_info(logger, "La IP es: %s", ip);
	log_info(logger, "El Puerto es: %s", puerto);



	conexion = crear_conexion(ip, puerto);


	terminar_programa(conexion, logger, config);

}



void terminar_programa(int conexion, t_log* logger, t_config* config){
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);

}
