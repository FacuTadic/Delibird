#ifndef ARCHIVOSYDIRECTORIOS_H_
#define ARCHIVOSYDIRECTORIOS_H_

#include "utilsGameCard.h"
#include<string.h>
#include <commons/log.h>



char* generadorDeRutaDeCreacionDeDirectorios(char* ruta, char* nombreDelDirectorio);
char* generadorDeRutaDeCreacionDeArchivos(char* rutaDirectorio,char* nombreDelArchivo, char* tipoDeDato);
bool noExisteDirectorio(char* ruta);
bool existeArchivo(char* ruta);
uint32_t tamanioDeUnArchivo(char* rutaDelArchivo);
void crearDirectorio(char* directorio, char* puntoMontaje);
void crearArchivoEnDirectorio(char* nombreDelArchivo, char* directorio);


bool puedeAbrirseArchivo(t_config* archivo);
void activarFlagDeLectura(t_config* archivo);
void eliminarKeyValueDe(t_config* archivoBlock,char* posicion);
void agregarCantidadSolicitadaAUnaKey(t_config* archivo,char* key, uint32_t cantidad);
void decrementarEnUnoEnLaPosicion(t_config* archivo,char* key);
uint32_t cantidadDePokemonEnUnaCoordenada(t_config* archivoBlock,char* posicion);
char* seleccionarBlockParaCargarPosiciones(char** blocksOcupados, char*posicion, uint32_t cantidad);
void agregarNuevaPosicionA(char* block, char* posicion, uint32_t cantidad);
void obtenerTodasLasPosiciones(char** blocks, t_queue* posicionesPokemon);

#endif /* ARCHIVOSYDIRECTORIOS_H_ */
