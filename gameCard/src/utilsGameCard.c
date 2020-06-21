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


void cargarInfoDelMetadata(char* rutaMetadata){

	char* archivoMetaData = generadorDeRutaDeCreacionDeArchivos(rutaMetadata,"Metadata",".bin");
	t_config* metaData=config_create(archivoMetaData);
	log_info(loggerDev, "Se creo el config de MetaData");
	blockSize = config_get_int_value(metaData,"BLOCK_SIZE");
	log_info(loggerDev, "El Block Size es: %i", blockSize);
	cantidadDeBloques = config_get_int_value(metaData,"BLOCKS");
	log_info(loggerDev, "La cantidad de Bloques es: %i", cantidadDeBloques);
	magicNumber = config_get_string_value(metaData, "MAGIC_NUMBER");
	log_info(loggerDev, "El Magic Number es: %s", magicNumber);
}

void levantarTallGrass(char* puntoMontaje){
	log_info(loggerDev, "Lenavando el Tall");
	//Seteo de variable global
	log_info(loggerDev, "El punto de montaje es: %s", puntoMontaje);
	rutaMetaData = generadorDeRutaDeCreacionDeDirectorios(puntoMontaje,"Metadata");
	cargarInfoDelMetadata(rutaMetaData);


	char* bitMap = generadorDeRutaDeCreacionDeArchivos(rutaMetaData,"BitMap",".bin");


	if(existeArchivo(bitMap)){
		log_error(loggerDev, "Ya existe el BitMap");
	} else {
		crearDirectorio("Metadata",puntoMontaje);
		crearArchivoEnDirectorio("BitMap",rutaMetaData);
		log_info(loggerDev, "Se creo el BitMap ubicado en: %s",rutaMetaData);
		crearDirectorioFile(puntoMontaje);
		crearDirectorioBlocks(puntoMontaje);
	}
}




bool existenPosicionesEnArchivo(char* posicion,char** blocks){
	uint32_t i = 0;
	while(blocks[i] != NULL){
		log_info(loggerDev, "Se esta analizando el block: %s",blocks[i]);
		char* rutaDeArchivo = generadorDeRutaDeCreacionDeArchivos(rutaBlocks,blocks[i],".bin");
		t_config* archivoBlock = config_create(rutaDeArchivo);
		if(config_has_property(archivoBlock,posicion)){
			return true;
		} else{
			config_destroy(archivoBlock);
			i++;
		}
	}
	return false;
}

char* blockDondeSeEncuentraLaPosicion(char* posicion, char** blocks){
	uint32_t i = 0;
	while(blocks[i] != NULL){
		log_info(loggerDev, "Se esta analizando el block: %s",blocks[i]);
		char* rutaDeArchivo = generadorDeRutaDeCreacionDeArchivos(rutaBlocks,blocks[i],".bin");
		t_config* archivoBlock = config_create(rutaDeArchivo);
		if(config_has_property(archivoBlock,posicion)){
			return blocks[i];
		} else{
			config_destroy(archivoBlock);
			i++;
		}
}




uint32_t calcularPesoTotalDeBlockEnMetadataPokemon(char** blocks){
	uint32_t i = 0;
	uint32_t pesoTotal = 0;
	while(blocks[i] != NULL){
		log_info(loggerDev, "Se esta analizando el block: %s",blocks[i]);
		char* rutaDeArchivo = generadorDeRutaDeCreacionDeArchivos(rutaBlocks,blocks[i],".bin");
		pesoTotal += tamanioDeUnArchivo(rutaDeArchivo);
		log_info(loggerDev, "Tamanio total actual: %i",tamanioDeUnArchivo);
		i++;
	}

	log_info(loggerDev, "Tamanio total final: %i",tamanioDeUnArchivo);
	return pesoTotal;
}



void validarPosicionesDeNew(char** blocksOcupados,char* posicion, uint32_t cantidad){
	if(existenPosicionesEnArchivo(posicion,blocksOcupados)){
			char* block = blockDondeSeEncuentraLaPosicion(posicion,blocksOcupados);
			char* rutaDelBlock = generadorDeRutaDeCreacionDeArchivos(rutaBlocks,block,".bin");
			t_config* archivoBlock = config_create(rutaDelBlock);
			agregarCantidadSolicitadaAUnaKey(archivoBlock,posicion,cantidad);
			config_destroy(archivoBlock);
		} else {
			char* blockOptimo = seleccionarBlockParaCargarPosiciones(blocksOcupados,posicion, cantidad);
			agregarNuevaPosicionA(blockOptimo,posicion, cantidad);

		}

}

bool validarPosicionesDeCatch(char** blocksOcupados, char* posicion){
	if(existenPosicionesEnArchivo(posicion,blocksOcupados)){
		char* block = blockDondeSeEncuentraLaPosicion(posicion,blocksOcupados);
		char* rutaDelBlock = generadorDeRutaDeCreacionDeArchivos(rutaBlocks,block,".bin");
		t_config* archivoBlock = config_create(rutaDelBlock);

		uint32_t cantidadPokemon = cantidadDePokemonEnUnaCoordenada(archivoBlock,posicion);

		if(cantidadPokemon == 1){
			eliminarKeyValueDe(archivoBlock,posicion);
			return true;
		}else{
			decrementarEnUnoEnLaPosicion(archivoBlock,posicion);
			return true;
		}
		config_destroy(archivoBlock);
	} else {
		log_error(loggerGameCard, "No existen las posiciones en el archivo");
		return false;
	}
}



void validarPosicionesDeGet(rutaDeArchivo){

}





void newPokemon(int socketCliente,t_newLlegada* new){

	char* pokemon = new->pokemon;
	uint32_t posX = new->pos_X;
	uint32_t posY = new->pos_Y;
	uint32_t cantidad = new->cantidad;

	char* rutaDeDirectorio = generadorDeRutaDeCreacionDeDirectorios(rutaFiles,pokemon);
	char* posicion = generadorDePosiciones(posX,posY);

	if(noExisteDirectorio(rutaDeDirectorio)){
		crearDirectorio(pokemon,rutaFiles);
		crearArchivoEnDirectorio(pokemon,rutaDeDirectorio);
		crearTemplateDeArchivoTipo(METADATA_POKEMON,pokemon,rutaDeDirectorio);
	}

	char* rutaDeArchivo = generadorDeRutaDeCreacionDeArchivos(generadorDeRutaDeCreacionDeDirectorios(rutaFiles,pokemon), pokemon, ".bin");

	t_config* archivoMetadataPokemon = config_create(rutaDeArchivo);
	char** blocksOcupados = config_get_array_value(archivoMetadataPokemon,"BLOCKS");
	log_info(loggerDev, "EL array de blocks es: %s", blocksOcupados);

	if(puedeAbrirseArchivo(archivoMetadataPokemon)){
		activarFlagDeLectura(archivoMetadataPokemon);
		validarPosicionesDeNew(blocksOcupados,posicion,cantidad);
	} else{
		sleep(tiempoReintentoOperacion);
		newPokemon(socketCliente,new);
		exit(0);
	}

	t_appeared* appearedGenerado = crearAppeared(new);
	config_destroy(archivoMetadataPokemon);

	sleep(tiempoRetardoOperacion);

	enviar_appeared(socketCliente,appearedGenerado);


}

void catchPokemon(int socketCliente,t_catchLlegada* catch){

	uint32_t idMensaje = catch->id;
	char* pokemon = catch->pokemon;
	uint32_t posX = catch->pos_X;
	uint32_t posY = catch->pos_Y;
	bool validacion;

	char* rutaDeDirectorio = generadorDeRutaDeCreacionDeDirectorios(rutaFiles,pokemon);
	char* posicion = generadorDePosiciones(posX, posY);

	if(noExisteDirectorio(rutaDeDirectorio)){
		log_error(loggerGameCard, "No existe directorio dentro de Files para el pokemon: %s",pokemon);
		exit(-1);
	}


	char* rutaDeArchivo = generadorDeRutaDeCreacionDeArchivos(rutaDeDirectorio, pokemon, ".bin");
	t_config* archivoMetadataPokemon = config_create(rutaDeArchivo);
	char** blocksOcupados = config_get_array_value(archivoMetadataPokemon,"BLOCKS");
	log_info(loggerDev, "EL array de blocks es: %s", blocksOcupados);

	if(puedeAbrirseArchivo(archivoMetadataPokemon)){
		activarFlagDeLectura(archivoMetadataPokemon);
		validacion = validarPosicionesDeCatch(blocksOcupados,posicion);
	}else{
		sleep(tiempoReintentoOperacion);
		catchPokemon(socketCliente,catch);
		exit(0);
	}

	t_caught* caughtAEnviar = crearCaught(idMensaje,validacion);
	config_destroy(archivoMetadataPokemon);

	sleep(tiempoRetardoOperacion);

	enviar_caught(socketCliente,caughtAEnviar);
}

void getPokemon(int socketCliente,char* pokemon){

	char* rutaDeDirectorio = generadorDeRutaDeCreacionDeDirectorios(rutaFiles,pokemon);

	if(noExisteDirectorio(rutaDeDirectorio)){

		exit(0);
	}

	char* rutaDeArchivo = generadorDeRutaDeCreacionDeArchivos(rutaDeDirectorio, pokemon, ".bin");
	t_config* archivoMetadataPokemon = config_create(rutaDeArchivo);
	char** blocksOcupados = config_get_array_value(rutaDeArchivo,"BLOCKS");
	log_info(loggerDev, "EL array de blocks es: %s", blocksOcupados);

	if(puedeAbrirseArchivo(archivoMetadataPokemon)){
		activarFlagDeLectura(archivoMetadataPokemon);
		validarPosicionesDeGet(rutaDeArchivo);
	} else{
		sleep(tiempoReintentoOperacion);
		getPokemon(socketCliente,pokemon);
		exit(0);
	}

	config_destroy(archivoMetadataPokemon);
	sleep(tiempoRetardoOperacion);

}


void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
