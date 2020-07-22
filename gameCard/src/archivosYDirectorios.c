#include "archivosYDirectorios.h"



char* generadorDeRutaDeCreacionDeDirectorios(char* ruta, char* nombreDelDirectorio){
	char* rutaDeCreacionDirectorio = string_new();
	string_append(&rutaDeCreacionDirectorio, ruta);
	string_append(&rutaDeCreacionDirectorio, "/");
	string_append(&rutaDeCreacionDirectorio, nombreDelDirectorio);


	return rutaDeCreacionDirectorio;

}


char* generadorDeRutaDeCreacionDeArchivos(char* rutaDirectorio,char* nombreDelArchivo, char* tipoDeDato){

	char* rutaDeCreacion = string_new();
	string_append(&rutaDeCreacion, rutaDirectorio);
	string_append(&rutaDeCreacion, "/");
	string_append(&rutaDeCreacion, nombreDelArchivo);
	string_append(&rutaDeCreacion, tipoDeDato);

	return rutaDeCreacion;
}


char* generadorDePosiciones(uint32_t posX, uint32_t posY){
	char* coordenadas= string_new();
	char* posXChar = string_itoa(posX);
	char* posYChar = string_itoa(posY);

	string_append(&coordenadas,posXChar);
	string_append(&coordenadas,"-");
	string_append(&coordenadas,posYChar);

	log_info(loggerDevArchDir, "La coordenada generada es: %s", coordenadas);
	return coordenadas;
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
	log_info(loggerDevArchDir, "EL tamanio calculado del archivo es: %i",tamanioArchivo);
	return tamanioArchivo;
}



bool hayEspacioEnElBlock(char* rutaDelBlock, char* posicion, uint32_t cantidad){
	char* keyValue = string_new();
	char* value = string_itoa(cantidad);

	string_append(&keyValue, posicion);
	string_append(&keyValue, "=");
	string_append(&keyValue, value);

	uint32_t tamanioBlock = tamanioDeUnArchivo(rutaDelBlock);
	log_info(loggerGameCardArchDir, "El archivo block pesa", tamanioBlock);
	uint32_t tamanioKeyValue = sizeof(keyValue);
	log_info(loggerGameCardArchDir, "El key value para agregar pesa", tamanioKeyValue);

	if(tamanioBlock+tamanioKeyValue <= blockSizeArchDir){
		log_info(loggerGameCardArchDir, "Puede cargarse en el bloque");
		return true;
	}else{
		log_info(loggerGameCardArchDir, "No puede cargarse en el bloque");
		return false;
	}

}




void crearDirectorio(char* directorio, char* puntoMontaje){
	char* rutaAbsoluta = generadorDeRutaDeCreacionDeDirectorios(puntoMontaje, directorio);

	if (noExisteDirectorio(rutaAbsoluta)) {
	    mkdir(rutaAbsoluta, 0700);
	    log_info(loggerDevArchDir, "Se creo el directorio: %s", directorio);
	}else{
		log_info(loggerDevArchDir, "Ya existe el directorioo: %s", rutaAbsoluta);
	}
}

void crearArchivoEnDirectorio(char* nombreDelArchivo, char* directorio){
	FILE *archivo;

	char* rutaDeCreacion = generadorDeRutaDeCreacionDeArchivos(directorio, nombreDelArchivo, ".bin");
	archivo = fopen(rutaDeCreacion,"w+");

	if(archivo == NULL){
	log_info(loggerDevArchDir, "No se pudo crear el archivo en el dicrectorio");
	exit(1);
	}
	fclose(archivo);
}


bool puedeAbrirseArchivo(t_config* archivo){
	log_info(loggerDevArchDir, "Se creo el falso config de la ruta");
	char* estadoArchivo = config_get_string_value(archivo, "OPEN");
	log_info(loggerDevArchDir, "Retorno el estado: %s", estadoArchivo);

	return !strcmp(estadoArchivo,"N");
}



void activarFlagDeLectura(t_config* archivo){
	config_set_value(archivo, "OPEN", "Y");
	config_save(archivo);
}

void desactivarFlagDeLectura(t_config* archivo){
	config_set_value(archivo, "OPEN", "N");
	config_save(archivo);
}


char** cargarVectorSinElValor(char** arrayViejo,char* block){
	char** nuevoArrayDeBlock;
	uint32_t i = 0;
	char* nada = string_new();


	while(arrayViejo[i] != NULL){
			if(strcasecmp(arrayViejo[i],block)){
				log_info(loggerDevArchDir, "Se asigno el block %s a la posicion %i",arrayViejo[i],i);

				string_append(&nada,arrayViejo[i]);
				string_append(&nada,"|");
				i++;
			} else {
				log_info(loggerDevArchDir, "Se macheo el block que hay que sacar");
				i++;
			}
		}

	char* nadaNueva = string_substring(nada,0,string_length(nada)-1);
	free(nada);

	nuevoArrayDeBlock = string_split(nadaNueva,"|");

	free(nadaNueva);

	return nuevoArrayDeBlock;
}


void borrarBloqueDe(t_config* archivoMetaData, char*block){
	char** blocksOcupados = config_get_array_value(archivoMetaData,"BLOCKS");

	char** nuevoArrayDeBlock = cargarVectorSinElValor(blocksOcupados,block);

	//Rutina de char** a char*

	char* arrayPlano = string_new();
	string_append(&arrayPlano,"[");
	uint32_t indexArray = 0;

	while(nuevoArrayDeBlock[indexArray] != NULL){

		if(nuevoArrayDeBlock[indexArray+1]==NULL){
			string_append(&arrayPlano,nuevoArrayDeBlock[indexArray]);
			string_append(&arrayPlano,"]");
		}

		string_append(&arrayPlano,nuevoArrayDeBlock[indexArray]);
		string_append(&arrayPlano,",");
	}


	config_set_value(archivoMetaData,"BLOCKS",arrayPlano);

	log_info(loggerDevArchDir, "Se seteo el valor del nuevo array en el metadata del pokemon");

	config_save(archivoMetaData);

}


void eliminarKeyValueDe(t_config* archivoBlock,char* posicion){
	config_remove_key(archivoBlock,posicion);
	config_save(archivoBlock);
}

void agregarCantidadSolicitadaAUnaKey(t_config* archivo,char* key, uint32_t cantidad){
	log_info(loggerGameCardArchDir, "Agregando %i pokemons a la posicion %i", key);

	uint32_t cantidadVieja= config_get_int_value(archivo,key);
	log_info(loggerDevArchDir, "La cantidad vieja de la posicion %s dentro del metadata es de %i", key, cantidadVieja);
	log_info(loggerGameCardArchDir, "La cantidad vieja de la posicion %s dentro del metadata es de %i", key, cantidadVieja);

	cantidad+= cantidadVieja;
	log_info(loggerDevArchDir, "La cantidad nueva de la posicion %s dentro del metadata es de %i", key, cantidad);
	log_info(loggerGameCardArchDir, "La cantidad nueva de la posicion %s dentro del metadata es de %i", key, cantidad);

	char* cantidadTotal = string_itoa(cantidad);
	log_info(loggerDevArchDir, "La conversion de int->char es: %s", cantidadTotal);

	config_set_value(archivo,key, cantidadTotal);
	config_save(archivo);
}

void decrementarEnUnoEnLaPosicion(t_config* archivo,char* key){

	uint32_t cantidadVieja= config_get_int_value(archivo,key);
	log_info(loggerDevArchDir, "La cantidad vieja en la posicion %s en el metadata es de: %i", key, cantidadVieja);
	log_info(loggerGameCardArchDir, "La cantidad vieja en la posicion %s en el metadata es de: %i", key, cantidadVieja);
	cantidadVieja -= 1;
	log_info(loggerDevArchDir, "La cantidad nueva en la posicion %s en el metadata es de: %i", key, cantidadVieja);
	log_info(loggerGameCardArchDir, "La cantidad nueva en la posicion %s en el metadata es de: %i", key, cantidadVieja);
	char* cantidadTotal = string_itoa(cantidadVieja);
	log_info(loggerDevArchDir, "La conversion de int->char es: %s", cantidadTotal);

	config_set_value(archivo,key, cantidadTotal);
	config_save(archivo);
}

uint32_t cantidadDePokemonEnUnaCoordenada(t_config* archivoBlock,char* posicion){
	return config_get_int_value(archivoBlock,posicion);
}




void agregarNuevaPosicionA(char* block, char* posicion, uint32_t cantidad){

	char* rutaArchivo = generadorDeRutaDeCreacionDeArchivos(rutaBlocksArchDir,block,".bin");
	t_config* archivoBlock = config_create(rutaArchivo);

	char* cantidadString = string_itoa(cantidad);
	config_set_value(archivoBlock,posicion,cantidadString);
	config_save(archivoBlock);
	config_destroy(archivoBlock);
}






void obtenerTodasLasPosiciones(char** blocks, t_queue* posicionesPokemon){

	uint32_t i = 0;

	while(blocks[i] != NULL){

			log_info(loggerDevArchDir, "Se esta analizando el block: %s",blocks[i]);
			char* rutaDeArchivo = generadorDeRutaDeCreacionDeArchivos(rutaBlocksArchDir,blocks[i],".txt");

			FILE *fp;
			char *line = NULL;
			size_t len = 0;
			ssize_t read;

			fp = fopen(rutaDeArchivo, "r");

			if (fp == NULL){
				log_error(loggerDevArchDir, "Archivo vacio");
				exit(EXIT_FAILURE);
			}

			while ((read = getline(&line, &len, fp)) != -1) {
				log_info(loggerDevArchDir, "Se esta leyendo la linea: %s",line);
				char** data = string_split(line,"=");
				log_info(loggerDevArchDir, "La coordenada de la linea es: %s",data[0]);
				char* posicion = string_new();
				string_append(&posicion,data[0]);
				log_info(loggerGameCardArchDir, "Posicion: %s",posicion);
				queue_push(posicionesPokemon, posicion);
				//free(data);
				//free(posicion);
			}

			free(line);
			fclose(fp);

			i++;
		}
}


