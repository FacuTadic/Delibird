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
#include "utilsGameCard.h"
#include<commons/config.h>
#include<commons/string.h>

t_paquete* crearPaquete(t_buffer* buffer);
void* serializar_paquete(t_paquete* paquete, uint32_t *bytes);
t_buffer* crearBufferAppeared(t_appeared* appearedAEnviar);
t_buffer* crearBufferCaught(t_caught* caughtAEnviar);
t_buffer* crearBufferLocalized(t_localized* localizedAEnviar);
t_localized* crearLocalizedDePokemonInexistente(t_getLlegada* getLlegado);
t_caught* crearCaught(uint32_t idMensaje, uint32_t flag);
t_appeared* crearAppeared(t_newLlegada* new);

#endif /* CREACIONDEESTRUCTURASPARAENVIO_H_ */
