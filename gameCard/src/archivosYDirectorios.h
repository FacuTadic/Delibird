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

#endif /* ARCHIVOSYDIRECTORIOS_H_ */
