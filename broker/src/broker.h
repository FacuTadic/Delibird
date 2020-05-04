/*
 * broker.h
 *
 *  Created on: 3 may. 2020
 *      Author: utnso
 */

#ifndef BROKER_H_
#define BROKER_H_

#include "utilsBroker.h"
#include "commons/collections/queue.h"
#include "commons/log.h"
#include "commons/config.h"
#include "pthread.h"
#include "semaphore.h"

#endif /* BROKER_H_ */

t_log* iniciar_logger(char* log_file);
t_config* leer_config(void);
t_config* leer_config(void);
void inicializar_colas(void);
void terminar_programa(t_log* logger, t_config* config);

void* recibir_new(int socket_cliente, int* size);
void* recibir_appeared(int socket_cliente, int* size);
void* recibir_catch(int socket_cliente, int* size);
void* recibir_caught(int socket_cliente, int* size);
void* recibir_get(int socket_cliente, int* size);
void* recibir_localized(int socket_cliente, int* size);
