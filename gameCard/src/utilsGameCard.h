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
#include<netdb.h>
#include<string.h>
#include<semaphore.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/string.h>
#include<commons/collections/queue.h>
#include<commons/bitarray.h>
#include "archivosYDirectorios.h"
#include "protocoloGameCard.h"
#include "creacionDeEstructurasParaEnvio.h"


int id_moduloGC;


int socketEscuchaGameBoy;

int socketEscuchaNew;
int socketEscuchaCatch;
int socketEscuchaGet;


char* ip;
char* puerto;
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

int estoy_conectado_al_broker; // 1 true 0 false
int estoy_conectado_al_broker_ultimo_chequeo;


t_log* loggerDev;
t_config* config;
t_log* loggerGameCard;

char* rutaMetaData;
char* rutaFiles;
char* rutaBlocks;
char* bitMap;


pthread_mutex_t estoy_conectado_al_broker_mutex;
pthread_mutex_t estoy_chequeando_existencia_de_bloque_mutex;
pthread_mutex_t estoy_leyendo_metadata_mutex;
sem_t bloques_bitmap;

sem_t sem_conexion_new;
sem_t sem_conexion_catch;
sem_t sem_conexion_get;



int crear_conexion(char* ip, char* puerto);
char* seleccionarBlockParaCargarPosiciones(char** blocksOcupados, char*posicion, uint32_t cantidad);
void escucharGameBoy(void* socket_escucha_game_boy);
void escucharNewDeBroker(void);
void escucharCatchDeBroker(void);
void escucharGetDeBroker(void);
void levantarTallGrass(char* puntoMontaje);
void newPokemon(int socketCliente,t_newLlegada* new);
void catchPokemon(int socketCliente,t_catchLlegada* catch);
void getPokemon(int socketCliente,t_getLlegada* getLlegada);
int iniciarEscuchaGameBoy(char* IP, char* PUERTO);
void verificarConexion(void);
void generar_ID_Modulo();
bool noExisteDirectorio(char* ruta);
void liberar_conexion(int socket_cliente);


//############################################################################ SE PRENDE FUEGO EL FS ######################################################################################

char** generarVectorDePaths(char** blocksOcupados);
void string_append_char(char** string, char c);
char** generarVectorDeKeyValue(char* linea);
char* aplanarVectorDeKeyValue(char** vector);
t_list* traerRegistrosBloques(char** bloques);
t_list* evaluarRegistroNew(char** registro, t_list* registros);
void vaciarBloques(char** blocksOcupados);
void escribirEnBloques(t_list* registros, t_config* archivoMetadata);
bool evaluarRegistroCatch(char* registro, t_list* registros);
t_list* obtenerTodasLasPosiciones(char** blocks);
char* getCoordenadaDeRegistro(char** vector);

void desconectar_broker();


#endif /* UTILSGAMECARD_H_ */
