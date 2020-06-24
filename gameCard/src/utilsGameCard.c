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

void enviar_mensaje(char* argv[], int socket_cliente){        // de GAMEBOY
	uint32_t bytes;

	log_info(loggerDev, "Creando buffer");
	t_buffer* buffer = crearBufferPorTipoDeMensaje(argv,loggerDev);
	log_info(loggerDev, "Buffer creado");


	log_info(loggerDev, "Creando paquete");
	t_paquete* paquete = crearPaquete(buffer);   // CREAR PAQUETE

	paquete->codigo_operacion = enumTipoMensaje(argv[2]);
	log_info(loggerDev, "CODIGO DE OPERACION: %i", paquete->codigo_operacion);

	log_info(loggerDev, "Paquete Creado");
	log_info(loggerDev, "la operacion a realizar es %i", paquete->codigo_operacion);

	log_info(loggerDev, "Serializando...");
	void* flujo = serializar_paquete(paquete,&bytes);                   //  SERIALIZAR PAQUETE
	log_info(loggerDev, "Serializacion completa");


	log_info(loggerDev, "El peso total es: %i",paquete->buffer->size);

	//    ENVIAR MENSAJE
	if (send(socket_cliente, flujo, bytes, 0) == -1){
		log_error(loggerDev, "Error: No se pudo enviar el mensaje");
	}

	free(flujo);
	free(buffer->stream);
	free(buffer);
	free(paquete);
}


// atender cliente y procesar request choripasteados de BKR
void procesar_request(int cod_op, int cliente_fd) {
	uint32_t size;

	mensaje_queue* mensaje_a_guardar = malloc(sizeof(mensaje_queue));
	log_info(extense_logger, "Generando id para el mensaje del socket %i", cliente_fd);
	mensaje_a_guardar->id = generar_id();
	log_info(extense_logger, "El id generado es %i", mensaje_a_guardar->id);

	switch (cod_op) {
	case NEW: ;
	log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un NEW", cliente_fd);
	t_new* new_msg = recibir_new(cliente_fd, &size, extense_logger);

	if (new_msg == NULL) {
		free(mensaje_a_guardar);
		pthread_exit(NULL);
	} else {
		mensaje_a_guardar->mensaje = (void*) new_msg;

		sem_wait(&gl_new_limite);

		log_info(extense_logger, "Encolando el mensaje NEW con id %i", mensaje_a_guardar->id);
		pthread_mutex_lock(&gl_new_queue_lock);
		queue_push(gl_new_pokemon_queue, mensaje_a_guardar);
		pthread_mutex_unlock(&gl_new_queue_lock);
		log_info(extense_logger, "Mensaje NEW con id %i encolado", mensaje_a_guardar->id);
		log_info(logger, "Mensaje NEW con id %i encolado", mensaje_a_guardar->id);

		sem_post(&gl_new_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);
	}
	break;

	case CATCH: ;
	log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un CATCH", cliente_fd);
	t_catch* catch_msg = recibir_catch(cliente_fd, &size, extense_logger);

	if (catch_msg == NULL) {
		free(mensaje_a_guardar);
		pthread_exit(NULL);
	} else {
		mensaje_a_guardar->mensaje = (void*) catch_msg;

		sem_wait(&gl_catch_limite);

		log_info(extense_logger, "Encolando el mensaje CATCH con id %i", mensaje_a_guardar->id);
		pthread_mutex_lock(&gl_catch_queue_lock);
		queue_push(gl_catch_pokemon_queue, mensaje_a_guardar);
		pthread_mutex_unlock(&gl_catch_queue_lock);
		log_info(extense_logger, "Mensaje CATCH con id %i encolado", mensaje_a_guardar->id);
		log_info(logger, "Mensaje CATCH con id %i encolado", mensaje_a_guardar->id);

		sem_post(&gl_catch_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);
	}
	break;

	case GET: ;
	log_info(extense_logger, "Codigo de operacion recibido del socket cliente %i corresponde a un GET", cliente_fd);
	t_get* get_msg = recibir_get(cliente_fd, &size, extense_logger);

	if (get_msg == NULL) {
		free(mensaje_a_guardar);
		pthread_exit(NULL);
	} else {
		mensaje_a_guardar->mensaje = (void*) get_msg;

		sem_wait(&gl_get_limite);

		log_info(extense_logger, "Encolando el mensaje GET con id %i", mensaje_a_guardar->id);
		pthread_mutex_lock(&gl_get_queue_lock);
		queue_push(gl_get_pokemon_queue, mensaje_a_guardar);
		pthread_mutex_unlock(&gl_get_queue_lock);
		log_info(extense_logger, "Mensaje GET con id %i encolado", mensaje_a_guardar->id);
		log_info(logger, "Mensaje GET con id %i encolado", mensaje_a_guardar->id);

		sem_post(&gl_get_mensajes);

		devolver_id(cliente_fd, mensaje_a_guardar->id, extense_logger);
	}
	break;


	}
	free(suscripcion);
	break;
	}
}

void atender_cliente(int* socket) {
	uint32_t cod_op;
	log_info(extense_logger, "Recibiendo codigo de operacion de socket %i", *socket);
	int status_recv = recv(*socket, &cod_op, sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1) {
		close(*socket);
		log_error(extense_logger, "Hubo un problema recibiendo codigo de operacion de socket %i", *socket);
		pthread_exit(NULL);
	}
	if (status_recv == 0) {
		close(*socket);
		log_warning(extense_logger, "El cliente acaba de cerrar la conexion correspondiente al socket %i", *socket);
		pthread_exit(NULL);
	}
	log_info(extense_logger, "Codigo de operacion de socket %i recibido: %i", *socket, cod_op);
	procesar_request(cod_op, *socket);
}

void esperar_cliente(int socket_servidor) {
	struct sockaddr_in dir_cliente;

	int tam_direccion = sizeof(struct sockaddr_in);

	int socket_cliente = accept(socket_servidor, (void*) &dir_cliente, &tam_direccion);

	log_info(extense_logger, "Socket cliente %i aceptado", socket_cliente);
	log_info(logger, "Nueva conexion de un proceso con socket cliente %i", socket_cliente);

	pthread_t thread;

	pthread_create(&thread, NULL, (void*) atender_cliente, &socket_cliente);
	pthread_detach(thread);
}

void esperar_clientes(void* socket_servidor) {
	int socket = (int) socket_servidor;
	log_info(extense_logger, "Esperando clientes en socket %i", socket);
	while(1) {
		esperar_cliente(socket);
	}
}


void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
