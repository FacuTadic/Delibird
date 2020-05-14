/*
 * Cliente.h
 *
 *  Created on: 28 feb. 2019
 *      Author: utnso
 */

#ifndef TP0_H_
#define TP0_H_

#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include <commons/collections/queue.h>
#include <commons/collections/list.h>

#include "utils.h"


t_log* iniciar_logger(void);
t_config* leer_config(void);
t_config* cargarConfigEntrenadores(void);
t_config* leer_config_broker(void);
void terminar_programa(int, t_log*, t_config*, t_config*);


#endif /* TP0_H_ */
