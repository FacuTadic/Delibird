/*
 * utilsGameCard.h
 *
 *  Created on: 26 may. 2020
 *      Author: utnso
 */
#ifndef UTILSGAMECARD_H_
#define UTILSGAMECARD_H_


#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include<commons/config.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "archivosYDirectorios.h"



char* ipBroker;
char* puertoBroker;
char* puntoMontaje;
uint32_t tiempoReintentoConexion;
uint32_t tiempoReintentoOperacion;
uint32_t tiempoRetardoOperacion;

//SACAR DEL METADATA/METADATA.BIN
uint32_t blockSize;
uint32_t cantidadDeBloques;




t_log* loggerDev;
t_config* config;
t_log* loggerGameCard;

char* rutaMetaData;
char* rutaFiles;
char* rutaBlocks;

struct stat st1;


typedef enum
{
	NEW          = 1,
    APPEARED     = 2,
    CATCH        = 3,
    CAUGHT       = 4,
	GET          = 5,
	LOCALIZED	 = 6,
	SUSCRIPTOR 	 = 7,
	MENSAJE 	 = 8//TEST
}op_code;

typedef enum
{
	BROKER       = 0,
	TEAM	     = 1,
	GAMECARD     = 2,
	SERVER		 = 3 //TEST
}op_codeModulo;


typedef enum
{
	METADATA =1,
	BITMAP = 2,
	METADATA_FILE = 3,
	METADATA_POKEMON = 4,
	BLOCK = 5
}tipoArchivo;



typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;



//########################################################### MENSAJE NEW - APPEARED ###########################################################

// New ‘Pikachu’ 5 10 2
typedef struct
{
	uint32_t idMensaje;
    uint32_t sizePokemon;
	char*	 pokemon;
    uint32_t  coordX;
    uint32_t  coordY;
    uint32_t cantidad;
}t_new;


// Appeared ‘Pikachu’ 1 5
typedef struct
{
	uint32_t sizePokemon;
	char*	 pokemon;
	uint32_t  coordX;
	uint32_t  coordY;
}t_appeared;

//###############################################################################################################################################




//########################################################### MENSAJE GET - LOCALIZED ###########################################################

typedef struct
{
	uint32_t sizePokemon;
    uint32_t  coordX;
    uint32_t  coordY;
}t_get;


// Localized ‘Pikachu’ 3 4 5 1 5 9 3
typedef struct
{
	uint32_t sizePokemon;
	char*	 pokemon;
    uint32_t lugares;
    uint32_t  coordX;
    uint32_t  coordY;

}t_localized;

//##############################################################################################################################################




//########################################################### MENSAJE CATCH - CAUGHT ###########################################################

// Catch ‘Pikachu’ 1 5
typedef struct
{
	uint32_t sizePokemon;
	char*	 pokemon;
    uint32_t  coordX;
    uint32_t  coordY;
}t_catch;

typedef struct{

 uint32_t flagCaught;

 }t_caught;

//##################################################################################################################################################



int crear_conexion(char* ip, char* puerto);
void enviar_mensaje(char*argv[], int socket_cliente);
void recibir_mensaje(int socket_cliente);
void crearDirectorio(char*, char*);
void eliminar_paquete(t_paquete* paquete);
void liberar_conexion(int socket_cliente);


#endif /* UTILSGAMECARD_H_ */
