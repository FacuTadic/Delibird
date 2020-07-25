#include "creacionDeEstructurasParaEnvio.h"

t_paquete* crearPaquete(t_buffer* buffer){
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = buffer;
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
	log_info(loggerDevEstructuras, "El ID cargado es %i", idMensaje);


	memcpy(bufferAppeared->stream + offset,&sizeNombrePokemon, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDevEstructuras, "Tamanio nombre pokemon %i", sizeNombrePokemon);


	memcpy(bufferAppeared->stream + offset,nombrePokemon,strlen(nombrePokemon) + 1);
	offset += sizeNombrePokemon;
	log_info(loggerDevEstructuras, "Nombre pokemon %s", nombrePokemon);


	memcpy(bufferAppeared->stream + offset,&posX, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDevEstructuras, "Posicion X %i", posX);


	memcpy(bufferAppeared->stream + offset,&posY, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDevEstructuras, "Posicion Y %i", posY);

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
	log_info(loggerDevEstructuras, "El ID cargado es %i", idMensaje);


	memcpy(bufferCaught->stream + offset,&flag, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDevEstructuras, "El flag cargado es %i", flag);

	return bufferCaught;
}

t_buffer* crearBufferLocalized(t_localized* localizedAEnviar){
	t_buffer* bufferLocalized = malloc(sizeof(t_buffer));
	uint32_t offset = 0;
	uint32_t idMensaje = localizedAEnviar->idMensaje;
	uint32_t sizeNombrePokemon = localizedAEnviar->sizePokemon;
	char* nombrePokemon = localizedAEnviar->pokemon;
	uint32_t cantLugares = localizedAEnviar->lugares;


	bufferLocalized->size = 3*sizeof(uint32_t) + strlen(nombrePokemon)+1 + localizedAEnviar->l_coordenadas->elements_count * sizeof(uint32_t); // sizeNombre, Cord X,Y + NOMBRE
	bufferLocalized->stream = malloc(bufferLocalized->size);

	memcpy(bufferLocalized->stream + offset,&idMensaje, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDevEstructuras, "El ID cargado es %i", idMensaje);


	memcpy(bufferLocalized->stream + offset,&sizeNombrePokemon, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDevEstructuras, "Tamanio nombre pokemon %i", sizeNombrePokemon);


	memcpy(bufferLocalized->stream + offset,nombrePokemon,strlen(nombrePokemon) + 1);
	offset += sizeNombrePokemon;
	log_info(loggerDevEstructuras, "Nombre pokemon %s", nombrePokemon);

	memcpy(bufferLocalized->stream + offset,&cantLugares, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDevEstructuras, "Cantidad de coordenadas donde aparece:  %i", cantLugares);


	for(int i = 0; i<(cantLugares*2); i++){

		uint32_t posX = *((uint32_t*) list_get(localizedAEnviar->l_coordenadas, i));
		log_info(loggerDevEstructuras, "Posicion X previa al mem %i", posX);

		i++;

		uint32_t posY = *((uint32_t*) list_get(localizedAEnviar->l_coordenadas, i));
		log_info(loggerDevEstructuras, "Posicion Y previa al mem %i", posY);




		memcpy(bufferLocalized->stream + offset,&posX, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		log_info(loggerDevEstructuras, "Posicion X %i", posX);

		memcpy(bufferLocalized->stream + offset,&posY, sizeof(uint32_t));
		offset += sizeof(uint32_t);
		log_info(loggerDevEstructuras, "Posicion Y %i", posY);

	}


	log_info(loggerDevEstructuras, "SAliendo");

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
	log_info(loggerDevEstructuras, "El ID de GC cargado es %i", idCliente);


	memcpy(bufferSuscriptor->stream + offset,&idCola, sizeof(uint32_t));
	offset += sizeof(uint32_t);
	log_info(loggerDevEstructuras, "El ID de la cola que quiero escuchar es %i", idCliente);


	return bufferSuscriptor;
}


t_appeared* crearAppeared(t_newLlegada* new){
	uint32_t idMensaje= new->id;
	char* pokemon = new->pokemon;
	uint32_t tamanio = strlen(pokemon)+1;
	uint32_t posX = new->pos_X;
	uint32_t posY = new->pos_Y;


	t_appeared* appearedAEnviar = malloc(sizeof(t_appeared));

	appearedAEnviar->idMensaje = idMensaje;
	log_info(loggerDevEstructuras, "El ID de mensaje cargado en el appeared es: %i", appearedAEnviar->idMensaje);

	appearedAEnviar->sizePokemon = tamanio;
	appearedAEnviar->pokemon = pokemon;

	log_info(loggerDevEstructuras, "El pokemoncargado en el appeared es: %s", appearedAEnviar->pokemon);
	log_info(loggerDevEstructuras, "El tamanio del nombre del pokemon cargado en el appeared es: %i", appearedAEnviar->sizePokemon);

	appearedAEnviar->coordX = posX;
	appearedAEnviar->coordY = posY;

	log_info(loggerDevEstructuras, "La coordenada del pokemon cargado en el appeared es: %i - %i", appearedAEnviar->coordX, appearedAEnviar->coordY);

	return appearedAEnviar;
}


uint32_t boolAInt(bool flag){
	if(flag){
		return 1;
	}else{
		return 0;
	}
}

t_caught* crearCaught(uint32_t idMensaje, bool flag){

	t_caught* caughtAEnviar = malloc(sizeof(t_caught));

	caughtAEnviar->idMensaje = idMensaje;
	log_info(loggerDevEstructuras, "El ID de mensaje cargado en el caught es: %i", caughtAEnviar->idMensaje);
	caughtAEnviar->flagCaught = boolAInt(flag);
	log_info(loggerDevEstructuras, "El Flag cargado en el caught es: %i", caughtAEnviar->flagCaught);

	return caughtAEnviar;
}


t_localized* crearLocalizedDePokemonInexistente(t_getLlegada* getLlegado){

	t_localized* localizedAEnviar = malloc(sizeof(t_localized));

	localizedAEnviar->idMensaje = getLlegado->id;
	localizedAEnviar->sizePokemon = strlen(getLlegado->pokemon);
	localizedAEnviar->pokemon = getLlegado->pokemon;

	return localizedAEnviar;
}

t_localized* crearLocalized(t_getLlegada* getLlegado, t_queue* dataPokemon){

	t_localized* localizedAEnviar = malloc(sizeof(t_localized));

	localizedAEnviar->idMensaje = getLlegado->id;
	log_info(loggerDevEstructuras, "El ID mensaje cargado en el localized es: %i", localizedAEnviar->idMensaje);
	localizedAEnviar->sizePokemon = strlen(getLlegado->pokemon);
	localizedAEnviar->pokemon = getLlegado->pokemon;
	log_info(loggerDevEstructuras, "El Pokemon cargado en el localized es: %s", localizedAEnviar->pokemon);
	log_info(loggerDevEstructuras, "El tamanio del nombre del pokemon cargado en el localized es: %i", localizedAEnviar->sizePokemon);
	localizedAEnviar->lugares = queue_size(dataPokemon);
	log_info(loggerDevEstructuras, "La cantidad de lugares donde apareche el pokemon cargado en el localized es: %i", localizedAEnviar->lugares);
	localizedAEnviar->l_coordenadas = list_create();


	for(int i = 0; i<localizedAEnviar->lugares ;i++){

		char* coordenada = queue_pop(dataPokemon);
		log_info(loggerDevEstructuras, "La coordenada sacada de la cola es: %s", coordenada);

		char** data = string_n_split(coordenada,2,"-");
		uint32_t posX = atoi(data[0]);
		log_info(loggerDevEstructuras, "La coordenada sacada de la cola es: %i", posX);
		uint32_t posY = atoi(data[1]);
		log_info(loggerDevEstructuras, "La coordenada sacada de la cola es: %i", posY);




		uint32_t* segundaX=malloc(sizeof(uint32_t));
		uint32_t* segundaY=malloc(sizeof(uint32_t));

		memcpy(segundaX,&posX,sizeof(uint32_t));
		memcpy(segundaY,&posY,sizeof(uint32_t));


		list_add(localizedAEnviar->l_coordenadas, segundaX);
		list_add(localizedAEnviar->l_coordenadas, segundaY);

		log_info(loggerDevEstructuras, "La coordenada cargada es (%i ; %i)", posX, posY);

	}

	for(int j = 0; j<localizedAEnviar->l_coordenadas->elements_count ;j++){

		log_info(loggerDevEstructuras, "El valor de index %i es: %i",j, *((uint32_t*) list_get(localizedAEnviar->l_coordenadas,j)));

	}


	return localizedAEnviar;

}

