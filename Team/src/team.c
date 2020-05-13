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
#include <pthread.h>
#include "utils.h"


// variables globales

t_list* idsGet;
char* ip;
char* puerto;
t_log* logger;
t_config* config;
t_config* entrenadoresCFG;
t_list* entrenadores;
t_list* objetivosTeam;
t_list* pokemonesAtrapados;

t_config* cargarConfigEntrenadores(){
	t_config* entrenadores;
	entrenadores = config_create("src/entrenadores.config");
	return entrenadores;
}
t_list* retornarLista(char* listaCFG,t_log* logger)
{
	t_list* lista = list_create();
	char* token = strtok(listaCFG,"|");
	if(token != NULL)
	{
		while(token != NULL)
		{
			list_add(lista,token);
			log_info(logger, "tengo elemento %s",token);
			token = strtok(NULL,"|");
		}
	}
	else
	{
		log_info(logger, "no tengo elementos");
	}

	return lista;
}
uint32_t convertir(char posicion)
{
	uint32_t  numero = posicion;
	return numero - 48;
}


t_entrenador* crearEntrenador(t_list* entrenadores,char* conjuntoObjetivos,char* posicion,char* conjuntoPokemon,t_list* objetivosTeam,t_log* logger)
{
	t_entrenador* entrenador= malloc(sizeof(t_entrenador));
	t_list* pokemenes = retornarLista(conjuntoPokemon,logger);
	t_list* objetivos = retornarLista(conjuntoObjetivos,logger);
	list_add_all(objetivosTeam,objetivos);
	entrenador->objetivo = list_create();
	list_add_all(entrenador->objetivo,objetivos);
	entrenador->pokemones = list_create();
	list_add_all(entrenador->pokemones,pokemenes);
	uint32_t* posicion1 = convertir(posicion[0]) ;
	uint32_t* posicion2 = convertir(posicion[2]) ;
	uint32_t* capacidad = list_size(objetivos);
	entrenador->coordx = posicion1;
	log_info(logger, "%d",entrenador->coordx);
	entrenador->coordY = posicion2;
	log_info(logger, "%d",entrenador->coordY);
	entrenador->capacidad = capacidad;
	log_info(logger, "%d",entrenador->capacidad);
	list_add(entrenadores,entrenador);
	return entrenador;

}

t_list* filtrarObjetivos(t_list* objetivos)
{
	t_list* objetivosFiltrados= list_create();
	list_add(objetivosFiltrados,list_get(objetivos,0));
	log_info(logger,"encontrado %d",list_size(objetivos));
	for(int i = 1; list_size(objetivos)>i;i++ )
	{
		char* pokemonBuscado = list_get(objetivos,i);
		log_info(logger,"encontrado %s",pokemonBuscado);
		bool encontrar(char* pokemon)
		{
			return string_equals_ignore_case(pokemonBuscado,pokemon);

		}
		char* pokemonEncontrado = list_find(objetivosFiltrados,(bool(*)(void*))&encontrar);
		if(pokemonEncontrado == NULL)
		{
			list_add(objetivosFiltrados,pokemonBuscado);
		}

	}


	return objetivosFiltrados;
}


void mainGet(t_list* objetivos){


	t_list* objetivosEnviar = filtrarObjetivos(objetivos);
	int cantidadMensajes= list_size(objetivosEnviar);
	for(int i =0;cantidadMensajes>i;i++)
	{
		char* objetivo = list_get(objetivosEnviar,i);
		int conexion = crear_conexion(ip, puerto);
		mensaje_get(objetivo,conexion,logger);
		uint32_t idMensajeRecibido = recibir_get(conexion,logger);
		list_add(idsGet,idMensajeRecibido);
		liberar_conexion(conexion);
	}

}

void mainAppeared(int conexionAppeared)
{
	 t_pokemon pokemonEncontrado = recibir_mensaje(socket,logger);
	 t_list* objetivoEncontrado = list_create();
	 t_list* objetivosExistentes = list_create();
	 objetivoEncontrado = filtrarObjetivos(objetivosTeam,pokemonEncontrado->nombre);
	 objetivosExistentes = filtrarObjetivos(pokemonesAtrapados,pokemonEncontrado->nombre);
	 if(list_size(objetivoEncontrado) > 0)
	 {
		 if( list_size(objetivoEncontrado) >  list_size(objetivosExistentes))
		 {

		 }

	 }

}


int main(void){
	int conexion;
	int conexionAppeared;
	int conexionCatch;
	int conexionLocalized;
	pthread_t* appeared;
	pthread_t* catch;
	pthread_t* localized;
	pthread_t* get;


	logger = iniciar_logger();
	config = leer_config();
	log_info(logger, "probar logger");

	ip 		= config_get_string_value(config, "IP");
	log_info(logger, "probar logger definir puerto");
	puerto 	= config_get_string_value(config, "PUERTO");
	conexion = crear_conexion(ip, puerto);
	conexionAppeared = crear_conexion(ip, puerto);
	conexionCatch = crear_conexion(ip, puerto);
	conexionLocalized = crear_conexion(ip, puerto);
	idsGet = list_create();


	entrenadores = list_create();
	objetivosTeam = list_create();
	int i = 0;
	entrenadoresCFG = cargarConfigEntrenadores();
	log_info(logger, "lista creada entrenadores");
	char** objetivos = config_get_array_value(entrenadoresCFG, "OBJETIVOS_ENTRENADORES"); // obtenemos los objetivos
	char** posiciones = config_get_array_value(entrenadoresCFG,"POSICIONES_ENTRENADORES");
	char** pokemones = config_get_array_value(entrenadoresCFG,"POKEMON_ENTRENADORES");
	//log_info(logger, "obtenemos objetivos");
	while(objetivos[i] != NULL && posiciones[i] != NULL)
	{
		char* objetivoEntrenador = (char*) objetivos[i];
		char* posicion = (char*) posiciones[i];
		char* conjuntoPokemon;
		if(pokemones[i]!= NULL)
			conjuntoPokemon = (char*) pokemones[i];

		log_info(logger, "obtenemos primer objetivo %s",objetivoEntrenador);
		t_entrenador* trainer = crearEntrenador(entrenadores,objetivoEntrenador,posicion,conjuntoPokemon,objetivosTeam,logger);
		log_info(logger, "entrenador cargado completamente");
		i = i + 1;
	}
	log_info(logger, "lista entrenadores completa");
	log_info(logger,"La cantidad de objetivos es %d",list_size(objetivosTeam));
	t_entrenador* primerEntrenador = list_get(entrenadores,0);
	log_info(logger, "coordenada x %d",primerEntrenador->coordx);
	log_info(logger, "coordenada y %d",primerEntrenador->coordY);
//	char* objetivoEntrenador= list_get(objetivosTeam,0);
//	log_info(logger, "objetivo 0 %s",objetivoEntrenador);
	char* pokemonObj= (char*)list_get(primerEntrenador->objetivo,1);
	log_info(logger, "objetivo 0 %s",pokemonObj);
	char* pokemon= (char*)list_get(primerEntrenador->pokemones,1);
	log_info(logger, "objetivo 0 %s",pokemon);
//	t_list* objetivosFiltrados = list_create();
//	objetivosFiltrados = filtrarObjetivos(objetivosTeam);
//	log_info(logger, "cantidad objetivos filtrados %d",list_size(objetivosFiltrados));
//	log_info(logger, "objetivo  %s",list_get(objetivosFiltrados,0));
//	log_info(logger, "objetivo  %s",list_get(objetivosFiltrados,1));
//	log_info(logger, "objetivo  %s",list_get(objetivosFiltrados,2));
//	log_info(logger, "objetivo  %s",list_get(objetivosFiltrados,3));
//	log_info(logger, "objetivo  %s",list_get(objetivosFiltrados,4));


// crear conexiones
//	 pthread_create (appeared,NULL,mainAppeared(),conexionAppeared);
//	 pthread_create (catch,NULL,mainCatch(),conexionCatch);
//	 pthread_create (localized,NULL,mainLocalized(),conexionLocalized);
//	 pthread_create (get,NULL,mainGet(),objetivosTeam);
//
////void mainAppeared();
////void mainCatch();
////void mainLocalized();
//
//
//	  pthread_join(appeared,0);
//	  pthread_join(catch,0);
//	  pthread_join(localized,0);
//	  pthread_join(get,0);



//      conexion = crear_conexion(ip, puerto);
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
	t_log* logger ;
	logger= log_create("src/tp0.log", "tp0", 1, LOG_LEVEL_INFO);
	return logger;
}

t_config* leer_config(){
		t_config* config;
		config = config_create("src/tp0.config");
		return config;
}
