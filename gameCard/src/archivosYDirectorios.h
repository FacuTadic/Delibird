#ifndef ARCHIVOSYDIRECTORIOS_H_
#define ARCHIVOSYDIRECTORIOS_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<sys/stat.h>
#include<string.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/string.h>
#include<commons/collections/queue.h>
#include <commons/bitarray.h>

t_log* loggerDevArchDir;
char* rutaBlocksArchDir;
uint32_t blockSizeArchDir;

char* generadorDeRutaDeCreacionDeDirectorios(char* ruta, char* nombreDelDirectorio);
char* generadorDeRutaDeCreacionDeArchivos(char* rutaDirectorio,char* nombreDelArchivo, char* tipoDeDato);
char* generadorDePosiciones(uint32_t posX, uint32_t posY);
bool noExisteDirectorio(char* ruta);
bool existeArchivo(char* ruta);
uint32_t tamanioDeUnArchivo(char* rutaDelArchivo);
void crearDirectorio(char* directorio, char* puntoMontaje);
void crearArchivoEnDirectorio(char* nombreDelArchivo, char* directorio);
void borrarBloqueDe(t_config* archivoMetaData, char*block);
bool puedeAbrirseArchivo(t_config* archivo);
bool hayEspacioEnElBlock(char* rutaDelBlock, char* posicion, uint32_t cantidad);
void activarFlagDeLectura(t_config* archivo);
void desactivarFlagDeLectura(t_config* archivo);
void eliminarKeyValueDe(t_config* archivoBlock,char* posicion);
void agregarCantidadSolicitadaAUnaKey(t_config* archivo,char* key, uint32_t cantidad);
void decrementarEnUnoEnLaPosicion(t_config* archivo,char* key);
uint32_t cantidadDePokemonEnUnaCoordenada(t_config* archivoBlock,char* posicion);
char* seleccionarBlockParaCargarPosiciones(char** blocksOcupados, char*posicion, uint32_t cantidad);
void agregarNuevaPosicionA(char* block, char* posicion, uint32_t cantidad);
void obtenerTodasLasPosiciones(char** blocks, t_queue* posicionesPokemon);

#endif /* ARCHIVOSYDIRECTORIOS_H_ */
