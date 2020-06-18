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
	t_paquete* paquete = malloc(sizeof(t_paquete));
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


void agregarIDMensaje(void* stream,uint32_t id, uint32_t* offset){
	memcpy(stream+*(offset),&id,sizeof(uint32_t));
	*offset += sizeof(uint32_t);
	log_info(loggerDev, "El ID de Mensaje copiado es %i", id);
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


	//Variable para el switch
	log_info(loggerDev, "MENSAJE PARA ENVIAR ES: %s",argv[2]);

	uint32_t nroMensaje = numeroPorTipoDeMensaje(argv[2]);


	log_info(loggerDev, "EL NUMERO DE MENSAJE ES: %i",nroMensaje);




	switch(nroMensaje){

		//Commons Dictionary Hash

		case 1 : //NEW

			bufferAux->size = 4*sizeof(uint32_t) + strlen(argv[3])+1; // ,sizeNombre, Cord X,Y , Cantidad + NOMBRE

			//CASO DE AGREGAR EL ID_MENSAJE
			//Comparacion de strings commons
			if(!strcmp(argv[1],"GAMECARD")){
				bufferAux->size += sizeof(uint32_t);
			}

			stream = malloc(bufferAux->size);


			if(!strcmp(argv[1],"GAMECARD")){
				idMensaje = atoi(argv[7]);
				log_info(loggerDev, "El ID convertido es %i", idMensaje);
				agregarIDMensaje(stream,idMensaje,&offset);
				log_info(loggerDev, "EL offset quedo en: %i", offset);
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
				log_info(loggerDev, "El ID convertido es %i", idMensaje);
				agregarIDMensaje(stream,idMensaje,&offset);
				log_info(loggerDev, "EL offset quedo en: %i", offset);
			}

			sizeNombrePokemon = strlen(argv[3]) + 1;
			memcpy(stream + offset,&sizeNombrePokemon, sizeof(uint32_t));
			offset += sizeof(uint32_t);
			log_info(loggerDev, "Tamanio nombre pokemon %i", sizeNombrePokemon);


			memcpy(stream + offset,argv[3],strlen(argv[3]) + 1);
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


			bufferAux->stream = stream;

			break;

		case 3: //CATCH

			bufferAux->size = 3*sizeof(uint32_t) + strlen(argv[3])+1;// sizeNombre, Cord X,Y + NOMBRE

			//CASO DE AGREGAR EL ID_MENSAJE
			if(!strcmp(argv[1],"GAMECARD")){
				bufferAux->size += sizeof(uint32_t);
			}

			stream = malloc(bufferAux->size);

			if(!strcmp(argv[1],"GAMECARD")){
				idMensaje = atoi(argv[6]);
				log_info(loggerDev, "El ID convertido es %i", idMensaje);
				agregarIDMensaje(stream,idMensaje,&offset);
				log_info(loggerDev, "EL offset quedo en: %i", offset);
			}


			sizeNombrePokemon = strlen(argv[3]) + 1;
			memcpy(stream + offset,&sizeNombrePokemon, sizeof(uint32_t));
			offset += sizeof(uint32_t);
			log_info(loggerDev, "Tamanio nombre pokemon %i", sizeNombrePokemon);


			memcpy(stream + offset,argv[3],strlen(argv[3]) + 1);
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



			bufferAux->stream = stream;

			break;

		case 4:  //CAUGHT
			bufferAux->size = 2*sizeof(uint32_t); // ID_MENSAJE + FLAG

			stream = malloc(bufferAux->size);

			idMensaje = atoi(argv[3]);
			log_info(loggerDev, "El ID convertido es %i", idMensaje);
			agregarIDMensaje(stream,idMensaje,&offset);
			log_info(loggerDev, "EL offset quedo en: %i", offset);

			uint32_t flag = flagCaught(argv[4]);

			if (flag == -1){
				log_error(loggerDev, "NO existe el flag enviado");
				exit(-1);
			}

			memcpy(stream + offset,&flag, sizeof(uint32_t));
			offset += sizeof(uint32_t);
			log_info(loggerDev, "El FLAG copiado es %i", flag);


			bufferAux->stream = stream;

			break;


		case 5:  //GET

			bufferAux->size = sizeof(uint32_t) + strlen(argv[3])+1; // sizeNombre


			//CASO DE AGREGAR EL ID_MENSAJE
			if(!strcmp(argv[1],"GAMECARD")){
				bufferAux->size += sizeof(uint32_t);
			}

			stream = malloc(bufferAux->size);


			if(!strcmp(argv[1],"GAMECARD")){
				idMensaje = atoi(argv[6]);
				log_info(loggerDev, "El ID convertido es %i", idMensaje);
				agregarIDMensaje(stream,idMensaje,&offset);
				log_info(loggerDev, "EL offset quedo en: %i", offset);
			}


			uint32_t sizeNombre = strlen(argv[3])+1;


			memcpy (stream + offset, &sizeNombre, sizeof(uint32_t));
			offset += sizeof(uint32_t);
			log_info(loggerDev, "Tamanio nombre pokemon %i", sizeNombre);

			memcpy (stream + offset, argv[3], strlen(argv[3])+1);
			offset += sizeNombre;
			log_info(loggerDev, "Nombre pokemon %s", argv[3]);

			bufferAux->stream = stream;

			break;


		case 7: //SUSCRIPTOR

			bufferAux->size = 2*sizeof(uint32_t); // Cola de Mensajes (representada por protocolo)

			stream = malloc(bufferAux->size);

			memcpy(stream,&idGameBoy, sizeof(uint32_t));
			offset += sizeof(uint32_t);
			log_info(loggerDev, "El ID de Modulo es %i", idGameBoy);

			log_info(loggerDev, "EL offset quedo en: %i", offset);

			log_info(loggerDev, "LA COLA DE MENSAJE ES: %s", argv[3]);


			uint32_t colaDeMensajesSolicitada = idColaPorTipoDeMensaje(argv[3]);
			memcpy(stream + offset, &colaDeMensajesSolicitada, sizeof(uint32_t));
			offset += sizeof(uint32_t);
			log_info(loggerDev, "EL NUMERO DE LA COLA ES: %i", colaDeMensajesSolicitada);


			bufferAux->stream = stream;

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



void recibir_mensaje(int socket_cliente)
{
	log_info(loggerDev, "yendo a buscar el codigo de operacion");

	t_paquete* paquete = malloc(sizeof(t_paquete));
	int status_recv = recv(socket_cliente, &(paquete->codigo_operacion) ,sizeof(uint32_t), MSG_WAITALL);
	if (status_recv == -1){
		log_error(loggerDev, "error: No se recibió el código de operación");
		liberar_conexion(socket_cliente);
		pthread_exit(NULL);
	}
	if (status_recv == 0){
		log_warning(loggerDev, "EL BROKER MATO LA CONEXION CARAJO");
		liberar_conexion(socket_cliente);
		pthread_exit(NULL);
	}

	uint32_t* size;

	if (recv(socket_cliente, &(size), sizeof(uint32_t),MSG_WAITALL) == -1){
		log_error(loggerDev, "error: No se recibió el tañaño del buffer");
		liberar_conexion(socket_cliente);
		pthread_exit(NULL);
	}

	switch(paquete->codigo_operacion){
			case NEW:
				log_info(loggerDev, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);
				log_info(loggerGameBoy, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);
				recibir_new(socket_cliente,size,loggerDev);
				break;


			case APPEARED:
				log_info(loggerGameBoy, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);
				recibir_appeared(socket_cliente,size,loggerDev);
				break;

			case CATCH:
				log_info(loggerGameBoy, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);
				recibir_catch(socket_cliente,size,loggerDev);
				break;

			case CAUGHT:
				log_info(loggerGameBoy, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);
				recibir_caught(socket_cliente,size,loggerDev);
				break;

			case GET:
				log_info(loggerGameBoy, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);
				recibir_get(socket_cliente,size,loggerDev);
				break;


			case LOCALIZED:
				log_info(loggerGameBoy, "Se recibio un mensaje de tipo %i",paquete->codigo_operacion);
				recibir_localized(socket_cliente,size,loggerDev);
				break;

			default:
				log_error(loggerDev, "Me mandaron cualquier cosa");
		}

	free(paquete);

}

void devolver_ack(int socket_cliente) {
	uint32_t ack = 1;
	int status_send = send(socket_cliente, (void *) &ack, sizeof(uint32_t), 0);
	if (status_send == -1) {
		log_error(loggerDev, "Error: no se pudo enviar el ACK al socket %i", socket_cliente);
		liberar_conexion(socket_cliente);
		pthread_exit(NULL);
	} else if (status_send == 0) {
		log_warning(loggerDev, "Broker ha cerrado el socket y no he podido enviar el ACK al socket %i", socket_cliente);
		liberar_conexion(socket_cliente);
		pthread_exit(NULL);
	}
	log_info(loggerGameBoy, "Devuelve acknowledgement ");

}

void recibirMensajesDeSuscripcion(int* socketCliente){
	while(1){
		recibir_mensaje(*socketCliente);
		devolver_ack(*socketCliente);
	}
}

void liberar_conexion(int socket_cliente)
{
	log_info(loggerGameBoy, "Cerrando la conexion: %i",socket_cliente);
	close(socket_cliente);
	log_info(loggerGameBoy, "Conexion cerrada");

}
