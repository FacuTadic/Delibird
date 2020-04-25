int main(void){

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
}


t_log* iniciar_logger(void){
	t_log* logger = log_create("gameBoy.log", "Game-watch-client", 1, LOG_LEVEL_INFO);
	return logger;
}


t_config* leer_config(){
		t_config* config;
		config = config_create("gameBoy.config");
		return config;
}

void terminar_programa(int conexion, t_log* logger, t_config* config){
	log_destroy(logger);
	config_destroy(config);
	liberar_conexion(conexion);

}
