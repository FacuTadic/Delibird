#include "archivosYDirectorios.h"



char* generadorDeRutaDeCreacionDeDirectorios(char* ruta, char* nombreDelDirectorio){
	char* rutaDeCreacionDirectorio = string_new();
	string_append(&rutaDeCreacionDirectorio, ruta);
	string_append(&rutaDeCreacionDirectorio, "/");
	string_append(&rutaDeCreacionDirectorio, nombreDelDirectorio);

	log_info(loggerDev, "La ruta final es: %s",rutaDeCreacionDirectorio);
	return rutaDeCreacionDirectorio;

}


char* generadorDeRutaDeCreacionDeArchivos(char* rutaDirectorio,char* nombreDelArchivo, char* tipoDeDato){

	char* rutaDeCreacion = string_new();
	string_append(&rutaDeCreacion, rutaDirectorio);
	string_append(&rutaDeCreacion, "/");
	string_append(&rutaDeCreacion, nombreDelArchivo);
	string_append(&rutaDeCreacion, tipoDeDato);

	log_info(loggerDev, "La ruta final es: %s",rutaDeCreacion);
	return rutaDeCreacion;
}


char* generadorDePosiciones(uint32_t posX, uint32_t posY){
	char* coordenadas= string_new();
	char* posXChar = string_itoa(posX);
	char* posYChar = string_itoa(posY);

	string_append(&coordenadas,posXChar);
	string_append(&coordenadas,"-");
	string_append(&coordenadas,posYChar);

	log_info(loggerDev, "La coordenada generada es: %s", coordenadas);
	return coordenadas;
}




bool noExisteDirectorio(char* ruta){
	return stat(ruta, &st1) == -1;
}

bool existeArchivo(char* ruta){
	return access(ruta, F_OK ) != -1;
}




uint32_t tamanioDeUnArchivo(char* rutaDelArchivo){
	FILE* archivo;
	archivo = fopen(rutaDelArchivo,"r");
	fseek(archivo, 0, SEEK_END);
	uint32_t tamanioArchivo = ftell(archivo);
	fseek(archivo, 0, SEEK_SET);
	fclose(archivo);
	log_info(loggerDev, "EL tamanio calculado del archivo es: %i",tamanioArchivo);
	return tamanioArchivo;
}



bool hayEspacioEnElBlock(char* rutaDelBlock, char* posicion, uint32_t cantidad){
	char* keyValue = string_new();
	string_append(&keyValue, posicion);
	string_append(&keyValue, "=");

	uint32_t tamanioBlock = tamanioDeUnArchivo(rutaDelBlock);
	uint32_t tamanioKeyValue = sizeof(keyValue);

	return tamanioBlock+tamanioKeyValue+sizeof(cantidad) < blockSize;
}




void crearDirectorio(char* directorio, char* puntoMontaje){
	char* rutaAbsoluta = generadorDeRutaDeCreacionDeDirectorios(puntoMontaje, directorio);

	if (noExisteDirectorio(rutaAbsoluta)) {
	    mkdir(rutaAbsoluta, 0700);
	    log_info(loggerDev, "Se creo el directorio: %s", directorio);
	}
}

void crearArchivoEnDirectorio(char* nombreDelArchivo, char* directorio){
	FILE *archivo;

	char* rutaDeCreacion = generadorDeRutaDeCreacionDeArchivos(directorio, nombreDelArchivo, ".bin");
	archivo = fopen(rutaDeCreacion,"w+");

	if(archivo == NULL){
	log_info(loggerDev, "No se pudo crear el archivo en el dicrectorio");
	exit(1);
	}
	fclose(archivo);
}


bool puedeAbrirseArchivo(t_config* archivo){
	log_info(loggerDev, "Se creo el falso config de la ruta");
	char* estadoArchivo = config_get_string_value(archivo, "OPEN");
	log_info(loggerDev, "Retorno el estado: %s", estadoArchivo);

	return !strcmp(estadoArchivo,"N");
}



void activarFlagDeLectura(t_config* archivo){
	config_set_value(archivo, "OPEN", "F");
	config_save(archivo);
}


void eliminarKeyValueDe(t_config* archivoBlock,char* posicion){
	config_remove_key(archivoBlock,posicion);
	config_save(archivoBlock);
}

void agregarCantidadSolicitadaAUnaKey(t_config* archivo,char* key, uint32_t cantidad){
	uint32_t cantidadVieja= config_get_int_value(archivo,key);
	log_info(loggerDev, "El size viejo del metadata es: %i", cantidadVieja);
	cantidad+= cantidadVieja;
	log_info(loggerDev, "El size nuevo del metadata es: %i", cantidad);
	char* cantidadTotal = string_itoa(cantidad);
	log_info(loggerDev, "La conversion de int->char es: %s", cantidadTotal);

	config_set_value(archivo,key, cantidadTotal);
	config_save(archivo);
}

void decrementarEnUnoEnLaPosicion(t_config* archivo,char* key){
	uint32_t cantidadVieja= config_get_int_value(archivo,key);
	cantidadVieja -= 1;
	log_info(loggerDev, "El size viejo del metadata es: %i", cantidadVieja);
	char* cantidadTotal = string_itoa(cantidadVieja);
	log_info(loggerDev, "La conversion de int->char es: %s", cantidadTotal);

	config_set_value(archivo,key, cantidadTotal);
	config_save(archivo);
}

uint32_t cantidadDePokemonEnUnaCoordenada(t_config* archivoBlock,char* posicion){
	return config_get_int_value(archivoBlock,posicion);
}


char* buscarPosicionDisponibleEnElBitMap(){

}



char* seleccionarBlockParaCargarPosiciones(char** blocksOcupados, char*posicion, uint32_t cantidad){
	if(blocksOcupados[0] == NULL){
		return buscarPosicionDisponibleEnElBitMap();
	}

	uint32_t i = 0;
		while(blocksOcupados[i] != NULL){
			log_info(loggerDev, "Se esta viendo su el el block %s cumple las condiciones",blocksOcupados[i]);
			char* rutaDeArchivo = generadorDeRutaDeCreacionDeArchivos(rutaBlocks,blocksOcupados[i],".bin");
			if(hayEspacioEnElBlock(rutaDeArchivo,posicion,cantidad)){
				return blocksOcupados[i];
			} else{
				i++;
			}
		}
}

void agregarNuevaPosicionA(char* block, char* posicion, uint32_t cantidad){

	char* rutaArchivo = generadorDeRutaDeCreacionDeArchivos(rutaBlocks,block,".bin");
	t_config* archivoBlock = config_create(rutaArchivo);

	char* cantidadString = string_itoa(cantidad);
	config_set_value(archivoBlock,posicion,cantidadString);
	config_save(archivoBlock);
	config_destroy(archivoBlock);
}


