#ifndef MEMORIA_BROKER_H_
#define MEMORIA_BROKER_H_

#include "commons/collections/dictionary.h"
#include "commons/collections/list.h"
#include "commons/log.h"
#include<netdb.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<limits.h>
#include "pthread.h"
#include "broker_msj_id.h"

#endif

t_log* extense_logger_memoria;
t_log* logger_memoria;

void* memoria;

t_dictionary* tabla_segmentos;

t_list* particiones_libres;

// FF 1
// BF 2
int algoritmo_particion_libre;

pthread_mutex_t mutex_memoria;

int tamanio_minimo_particion;

typedef struct {
	uint32_t id;
	uint32_t id_correlativo;
	int tipo;
	t_list* envios;
	t_list* acknowledgements;
	int limit;
	void* base;
} data_tabla; // registro de la tabla

typedef struct {
	void* desde;
	void* hasta;
} particion_libre; // elemento de la lista de particiones libres

typedef struct {
	data_tabla* registro;
	void* mensaje;
} segmento_memoria; // elemento que agrupa todos los datos de un mensaje en memoria

void guardar_info_envios(uint32_t id, t_list* mandados, t_list* acks);
void guardar_mensaje_en_memoria(data_tabla* registro, void* mensaje);
particion_libre* encontrar_lugar_con_ff(int limit);
particion_libre* encontrar_lugar_con_bf(int limit);
void* encontrar_lugar_en_memoria(int limit);
int tamanio_particion_libre(particion_libre* particion);
void ocupar_lugar_en_particiones_libres(particion_libre* particion, int limit);
t_list* obtener_segmentos_new(uint32_t id_cliente); // lista de segmento_memoria
t_list* obtener_segmentos_appeared(uint32_t id_cliente); // lista de segmento_memoria
t_list* obtener_segmentos_get(uint32_t id_cliente); // lista de segmento_memoria
t_list* obtener_segmentos_localized(uint32_t id_cliente); // lista de segmento_memoria
t_list* obtener_segmentos_catch(uint32_t id_cliente); // lista de segmento_memoria
t_list* obtener_segmentos_caught(uint32_t id_cliente); // lista de segmento_memoria














