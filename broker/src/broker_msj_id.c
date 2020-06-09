#include "utilsBroker.h"
#include "commons/collections/queue.h"
#include "commons/log.h"
#include "commons/config.h"
#include "pthread.h"
#include "semaphore.h"
#include "broker_msj_id.h"


uint32_t id = 1;
pthread_mutex_t lock;

void crea_generador_id(void){
	pthread_mutex_init(&lock, NULL);
}

void destruir_generador_id(void){
	pthread_mutex_destroy(&lock);
}

uint32_t generar_id(void){
	pthread_mutex_lock(&lock);
	uint32_t id_aux = id;
	id++;
	pthread_mutex_unlock(&lock);
	return id_aux;

}

uint32_t obtener_id_actual(void) {
	return id;
}

