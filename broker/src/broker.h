#ifndef BROKER_H_
#define BROKER_H_

#include "utilsBroker.h"
#include "commons/collections/queue.h"
#include "commons/log.h"
#include "commons/config.h"
#include "pthread.h"
#include "semaphore.h"
#include "broker_msj_id.h"

#endif /* BROKER_H_ */

t_log* iniciar_logger(char* log_file);
t_config* leer_config(void);
t_config* leer_config(void);
void inicializar_colas(void);
void terminar_programa(t_config* config);
