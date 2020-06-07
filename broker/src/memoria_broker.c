#include "memoria_broker.h"

void guardar_info_envios(uint32_t id, t_list* mandados, t_list* acks) {
	char* id_char = malloc(sizeof(char));
	sprintf(id_char, "%u", id);

	pthread_mutex_lock(&mutex_memoria);

	data_tabla* registro_tabla = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

	registro_tabla->envios = mandados;
	registro_tabla->acknowledgements = acks;

	pthread_mutex_unlock(&mutex_memoria);

	free(id_char);
}

void guardar_mensaje_en_memoria(data_tabla* registro, void* mensaje) {
	pthread_mutex_lock(&mutex_memoria);

	particion_libre* lugar_en_memoria = encontrar_lugar_en_memoria(registro->limit);

	if (NULL == lugar_en_memoria) {
		// no encontramos lugar en memoria
		// keasemo?
		// y esta bien el chequeo == NULL ???
		// y si es un error como hacemos ???
	} else {

		char* id_char = malloc(sizeof(char));
		sprintf(id_char, "%u", registro->id);
		dictionary_put(tabla_segmentos, id_char, (void *) registro);
		free(id_char);

		memcpy(lugar_en_memoria->desde, mensaje, registro->limit);

		ocupar_lugar_en_particiones_libres(lugar_en_memoria, registro->limit);

	}

	pthread_mutex_unlock(&mutex_memoria);
}

void ocupar_lugar_en_particiones_libres(particion_libre* lugar_en_memoria, int limit) {
	lugar_en_memoria->desde = lugar_en_memoria->desde + limit;

	int tamanio = tamanio_particion_libre(lugar_en_memoria);

	if (tamanio == 0) {
		int j = -1;
		size_t hasta_buscable = (size_t) lugar_en_memoria->hasta;
		for (int i = 0; i < particiones_libres->elements_count; i++) {
			particion_libre* particion = list_get(particiones_libres, i);
			size_t hasta_lista = (size_t) particion->hasta;
			if (hasta_buscable == hasta_lista) {
				j = i;
			}
		}
		if (j == -1) {
			// ????????
			// no encontraste ninguno maquina?
		} else if (j >= particiones_libres->elements_count) {
			// la manqueamos
		} else {
			particion_libre* particion = list_get(particiones_libres, j);
			free(particion->desde);
			free(particion->hasta);
			free(particion);
			list_remove(particiones_libres, j);
		}

	}

}

void* encontrar_lugar_en_memoria(int limit) {
	void* lugar;

	if (1 == algoritmo_particion_libre) { // se puede poner polimosfirmo aca?
		lugar = encontrar_lugar_con_ff(limit);
	} else {
		lugar = encontrar_lugar_con_bf(limit);
	}

	return lugar;
}

particion_libre* encontrar_lugar_con_ff(int limit) {
	for (int i = 0; i < particiones_libres->elements_count; i++) {
		particion_libre* particion = list_get(particiones_libres, i);
		if (tamanio_particion_libre(particion) > limit) {
			return particion;
		}
	}

	return NULL; // tal vez error, revisar
}

particion_libre* encontrar_lugar_con_bf(int limit) {
	int menor_tamanio_presente = INT_MAX;
	particion_libre* menor_particion_presente;

	for (int i = 0; i < particiones_libres->elements_count; i++) {
		particion_libre* particion = list_get(particiones_libres, i);
		int tamanio = tamanio_particion_libre(particion);
		if (tamanio > limit && tamanio < menor_tamanio_presente) {
			menor_tamanio_presente = tamanio;
			menor_particion_presente = particion;
		}
	}

	if (INT_MAX != menor_tamanio_presente) {
		return menor_particion_presente;
	} else {
		return NULL; // tal vez error, revisar
	}
}

int tamanio_particion_libre(particion_libre* particion) {
	size_t desde = (size_t) particion->desde;
	size_t hasta = (size_t) particion->hasta;

	return (int) hasta - desde; // esta bien esta cuenta? o hay que hacer /4 o *4 ???
}


















