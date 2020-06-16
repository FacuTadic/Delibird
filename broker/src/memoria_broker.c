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

	int lugar_a_ubicar = encontrar_lugar_a_ubicar(registro->limit);

	registro->tamanio_particion = lugar_a_ubicar;

	particion_libre* lugar_en_memoria = encontrar_lugar_en_memoria(lugar_a_ubicar);

	if (NULL == lugar_en_memoria) {
		// no encontramos lugar en memoria
		// keasemo?
		// y esta bien el chequeo == NULL ???
		// y si es un error como hacemos ???
	} else {

		log_info(extense_logger_memoria, "Guardando mensaje con id %i en particion %i", registro->id, (size_t) lugar_en_memoria->desde);
		log_info(logger_memoria, "Guardando mensaje con id %i en particion %i", registro->id, (size_t) lugar_en_memoria->desde);
		char* id_char = malloc(sizeof(char));
		sprintf(id_char, "%u", registro->id);
		registro->base = lugar_en_memoria->desde;
		dictionary_put(tabla_segmentos, id_char, (void *) registro);
		free(id_char);

		memcpy(lugar_en_memoria->desde, mensaje, registro->limit);

		ocupar_lugar_en_particiones_libres(lugar_en_memoria, lugar_a_ubicar);

	}

	pthread_mutex_unlock(&mutex_memoria);
}

void ocupar_lugar_en_particiones_libres(particion_libre* lugar_en_memoria, int lugar_a_ubicar) {
	lugar_en_memoria->desde = lugar_en_memoria->desde + lugar_a_ubicar;

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

void* encontrar_lugar_en_memoria(int lugar_a_ubicar) {
	void* lugar;

	if (1 == algoritmo_particion_libre) {
		lugar = encontrar_lugar_con_ff(lugar_a_ubicar);
	} else {
		lugar = encontrar_lugar_con_bf(lugar_a_ubicar);
	}

	return lugar;
}

int encontrar_lugar_a_ubicar(int limit) {
	if (limit > tamanio_minimo_particion) {
		return limit;
	} else {
		return tamanio_minimo_particion;
	}
}

particion_libre* encontrar_lugar_con_ff(int lugar_a_ubicar) {
	for (int i = 0; i < particiones_libres->elements_count; i++) {
		particion_libre* particion = list_get(particiones_libres, i);
		if (tamanio_particion_libre(particion) > lugar_a_ubicar) {
			return particion;
		}
	}

	return NULL;
}

particion_libre* encontrar_lugar_con_bf(int lugar_a_ubicar) {
	int menor_tamanio_presente = INT_MAX;
	particion_libre* menor_particion_presente;

	for (int i = 0; i < particiones_libres->elements_count; i++) {
		particion_libre* particion = list_get(particiones_libres, i);
		int tamanio = tamanio_particion_libre(particion);
		if (tamanio > lugar_a_ubicar && tamanio < menor_tamanio_presente) {
			menor_tamanio_presente = tamanio;
			menor_particion_presente = particion;
		}
	}

	if (INT_MAX != menor_tamanio_presente) {
		return menor_particion_presente;
	} else {
		return NULL;
	}
}

int tamanio_particion_libre(particion_libre* particion) {
	size_t desde = (size_t) particion->desde;
	size_t hasta = (size_t) particion->hasta;

	return (int) hasta - desde;
}

t_list* obtener_segmentos_new(uint32_t id_cliente) {

	t_list* segmentos = list_create();

	log_info(extense_logger_memoria, "Tamanio tabla de segmentos: %i", tabla_segmentos->elements_amount);

	for (uint32_t i = 1; i <= tabla_segmentos->elements_amount; i++) {

		char* id_char = malloc(sizeof(char));
		sprintf(id_char, "%u", i);

		data_tabla* registro = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

		if (registro == NULL) {
			log_warning(extense_logger_memoria, "Registro NULL leido");
		} else {
			if (1 == registro->tipo) {
				int has_to_send = 1;
				for (int j = 0; j < registro->acknowledgements->elements_count; j++) {
					uint32_t* id_ack = (uint32_t*) list_get(registro->acknowledgements, j);
					if (*id_ack == id_cliente) {
						has_to_send = 0;
					}
				}

				if (1 == has_to_send) {
					segmento_memoria* segmento = malloc(sizeof(segmento_memoria));
					segmento->registro = registro;
					segmento->mensaje = registro->base;
					list_add(segmentos, segmento);
				}
			}
		}

		free(id_char);
	}

	return segmentos;

}

t_list* obtener_segmentos_appeared(uint32_t id_cliente) {

	t_list* segmentos = list_create();

	log_info(extense_logger_memoria, "Tamanio tabla de segmentos: %i", tabla_segmentos->elements_amount);

	for (uint32_t i = 1; i <= tabla_segmentos->elements_amount; i++) {

		char* id_char = malloc(sizeof(char));
		sprintf(id_char, "%u", i);

		data_tabla* registro = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

		if (registro == NULL) {
			log_warning(extense_logger_memoria, "Registro NULL leido");
		} else {
			if (2 == registro->tipo) {
				int has_to_send = 1;
				for (int j = 0; j < registro->acknowledgements->elements_count; j++) {
					uint32_t* id_ack = (uint32_t*) list_get(registro->acknowledgements, j);
					if (*id_ack == id_cliente) {
						has_to_send = 0;
					}
				}

				if (1 == has_to_send) {
					segmento_memoria* segmento = malloc(sizeof(segmento_memoria));
					segmento->registro = registro;
					segmento->mensaje = registro->base;
					list_add(segmentos, segmento);
				}
			}
		}

		free(id_char);
	}

	return segmentos;

}

t_list* obtener_segmentos_get(uint32_t id_cliente) {

	t_list* segmentos = list_create();

	log_info(extense_logger_memoria, "Tamanio tabla de segmentos: %i", tabla_segmentos->elements_amount);

	for (uint32_t i = 1; i <= tabla_segmentos->elements_amount; i++) {

		char* id_char = malloc(sizeof(char));
		sprintf(id_char, "%u", i);

		data_tabla* registro = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

		if (registro == NULL) {
			log_warning(extense_logger_memoria, "Registro NULL leido");
		} else {
			if (5 == registro->tipo) {
				int has_to_send = 1;
				for (int j = 0; j < registro->acknowledgements->elements_count; j++) {
					uint32_t* id_ack = (uint32_t*) list_get(registro->acknowledgements, j);
					if (*id_ack == id_cliente) {
						has_to_send = 0;
					}
				}

				if (1 == has_to_send) {
					segmento_memoria* segmento = malloc(sizeof(segmento_memoria));
					segmento->registro = registro;
					segmento->mensaje = registro->base;
					list_add(segmentos, segmento);
				}
			}
		}

		free(id_char);
	}

	return segmentos;

}

t_list* obtener_segmentos_localized(uint32_t id_cliente) {

	t_list* segmentos = list_create();

	log_info(extense_logger_memoria, "Tamanio tabla de segmentos: %i", tabla_segmentos->elements_amount);

	for (uint32_t i = 1; i <= tabla_segmentos->elements_amount; i++) {

		char* id_char = malloc(sizeof(char));
		sprintf(id_char, "%u", i);

		data_tabla* registro = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

		if (registro == NULL) {
			log_warning(extense_logger_memoria, "Registro NULL leido");
		} else {
			if (6 == registro->tipo) {
				int has_to_send = 1;
				for (int j = 0; j < registro->acknowledgements->elements_count; j++) {
					uint32_t* id_ack = (uint32_t*) list_get(registro->acknowledgements, j);
					if (*id_ack == id_cliente) {
						has_to_send = 0;
					}
				}

				if (1 == has_to_send) {
					segmento_memoria* segmento = malloc(sizeof(segmento_memoria));
					segmento->registro = registro;
					segmento->mensaje = registro->base;
					list_add(segmentos, segmento);
				}
			}
		}

		free(id_char);
	}

	return segmentos;

}

t_list* obtener_segmentos_catch(uint32_t id_cliente) {

	t_list* segmentos = list_create();

	log_info(extense_logger_memoria, "Tamanio tabla de segmentos: %i", tabla_segmentos->elements_amount);

	for (uint32_t i = 1; i <= tabla_segmentos->elements_amount; i++) {

		char* id_char = malloc(sizeof(char));
		sprintf(id_char, "%u", i);

		data_tabla* registro = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

		if (registro == NULL) {
			log_warning(extense_logger_memoria, "Registro NULL leido");
		} else {
			if (3 == registro->tipo) {
				int has_to_send = 1;
				for (int j = 0; j < registro->acknowledgements->elements_count; j++) {
					uint32_t* id_ack = (uint32_t*) list_get(registro->acknowledgements, j);
					if (*id_ack == id_cliente) {
						has_to_send = 0;
					}
				}

				if (1 == has_to_send) {
					segmento_memoria* segmento = malloc(sizeof(segmento_memoria));
					segmento->registro = registro;
					segmento->mensaje = registro->base;
					list_add(segmentos, segmento);
				}
			}
		}

		free(id_char);
	}

	return segmentos;

}

t_list* obtener_segmentos_caught(uint32_t id_cliente) {

	t_list* segmentos = list_create();

	log_info(extense_logger_memoria, "Tamanio tabla de segmentos: %i", tabla_segmentos->elements_amount);

	for (uint32_t i = 1; i <= tabla_segmentos->elements_amount; i++) {

		char* id_char = malloc(sizeof(char));
		sprintf(id_char, "%u", i);

		data_tabla* registro = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

		if (registro == NULL) {
			log_warning(extense_logger_memoria, "Registro NULL leido");
		} else {
			if (4 == registro->tipo) {
				int has_to_send = 1;
				for (int j = 0; j < registro->acknowledgements->elements_count; j++) {
					uint32_t* id_ack = (uint32_t*) list_get(registro->acknowledgements, j);
					if (*id_ack == id_cliente) {
						has_to_send = 0;
					}
				}

				if (1 == has_to_send) {
					segmento_memoria* segmento = malloc(sizeof(segmento_memoria));
					segmento->registro = registro;
					segmento->mensaje = registro->base;
					list_add(segmentos, segmento);
				}
			}
		}

		free(id_char);
	}

	return segmentos;

}
