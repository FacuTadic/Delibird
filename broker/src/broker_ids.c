#include "utilsBroker.h"
#include "commons/collections/queue.h"
#include "commons/log.h"
#include "commons/config.h"
#include "pthread.h"
#include "semaphore.h"

#include "broker_ids.h"

uint32_t id_mensaje = 1;
pthread_mutex_t lock_mensaje;

int id_temporal = 1;
pthread_mutex_t lock_temporal;

int id_modificacion = 1;
pthread_mutex_t lock_modificacion;

void crea_generador_id(void){
	pthread_mutex_init(&lock_mensaje, NULL);
	pthread_mutex_init(&lock_temporal, NULL);
	pthread_mutex_init(&lock_modificacion, NULL);
}

void destruir_generador_id(void){
	pthread_mutex_destroy(&lock_mensaje);
	pthread_mutex_destroy(&lock_temporal);
	pthread_mutex_destroy(&lock_modificacion);
}

uint32_t generar_id(void){
	pthread_mutex_lock(&lock_mensaje);
	uint32_t id_aux = id_mensaje;
	id_mensaje++;
	pthread_mutex_unlock(&lock_mensaje);
	return id_aux;
}

int generar_id_temporal(void){
	pthread_mutex_lock(&lock_temporal);
	int id_aux = id_temporal;
	id_temporal++;
	pthread_mutex_unlock(&lock_temporal);
	return id_aux;
}

int generar_id_modificacion(void){
	pthread_mutex_lock(&lock_modificacion);
	int id_aux = id_modificacion;
	id_modificacion++;
	pthread_mutex_unlock(&lock_modificacion);
	return id_aux;
}

uint32_t obtener_id_actual(void) {
	return id_mensaje;
}
