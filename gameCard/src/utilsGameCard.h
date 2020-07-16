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
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/string.h>
#include<commons/collections/queue.h>
#include <commons/bitarray.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "archivosYDirectorios.h"
#include "protocoloGameCard.h"
#include "bitArray.h"



char* ipBroker;
char* puertoBroker;
char* puntoMontaje;
uint32_t tiempoReintentoConexion;
uint32_t tiempoReintentoOperacion;
uint32_t tiempoRetardoOperacion;

//SACAR DEL METADATA/METADATA.BIN
uint32_t blockSize;
uint32_t cantidadDeBloques;
t_bitarray *bitarray;
char* magicNumber;




t_log* loggerDev;
t_config* config;
t_log* loggerGameCard;

char* rutaMetaData;
char* rutaFiles;
char* rutaBlocks;
char* bitMap;




struct stat st1;


typedef enum
{
	METADATA =1,
	BITMAP = 2,
	METADATA_FILE = 3,
	METADATA_POKEMON = 4,
	BLOCK = 5
}tipoArchivo;



int crear_conexion(char* ip, char* puerto);
void enviar_mensaje(char*argv[], int socket_cliente);
void recibir_mensaje(int socket_cliente);
void crearDirectorio(char*, char*);
void liberar_conexion(int socket_cliente);


#endif /* UTILSGAMECARD_H_ */
