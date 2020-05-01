/*
 * main.c
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#include "tp0.h"
#include <commons/string.h>
#include <stdio.h>

void encolar(t_entrenador* entrenador, char* token){


}

void insertarObjetivo(t_entrenador* listaEntrenadores, char* objetivo, t_log* logger){

	uint32_t capacidad = 0;
	t_objetivo* objetivoNuevo = listaEntrenadores->objetivo;

	char* token = strtok(objetivo, "|");
	while (token != NULL) {
		int size = strlen(token) + 1;
		objetivoNuevo = malloc(sizeof(t_objetivo));
		objetivoNuevo->nombre = malloc(size);
		memcpy(objetivoNuevo->nombre, token,size); // acá
		log_info(logger, "asignado objetivo: %s", objetivoNuevo->nombre);
		objetivoNuevo = objetivoNuevo->sgte;
		token = strtok(NULL, "|");
		capacidad++;
	}
	listaEntrenadores->capacidad = capacidad;
	log_info(logger, "terminado de cargar objetivos. La cantidad de pokebolas de entrenador es de: %d", capacidad);
}


int asignarObjetivos(t_entrenador* listaEntrenadores, char** objetivos,int cantEntrenadores, t_log* logger) {
	int i = 0;

	//listaEntrenadores->objetivo->sgte = NULL;
	while (objetivos[i] != NULL) {
	log_info(logger, "el id %d tiene los siguientes objetivos %s",i+1 ,objetivos[i]);
		insertarObjetivo(listaEntrenadores, objetivos[i], logger);
		log_info(logger, "terminado de cargar Objetivos de entrenador nro %i:", i+1);
		i++;
	}
	return i;
}


t_entrenador* crearEntrenadores(int cantidad, t_log* logger){
	t_entrenador* entrenador = malloc(sizeof(t_entrenador));
	t_entrenador* aux = entrenador;
	int i;
	aux->id = 1;
	log_info(logger, "Se inicializó un entrenador con id nro: %d", aux->id);
	for (i=2; i<=cantidad; i++) {
		aux->sgte = malloc(sizeof(t_entrenador));
		aux = aux->sgte;
		aux->id = i;
		log_info(logger, "Se inicializó un entrenador con id nro: %d", aux->id);
	}
	aux->sgte = NULL;
	return entrenador;
}

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
	t_config* entrenadores;

	logger = iniciar_logger();

	config = leer_config();

	char* ip 		= config_get_string_value(config, "IP");
	char* puerto 	= config_get_string_value(config, "PUERTO");
	log_info(logger, "La IP es: %s", ip);
	log_info(logger, "El Puerto es: %s", puerto);
	conexion = crear_conexion(ip, puerto);


	entrenadores = cargarConfigEntrenadores(); // Cargamos la config
	cantEntrenadores = config_keys_amount(entrenadores); // obtenemos la cardinalidad
	log_info(logger, "La cantidad de entrenadores a cargar es de: %d", cantEntrenadores);

	t_entrenador* listaEntrenadores = crearEntrenadores(cantEntrenadores, logger); // inicializamos la lista de entrenadores con sus ID (MOLDE) - preguntar si es NULL
	char** objetivos = config_get_array_value(entrenadores, "OBJETIVOS_ENTRENADORES"); // obtenemos los objetivos

	int i = asignarObjetivos(listaEntrenadores, objetivos,cantEntrenadores, logger);

	log_info(logger, "hay %d objetivos en total", i);


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


