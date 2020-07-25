#include "archivosYDirectorios.h"

void crearTemplateDeArchivoTipo(tipoArchivo tipo, char* nombreDelArchivo ,char* ruta){
	FILE *archivo;

	char* rutaDeCreacion = generadorDeRutaDeCreacionDeArchivos(ruta, nombreDelArchivo, ".bin");

	archivo = fopen(rutaDeCreacion,"w+");
	t_config* archivoMetadata = config_create(rutaDeCreacion);

	switch(tipo){
	case METADATA:
		log_info(loggerDevArchDir, "Se intento entrar por METADATA");
		break;

	case BITMAP:
		break;

	case METADATA_FILE:

		config_set_value(archivoMetadata,"DIRECTORY","Y");
		config_save(archivoMetadata);

//		fprintf(archivo	,"DIRECTORY" "=" "Y" "\n");
//		fprintf(archivo	,"OPEN" "=" "N" "\n");

		break;

	case METADATA_POKEMON:

		config_set_value(archivoMetadata,"OPEN","N");
		config_set_value(archivoMetadata,"DIRECTORY","N");
		config_set_value(archivoMetadata,"BLOCKS","[]");
		config_set_value(archivoMetadata,"SIZE","0");

		config_save(archivoMetadata);

//		fprintf(archivo	,"OPEN" "=" "N \n");
//		fprintf(archivo	,"DIRECTORY" "=" "N \n");
//		fprintf(archivo	,"SIZE" "=" " \n");
//		fprintf(archivo	,"BLOCKS" "=" " \n");

		break;

	case BLOCK:
		break;

	default:
		log_info(loggerDevArchDir, "No se pudo reconocer el tipo de Archivo");
		break;
	}

	config_destroy(archivoMetadata);
	fclose(archivo);

	free(rutaDeCreacion);
}




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
	free(posXChar);
	free(posYChar);
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


bool existeBlockEnMetadata(char** blocksActuales, char* block){
	uint32_t indexBlock=0;

	while(blocksActuales[indexBlock] != NULL){
		log_info(loggerDevArchDir, "Comparo el block: %s con el block: %s",blocksActuales[indexBlock],block);
		if(string_equals_ignore_case(blocksActuales[indexBlock],block)){
			log_info(loggerDevArchDir, "Igual");
			return true;
		}
		indexBlock++;
	}

	log_info(loggerDevArchDir, "No pertenece a los blocks actuales");

	return false;
}


void actualizarBlockMetadata(t_config* archivoMetadata, char*blockAAgregar){
	char** blocksActuales = config_get_array_value(archivoMetadata,"BLOCKS");
	char* listaNueva = string_new();
	uint32_t indexBlock=0;

	if(blocksActuales[0] == NULL){
		log_info(loggerDevArchDir, "Entro por NULL");
		string_append(&listaNueva,"[");
		string_append(&listaNueva,blockAAgregar);
		string_append(&listaNueva,"]");

	} else {
		log_info(loggerDevArchDir, "Entro por distinto de NULL");

		if(existeBlockEnMetadata(blocksActuales,blockAAgregar)){
			log_info(loggerDevArchDir, "El bloque ya pertenece a la lista. No se agrega nada");
			listaNueva = cargarArrayEnChar(blocksActuales);

		} else {

			log_info(loggerDevArchDir, "El bloque no existe en la lista actual");

			string_append(&listaNueva,"[");

			while(blocksActuales[indexBlock] != NULL){
				string_append(&listaNueva,blocksActuales[indexBlock]);
				string_append(&listaNueva,",");
				indexBlock++;
			}
			string_append(&listaNueva,blockAAgregar);
			string_append(&listaNueva,"]");
		}

	}

	config_set_value(archivoMetadata,"BLOCKS",listaNueva);
	config_save(archivoMetadata);

	free(listaNueva);

	uint32_t indexParaBorrar = 0;
	while(blocksActuales[indexParaBorrar] != NULL){
		free(blocksActuales[indexParaBorrar]);
		indexParaBorrar++;
	}

	free(blocksActuales);

}


void actualizarSizeMetadataPokemon(t_config* archivoMetadata){
	char** blocksOcupados = config_get_array_value(archivoMetadata,"BLOCKS");
	uint32_t i = 0;
	uint32_t cantidad = 0;

	while(blocksOcupados[i] != NULL){
		char* rutaDelBlock = generadorDeRutaDeCreacionDeArchivos(rutaBlocksArchDir,blocksOcupados[i],".bin");
		uint32_t peso = tamanioDeUnArchivo(rutaDelBlock);
		log_info(loggerGameCardArchDir, "El archivo %s pesa: %i",blocksOcupados[i], peso);
		cantidad += peso;
		i++;
		free(rutaDelBlock);
	}

	char* posicionEnChar = string_itoa(cantidad);

	config_set_value(archivoMetadata,"SIZE",posicionEnChar);
	config_save(archivoMetadata);

	log_info(loggerGameCardArchDir, "El peso total de los blocks es de: %s", posicionEnChar);

	free(posicionEnChar);

	uint32_t indexParaBorrar = 0;
	while(blocksOcupados[indexParaBorrar] != NULL){
		free(blocksOcupados[indexParaBorrar]);
		indexParaBorrar++;
	}

	free(blocksOcupados);
}



bool hayEspacioEnElBlock(char* rutaDelBlock, char* posicion, uint32_t cantidad){
	char* keyValue = string_new();
	char* value = string_itoa(cantidad);

	string_append(&keyValue, posicion);
	string_append(&keyValue, "=");
	string_append(&keyValue, value);

	uint32_t tamanioBlock = tamanioDeUnArchivo(rutaDelBlock);
	log_info(loggerGameCardArchDir, "El archivo block pesa: %i", tamanioBlock);

	uint32_t tamanioKeyValue = sizeof(keyValue);
	log_info(loggerGameCardArchDir, "El KEY-VALUE para agregar pesa: %i", tamanioKeyValue);

	if(tamanioBlock+tamanioKeyValue <= blockSizeArchDir){
		log_info(loggerGameCardArchDir, "Puede cargarse en el bloque");
		free(keyValue);
		free(value);
		return true;
	}else{
		log_info(loggerGameCardArchDir, "No puede cargarse en el bloque");
		free(keyValue);
		free(value);
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

	free(rutaAbsoluta);
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
	free(rutaDeCreacion);
}


bool puedeAbrirseArchivo(t_config* archivo){
	log_info(loggerDevArchDir, "Se creo el falso config de la ruta");
	char* estadoArchivo = config_get_string_value(archivo, "OPEN");
	log_info(loggerDevArchDir, "Retorno el estado: %s", estadoArchivo);

	return !strcmp(estadoArchivo,"N");
}



void activarFlagDeLectura(t_config* archivo){
	config_set_value(archivo, "OPEN", "Y");
	log_info(loggerDevArchDir, "Se creo el falso config de la ruta");
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


char* cargarArrayEnChar(char** arrayViejo){
	char* arrayPlano = string_new();

	string_append(&arrayPlano,"[");
	uint32_t indexArray = 0;

	while(arrayViejo[indexArray+1] != NULL){
		string_append(&arrayPlano,arrayViejo[indexArray]);
		string_append(&arrayPlano,",");
		indexArray++;
	}

	string_append(&arrayPlano,arrayViejo[indexArray]);
	string_append(&arrayPlano,"]");


	return arrayPlano;
}


void borrarBloqueDe(t_config* archivoMetaData, char*block){
	char** blocksOcupados = config_get_array_value(archivoMetaData,"BLOCKS");
	uint32_t i= 0;

	if(blocksOcupados[i+1] == NULL){
		log_info(loggerDevArchDir, "No hay mas bloques");
		config_set_value(archivoMetaData,"BLOCKS","[]");
		config_save(archivoMetaData);

		while(blocksOcupados[i] != NULL){
			free(blocksOcupados[i]);
			i++;
		}
		free(blocksOcupados);

		return;
	}


	char** nuevoArrayDeBlock = cargarVectorSinElValor(blocksOcupados,block);

	char* arrayPlano = cargarArrayEnChar(nuevoArrayDeBlock);

	config_set_value(archivoMetaData,"BLOCKS",arrayPlano);

	log_info(loggerDevArchDir, "Se seteo el valor del nuevo array en el metadata del pokemon");

	config_save(archivoMetaData);

	uint32_t j =0;
	while(nuevoArrayDeBlock[j] != NULL){
			free(nuevoArrayDeBlock[j]);
			j++;
		}
	free(nuevoArrayDeBlock);
	free(arrayPlano);

}


void eliminarKeyValueDe(t_config* archivoBlock,char* posicion){
	config_remove_key(archivoBlock,posicion);
	config_save(archivoBlock);
}

void agregarCantidadSolicitadaAUnaKey(t_config* archivo,char* key, uint32_t cantidad){
	log_info(loggerGameCardArchDir, "Agregando %i pokemons a la posicion %s", cantidad, key);

	uint32_t cantidadVieja= config_get_int_value(archivo,key);
	log_info(loggerDevArchDir, "La cantidad vieja de la posicion %s dentro del metadata es de %i", key, cantidadVieja);
	log_info(loggerGameCardArchDir, "La cantidad vieja de la posicion %s dentro del metadata es de %i", key, cantidadVieja);

	cantidad+= cantidadVieja;
	log_info(loggerDevArchDir, "La cantidad nueva de la posicion %s dentro del metadata es de %i", key, cantidad);
	log_info(loggerGameCardArchDir, "La cantidad nueva de la posicion %s dentro del metadata es de %i", key, cantidad);

	char* cantidadTotal = string_itoa(cantidad);

	config_set_value(archivo,key, cantidadTotal);
	config_save(archivo);

	free(cantidadTotal);
}

void decrementarEnUnoEnLaPosicion(t_config* archivo,char* key){

	uint32_t cantidadVieja= config_get_int_value(archivo,key);
	log_info(loggerDevArchDir, "La cantidad vieja en la posicion %s en el metadata es de: %i", key, cantidadVieja);
	log_info(loggerGameCardArchDir, "La cantidad vieja en la posicion %s en el metadata es de: %i", key, cantidadVieja);
	cantidadVieja -= 1;
	log_info(loggerDevArchDir, "La cantidad nueva en la posicion %s en el metadata es de: %i", key, cantidadVieja);
	log_info(loggerGameCardArchDir, "La cantidad nueva en la posicion %s en el metadata es de: %i", key, cantidadVieja);
	char* cantidadTotal = string_itoa(cantidadVieja);


	config_set_value(archivo,key, cantidadTotal);
	config_save(archivo);

	free(cantidadTotal);
}

uint32_t cantidadDePokemonEnUnaCoordenada(t_config* archivoBlock,char* posicion){
	return config_get_int_value(archivoBlock,posicion);
}


void agregarNuevaPosicionA(char* block, char* posicion, uint32_t cantidad){
	char* rutaArchivo = generadorDeRutaDeCreacionDeArchivos(rutaBlocksArchDir,block,".bin");
	log_info(loggerDevArchDir, "Agregamos nueva posicion con direccion del archivo: %s",rutaArchivo);

	if(noExisteDirectorio(rutaArchivo)){
		log_info(loggerDevArchDir, "Creamos el archivo");
		crearTemplateDeArchivoTipo(BLOCK,block,rutaBlocksArchDir);
	}

	log_info(loggerDevArchDir, "Seteando nueva posicion: %s",posicion);

	t_config* archivoBlock = config_create(rutaArchivo);
	uint32_t cantidadPos = config_keys_amount(archivoBlock);

	log_info(loggerDevArchDir, "Actualmente hay %i de posiciones en el archivo",cantidadPos);

	char* cantidadString = string_itoa(cantidad);
	config_set_value(archivoBlock,posicion,cantidadString);
	config_save(archivoBlock);

	log_info(loggerDevArchDir, "ACTUALIZACION : Actualmente hay %i de posiciones en el archivo",cantidadPos);
	config_destroy(archivoBlock);

	free(rutaArchivo);
	free(cantidadString);
}


void obtenerTodasLasPosiciones(char** blocks, t_queue* posicionesPokemon){

	uint32_t i = 0;

	while(blocks[i] != NULL){

			log_info(loggerDevArchDir, "Se esta analizando el block: %s",blocks[i]);
			char* rutaDeArchivo = generadorDeRutaDeCreacionDeArchivos(rutaBlocksArchDir,blocks[i],".bin");
			log_info(loggerDevArchDir, "La ruta del archivo es: %s",rutaDeArchivo);

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

				uint32_t j = 0;
				while(data[j] != NULL){
					free(data[j]);
					j++;
				}
				free(data);
			}

			free(rutaDeArchivo);
			free(line);
			fclose(fp);

			i++;
		}
}



bool noExisteDirectorio(char* ruta){
	return stat(ruta, &st1) == -1;
}
