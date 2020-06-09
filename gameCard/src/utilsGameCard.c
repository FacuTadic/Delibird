#include "utilsGameCard.h"
#include<string.h>
#include <commons/log.h>

int crear_conexion(char *ip, char* puerto)
{
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if(connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1)
		printf("error");

	freeaddrinfo(server_info);

	return socket_cliente;
}



void crearTemplateDeArchivoTipo(tipoArchivo tipo, char* nombreDelArchivo ,char* ruta){
	FILE *archivo;

	char* rutaDeCreacion = generadorDeRutaDeCreacionDeArchivos(ruta, nombreDelArchivo, ".bin");
	archivo = fopen(rutaDeCreacion,"w+");

	switch(tipo){
	case METADATA:
		log_info(loggerDev, "Se intento entrar por METADATA");
		break;

	case BITMAP:
		break;

	case METADATA_FILE:
		fprintf(archivo	,"DIRECTORY" "=" "Y" "\n");
		fprintf(archivo	,"OPEN" "=" "N" "\n");

		break;

	case METADATA_POKEMON:
		fprintf(archivo	,"OPEN" "=" "N \n");
		fprintf(archivo	,"DIRECTORY" "=" "N \n");
		fprintf(archivo	,"SIZE" "=" " \n");
		fprintf(archivo	,"BLOCKS" "=" " \n");

		break;

	case BLOCK:
		break;

	default:
		log_info(loggerDev, "No se pudo reconocer el tipo de Archivo");
		break;
	}

	fclose(archivo);
}





void crearDirectorioFile(char* puntoMontaje){
	//Crear Directorio
	crearDirectorio("Files",puntoMontaje);
	//Seteo de variable global
	rutaFiles = generadorDeRutaDeCreacionDeDirectorios(puntoMontaje,"Files");
	log_info(loggerDev, "Se creo el directorio Files ubicado en: %s",rutaFiles);

	//Crear Archivo Metadata
	crearArchivoEnDirectorio("Metadata",rutaFiles);
	log_info(loggerDev, "Se crel el archivo Metadata de Files");

	//Escribir Archivo con el template
	crearTemplateDeArchivoTipo(METADATA_FILE,"Metadata",rutaFiles);
	log_info(loggerDev, "Se completo el tamplate de directorio File");
}


void crearDirectorioBlocks(char* puntoMontaje){
	//Crear Directorio
	crearDirectorio("Blocks",puntoMontaje);
	//Seteo de variable global
	rutaBlocks = generadorDeRutaDeCreacionDeDirectorios(puntoMontaje,"Blocks");
	log_info(loggerDev, "Se creo el directorio Blocks ubicado en: %s",rutaBlocks);
}


char* puedeAbrirseArchivo(char* rutaArchivo){
	log_error(loggerDev, "Se crea el archivo");
	FILE* archivo;
	archivo = fopen(rutaArchivo,"r+");
	log_error(loggerDev, "Se abre el archivo");
	//FORMA DE USAR ESTAS FUNCIONES DE CONFIG CON ARCHIVOS .TXT
	log_error(loggerDev, "A punto de hacer el casteo");
	char* estadoArchivo = config_get_string_value((t_config*)archivo, "OPEN");
	log_error(loggerDev, "Si, exploto");
	fclose(archivo);

	log_error(loggerDev, "Deberia retornar: %s",estadoArchivo);
	return estadoArchivo;
}



void levantarTallGrass(char* puntoMontaje){
	log_info(loggerDev, "Lenavando el Tall");
	//Seteo de variable global
	log_info(loggerDev, "El punto de montaje es: %s", puntoMontaje);
	rutaMetaData = generadorDeRutaDeCreacionDeDirectorios(puntoMontaje,"Metadata");
	char* bitMap = generadorDeRutaDeCreacionDeArchivos(rutaMetaData,"BitMap",".bin");


	if(existeArchivo(bitMap)){
		log_error(loggerDev, "Ya existe el BitMap");
	} else {
		crearDirectorio("Metadata",puntoMontaje);
		crearArchivoEnDirectorio("BitMap",rutaMetaData);
		log_info(loggerDev, "Se creo el BitMap ubicado en: %s",rutaMetaData);
		crearDirectorioFile(puntoMontaje);
		crearDirectorioBlocks(puntoMontaje);
		char* rutaMetaFile = generadorDeRutaDeCreacionDeArchivos(rutaFiles,"Metadata",".bin");
		char* pruebaValue = puedeAbrirseArchivo(rutaMetaFile);
		log_error(loggerDev, "EL valor OPEN del directorio File es: %s",pruebaValue);
	}
}



bool existenPosicionesEnArchivo(char* rutaArchivo){


}





void leerArchivo(char* rutaArchivo){
	FILE* archivo;
	archivo = fopen(rutaArchivo,"r+");

	//FORMA DE USAR ESTAS FUNCIONES DE CONFIG CON ARCHIVOS .TXT
	config_set_value(archivo, "OPEN", "F");

	fclose(archivo);
}





void validarPosicionesDeNew(char* rutaArchivo){

}

void validarPosicionesDeCatch(rutaDeArchivo){
	if(existenPosicionesEnArchivo(rutaDeArchivo)){
		/*

		validar cantidad = 1 => elimino la linea
		Sino decremento en -1
		*/
	} else {
		log_error(loggerGameCard, "No existen las posiciones en el archivo");
	}
}

void validarPosicionesDeGet(rutaDeArchivo){

}


bool hayEspacioEnElBlock(char* rutaDelBlock, char* mensaje){
	uint32_t tamanioBlock = tamanioDeUnArchivo(rutaDelBlock);
	uint32_t tamanioMensaje = sizeof(mensaje);

	return tamanioBlock+tamanioMensaje < blockSize;
}




void newPokemon(char* pokemon){
	char* rutaDeDirectorio = generadorDeRutaDeCreacionDeDirectorios(rutaFiles,pokemon);

	if(noExisteDirectorio(rutaDeDirectorio)){
		crearDirectorio(pokemon,rutaFiles);
		crearArchivoEnDirectorio(pokemon,rutaDeDirectorio);
		crearTemplateDeArchivoTipo(METADATA_POKEMON,pokemon,rutaDeDirectorio);
	}

	FILE *archivo;

	char* rutaDeArchivo = generadorDeRutaDeCreacionDeArchivos(generadorDeRutaDeCreacionDeDirectorios(rutaFiles,pokemon), pokemon, ".bin");
	archivo = fopen(rutaDeArchivo,"w+");

	if(puedeAbrirseArchivo(rutaDeArchivo)){
		leerArchivo(rutaDeArchivo);
		validarPosicionesDeNew(rutaDeArchivo);
	} else{
		sleep(tiempoReintentoOperacion);
		newPokemon(pokemon);
		exit(0);
	}

	sleep(tiempoRetardoOperacion);

	fclose(archivo);
}

void catchPokemon(char* pokemon){

	char* rutaDeDirectorio = generadorDeRutaDeCreacionDeDirectorios(rutaFiles,pokemon);

	if(noExisteDirectorio(rutaDeDirectorio)){
		log_error(loggerGameCard, "No existe directorio dentro de Files para el pokemon: %s",pokemon);
		exit(-1);
	}


	char* rutaDeArchivo = generadorDeRutaDeCreacionDeArchivos(generadorDeRutaDeCreacionDeDirectorios(rutaFiles,pokemon), pokemon, ".bin");

	if(puedeAbrirseArchivo(rutaDeArchivo)){
		leerArchivo(rutaDeArchivo);
		validarPosicionesDeCatch(rutaDeArchivo);
	}else{
		sleep(tiempoReintentoOperacion);
		catchPokemon(pokemon);
		exit(0);
	}

	sleep(tiempoRetardoOperacion);

}

void getPokemon(char* pokemon){

	char* rutaDeDirectorio = generadorDeRutaDeCreacionDeDirectorios(rutaFiles,pokemon);

	if(noExisteDirectorio(rutaDeDirectorio)){

		exit(0);
	}

	char* rutaDeArchivo = generadorDeRutaDeCreacionDeArchivos(generadorDeRutaDeCreacionDeDirectorios(rutaFiles,pokemon), pokemon, ".bin");

	if(puedeAbrirseArchivo(rutaDeArchivo)){
		leerArchivo(rutaDeArchivo);
		validarPosicionesDeGet(rutaDeArchivo);
	} else{
		sleep(tiempoReintentoOperacion);
		getPokemon(pokemon);
		exit(0);
	}

	sleep(tiempoRetardoOperacion);
}





void* serializar_paquete(t_paquete* paquete, uint32_t *bytes)
{
	*bytes = sizeof(uint32_t) *2 + paquete->buffer->size; // sizeof(int) *2  int del size del buffer y del opcode

	void * flujo = malloc(*bytes);
	uint32_t desplazamiento = 0;

	memcpy(flujo + desplazamiento, &(paquete->codigo_operacion), sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(flujo + desplazamiento, &(paquete->buffer->size), sizeof(uint32_t));
	desplazamiento+= sizeof(uint32_t);
	memcpy(flujo + desplazamiento, paquete->buffer->stream, paquete->buffer->size);
	desplazamiento+= paquete->buffer->size;

	return flujo;
}


//crearPaquete: Crea una estructura de tipo t_paquete y le enchufa el buffer dentro.

t_paquete* crearPaquete(t_buffer* buffer){
	t_paquete* paquete = malloc(sizeof(paquete));
	paquete->buffer = malloc(buffer->size + sizeof(buffer->size));
	memcpy (paquete->buffer,  buffer, buffer->size + sizeof(uint32_t));
	return paquete;
}








/*
void recibir_mensaje(int socket_cliente)
{
	t_paquete* paquete = malloc(sizeof(paquete));
	if (recv(socket_cliente, &paquete->codigo_operacion ,sizeof(uint32_t),MSG_WAITALL) == -1){
		log_error(loggerDev, "error: No se recibió el código de operación");
	}

	paquete->buffer = malloc (sizeof(paquete->buffer));
	if (recv(socket_cliente, &paquete->buffer->size, sizeof(uint32_t),MSG_WAITALL) == -1){
		log_error(loggerDev, "error: No se recibió el tañaño del buffer");
	}


	paquete->buffer->stream = malloc(paquete->buffer->size);
	if (recv(socket_cliente, paquete->buffer->stream, paquete->buffer->size,MSG_WAITALL) == -1){
		log_error(loggerDev, "error: No se pudo recibir el buffer");
	}

	switch(paquete->codigo_operacion){
			case NEW:
				log_error(loggerGameCard, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);
				desserializarNew(paquete->buffer->stream);
				break;


			case APPEARED:
				log_error(loggerGameCard, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);
				deserializarAppeard(paquete->buffer->stream);
				break;

			case CATCH:
				log_error(loggerGameCard, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);
				deserializarCatch(paquete->buffer->stream);
				break;

			case CAUGHT:
				log_error(loggerGameCard, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);
				deserializarCaught(paquete->buffer->stream);
				break;

			case GET:
				log_error(loggerGameCard, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);

				t_get* mensajeGet = malloc(sizeof(mensajeGet));
				uint32_t idPropio;

				memcpy(&(idPropio),paquete->buffer->stream, sizeof(uint32_t));
				paquete->buffer->stream += sizeof(uint32_t);
				log_error(loggerGameCard, "Llego con el ID del Broker: %i",idPropio);


//				#################		NOMBRE POKEMON		##################
				memcpy(&(mensajeGet->sizePokemon),paquete->buffer->stream, sizeof(uint32_t));
				paquete->buffer->stream += sizeof(uint32_t);
				memcpy(&(mensajeGet->pokemon), paquete->buffer->stream, sizeof(mensajeGet->sizePokemon));
				log_error(loggerGameCard, "Llego el pokemon: %s",mensajeGet->sizePokemon);

				free(mensajeGet);

				break;


			case LOCALIZED:
				log_error(loggerGameCard, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);
				deserializarLocalized(paquete->buffer->stream);
				break;


			case MENSAJE: ;
				char* mensaje = malloc(paquete->buffer->size);
				memcpy(mensaje, paquete->buffer->stream,paquete->buffer->size);
				free(paquete->buffer->stream);
				free(paquete->buffer);
				free(paquete);
				break;

			default:
				log_error(loggerDev, "Me mandaron cualquier cosa");
		}

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);


}

*/


void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
