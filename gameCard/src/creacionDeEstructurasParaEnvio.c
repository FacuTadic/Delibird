#include "creacionDeEstructurasParaEnvio.h"

t_paquete* crearPaquete(t_buffer* buffer){
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(buffer->size + sizeof(buffer->size));
	memcpy (paquete->buffer,  buffer, buffer->size + sizeof(uint32_t));
	return paquete;
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


t_buffer* crearBufferAppeared(t_appeared* appearedAEnviar){
	t_buffer* bufferAppeared = malloc(sizeof(t_buffer));
	uint32_t offset = 0;
	uint32_t idMensaje = appearedAEnviar->idMensaje;
	uint32_t sizeNombrePokemon = appearedAEnviar->sizePokemon;
	char* nombrePokemon = appearedAEnviar->pokemon;
	uint32_t posX = appearedAEnviar->coordX;
	uint32_t posY = appearedAEnviar->coordY;

	bufferAppeared->size = 4*sizeof(uint32_t) + strlen(nombrePokemon)+1; // sizeNombre, Cord X,Y + NOMBRE
	bufferAppeared->stream = malloc(bufferAppeared->size);

	memcpy(bufferAppeared->stream + offset,&idMensaje, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDev, "El ID cargado es %i", idMensaje);


	memcpy(bufferAppeared->stream + offset,&sizeNombrePokemon, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDev, "Tamanio nombre pokemon %i", sizeNombrePokemon);


	memcpy(bufferAppeared->stream + offset,nombrePokemon,strlen(nombrePokemon) + 1);
	offset += sizeNombrePokemon;
	log_info(loggerDev, "Nombre pokemon %s", nombrePokemon);


	memcpy(bufferAppeared->stream + offset,&posX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDev, "Posicion X %i", posX);


	memcpy(bufferAppeared->stream + offset,&posY, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDev, "Posicion Y %i", posY);

	return bufferAppeared;
}


t_buffer* crearBufferCaught(t_caught* caughtAEnviar){
	t_buffer* bufferCaught = malloc(sizeof(t_buffer));
	uint32_t offset = 0;
	uint32_t idMensaje = caughtAEnviar->idMensaje;
	uint32_t flag = caughtAEnviar->flagCaught;

	bufferCaught->size = 2*sizeof(uint32_t);
	bufferCaught->stream = malloc(bufferCaught->size);

	memcpy(bufferCaught->stream + offset,&idMensaje, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDev, "El ID cargado es %i", idMensaje);


	memcpy(bufferCaught->stream + offset,&flag, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDev, "El flag cargado es %i", flag);

	return bufferCaught;
}

t_buffer* crearBufferLocalized(t_localized* localizedAEnviar){
	t_buffer* bufferLocalized = malloc(sizeof(t_buffer));
	uint32_t offset = 0;
	uint32_t idMensaje = localizedAEnviar->idMensaje;
	uint32_t sizeNombrePokemon = localizedAEnviar->sizePokemon;
	char* nombrePokemon = localizedAEnviar->pokemon;
	uint32_t posX = localizedAEnviar->coordX;
	uint32_t posY = localizedAEnviar->coordY;

	bufferLocalized->size = 4*sizeof(uint32_t) + strlen(nombrePokemon)+1; // sizeNombre, Cord X,Y + NOMBRE
	bufferLocalized->stream = malloc(bufferLocalized->size);

	memcpy(bufferLocalized->stream + offset,&idMensaje, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDev, "El ID cargado es %i", idMensaje);


	memcpy(bufferLocalized->stream + offset,&sizeNombrePokemon, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDev, "Tamanio nombre pokemon %i", sizeNombrePokemon);


	memcpy(bufferLocalized->stream + offset,nombrePokemon,strlen(nombrePokemon) + 1);
	offset += sizeNombrePokemon;
	log_info(loggerDev, "Nombre pokemon %s", nombrePokemon);


	memcpy(bufferLocalized->stream + offset,&posX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDev, "Posicion X %i", posX);


	memcpy(bufferLocalized->stream + offset,&posY, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDev, "Posicion Y %i", posY);

	return bufferLocalized;
}

t_buffer* crearBufferSuscriptor(t_suscripcion* suscriptorAEnviar){
	t_buffer* bufferSuscriptor = malloc(sizeof(t_buffer));
	uint32_t offset = 0;
	uint32_t idCliente = suscriptorAEnviar->id_cliente;
	uint32_t idCola = suscriptorAEnviar->id_cola;

	bufferSuscriptor->size = 2*sizeof(uint32_t); // idCola y Id cliente
	bufferSuscriptor->stream = malloc(bufferSuscriptor->size);

	memcpy(bufferSuscriptor->stream + offset,&idCliente, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDev, "El ID de GC cargado es %i", idCliente);


	memcpy(bufferSuscriptor->stream + offset,&idCola, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDev, "El ID de la cola que quiero escuchar es %i", idCliente);


	return bufferSuscriptor;
}


t_appeared* crearAppeared(t_newLlegada* new){
	uint32_t idMensaje= new->id;
	char* pokemon = new->pokemon;
	uint32_t tamanio = strlen(pokemon)+1;
	uint32_t posX = new->pos_X;
	uint32_t posY = new->pos_Y;
	uint32_t cantidad = new->cantidad;

	t_appeared* appearedAEnviar = malloc(sizeof(t_appeared));

	appearedAEnviar->idMensaje = idMensaje;
	appearedAEnviar->sizePokemon = tamanio;
	appearedAEnviar->pokemon = pokemon;
	appearedAEnviar->coordX = posX;
	appearedAEnviar->coordY = posY;

	return appearedAEnviar;
}


uint32_t boolAInt(bool flag){
	if(flag){
		return 1;
	}else{
		return 0;
	}
}

t_caught* crearCaught(uint32_t idMensaje, uint32_t flag){

	t_caught* caughtAEnviar = malloc(sizeof(t_caught));

	caughtAEnviar->idMensaje = idMensaje;
	caughtAEnviar->flagCaught = boolAInt(flag);

	return caughtAEnviar;
}


t_localized* crearLocalizedDePokemonInexistente(t_getLlegada* getLlegado){

	t_localized* localizedAEnviar = malloc(sizeof(t_localized));

	localizedAEnviar->idMensaje = getLlegado->id;
	localizedAEnviar->pokemon = getLlegado->pokemon;

	return localizedAEnviar;
}

