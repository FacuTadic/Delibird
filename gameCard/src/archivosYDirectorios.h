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
t_log* loggerGameCardArchDir;
char* rutaBlocksArchDir;
uint32_t blockSizeArchDir;


typedef enum
{
	METADATA =1,
	BITMAP = 2,
	METADATA_FILE = 3,
	METADATA_POKEMON = 4,
	BLOCK = 5
}tipoArchivo;


struct stat st1;

void crearTemplateDeArchivoTipo(tipoArchivo tipo, char* nombreDelArchivo ,char* ruta);
char* generadorDeRutaDeCreacionDeDirectorios(char* ruta, char* nombreDelDirectorio);
char* generadorDeRutaDeCreacionDeArchivos(char* rutaDirectorio,char* nombreDelArchivo, char* tipoDeDato);
char* generadorDePosiciones(uint32_t posX, uint32_t posY);
bool existeArchivo(char* ruta);
uint32_t tamanioDeUnArchivo(char* rutaDelArchivo);
void actualizarBlockMetadata(t_config* archivoMetadata, char*blockAAgregar);
void actualizarSizeMetadataPokemon(t_config* archivoMetadata);
bool hayEspacioEnElBlock(char* rutaDelBlock, char* posicion, uint32_t cantidad);
void crearDirectorio(char* directorio, char* puntoMontaje);
void crearArchivoEnDirectorio(char* nombreDelArchivo, char* directorio);
bool puedeAbrirseArchivo(t_config* archivo);
void activarFlagDeLectura(t_config* archivo);
void desactivarFlagDeLectura(t_config* archivo);
char** cargarVectorSinElValor(char** arrayViejo,char* block);
void borrarBloqueDe(t_config* archivoMetaData, char*block);
void eliminarKeyValueDe(t_config* archivoBlock,char* posicion);
char* cargarArrayEnChar(char** arrayViejo);
void agregarCantidadSolicitadaAUnaKey(t_config* archivo,char* key, uint32_t cantidad);
void decrementarEnUnoEnLaPosicion(t_config* archivo,char* key);
uint32_t cantidadDePokemonEnUnaCoordenada(t_config* archivoBlock,char* posicion);
void agregarNuevaPosicionA(char* block, char* posicion, uint32_t cantidad);
void obtenerTodasLasPosiciones(char** blocks, t_queue* posicionesPokemon);
bool noExisteDirectorio(char* ruta);
#endif /* ARCHIVOSYDIRECTORIOS_H_ */
