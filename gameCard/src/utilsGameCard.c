#include "utilsGameCard.h"
#include "protocoloGameCard.h"

#include<string.h>
#include <commons/log.h>


int crear_conexion(char *ip, char* puerto){
	struct addrinfo hints;
	struct addrinfo *server_info;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(ip, puerto, &hints, &server_info);

	int socket_cliente = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol);

	if (connect(socket_cliente, server_info->ai_addr, server_info->ai_addrlen) == -1) {
		liberar_conexion(socket_cliente);
		freeaddrinfo(server_info);
		return -1;
	}

	freeaddrinfo(server_info);

	return socket_cliente;
}



//#######################################################################			BITARRAY			###########################################################################

void setearBitarray() {
	uint32_t i = 0;
	char c;

	FILE *fp;
	fp = fopen(bitMap, "r+b");
	if (fp == NULL) {
		log_error(loggerDev, "Hubo un error abriendo el BITMAP");
		return;
	}

	fseek(fp, 0L, SEEK_END);
	uint32_t file_size = ftell(fp);

	if (file_size == 0) {	//Bitmap vacio
		fseek(fp, 0L, SEEK_SET);
		for (i = 0; i < cantidadDeBloques; i++) {
			bitarray_clean_bit(bitarray, i);
			fprintf(fp, "%d", bitarray_test_bit(bitarray, i));
		}
		//crearTodosLosBloquesFS(cantidadDeBloques);	TENGO QUE CREAR TODOS LOS BLOQUES CUANDO LEVANTO EL FS?
	} else {
		log_info(loggerDev,"Bitmap ya seteado");
		fseek(fp, 0L, SEEK_SET);
		while (!feof(fp)) {
			c = fgetc(fp);
			if (!feof(fp)) {
				if (atoi(&c) == 1) {
					bitarray_set_bit(bitarray, i);
				} else {
					bitarray_clean_bit(bitarray, i);
				}
				i++;
			}
		}
	}
	fclose(fp);
}


void guardarBitarray(uint32_t index) {

	log_info(loggerDev, "Guardando el bitarray");

	FILE *fp = fopen(bitMap, "r+b");
	fp->_offset = index;
	fseek(fp, 0l, SEEK_CUR);
	fprintf(fp, "%i", bitarray_test_bit(bitarray, index));
	fclose(fp);

	log_info(loggerDev, "Termino el guardado");
}


void crearBitarray() {
	char *data;
	data = malloc(cantidadDeBloques);
	memset(data, '0', cantidadDeBloques);

	bitarray = bitarray_create_with_mode(data, cantidadDeBloques, LSB_FIRST);
	setearBitarray(cantidadDeBloques);
}


char* buscarPosicionDisponibleEnElBitMap() {
	int free_block=0;
	int i = 0;
	int flag_free_block = 0;

	log_info(loggerDev, "Entro a buscar posiciones");

	sem_wait(&bloques_bitmap);
	log_info(loggerDev, "Bloqueo");

	log_info(loggerDev, "EL valor de i es de : %i",i);
	log_info(loggerDev, "EL valor de cant de bloques es de : %i",cantidadDeBloques);

	for (i = 0; i < cantidadDeBloques; i++) {
		log_info(loggerDev, "EL valor de i es de : %i",i);
		log_info(loggerDev, "EL valor de cant de bloques es de : %i",cantidadDeBloques);


		if (flag_free_block == 0) {
			log_info(loggerDev, "Entro primer IF");

			if (bitarray_test_bit(bitarray, i) == 0) {
				log_info(loggerDev, "Entro segundo IF");
				flag_free_block = 1;
				free_block = i;
				bitarray_set_bit(bitarray, i);
				guardarBitarray(i);
				sem_post(&bloques_bitmap);
				char* block = string_itoa(free_block);
				log_info(loggerDev, "EL block %s esta libre",block);
				//log_info(loggerGameCard, "El bloque seleccionado para cargar es: %i",free_block);
				log_info(loggerDev, "EL block %s esta libre, vuelvo a chequear",block);
				return block;
			}
		}

		log_info(loggerDev, "EL valor FINAL de i es de : %i",i);
		log_info(loggerDev, "EL valor FINAL de cant de bloques es de : %i",cantidadDeBloques);



	}
	sem_post(&bloques_bitmap);

	log_error(loggerDev, "No hay bloques libres diponibles");

	return NULL;
}



void desmarcarBloqueBitmap(char* block){
	uint32_t index = atoi(block);
	uint32_t estadoBloque =bitarray_test_bit(bitarray, index);
	log_info(loggerDev, "El valor del block %s es de: %i",block,estadoBloque);
	bitarray_clean_bit(bitarray, index);
	estadoBloque =bitarray_test_bit(bitarray, index);
	log_info(loggerDev, "El block %s fue desmarcado con el valor de: %i",block, estadoBloque);
	guardarBitarray(index);
}



char* seleccionarBlockParaCargarPosiciones(char** blocksOcupados, char*posicion, uint32_t cantidad){
	if(blocksOcupados[0] == NULL){
		log_info(loggerGameCard, "El pokemon actual no tiene ningun bloque");
		log_info(loggerGameCard, "Se busca en el bitmap bloques disponibles");
		return buscarPosicionDisponibleEnElBitMap();
	}

	uint32_t i = 0;
	while(blocksOcupados[i] != NULL){
		log_info(loggerDevArchDir, "Se esta viendo su el el block %s cumple las condiciones",blocksOcupados[i]);
		char* rutaDeArchivo = generadorDeRutaDeCreacionDeArchivos(rutaBlocksArchDir,blocksOcupados[i],".bin");

		log_info(loggerGameCard, "Calculando el peso de: %s",blocksOcupados[i]);
		if(hayEspacioEnElBlock(rutaDeArchivo,posicion,cantidad)){
			log_info(loggerDev, "Se encontro el block %s libre",blocksOcupados[i]);
			log_info(loggerGameCard, "El bloque seleccionado para cargar es: %s",blocksOcupados[i]);
			free(rutaDeArchivo);
			return blocksOcupados[i];
		} else{
			i++;
		}
	}

	log_info(loggerGameCard, "Ningun bloque actual puede cargar la posicion");
	log_info(loggerGameCard, "Buscando nuevo bloque");

	return buscarPosicionDisponibleEnElBitMap();
}



//############################################################################ SE PRENDE FUEGO EL FS ######################################################################################

// recibe array de strings numeros y devuelve array de paths bloques
char** generarVectorDePaths(char** blocksOcupados){

	log_info(loggerDev, "Generando los paths de los bloques");

	uint32_t indexBlocks = 0;
	char* pathsDeBlocks = string_new();


	if(blocksOcupados[indexBlocks + 1] == NULL){
		log_info(loggerDev, "Unico bloque");

		log_info(loggerDev, "Se genera la ruta para: %s", blocksOcupados[indexBlocks]);
		char* rutaDelUltimoBlock = generadorDeRutaDeCreacionDeArchivos(rutaBlocks,blocksOcupados[indexBlocks],".bin");
		log_info(loggerDev, "El path del block es: %s", rutaDelUltimoBlock);
		string_append(&pathsDeBlocks,rutaDelUltimoBlock);
		string_append(&pathsDeBlocks,",");

		char** blockUnico = string_split(pathsDeBlocks,",");

		free(rutaDelUltimoBlock);
		free(pathsDeBlocks);

		log_info(loggerDev, "La posicion 0 del array de path es: %s", blockUnico[0]);
		return blockUnico;
	}


	while(blocksOcupados[indexBlocks] != NULL){

		if(blocksOcupados[indexBlocks+1] == NULL){
			log_info(loggerDev, "Se genera la ruta para: %s", blocksOcupados[indexBlocks]);
			char* rutaDelUltimoBlock = generadorDeRutaDeCreacionDeArchivos(rutaBlocks,blocksOcupados[indexBlocks],".bin");
			log_info(loggerDev, "El path del block es: %s", rutaDelUltimoBlock);
			string_append(&pathsDeBlocks,rutaDelUltimoBlock);
			free(rutaDelUltimoBlock);
			log_info(loggerDev, "Ultimo bloque");
		} else {
			log_info(loggerDev, "Se genera la ruta para: %s", blocksOcupados[indexBlocks]);
			char* rutaDelBlock = generadorDeRutaDeCreacionDeArchivos(rutaBlocks,blocksOcupados[indexBlocks],".bin");
			log_info(loggerDev, "El path del block es: %s", rutaDelBlock);
			string_append(&pathsDeBlocks,rutaDelBlock);
			string_append(&pathsDeBlocks,",");

			free(rutaDelBlock);
		}

		indexBlocks++;
	}

	log_info(loggerDev, "Devolviendo el char**");
	return string_n_split(pathsDeBlocks,indexBlocks,",");
}


void string_append_char(char** string, char c){
	char str[2];
	str[0] = c;
	str[1]='\0';
	string_append(string,str);
}

// conversor de char* registro a array [pos,cant]
char** generarVectorDeKeyValue(char* linea){
	return string_split(linea,"=");
}
// conversor de array [pos,cant] a char* registro
char* aplanarVectorDeKeyValue(char** vector){
	char* registro = string_new();
	string_append(&registro, vector[0]);
	string_append(&registro, "=");
	string_append(&registro, vector[1]);
	string_append(&registro, "\n");
	log_info(loggerDev, "El registro aplanado quedo como: %s", registro);
	return registro;
}

char* getCoordenadaDeRegistro(char** vector){
	char* coordenadaSaca = string_new();
	string_append(&coordenadaSaca,vector[0]);
	return coordenadaSaca;
}

// recibe array de paths bloques y devuelve lista de registros [pos,cant]
t_list* traerRegistrosBloques(char** bloques){

	//Creo la lista de registros (coordenadas+cantidad) donde voy a cargar mis char** con ambos valores
	t_list* registros = list_create();
	char* registroActual = string_new();

	//Lo pongo adentro porque cuando lo saco de la funcion hay un puntero que se pierde
	void leerBloque(char* nombreBloque){
		FILE* archivoDeBloque = fopen(nombreBloque,"r");
		//Leo el primer caracter
		char nuevoCaracter = fgetc(archivoDeBloque);
		//Mientras no sea fin de archivo
		while(nuevoCaracter != EOF){
			//Si es Nueva Linea (ASCII) y no esta vacio
			if((nuevoCaracter == 10) && !string_is_empty(registroActual)){
				//Agrego el valor a la lista
				list_add(registros, generarVectorDeKeyValue(registroActual));
				free(registroActual);
				registroActual = string_new();
			}else{
				string_append_char(&registroActual,nuevoCaracter);
			}
			nuevoCaracter = fgetc(archivoDeBloque);
		}
		fclose(archivoDeBloque);
	}

	string_iterate_lines(bloques, (void*) leerBloque);

	// Esto aplica si la ultima linea del ultimo bloque no termina en \n
	if (!string_is_empty(registroActual) && (strcmp(registroActual,"\n") != 0)){ //TODO: Marca entraba como \n
		list_add(registros, generarVectorDeKeyValue(registroActual));
		free(registroActual);
		registroActual = string_new();
	}

	free(registroActual);
	return registros;
}


// recibe registro[pos,cant] y lista de registros [pos,cant], devuelve lista de registros [pos,cant]
//Mi char** registro es -> [POSICION,CANTIDAD]
//Hacerla void y que modifique la lista simplemente
t_list* evaluarRegistroNew(char** registro, t_list* registros){

	for(uint32_t i=0;i < list_size(registros);i++){
		char** registroActual = list_get(registros,i);
		if(!strcmp(registro[0],registroActual[0])){
			uint32_t sumaTotal = 0;
			log_info(loggerGameCard, "La coordenada %s fue encontrada",registro[0]);

			sumaTotal += atoi(registroActual[1]);
			log_info(loggerDev, "La vieja cantidad es: %i",sumaTotal);

			log_info(loggerGameCard, "La coordenada %s tenia %s pokemons en dicha localizacion",registroActual[0],registroActual[1]);

			sumaTotal += atoi(registro[1]);
			log_info(loggerDev, "La nueva cantidad es: %i",sumaTotal);

			log_info(loggerGameCard, "La coordenada %s ahora tiene %i pokemons en dicha localizacion",registroActual[0],sumaTotal);

			free(registroActual[1]);
			registroActual[1] = string_itoa(sumaTotal);
			//POLEMICO ACA BROOOOO CHEQUEA ESE REGISTRO ACTUAL SI HAY QUE METERLO EN LA LIST CON UN REPLACE
			return registros;
		}
	}


	log_info(loggerDev, "La coordenada %s es nueva. Se agrega al los registros",registro[0]);
	log_info(loggerGameCard, "La coordenada %s es nueva. Se agrega al los registros",registro[0]);
	list_add(registros,registro);
	return registros;
}


bool evaluarRegistroCatch(char* registro, t_list* registros){

	for(uint32_t i=0;i < list_size(registros);i++){
		char** registroActual = list_get(registros,i);
		uint32_t cantidadReg = atoi(registroActual[1]);
		if(!strcmp(registro,registroActual[0])){
			log_info(loggerGameCard, "La coordenada %s fue encontrada",registro);

			if( cantidadReg > 1){
				uint32_t cantActualizada = atoi(registroActual[1]);
				log_info(loggerGameCard, "La coordenada %s tenia %s pokemons en dicha localizacion",registroActual[0],registroActual[1]);

				cantActualizada --;
				log_info(loggerGameCard, "La coordenada %s ahora tiene %i pokemons en dicha localizacion",registroActual[0],cantActualizada);

				free(registroActual[1]);
				registroActual[1] = string_itoa(cantActualizada);
			} else {
				log_info(loggerGameCard, "La coordenada %s tiene unicamente %s posicion", registro,registroActual[1]);
				log_info(loggerGameCard, "La coordenada %s se borra", registro);
				list_remove_and_destroy_element(registros,i,(void*) elDestroyerDeCorchetazos);
			}
			return true;
		}
	}
	log_error(loggerGameCard, "La coordenada %s no fue encontrada",registro);
	return false;
}



t_list* obtenerTodasLasPosiciones(char** blocks){
	t_list* coordenadas;

	if(blocks[0] == NULL){
		log_info(loggerDev, "No hay coordenadas bro te equivocaste, suerte para la proxima");
		coordenadas = list_create();
	}else{
		char** pathsDeBlocks = generarVectorDePaths(blocks);
		t_list* registros = traerRegistrosBloques(pathsDeBlocks);
		coordenadas = list_map(registros, (void*) getCoordenadaDeRegistro);


		list_destroy_and_destroy_elements(registros,(void*) elDestroyerDeCorchetazos);

		uint32_t indexPaths = 0;
		while(pathsDeBlocks[indexPaths] != NULL){
			free(pathsDeBlocks[indexPaths]);
			indexPaths++;
		}
		free(pathsDeBlocks);
	}

	return coordenadas;
}







//Recibe char** bloquesOcupados y limpia los block y los libera del bitmap

void vaciarBloques(char** blocksOcupados){
	log_info(loggerDev, "Vaciando blocks");
	uint32_t i = 0;
	while(blocksOcupados[i] != NULL){
		char* pathBlock = generadorDeRutaDeCreacionDeArchivos(rutaBlocks,blocksOcupados[i],".bin");
		FILE* archivo = fopen(pathBlock,"w");
		fclose(archivo);
		free(pathBlock);
		desmarcarBloqueBitmap(blocksOcupados[i]);
		i++;
	}


}


// recibe lista de registros char* y un archivo metadata, actualiza metadata despues de escribir todos los registros
void escribirEnBloques(t_list* registros, t_config* archivoMetadata) {

	if(list_is_empty(registros)){
		log_info(loggerGameCard, "El metadata del pokemon no tiene mas registros. SIZE = 0 y BLOCKS = [] ");
		config_set_value(archivoMetadata,"SIZE","0");
		config_set_value(archivoMetadata,"BLOCKS","[]");
		config_save(archivoMetadata);
		return;
	}


	int cantidadBytesEscritos = 0;
	int cantidadBytesTotales = 0;

	char* arrayBloques = string_new();
	string_append(&arrayBloques,"[");

	char* block = buscarPosicionDisponibleEnElBitMap();
	log_info(loggerDev, "El block libre seleccionado es: %s",block);

	char* pathBlock = generadorDeRutaDeCreacionDeArchivos(rutaBlocks,block,".bin");
	log_info(loggerDev, "La ruta del block es: %s",pathBlock);

	FILE* archivoBloque;

	archivoBloque = fopen(pathBlock,"w");

	string_append(&arrayBloques,block);
	string_append(&arrayBloques,",");

	void escribirRegistro(char* registro){
		for(int i = 0; i < string_length(registro); i++){
			if(cantidadBytesEscritos == blockSize){
				fclose(archivoBloque);
				free(pathBlock);
				char* blockNuevo = buscarPosicionDisponibleEnElBitMap();
				log_info(loggerDev, "El block libre seleccionado FUNCION ESCRIBIR_REGISTRO es: %s",block);

				pathBlock = generadorDeRutaDeCreacionDeArchivos(rutaBlocks,blockNuevo,".bin");
				log_info(loggerDev, "La ruta del block FUNCION ESCRIBIR_REGISTRO es: %s",pathBlock);

				archivoBloque = fopen(pathBlock,"w");

				string_append(&arrayBloques,blockNuevo);
				string_append(&arrayBloques,",");
				cantidadBytesEscritos = 0;
			}
			fputc(registro[i], archivoBloque);
			cantidadBytesEscritos++;
			cantidadBytesTotales++;
		}
	}

	log_info(loggerDev, "Iterando la lista");
	list_iterate(registros,(void*)escribirRegistro);
	fclose(archivoBloque);

	char* arrayBloquesFinalizado = 	string_substring_until(arrayBloques,string_length(arrayBloques)-1);
	string_append(&arrayBloquesFinalizado,"]");
	char* sizeTotal = string_itoa(cantidadBytesTotales);

	config_set_value(archivoMetadata,"SIZE",sizeTotal);
	config_set_value(archivoMetadata,"BLOCKS",arrayBloquesFinalizado);
	config_save(archivoMetadata);

	log_info(loggerGameCard, "El metadata del pokemon fue actualizado");
	log_info(loggerGameCard, "SIZE = %s",sizeTotal);
	log_info(loggerGameCard, "BLOCKS = %s",arrayBloquesFinalizado);

	free(block);
	free(pathBlock);
	free(sizeTotal);
	free(arrayBloques);
	free(arrayBloquesFinalizado);
}



//################################################################################################################################################################################################

void procesarRequestDeGameBoy(int cod_op, int socketGameBoy) {
	log_info(loggerDev, "Procesando request de GameBoy");
	uint32_t size;
	switch(cod_op){
	case NEW:
		log_info(loggerDev, "Codigo de operacion recibido del socket cliente %i corresponde a un NEW", socketGameBoy);
		t_newLlegada* newGameBoy = recibir_new(socketGameBoy,&size,loggerDev);
		log_info(loggerDev, "NEW recibido del modulo Game Boy socket %i", socketGameBoy);
		newPokemon(socketEscuchaNew,newGameBoy);
		//free(newGameBoy);
		break;

	case CATCH:
		log_info(loggerDev, "Codigo de operacion recibido del socket cliente %i corresponde a un CATCH", socketGameBoy);
		t_catchLlegada* catchGameBoy = recibir_catch(socketGameBoy,&size,loggerDev);
		log_info(loggerDev, "CATCH recibido del modulo Game Boy socket %i", socketGameBoy);
		catchPokemon(socketEscuchaCatch,catchGameBoy);
		break;

	case GET:
		log_info(loggerDev, "Codigo de operacion recibido del socket cliente %i corresponde a un GET", socketGameBoy);
		t_getLlegada* getGameBoy = recibir_get(socketGameBoy,&size,loggerDev);
		log_info(loggerDev, "GET recibido del modulo Game Boy socket %i", socketGameBoy);
		getPokemon(socketEscuchaGet,getGameBoy);
		//free(getGameBoy);
		break;

	default:
		log_error(loggerDev, "NO SE RECONOCE EL TIPO DE MENSAJE");
	}

	liberar_conexion(socketGameBoy);;
}

void atenderGameBoy(int* socketGameBoy) {
	uint32_t cod_op;
	int socketCliente = *socketGameBoy;
	free(socketGameBoy);
	log_info(loggerDev, "Recibiendo codigo de operacion de GameBoy en socket %i", socketCliente);
	int status_recv = recv(socketCliente, &cod_op, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		liberar_conexion(socketCliente);
		log_error(loggerDev, "Hubo un problema recibiendo codigo de operacion de GameBoy en socket %i", socketCliente);
		pthread_exit(NULL);
	}
	if (status_recv == 0) {
		liberar_conexion(socketCliente);
		log_warning(loggerDev, "Game boy acaba de cerrar la conexion correspondiente al socket %i", socketCliente);
		pthread_exit(NULL);
	}
	log_info(loggerDev, "Codigo de operacion de socket %i recibido: %i", socketCliente, cod_op);
	procesarRequestDeGameBoy(cod_op, socketCliente);
}

void esperarGameBoy(int socketEscuchaGameBoyPar) {
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socketCliente = accept(socketEscuchaGameBoyPar, (void*) &dir_cliente, &tam_direccion);
	log_info(loggerDev, "Socket %i de gameBoy aceptado", socketCliente);

	if(socketCliente != -1){
		pthread_t thread;

		int* socketSaraza = malloc(sizeof(int));
		*socketSaraza = socketCliente;

		pthread_create(&thread, NULL, (void*) atenderGameBoy, socketSaraza);
		pthread_detach(thread);
	}

}

void escucharGameBoy(void* socketEscuchaGameBoyPar) {
	int socketEscucha = (int) socketEscuchaGameBoyPar;
	log_info(loggerDev, "Esperando mensajes de game boy en socket %i", socketEscucha);

	while(1) {
		esperarGameBoy(socketEscucha);
	}
}

void escucharNewDeBroker(void) {
	uint32_t id_cola = 11;

	log_info(loggerDev, "Escuchando NEW de BROKER" );
	log_info(loggerDev, "Socker de NEW: %i", socketEscuchaNew);
	int status_envio = mandar_suscripcion(socketEscuchaNew, id_cola);

	if (status_envio == -1) {
		pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
		estoy_conectado_al_broker = 0;
		pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
		log_error(loggerDev, "Hubo un problema enviando la suscripcion a la cola NEW del BROKER");
	}

	while (1) {
		if (estoy_conectado_al_broker == 1) {

			uint32_t cod_op;
			log_info(loggerDev, "Recibiendo codigo de operacion de BROKER en socket %i", socketEscuchaNew);

			int status_recv = recv(socketEscuchaNew, &cod_op, sizeof(uint32_t), MSG_WAITALL);

			if (status_recv == -1) {
				liberar_conexion(socketEscuchaNew);
				log_error(loggerDev, "Hubo un problema recibiendo codigo de operacion de BROKER en socket %i", socketEscuchaNew);
				pthread_exit(NULL);
			}
			if (status_recv == 0) {
				liberar_conexion(socketEscuchaNew);
				log_warning(loggerDev, "BROKER acaba de cerrar la conexion correspondiente al socket %i", socketEscuchaNew);
				estoy_conectado_al_broker = 0;
				pthread_exit(NULL);
			}

			log_info(loggerDev, "Se recibio el opcode %i", cod_op);


			uint32_t size;
			log_info(loggerDev, "Codigo de operacion recibido del socket cliente %i corresponde a un NEW", socketEscuchaNew);
			t_newLlegada* newBroker = recibir_new(socketEscuchaNew,&size,loggerDev);
			devolver_ack(socketEscuchaNew);
			log_info(loggerDev, "NEW recibido del modulo BROKER socket %i", socketEscuchaNew);

			if (newBroker == NULL) {
				pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
				log_warning(loggerDev, "No se recibe conexion del modulo BROKER");
				estoy_conectado_al_broker = 0;
				pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
			} else {
				log_info(loggerDev, "NEW recibido");
				newPokemon(socketEscuchaNew,newBroker);

			}
			//free(newBroker);
		}
	}
}


void escucharCatchDeBroker(void) {
	uint32_t id_cola = 13;
	log_info(loggerDev, "Escucho CATCH de BROKER" );
	int status_envio = mandar_suscripcion(socketEscuchaCatch, id_cola);

	if (status_envio == -1) {
		pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
		estoy_conectado_al_broker = 0;
		pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
		log_error(loggerDev, "Hubo un problema enviando la suscripcion a la cola CATCH del BROKER");
	}

	while (1) {
		if (estoy_conectado_al_broker == 1) {

			uint32_t cod_op;
			log_info(loggerDev, "Recibiendo codigo de operacion de BROKER en socket %i", socketEscuchaCatch);

			int status_recv = recv(socketEscuchaCatch, &cod_op, sizeof(uint32_t), MSG_WAITALL);

			if (status_recv == -1) {
				liberar_conexion(socketEscuchaCatch);
				log_error(loggerDev, "Hubo un problema recibiendo codigo de operacion de BROKER en socket %i", socketEscuchaCatch);
				pthread_exit(NULL);
			}
			if (status_recv == 0) {
				liberar_conexion(socketEscuchaCatch);
				log_warning(loggerDev, "BROKER acaba de cerrar la conexion correspondiente al socket %i", socketEscuchaCatch);
				estoy_conectado_al_broker = 0;
				pthread_exit(NULL);
			}

			log_info(loggerDev, "Se recibio el opcode %i", cod_op);


			uint32_t size;
			log_info(loggerDev, "Codigo de operacion recibido del socket cliente %i corresponde a un CATCH", socketEscuchaCatch);
			t_catchLlegada* catchGameBoy = recibir_catch(socketEscuchaCatch,&size,loggerDev);
			devolver_ack(socketEscuchaCatch);
			log_info(loggerDev, "CATCH recibido del modulo BROKER socket %i", socketEscuchaCatch);

			if (catchGameBoy == NULL) {
				pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
				estoy_conectado_al_broker = 0;
				pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
			} else {
				log_info(loggerDev, "CATCH recibido");
				catchPokemon(socketEscuchaCatch,catchGameBoy);
			}

			//free(catchGameBoy);

		}
	}
}


void escucharGetDeBroker(void) {
	uint32_t id_cola = 15;
	log_info(loggerDev, "Escucho GET de BROKER" );
	int status_envio = mandar_suscripcion(socketEscuchaGet, id_cola);

	if (status_envio == -1) {
		pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
		estoy_conectado_al_broker = 0;
		pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
		log_error(loggerDev, "Hubo un problema enviando la suscripcion a la cola GET del broker");
	}

	while (1) {
		if (estoy_conectado_al_broker == 1) {

			uint32_t cod_op;
			log_info(loggerDev, "Recibiendo codigo de operacion de BROKER en socket %i", socketEscuchaGet);

			int status_recv = recv(socketEscuchaGet, &cod_op, sizeof(uint32_t), MSG_WAITALL);

			if (status_recv == -1) {
				liberar_conexion(socketEscuchaGet);
				log_error(loggerDev, "Hubo un problema recibiendo codigo de operacion de BROKER en socket %i", socketEscuchaGet);
				pthread_exit(NULL);
			}
			if (status_recv == 0) {
				liberar_conexion(socketEscuchaGet);
				log_warning(loggerDev, "BROKER acaba de cerrar la conexion correspondiente al socket %i", socketEscuchaGet);
				estoy_conectado_al_broker = 0;
				pthread_exit(NULL);
			}

			log_info(loggerDev, "Se recibio el opcode %i", cod_op);


			uint32_t size;
			log_info(loggerDev, "Codigo de operacion recibido del socket cliente %i corresponde a un GET", socketEscuchaGet);
			t_getLlegada* getGameBoy = recibir_get(socketEscuchaGet,&size,loggerDev);
			devolver_ack(socketEscuchaGet);
			log_info(loggerDev, "GET recibido del modulo BROKER socket %i", socketEscuchaGet);

			if (getGameBoy == NULL) {
				pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
				log_warning(loggerDev, "No se recibe conexion del modulo BROKER");
				estoy_conectado_al_broker = 0;
				pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
			} else {
				log_info(loggerDev, "GET recibido");
				getPokemon(socketEscuchaGet,getGameBoy);
			}

			//free(getGameBoy);
		}
	}
}

void reconectarAlBroker() {
	int conexion_al_broker = crear_conexion(ipBroker, puertoBroker);
	if (conexion_al_broker != -1) {

		uint32_t id_colaNew = 11;
		uint32_t id_colaCatch = 13;
		uint32_t id_colaGet = 15;

		socketEscuchaNew = conexion_al_broker;
		int status_envioNew = mandar_suscripcion(socketEscuchaNew, id_colaNew);

		if (status_envioNew == -1) {
			pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
			estoy_conectado_al_broker = 0;
			pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
			log_error(loggerDev, "Hubo un problema enviando la suscripcion a la cola NEW del BROKER");
			return;
		}
		log_info(loggerDev, "Socket de reconexion NEW: %i",socketEscuchaNew);



		socketEscuchaCatch = crear_conexion(ipBroker, puertoBroker);
		int status_envioCatch = mandar_suscripcion(socketEscuchaCatch, id_colaCatch);

		if (status_envioCatch == -1) {
			pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
			estoy_conectado_al_broker = 0;
			pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
			log_error(loggerDev, "Hubo un problema enviando la suscripcion a la cola NEW del BROKER");
			return;
		}
		log_info(loggerDev, "Socket de reconexion CATCH: %i",socketEscuchaCatch);



		socketEscuchaGet = crear_conexion(ipBroker, puertoBroker);
		int status_envioGet = mandar_suscripcion(socketEscuchaGet, id_colaGet);

		if (status_envioGet == -1) {
			pthread_mutex_lock(&estoy_conectado_al_broker_mutex);
			estoy_conectado_al_broker = 0;
			pthread_mutex_unlock(&estoy_conectado_al_broker_mutex);
			log_error(loggerDev, "Hubo un problema enviando la suscripcion a la cola NEW del BROKER");
			return;
		}
		log_info(loggerDev, "Socket de reconexion GET: %i",socketEscuchaGet);

		log_info(loggerDev, "Conexion establecida con BROKER, IP: %s PUERTO : %s", ipBroker, puertoBroker);
		estoy_conectado_al_broker = 1;
		log_info(loggerDev, "Reconexion con BROKER exitosa, verificando nuevamente en %i segundos", tiempoReintentoConexion);
	}else{
		log_error(loggerDev, "Reconexion con BROKER fallida, intentando nuevamente en %i segundos", tiempoReintentoConexion);

	}
}


void verificarConexion(void) {
	log_info(loggerDev, "Hilo de verificacion periodica de conexion con BROKER iniciado");
	while(1) {
		sleep(tiempoReintentoConexion);
		log_info(loggerDev, "Iniciando verificacion de conexion con BROKER...");
		if (estoy_conectado_al_broker == 0) {
			reconectarAlBroker();
		} else {
			log_info(loggerDev, "Conexion con BROKER en buen estado, verificando nuevamente en %i segundos", tiempoReintentoConexion);
		}
	}
}



//#############################################################################################################################################################3


void crearDirectorioFile(char* puntoMontaje){

	//Seteo de variable global
	rutaFiles = generadorDeRutaDeCreacionDeDirectorios(puntoMontaje,"Files");
	log_info(loggerDev, "Se creo el directorio Files ubicado en: %s",rutaFiles);

	//Crear Directorio
	log_info(loggerDev, "Creando directorio FILES");
	crearDirectorio("Files",puntoMontaje);

	//Crear Archivo Metadata
	crearArchivoEnDirectorio("Metadata",rutaFiles);
	log_info(loggerDev, "Se creo el archivo METADATA de Files");

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

	char* metadata = "Metadata";
	char* bin = ".bin";

	char* archivoMetaData = generadorDeRutaDeCreacionDeArchivos(rutaMetadata,metadata,bin);
	t_config* metaData=config_create(archivoMetaData);

	log_info(loggerDev, "Se creo el config de MetaData");
	blockSize = config_get_int_value(metaData,"BLOCK_SIZE");

	log_info(loggerDev, "El Block Size es: %i", blockSize);
	cantidadDeBloques = config_get_int_value(metaData,"BLOCKS");

	log_info(loggerDev, "La cantidad de Bloques es: %i", cantidadDeBloques);
	magicNumber = config_get_string_value(metaData, "MAGIC_NUMBER");

	log_info(loggerDev, "El Magic Number es: %s", magicNumber);
	config_destroy(metaData);

	free(archivoMetaData);

}

void levantarTallGrass(char* puntoMontaje){
	log_info(loggerGameCard, "Lenavando el Tall Grass");
	//Seteo de variable global
	log_info(loggerDev, "El punto de montaje es: %s", puntoMontaje);
	rutaMetaData = generadorDeRutaDeCreacionDeDirectorios(puntoMontaje,"Metadata");
	log_info(loggerDev, "La ruta del metadataes: %s", rutaMetaData);


	cargarInfoDelMetadata(rutaMetaData);


	bitMap = generadorDeRutaDeCreacionDeArchivos(rutaMetaData,"BitMap",".bin");


	if(existeArchivo(bitMap)){
		crearBitarray();
		rutaFiles = generadorDeRutaDeCreacionDeDirectorios(puntoMontaje,"Files");
		rutaBlocks = generadorDeRutaDeCreacionDeDirectorios(puntoMontaje,"Blocks");

		log_error(loggerDev, "Ya existe el BitMap");

	} else {
		crearDirectorio("Metadata",puntoMontaje);
		crearArchivoEnDirectorio("BitMap",rutaMetaData);
		crearBitarray();
		crearDirectorioFile(puntoMontaje);
		crearDirectorioBlocks(puntoMontaje);
	}
	log_info(loggerGameCard, "Punto de montaje del directorio Metadata es: %s",rutaMetaData);
	log_info(loggerGameCard, "El BitMap se encuentra en: %s",bitMap);
	log_info(loggerGameCard, "Punto de montaje del directorio Files es: %s",rutaFiles);
	log_info(loggerGameCard, "Punto de montaje del directorio Blocks es: %s",rutaBlocks);

}




bool existenPosicionesEnArchivo(char* posicion,char** blocks){
	uint32_t i = 0;

	log_info(loggerDev, "Valido posicion");
	log_info(loggerDev, "EL array de blocks es: %s", blocks[0]);


	if(blocks[0] == NULL){
		log_info(loggerDev, "Entro por NULL");
		return false;
	}

	while(blocks[i] != NULL){
		log_info(loggerDev, "Se esta analizando el block: %s",blocks[i]);
		char* rutaDeArchivo = generadorDeRutaDeCreacionDeArchivos(rutaBlocks,blocks[i],".bin");
		t_config* archivoBlock = config_create(rutaDeArchivo);
		if(config_has_property(archivoBlock,posicion)){
			log_error(loggerGameCard, "La posicion %s se encuentra en el block %s",posicion,blocks[i]);
			config_destroy(archivoBlock);
			free(rutaDeArchivo);
			return true;
		} else{
			config_destroy(archivoBlock);
			free(rutaDeArchivo);
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
			log_info(loggerDev, "Encontro el block donde esta la posicion: %s",blocks[i]);
			config_destroy(archivoBlock);
			free(rutaDeArchivo);
			return blocks[i];
		} else{
			config_destroy(archivoBlock);
			free(rutaDeArchivo);
			i++;
		}
	}
	log_info(loggerDev, "Yyyy mira, no tiro el return en el if, no encontro una mierda");

	return NULL;
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


void validarPosicionesDeNew(t_config* archivoMetadataPokemon, char** blocksOcupados, char** keyValue){

	if(blocksOcupados[0] != NULL){
		log_info(loggerDev, "Pokemon ya conocido");
		char** pathsDeBlocks = generarVectorDePaths(blocksOcupados);
		t_list* registros = traerRegistrosBloques(pathsDeBlocks);
		registros = evaluarRegistroNew(keyValue,registros);
		t_list* registrosAEscribir = list_map(registros, (void*) aplanarVectorDeKeyValue);
		vaciarBloques(blocksOcupados);
		escribirEnBloques(registrosAEscribir,archivoMetadataPokemon);

		list_destroy_and_destroy_elements(registrosAEscribir,(void*) elLimpiaCharDinamicos);
		list_destroy_and_destroy_elements(registros,(void*) elDestroyerDeCorchetazos);

		uint32_t indexPaths = 0;
		while(pathsDeBlocks[indexPaths] != NULL){
			free(pathsDeBlocks[indexPaths]);
			indexPaths++;
		}
		free(pathsDeBlocks);

	} else {
		log_info(loggerDev, "No tiene blocks asignados. Primera vez");
		t_list* registroNuevo = list_create();
		list_add(registroNuevo, aplanarVectorDeKeyValue(keyValue));
		escribirEnBloques(registroNuevo,archivoMetadataPokemon);

		free(keyValue[0]);
		free(keyValue[1]);
		free(keyValue);

		list_destroy_and_destroy_elements(registroNuevo,(void*) elLimpiaCharDinamicos);
	}
}



bool validarPosicionesDeCatch(t_config* archivoMetadataPokemon, char** blocksOcupados, char* key){

	if(blocksOcupados[0] == NULL){
		log_info(loggerDev, "No hay bloques perri");
		log_error(loggerGameCard, "No hay bloques");
		return false;
	}

	char** pathsDeBlocks = generarVectorDePaths(blocksOcupados);
	t_list* registros = traerRegistrosBloques(pathsDeBlocks);

	bool flagAtrapado = evaluarRegistroCatch(key,registros);
	log_info(loggerDev, "EL FLAG DEL CATCH ES: %i", flagAtrapado);

	t_list* registrosAEscribir = list_map(registros, (void*) aplanarVectorDeKeyValue);
	vaciarBloques(blocksOcupados);
	escribirEnBloques(registrosAEscribir,archivoMetadataPokemon);

	list_destroy_and_destroy_elements(registrosAEscribir,(void*) elLimpiaCharDinamicos);
	list_destroy_and_destroy_elements(registros,(void*) elDestroyerDeCorchetazos);



	uint32_t indexPaths = 0;
	while(pathsDeBlocks[indexPaths] != NULL){
		free(pathsDeBlocks[indexPaths]);
		indexPaths++;

	}
	free(pathsDeBlocks);

	return flagAtrapado;
}



void validarPosicionesDeGetAntiInvalidFree(char** blocksOcupados,t_list** registros){

	log_info(loggerGameCard, "Obteniendo las posiciones en las que se encuentra el pokemon");

	if(blocksOcupados[0] == NULL){
		log_info(loggerDev, "No hay coordenadas bro te equivocaste, suerte para la proxima");
		log_error(loggerGameCard, "No hay bloques");
	}else{
		char** pathsDeBlocks = generarVectorDePaths(blocksOcupados);

		t_list* registrosDeBLoques = traerRegistrosBloques(pathsDeBlocks);

		*registros = list_map(registrosDeBLoques, (void*) getCoordenadaDeRegistro);

		uint32_t indexPaths = 0;
		while(pathsDeBlocks[indexPaths] != NULL){
			free(pathsDeBlocks[indexPaths]);
			indexPaths++;

		}
		free(pathsDeBlocks);

	}

	for(int j=0; j<(*registros)->elements_count;j++){
		char* saraza2 = list_get((*registros),j);
		log_info(loggerDev, "Las coordenadas encontradas son: %s", saraza2);
		log_info(loggerGameCard, "Las coordenadas encontradas son: %s", saraza2);
	}
}



t_list* validarPosicionesDeGet(char** blocksOcupados){

	log_info(loggerGameCard, "Obteniendo las posiciones en las que se encuentra el pokemon");
	t_list* posicionesPokemon = obtenerTodasLasPosiciones(blocksOcupados);

	for(int j=0; j<posicionesPokemon->elements_count;j++){
			char* saraza2 = list_get(posicionesPokemon,j);
			log_info(loggerDev, "Las coordenadas encontradas son: %s", saraza2);
			log_info(loggerGameCard, "Las coordenadas encontradas son: %s", saraza2);
	}

	return posicionesPokemon;
}





void newPokemon(int socketCliente,t_newLlegada* new){

	log_info(loggerDev,"Llegamo");

	char* pokemon = new->pokemon;
	uint32_t posX = new->pos_X;
	uint32_t posY = new->pos_Y;
	uint32_t cantidad = new->cantidad;
	string_to_lower(pokemon);

	log_info(loggerDev,"Pokemon: %s",pokemon);
	log_info(loggerDev,"PosX: %i",posX);
	log_info(loggerDev,"PosY: %i",posY);
	log_info(loggerDev,"Cantidad: %i",cantidad);


	char* rutaDeDirectorio = generadorDeRutaDeCreacionDeDirectorios(rutaFiles,pokemon);
	log_info(loggerDev,"Ruta de Directorio: %s",rutaDeDirectorio);

	char* posicion = generadorDePosiciones(posX,posY);
	char** registroPokemon = generadorDeRegistros(posicion,cantidad);

	if(noExisteDirectorio(rutaDeDirectorio)){
		log_info(loggerGameCard,"El pokemon %s no existe en el FS, se pasa a crear su directorio", pokemon);
		crearDirectorio(pokemon,rutaFiles);
		crearArchivoEnDirectorio(pokemon,rutaDeDirectorio);
		crearTemplateDeArchivoTipo(METADATA_POKEMON,pokemon,rutaDeDirectorio);
	}

	char* directorioPokemon = generadorDeRutaDeCreacionDeDirectorios(rutaFiles,pokemon);
	log_info(loggerDev,"Ruta del directorio pokemon %s es: %s",pokemon, directorioPokemon);
	char* rutaDeArchivo = generadorDeRutaDeCreacionDeArchivos(directorioPokemon, pokemon, ".bin");
	log_info(loggerDev,"Ruta del Archivo: %s",rutaDeArchivo);


	t_config* archivoMetadataPokemon = config_create(rutaDeArchivo);
	log_info(loggerDev,"Se creo el archivo config");

	char** blocksOcupados = config_get_array_value(archivoMetadataPokemon,"BLOCKS");
	log_info(loggerDev, "EL array de blocks es: %s", blocksOcupados[0]);

	pthread_mutex_lock(&estoy_leyendo_metadata_mutex);
	if(puedeAbrirseArchivo(archivoMetadataPokemon)){
		activarFlagDeLectura(archivoMetadataPokemon);
		pthread_mutex_unlock(&estoy_leyendo_metadata_mutex);

		validarPosicionesDeNew(archivoMetadataPokemon, blocksOcupados, registroPokemon);

	} else{
		pthread_mutex_unlock(&estoy_leyendo_metadata_mutex);
		log_error(loggerGameCard, "Ya existe un proceso utilizando el archivo Metadata de %s",pokemon);
		sleep(tiempoReintentoOperacion);
		newPokemon(socketCliente,new);
		return;
	}

	desactivarFlagDeLectura(archivoMetadataPokemon);

	t_appeared* appearedGenerado = crearAppeared(new);
	sleep(tiempoRetardoOperacion);

	int socket_envio = crear_conexion(ipBroker, puertoBroker);

	log_info(loggerGameCard, "Enviando APPEARED por socket: %i",socket_envio);
	enviar_appeared(socket_envio, appearedGenerado);
	log_info(loggerGameCard, "APPEARED enviado");

	config_destroy(archivoMetadataPokemon);

	free(appearedGenerado->pokemon);
	free(appearedGenerado);


	free(rutaDeDirectorio);
	free(rutaDeArchivo);
	//free(posicion);
	free(directorioPokemon);
	uint32_t indexParaBorrar = 0;
	while(blocksOcupados[indexParaBorrar] != NULL){

		if(blocksOcupados[indexParaBorrar] == NULL){
			log_info(loggerDev,"EL BLOCK ES NULL");
		}

		free(blocksOcupados[indexParaBorrar]);
		indexParaBorrar++;
	}
	free(blocksOcupados);

	free(new);

}


void catchPokemon(int socketCliente,t_catchLlegada* catch){

	uint32_t idMensaje = catch->id;
	char* pokemon = catch->pokemon;
	uint32_t posX = catch->pos_X;
	uint32_t posY = catch->pos_Y;
	bool validacion = false;
	string_to_lower(pokemon);

	char* rutaDeDirectorio = generadorDeRutaDeCreacionDeDirectorios(rutaFiles,pokemon);
	char* posicion = generadorDePosiciones(posX, posY);

	if(noExisteDirectorio(rutaDeDirectorio)){
		log_error(loggerGameCard, "No existe directorio dentro de Files para el pokemon: %s",pokemon);
		return;
	}


	char* rutaDeArchivo = generadorDeRutaDeCreacionDeArchivos(rutaDeDirectorio, pokemon, ".bin");
	t_config* archivoMetadataPokemon = config_create(rutaDeArchivo);
	char** blocksOcupados = config_get_array_value(archivoMetadataPokemon,"BLOCKS");
	log_info(loggerDev, "EL array de blocks es: %s", blocksOcupados[0]);

	pthread_mutex_lock(&estoy_leyendo_metadata_mutex);
	if(puedeAbrirseArchivo(archivoMetadataPokemon)){
		activarFlagDeLectura(archivoMetadataPokemon);
		pthread_mutex_unlock(&estoy_leyendo_metadata_mutex);
		validacion = validarPosicionesDeCatch(archivoMetadataPokemon,blocksOcupados,posicion);
	}else{
		pthread_mutex_unlock(&estoy_leyendo_metadata_mutex);
		log_error(loggerGameCard, "Ya existe un proceso utilizando el archivo Metadata de %s",pokemon);
		sleep(tiempoReintentoOperacion);
		catchPokemon(socketCliente,catch);
		return;
	}

	desactivarFlagDeLectura(archivoMetadataPokemon);

	t_caught* caughtAEnviar = crearCaught(idMensaje,validacion);
	sleep(tiempoRetardoOperacion);

	int socket_envio = crear_conexion(ipBroker, puertoBroker);

	log_info(loggerGameCard, "Enviando CAUGHT por socket: %i",socket_envio);
	enviar_caught(socket_envio, caughtAEnviar);
	log_info(loggerGameCard, "CAUGHT enviado");


	config_destroy(archivoMetadataPokemon);

	free(caughtAEnviar);
	free(rutaDeDirectorio);
	free(rutaDeArchivo);
	free(posicion);
	uint32_t indexParaBorrar = 0;
	while(blocksOcupados[indexParaBorrar] != NULL){
		free(blocksOcupados[indexParaBorrar]);
		indexParaBorrar++;
	}
	free(blocksOcupados);

	free(catch->pokemon);
	free(catch);

}

void getPokemon(int socketCliente,t_getLlegada* getLlegada){

	char* pokemon = getLlegada->pokemon;

	//t_list* coordenadas=list_create();
	t_list* coordenadas;

	string_to_lower(pokemon);
	char* rutaDeDirectorio = generadorDeRutaDeCreacionDeDirectorios(rutaFiles,pokemon);

	if(noExisteDirectorio(rutaDeDirectorio)){
		log_info(loggerGameCard,"El pokemon %s no existe en el FS, enviando Localized sin posiciones", pokemon);
		t_localized* localizedAEnviar = crearLocalizedDePokemonInexistente(getLlegada);

		int socketLocalizedFrula = crear_conexion(ipBroker, puertoBroker);
		log_info(loggerGameCard, "Enviando LOCALIZED por socket: %i",socketLocalizedFrula);
		enviar_localized(socketLocalizedFrula, localizedAEnviar);
		log_info(loggerGameCard, "LOCALIZED enviado");

		free(localizedAEnviar->pokemon);

		list_destroy(localizedAEnviar->l_coordenadas);
		free(localizedAEnviar);

		free(rutaDeDirectorio);
		return;
	}

	char* rutaDeArchivo = generadorDeRutaDeCreacionDeArchivos(rutaDeDirectorio, pokemon, ".bin");
	t_config* archivoMetadataPokemon = config_create(rutaDeArchivo);


	char** blocksOcupados = config_get_array_value(archivoMetadataPokemon,"BLOCKS");
	log_info(loggerDev, "EL array de blocks es: %s", blocksOcupados[0]);

	pthread_mutex_lock(&estoy_leyendo_metadata_mutex);
	if(puedeAbrirseArchivo(archivoMetadataPokemon)){
		activarFlagDeLectura(archivoMetadataPokemon);
		pthread_mutex_unlock(&estoy_leyendo_metadata_mutex);
		//validarPosicionesDeGetAntiInvalidFree(blocksOcupados,&coordenadas);
		coordenadas = validarPosicionesDeGet(blocksOcupados);
	} else{
		pthread_mutex_unlock(&estoy_leyendo_metadata_mutex);
		log_error(loggerGameCard, "Ya existe un proceso utilizando el archivo Metadata de %s",pokemon);
		sleep(tiempoReintentoOperacion);
		getPokemon(socketCliente,getLlegada);
		return;
	}

	desactivarFlagDeLectura(archivoMetadataPokemon);

	t_localized* localizedAEnviar = crearLocalized(getLlegada, coordenadas);

	sleep(tiempoRetardoOperacion);

	int socket_envio = crear_conexion(ipBroker, puertoBroker);

	log_info(loggerGameCard, "Enviando LOCALIZED por socket: %i",socket_envio);
	enviar_localized(socket_envio, localizedAEnviar);
	log_info(loggerGameCard, "LOCALIZED enviado");

	config_destroy(archivoMetadataPokemon);

	list_destroy_and_destroy_elements(coordenadas,(void *) elLimpiaCharDinamicos);

	free(rutaDeDirectorio);
	free(rutaDeArchivo);

	uint32_t indexParaBorrar = 0;
	while(blocksOcupados[indexParaBorrar] != NULL){
		free(blocksOcupados[indexParaBorrar]);
		indexParaBorrar++;
	}

	free(blocksOcupados);
	free(localizedAEnviar->pokemon);

	for(int w=0; w<localizedAEnviar->l_coordenadas->elements_count;w++){
		free(list_get(localizedAEnviar->l_coordenadas,w));
	}

	list_destroy(localizedAEnviar->l_coordenadas);
	free(localizedAEnviar);

	free(getLlegada);

}


void generar_ID_Modulo(){
	id_moduloGC = rand();
}


int iniciarEscuchaGameBoy(char* IP, char* PUERTO) {
	int socket_servidor;

	struct addrinfo hints, *servinfo, *p;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(IP, PUERTO, &hints, &servinfo);

	for (p=servinfo; p != NULL; p = p->ai_next)
	{
		if ((socket_servidor = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
			continue;

		if (bind(socket_servidor, p->ai_addr, p->ai_addrlen) == -1) {
			close(socket_servidor);
			continue;
		}
		break;
	}

	listen(socket_servidor, SOMAXCONN);

	freeaddrinfo(servinfo);

	return socket_servidor;
}


