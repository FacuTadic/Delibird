#include "utilsGameBoy.h"
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


uint32_t idColaPorTipoDeMensaje(char* mensajeAEnviar){

	uint32_t tipoDeMensaje;

	if(!strcmp(mensajeAEnviar,"NEW_POKEMON")){
		tipoDeMensaje=11;
	}else if(!strcmp(mensajeAEnviar,"APPEARED_POKEMON")){
		tipoDeMensaje=12;
	} else if(!strcmp(mensajeAEnviar,"CATCH_POKEMON")){
		tipoDeMensaje=13;
	}else if(!strcmp(mensajeAEnviar,"CAUGHT_POKEMON")){
		tipoDeMensaje=14;
	}else if(!strcmp(mensajeAEnviar,"GET_POKEMON")){
		tipoDeMensaje=15;
	}else if(!strcmp(mensajeAEnviar,"LOCALIZED")){
		tipoDeMensaje=16;
	}
	return tipoDeMensaje;
}

uint32_t numeroPorTipoDeMensaje(char* mensajeAEnviar){

	uint32_t tipoDeMensaje;

	if(!strcmp(mensajeAEnviar,"NEW_POKEMON")){
		tipoDeMensaje=1;
	}else if(!strcmp(mensajeAEnviar,"APPEARED_POKEMON")){
		tipoDeMensaje=2;
	} else if(!strcmp(mensajeAEnviar,"CATCH_POKEMON")){
		tipoDeMensaje=3;
	}else if(!strcmp(mensajeAEnviar,"CAUGHT_POKEMON")){
		tipoDeMensaje=4;
	}else if(!strcmp(mensajeAEnviar,"GET_POKEMON")){
		tipoDeMensaje=5;
	}else if(!strcmp(mensajeAEnviar,"LOCALIZED")){
		tipoDeMensaje=6;
	}else if(!strcmp(mensajeAEnviar,"SUSCRIPTOR")){
		tipoDeMensaje=7;
	}else if(!strcmp(mensajeAEnviar,"MENSAJE")){
		tipoDeMensaje=8;
	}
	return tipoDeMensaje;
}


void agregarID(void* stream,uint32_t id, uint32_t* offset){
	memcpy(stream,&id,sizeof(uint32_t));
	*offset += sizeof(uint32_t);
}


int flagCaught(char* flag){

	uint32_t flagCorrespondiente = -1;

	if(!strcmp(flag,"OK")){
		flagCorrespondiente = 1;
	} else if(!strcmp(flag,"FAIL")){
		flagCorrespondiente = 0;
	}

	return flagCorrespondiente;

}




t_buffer* crearBufferPorTipoDeMensaje(char*argv[],t_log* loggerDev ){

	//Variables Buffer
	t_buffer* bufferAux = malloc(sizeof(t_buffer));
	void* stream;
	uint32_t offset = 0;

	//Variables para estructura
	uint32_t sizeNombrePokemon;
	uint32_t posX;
	uint32_t posY;
	uint32_t cantidadPokemon;
	uint32_t idMensaje;
	uint32_t sizeIP = strlen(ip_game_boy) + 1;
	uint32_t sizePuerto = strlen(puerto_game_boy) + 1;

	//Variable para el switch
	log_info(loggerDev, "MENSAJE; %s",argv[2]);

	uint32_t nroMensaje = numeroPorTipoDeMensaje(argv[2]);


	log_info(loggerDev, "MENSAJE; %i",nroMensaje);




	switch(nroMensaje){

		//Commons Dictionary Hash

		case 1 : //NEW

			bufferAux->size = 4*sizeof(uint32_t) + strlen(argv[3])+1; // sizeNombre, Cord X,Y , Cantidad + NOMBRE

			//CASO DE AGREGAR EL ID_MENSAJE
			//Comparacion de strings commons
			if(!strcmp(argv[1],"GAMECARD")){
				bufferAux->size += sizeof(uint32_t);
			}

			stream = malloc(bufferAux->size);

			if(!strcmp(argv[1],"GAMECARD")){
				idMensaje = atoi(argv[7]);
				agregarID(stream,idMensaje,&offset);
			}

			sizeNombrePokemon = strlen(argv[3]) + 1;
			memcpy(stream + offset,&sizeNombrePokemon, sizeof(uint32_t));
			offset += sizeof(uint32_t);

			log_info(loggerDev, "Tamanio nombre pokemon %i", sizeNombrePokemon);

			memcpy(stream + offset,argv[3], sizeNombrePokemon);
			offset += sizeNombrePokemon;

			log_info(loggerDev, "Nombre pokemon %s", argv[3]);

			posX = atoi(argv[4]);

			memcpy(stream + offset,&posX, sizeof(uint32_t));
			offset += sizeof(uint32_t);

			log_info(loggerDev, "Posicion X %i", posX);

			posY = atoi(argv[5]);

			memcpy(stream + offset,&posY, sizeof(uint32_t));
			offset += sizeof(uint32_t);

			log_info(loggerDev, "Posicion Y %i", posY);

			cantidadPokemon = atoi(argv[6]);

			memcpy(stream + offset,&cantidadPokemon, sizeof(uint32_t));
			offset += sizeof(uint32_t);

			log_info(loggerDev, "Cantidad %i", cantidadPokemon);

			bufferAux->stream = stream;

			break;

		case 2: //APPEARED

			bufferAux->size = 3*sizeof(uint32_t) + strlen(argv[3])+1; // sizeNombre, Cord X,Y + NOMBRE

			//CASO DE AGREGAR EL ID_MENSAJE
			if(!strcmp(argv[1],"BROKER")){
			bufferAux->size += sizeof(uint32_t);						// +ID_MENSAJE
			}

			stream = malloc(bufferAux->size);


			if(!strcmp(argv[1],"BROKER")){
				idMensaje = atoi(argv[6]);
				agregarID(stream,idMensaje,&offset);

			}

			sizeNombrePokemon = strlen(argv[3]) + 1;
			memcpy(stream + offset,&sizeNombrePokemon, sizeof(uint32_t));
			offset += sizeof(uint32_t);

			memcpy(stream + offset,argv[3],strlen(argv[3]) + 1);
			offset += sizeNombrePokemon;

			posX = atoi(argv[4]);

			memcpy(stream + offset,&posX, sizeof(uint32_t));
			offset += sizeof(uint32_t);

			posY = atoi(argv[5]);

			memcpy(stream + offset,&posY, sizeof(uint32_t));
			offset += sizeof(uint32_t);

			bufferAux->stream = stream;

			break;

		case 3: //CATCH

			bufferAux->size = 3*sizeof(uint32_t) + strlen(argv[3])+1;// sizeNombre, Cord X,Y , Cantidad + NOMBRE

			//CASO DE AGREGAR EL ID_MENSAJE
			if(!strcmp(argv[1],"GAMECARD")){
				bufferAux->size += sizeof(uint32_t);
			}

			stream = malloc(bufferAux->size);

			if(!strcmp(argv[1],"GAMECARD")){
				idMensaje = atoi(argv[6]);
				agregarID(stream,idMensaje,&offset);
			}


			sizeNombrePokemon = strlen(argv[3]) + 1;

			memcpy(stream + offset,&sizeNombrePokemon, sizeof(uint32_t));
			offset += sizeof(uint32_t);

			memcpy(stream + offset,argv[3],strlen(argv[3]) + 1);
			offset += sizeNombrePokemon;

			posX = atoi(argv[4]);

			memcpy(stream + offset,&posX, sizeof(uint32_t));
			offset += sizeof(uint32_t);

			posY = atoi(argv[5]);

			memcpy(stream + offset,&posY, sizeof(uint32_t));
			offset += sizeof(uint32_t);

			bufferAux->stream = stream;

			break;

		case 4:  //CAUGHT
			bufferAux->size = 2*sizeof(uint32_t); // sizeNombre, Cord X,Y + NOMBRE

			stream = malloc(bufferAux->size);

			idMensaje = atoi(argv[3]);
			agregarID(stream,idMensaje,&offset);

			uint32_t flag = flagCaught(argv[4]);
			if (flag == -1){
				log_error(loggerDev, "NO existe el flag enviado");
				exit(-1);
			}

			memcpy(stream + offset,&flag, sizeof(uint32_t));
			offset += sizeof(uint32_t);

			bufferAux->stream = stream;

			break;


		case 5:  //GET

			bufferAux->size = 3*sizeof(uint32_t) + strlen(argv[3])+1; // sizeNombre
			stream = malloc(bufferAux->size);

			uint32_t sizeVerga = strlen(argv[3])+1;


			memcpy (stream + offset, &sizeVerga, sizeof(uint32_t));
			offset += sizeof(uint32_t);

			memcpy (stream + offset, argv[3], strlen(argv[3])+1);
			offset += sizeVerga;

			bufferAux->stream = stream;

			break;


		case 7: //SUSCRIPTOR

			bufferAux->size = 3*sizeof(uint32_t) + sizeIP + sizePuerto; // Cola de Mensajes (representada por protocolo)

			stream = malloc(bufferAux->size);

			log_info(loggerDev, "LA COLA DE MENSAJE ES: %s", argv[3]);

			uint32_t colaDeMensajesSolicitada = idColaPorTipoDeMensaje(argv[3]);

			log_info(loggerDev, "EL NUMERO DE LA COLA ES: %i", colaDeMensajesSolicitada);

			memcpy(stream + offset, &colaDeMensajesSolicitada, sizeof(uint32_t));
			offset += sizeof(uint32_t);

			memcpy(stream + offset, &sizeIP, sizeof(uint32_t));
			offset += sizeof(uint32_t);
			log_info(loggerDev, "El peso de la IP es: %i", sizeIP);


			memcpy(stream + offset, ip_game_boy, sizeIP);
			offset += sizeIP;
			log_info(loggerDev, "Se mando la IP: %s", ip_game_boy);


			memcpy(stream + offset, &sizePuerto, sizeof(uint32_t));
			offset += sizeof(uint32_t);
			log_info(loggerDev, "El peso del Puerto es: %i",sizePuerto);

			memcpy(stream + offset, puerto_game_boy, sizePuerto);
			offset += sizePuerto;
			log_info(loggerDev, "Se mando el Puerto: %s", puerto_game_boy);

			bufferAux->stream = stream;

			log_info(loggerDev, "SE ENCHUFO BIEN EL BUFFER");

			break;


		default:
			log_error(loggerDev, "NO existe el mensaje ingresado");
			exit(0);
	}

	return bufferAux;
}

op_code enumTipoMensaje(char* mensaje){

	op_code tipoMensaje;
	uint32_t nroMensajeAEnviar = numeroPorTipoDeMensaje(mensaje);

	switch(nroMensajeAEnviar){
		case 1:
			tipoMensaje = NEW;
			break;

		case 2:
			tipoMensaje = APPEARED;
			break;

		case 3:
			tipoMensaje = CATCH;
			break;

		case 4:
			tipoMensaje = CAUGHT;
			break;

		case 5:
			tipoMensaje = GET;
			break;

		case 7:
			tipoMensaje = SUSCRIPTOR;
			break;
	}
	log_info(loggerDev, "Te mando el opcode: %i", tipoMensaje);

	return tipoMensaje;

}
/**********************************       ENVIAR MENSAJE       *********************************************** */
void enviar_mensaje(char* argv[], int socket_cliente){
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

	log_error(loggerDev, "Serializando...");
	void* flujo = serializar_paquete(paquete,&bytes);                   //  SERIALIZAR PAQUETE
	log_error(loggerDev, "Serializacion completa");


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

/**********************************       RECIBIR MENSAJE       *************************************************/


void desserializarNew(void* streamLlegada){
	t_new* mensajeNew = malloc(sizeof(mensajeNew));
	uint32_t idPropio;

	memcpy(&(idPropio),streamLlegada, sizeof(uint32_t));
	streamLlegada += sizeof(uint32_t);
	log_error(loggerGameBoy, "Llego con el ID del Broker: %i",idPropio);


//	#################		NOMBRE POKEMON		##################
	memcpy(&(mensajeNew->sizePokemon),streamLlegada, sizeof(uint32_t));
	streamLlegada += sizeof(uint32_t);
	memcpy(&(mensajeNew->pokemon), streamLlegada, sizeof(mensajeNew->sizePokemon));
	streamLlegada += sizeof(mensajeNew->sizePokemon);
	log_error(loggerGameBoy, "Llego el pokemon: %s",mensajeNew->sizePokemon);

//	#################		COORDENADAS		#####################
	memcpy(&(mensajeNew->coordX), streamLlegada, sizeof(uint32_t));
	streamLlegada += sizeof(uint32_t);
	log_error(loggerGameBoy, "Coordenada X: %i",mensajeNew->coordX);

	memcpy(&(mensajeNew->coordY), streamLlegada, sizeof(uint32_t));
	streamLlegada += sizeof(uint32_t);
	log_error(loggerGameBoy, "Coordenada Y: %i",mensajeNew->coordY);


//	################		CANTIDAD		#####################
	memcpy(&(mensajeNew->cantidad), streamLlegada, sizeof(uint32_t));
	log_error(loggerGameBoy, "Cantidad: %i",mensajeNew->cantidad);

	free(mensajeNew);

}


void deserializarAppeard(void* streamLlegada){
	t_appeared* mensajAppeared = malloc(sizeof(mensajAppeared));
	uint32_t idPropio;
	uint32_t idCorrelativo;

	memcpy(&(idPropio),streamLlegada, sizeof(uint32_t));
	streamLlegada += sizeof(uint32_t);
	log_error(loggerGameBoy, "Llego con el ID del Broker: %i",idPropio);

	memcpy(&(idCorrelativo),streamLlegada, sizeof(uint32_t));
	streamLlegada += sizeof(uint32_t);
	log_error(loggerGameBoy, "Llego con el ID Correlativo del Broker: %i",idPropio);



//	#################		NOMBRE POKEMON		##################
	memcpy(&(mensajAppeared->sizePokemon),streamLlegada, sizeof(uint32_t));
	streamLlegada += sizeof(uint32_t);
	memcpy(&(mensajAppeared->pokemon), streamLlegada, sizeof(mensajAppeared->sizePokemon));
	streamLlegada += sizeof(mensajAppeared->sizePokemon);
								log_error(loggerGameBoy, "Llego el pokemon: %s",mensajAppeared->sizePokemon);

//	#################		COORDENADAS		#####################
	memcpy(&(mensajAppeared->coordX), streamLlegada, sizeof(uint32_t));
	streamLlegada += sizeof(uint32_t);
	log_error(loggerGameBoy, "Coordenada X: %i",mensajAppeared->coordX);

	memcpy(&(mensajAppeared->coordY), streamLlegada, sizeof(uint32_t));
	streamLlegada += sizeof(uint32_t);
	log_error(loggerGameBoy, "Coordenada Y: %i",mensajAppeared->coordY);

	free(mensajAppeared);
}

void deserializarCatch(void* streamLlegada){
	t_catch* mensajeCatch = malloc(sizeof(mensajeCatch));
		uint32_t idPropio;

		memcpy(&(idPropio),streamLlegada, sizeof(uint32_t));
		streamLlegada += sizeof(uint32_t);
		log_error(loggerGameBoy, "Llego con el ID del Broker: %i",idPropio);


	//	#################		NOMBRE POKEMON		##################
		memcpy(&(mensajeCatch->sizePokemon),streamLlegada, sizeof(uint32_t));
		streamLlegada += sizeof(uint32_t);
		memcpy(&(mensajeCatch->pokemon), streamLlegada, sizeof(mensajeCatch->sizePokemon));
		streamLlegada += sizeof(mensajeCatch->sizePokemon);
		log_error(loggerGameBoy, "Llego el pokemon: %s",mensajeCatch->sizePokemon);

	//	#################		COORDENADAS		#####################
		memcpy(&(mensajeCatch->coordX), streamLlegada, sizeof(uint32_t));
		streamLlegada += sizeof(uint32_t);
		log_error(loggerGameBoy, "Coordenada X: %i",mensajeCatch->coordX);

		memcpy(&(mensajeCatch->coordY), streamLlegada, sizeof(uint32_t));
		streamLlegada += sizeof(uint32_t);
		log_error(loggerGameBoy, "Coordenada Y: %i",mensajeCatch->coordY);

		free(mensajeCatch);
}

void deserializarCaught(void* streamLlegada){
	t_caught* mensajeCaught = malloc(sizeof(mensajeCaught));
	uint32_t idPropio;
	uint32_t idCorrelativo;

	memcpy(&(idPropio),streamLlegada, sizeof(uint32_t));
	streamLlegada += sizeof(uint32_t);
	log_error(loggerGameBoy, "Llego con el ID del Broker: %i",idPropio);

	memcpy(&(idCorrelativo),streamLlegada, sizeof(uint32_t));
	streamLlegada += sizeof(uint32_t);
	log_error(loggerGameBoy, "Llego con el ID Correlativo del Broker: %i",idPropio);


//	#################		FALG		##################
	memcpy(&(mensajeCaught->flagCaught),streamLlegada, sizeof(uint32_t));
	log_error(loggerGameBoy, "Llego con el Flag: %i",mensajeCaught->flagCaught);
	streamLlegada += sizeof(uint32_t);


	free(mensajeCaught);
}


void deserializarLocalized(void* streamLlegada){

	t_localized* mensajeLocalized = malloc(sizeof(mensajeLocalized));
	uint32_t idPropio;
	uint32_t idCorrelativo;

	memcpy(&(idPropio),streamLlegada, sizeof(uint32_t));
	streamLlegada += sizeof(uint32_t);
	log_error(loggerGameBoy, "Llego con el ID del Broker: %i",idPropio);

	memcpy(&(idCorrelativo),streamLlegada, sizeof(uint32_t));
	streamLlegada += sizeof(uint32_t);
	log_error(loggerGameBoy, "Llego con el ID Correlativo del Broker: %i",idPropio);



//				#################		NOMBRE POKEMON		##################
	memcpy(&(mensajeLocalized->sizePokemon),streamLlegada, sizeof(uint32_t));
	streamLlegada += sizeof(uint32_t);
	memcpy(&(mensajeLocalized->pokemon), streamLlegada, sizeof(mensajeLocalized->sizePokemon));
	streamLlegada += sizeof(mensajeLocalized->sizePokemon);
	log_error(loggerGameBoy, "Llego el pokemon: %s",mensajeLocalized->sizePokemon);


	uint32_t cantidad;
	memcpy(&(cantidad),streamLlegada, sizeof(uint32_t));
	streamLlegada += sizeof(uint32_t);
	log_error(loggerGameBoy, "Llegaron %i del pokemon del Broker",cantidad);

	for(uint32_t i = 0; i<cantidad; i++){
		uint32_t posX;
		uint32_t posY;

		memcpy(&(posX),streamLlegada, sizeof(uint32_t));
		streamLlegada += sizeof(uint32_t);

		memcpy(&(posY),streamLlegada, sizeof(uint32_t));
		streamLlegada += sizeof(uint32_t);

		log_error(loggerGameBoy, "Posicion X: %i",posX);
		log_error(loggerGameBoy, "Posicion Y: %i",posY);
	}




	//
/*/				#################		COORDENADAS		#####################
	memcpy(&(mensajeLocalized->coordX), streamLlegada, sizeof(uint32_t));
	streamLlegada += sizeof(uint32_t);
	log_error(loggerGameBoy, "Coordenada X: %i",mensajeLocalized->coordX);

	memcpy(&(mensajeLocalized->coordY), streamLlegada, sizeof(uint32_t));
	streamLlegada += sizeof(uint32_t);
	log_error(loggerGameBoy, "Coordenada Y: %i",mensajeLocalized->coordY);
*/
	free(mensajeLocalized);


}


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
				log_error(loggerGameBoy, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);
				desserializarNew(paquete->buffer->stream);
				break;


			case APPEARED:
				log_error(loggerGameBoy, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);
				deserializarAppeard(paquete->buffer->stream);
				break;

			case CATCH:
				log_error(loggerGameBoy, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);
				deserializarCatch(paquete->buffer->stream);
				break;

			case CAUGHT:
				log_error(loggerGameBoy, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);
				deserializarCaught(paquete->buffer->stream);
				break;

			case GET:
				log_error(loggerGameBoy, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);

				t_get* mensajeGet = malloc(sizeof(mensajeGet));
				uint32_t idPropio;

				memcpy(&(idPropio),paquete->buffer->stream, sizeof(uint32_t));
				paquete->buffer->stream += sizeof(uint32_t);
				log_error(loggerGameBoy, "Llego con el ID del Broker: %i",idPropio);


//				#################		NOMBRE POKEMON		##################
				memcpy(&(mensajeGet->sizePokemon),paquete->buffer->stream, sizeof(uint32_t));
				paquete->buffer->stream += sizeof(uint32_t);
				memcpy(&(mensajeGet->pokemon), paquete->buffer->stream, sizeof(mensajeGet->sizePokemon));
				log_error(loggerGameBoy, "Llego el pokemon: %s",mensajeGet->sizePokemon);

				free(mensajeGet);

				break;


			case LOCALIZED:
				log_error(loggerGameBoy, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);
				deserializarLocalized(paquete->buffer->stream);
				break;

			default:
				log_error(loggerDev, "Me mandaron cualquier cosa");
		}

	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);


}

void devolver_ack(int socket_cliente) {
	uint32_t ack = 1;
	send(socket_cliente, (void *) &ack, sizeof(uint32_t), 0);
}

void recibirMensajesDeSuscripcion(int socketCliente){
	while(1){
		recibir_mensaje(socketCliente);
		devolver_ack(socketCliente);
	}
}

void liberar_conexion(int socket_cliente)
{
	close(socket_cliente);
}
