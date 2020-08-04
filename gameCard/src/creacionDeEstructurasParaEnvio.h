#ifndef CREACIONDEESTRUCTURASPARAENVIO_H_
#define CREACIONDEESTRUCTURASPARAENVIO_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<signal.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/config.h>
#include<commons/string.h>
#include<commons/collections/queue.h>
#include "protocoloGameCard.h"


t_log* loggerDevEstructuras;

t_paquete* crearPaquete(t_buffer* buffer);
void* serializar_paquete(t_paquete* paquete, uint32_t *bytes);
t_buffer* crearBufferSuscriptor(t_suscripcion* suscriptorAEnviar);
t_buffer* crearBufferAppeared(t_appeared* appearedAEnviar);
t_buffer* crearBufferCaught(t_caught* caughtAEnviar);
t_buffer* crearBufferLocalized(t_localized* localizedAEnviar);
t_localized* crearLocalizedDePokemonInexistente(t_getLlegada* getLlegado);
t_appeared* crearAppeared(t_newLlegada* new);
t_caught* crearCaught(uint32_t idMensaje, bool flag);
t_localized* crearLocalizedDePokemonInexistente(t_getLlegada* getLlegado);
t_localized* crearLocalized(t_getLlegada* getLlegado, t_list* dataPokemon);

#endif /* CREACIONDEESTRUCTURASPARAENVIO_H_ */
