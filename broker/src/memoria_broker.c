#include "memoria_broker.h"

void guardar_info_envios(uint32_t id, t_list* mandados, t_list* acks) {
	char* id_char = string_itoa(id);

	pthread_mutex_lock(&mutex_memoria);

	data_tabla* registro_tabla = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

	if (registro_tabla != NULL) {
		registro_tabla->envios = mandados;
		registro_tabla->acknowledgements = acks;
		registro_tabla->id_modificacion = generar_id_modificacion();
	}

	pthread_mutex_unlock(&mutex_memoria);

	free(id_char);
}

void guardar_mensaje_en_memoria(data_tabla* registro, void* mensaje) {
	if (registro->limit > tamanio_memoria) {
		log_error(extense_logger_memoria, "No se ha podido guardar el mensaje, es mayor al tamanio maximo permitido. Tamanio del mensaje: %i. Tamanio maximo permitido: %i", registro->limit, tamanio_memoria);
		list_clean(registro->acknowledgements);
		list_destroy(registro->acknowledgements);
		list_clean(registro->envios);
		list_destroy(registro->envios);
		free(registro);
		return;
	}

	pthread_mutex_lock(&mutex_memoria);

	int lugar_a_ubicar = encontrar_lugar_a_ubicar(registro->limit);

	registro->tamanio_particion = lugar_a_ubicar;

	particion_libre* lugar_en_memoria = encontrar_lugar_en_memoria(lugar_a_ubicar);

	if (NULL == lugar_en_memoria) {
		hacer_lugar_en_memoria();

		pthread_mutex_unlock(&mutex_memoria);

		guardar_mensaje_en_memoria(registro, mensaje);
	} else {
		log_info(extense_logger_memoria, "Guardando mensaje con id %i en particion %06p", registro->id, (size_t) lugar_en_memoria->desde);
		log_info(logger_memoria, "Guardando mensaje con id %i en particion %06p", registro->id, (size_t) lugar_en_memoria->desde);
		char* id_char = string_itoa(registro->id);
		registro->base = lugar_en_memoria->desde;
		dictionary_put(tabla_segmentos, id_char, (void *) registro);
		free(id_char);

		memcpy(lugar_en_memoria->desde, mensaje, registro->limit);

		ocupar_lugar_en_particiones_libres(lugar_en_memoria, lugar_a_ubicar);

		pthread_mutex_unlock(&mutex_memoria);

		free(mensaje);
	}

}

void hacer_lugar_en_memoria() {

	// F 5         C 0
	// BUSCA Y NO ENCUENTRA
	// INTENTA HACER LUGAR -> C != F -> ELIMINA
	// F 5         C 1
	// BUSCA Y NO ENCUENTRA
	// INTENTA HACER LUGAR -> C != F -> ELIMINA
	// F 5         C 2
	// BUSCA Y NO ENCUENTRA
	// INTENTA HACER LUGAR -> C != F -> ELIMINA
	// F 5         C 3
	// BUSCA Y NO ENCUENTRA
	// INTENTA HACER LUGAR -> C != F -> ELIMINA
	// F 5         C 4
	// BUSCA Y NO ENCUENTRA
	// INTENTA HACER LUGAR -> C != F -> ELIMINA
	// F 5         C 5
	// BUSCA Y NO ENCUENTRA
	// INTENTA HACER LUGAR -> C == f -> COMPACTA Y PONE C = 0
	// F 5         C 0

	if (frecuencia_compactacion == -1) {
		eliminar_particion();
	} else {
		if (cantidad_eliminacion != frecuencia_compactacion) {
			if (eliminar_particion() != -1) {
				cantidad_eliminacion++;
			}
		} else {
			compactar_memoria();
			cantidad_eliminacion = 0;
		}
	}

}

int eliminar_particion() {
	data_tabla* registro = encontrar_particion_a_eliminar();
	char* id_char = string_itoa(registro->id);
	data_tabla* elemento_removido = dictionary_remove(tabla_segmentos, id_char);

	if (elemento_removido == NULL) {
		log_error(extense_logger_memoria, "No se ha podido eliminar el registro con id %s de la tabla de segmentos", id_char);
		free(id_char);
		return -1;
	} else {
		log_info(extense_logger_memoria, "Se ha eliminado el registro con id %s ubicado en %06p de la tabla de segmentos", id_char, registro->base);
		log_info(logger_memoria, "Se ha eliminado el registro con id %s ubicado en %06p de la tabla de segmentos", id_char, registro->base);
		free(id_char);
		liberar_registro_en_particiones_libres(registro);
		list_clean(registro->acknowledgements);
		list_destroy(registro->acknowledgements);
		list_clean(registro->envios);
		list_destroy(registro->envios);
		free(registro);
		return 1;
	}
}

void liberar_registro_en_particiones_libres(data_tabla* registro) {

	// hacer elemento para agregar

	particion_libre* nueva_particion = malloc(sizeof(particion_libre));
	nueva_particion->desde = registro->base;
	nueva_particion->hasta = registro->base + registro->tamanio_particion;

	size_t desde_nuevo = (size_t) nueva_particion->desde;

	// obtener donde agregar

	int index_para_agregar = 0;

	for (int i = 0; i < particiones_libres->elements_count; i++) {
		particion_libre* particion_lista = list_get(particiones_libres, i);
		size_t desde_lista = (size_t) particion_lista->desde;

		if (desde_nuevo > desde_lista) {
			index_para_agregar = i + 1;
		} else {
			i = particiones_libres->elements_count;
		}
	}

	// agregar

	list_add_in_index(particiones_libres, index_para_agregar, nueva_particion);

	// limpiar lista

	corregir_particiones_libres();
}

void corregir_particiones_libres() {
	int j;
	for (j = 1; j < particiones_libres->elements_count; j++) {
		if (j > 0) {
			particion_libre* particion_actual = list_get(particiones_libres, j);
			particion_libre* particion_anterior = list_get(particiones_libres, j - 1);
			size_t comienzo_actual = (size_t) particion_actual->desde;
			size_t final_anterior = (size_t) particion_anterior->hasta;
			if (comienzo_actual == final_anterior) {
				particion_anterior->hasta = particion_actual->hasta;
				particion_libre* particion_a_eliminar = list_remove(particiones_libres, j);
				j = -1;
				free(particion_a_eliminar);
			}
		}
	}

	//ordenar_particiones_libres();

	if (j == -1) {
		corregir_particiones_libres();
	}
}

void ordenar_particiones_libres() {
	for (int i = 0; i < particiones_libres->elements_count - 1; i++) {
		particion_libre* particion_actual = list_get(particiones_libres, i);
		particion_libre* particion_siguiente = list_get(particiones_libres, i + 1);

		size_t comienzo_actual = (size_t) particion_actual->desde;
		size_t comienzo_siguiente = (size_t) particion_siguiente->desde;

		if (comienzo_actual > comienzo_siguiente) {
			list_remove(particiones_libres, i + 1);
			list_add_in_index(particiones_libres, i, particion_siguiente);
		}
	}
}

data_tabla* encontrar_particion_a_eliminar() {
	void* lugar;

	if (1 == algoritmo_reemplazo_particiones) {
		return encontrar_particion_con_fifo();
	} else {
		return encontrar_particion_con_lru();
	}

	return lugar;
}

data_tabla* encontrar_particion_con_fifo() {
	int menor_id_temporal = INT_MAX;
	data_tabla* particion_mas_vieja;

	int id_a_buscar = 1;
	int cantidad_registros_leidos = 0;
	while (cantidad_registros_leidos < tabla_segmentos->elements_amount) {
		char* id_char = string_itoa(id_a_buscar);

		data_tabla* registro = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

		if (registro != NULL) {
			if (registro->id_temporal < menor_id_temporal) {
				menor_id_temporal = registro->id_temporal;
				particion_mas_vieja = registro;
			}
			cantidad_registros_leidos++;
		}

		id_a_buscar++;

		free(id_char);
	}

	if (INT_MAX != menor_id_temporal) {
		return particion_mas_vieja;
	} else {
		return NULL;
	}
}

data_tabla* encontrar_particion_con_lru() {
	int menor_id_modificacion = INT_MAX;
	data_tabla* particion_mas_vieja;

	uint32_t id_a_buscar = 1;
	int cantidad_registros_leidos = 0;
	while (cantidad_registros_leidos < tabla_segmentos->elements_amount) {
		char* id_char = string_itoa(id_a_buscar);

		data_tabla* registro = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

		if (registro != NULL) {
			if (registro->id_modificacion < menor_id_modificacion) {
				menor_id_modificacion = registro->id_modificacion;
				particion_mas_vieja = registro;
			}
			cantidad_registros_leidos++;
		}

		id_a_buscar++;

		free(id_char);
	}

	if (INT_MAX != menor_id_modificacion) {
		return particion_mas_vieja;
	} else {
		return NULL;
	}
}

void compactar_memoria() {
	size_t final_memoria = (size_t) (memoria + tamanio_memoria);
	particion_libre* primer_particion_libre = list_get(particiones_libres, 0);
	size_t final_primer_particion = (size_t) primer_particion_libre->hasta;
	log_info(extense_logger_memoria, "Compactando memoria");
	log_info(logger_memoria, "Compactando memoria");
	while (final_memoria != final_primer_particion) {
		data_tabla* registro_proximo = obtener_registro_proximo(primer_particion_libre);

		log_info(extense_logger_memoria, "Moviendo registro de %06p a %06p", registro_proximo->base, primer_particion_libre->desde);

		void* aux = malloc(registro_proximo->tamanio_particion);
		memcpy(aux, registro_proximo->base, registro_proximo->tamanio_particion);
		memcpy(primer_particion_libre->desde, aux, registro_proximo->tamanio_particion);

		free(aux);

		registro_proximo->base = primer_particion_libre->desde;

		primer_particion_libre->desde = primer_particion_libre->desde + registro_proximo->tamanio_particion;
		primer_particion_libre->hasta = primer_particion_libre->hasta + registro_proximo->tamanio_particion;

		corregir_particiones_libres();

		primer_particion_libre = list_get(particiones_libres, 0);
		final_primer_particion = (size_t) primer_particion_libre->hasta;
	}
}

data_tabla* obtener_registro_proximo(particion_libre* particion_libre) {
	size_t fin_particion_libre = (size_t) particion_libre->hasta;

	uint32_t id_a_buscar = 1;
	int cantidad_registros_leidos = 0;
	while (cantidad_registros_leidos < tabla_segmentos->elements_amount) {
		char* id_char = string_itoa(id_a_buscar);

		data_tabla* registro = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

		if (registro != NULL) {
			size_t comienzo_registro = (size_t) registro->base;

			if (comienzo_registro == fin_particion_libre) {
				free(id_char);
				return registro;
			}
			cantidad_registros_leidos++;
		}

		id_a_buscar++;
		free(id_char);
	}

	log_warning(extense_logger_memoria, "No se ha encontrado ningun registro de memoria que comience en la direccion de memoria buscada");
	return NULL;
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
//			free(particion->desde);
//			free(particion->hasta);
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
		log_info(extense_logger_memoria, "Tamanio de particion libre %i", tamanio_particion_libre(particion));
		log_info(logger_memoria, "Tamanio de particion libre %i", tamanio_particion_libre(particion));
		if (tamanio_particion_libre(particion) > lugar_a_ubicar || tamanio_particion_libre(particion) == lugar_a_ubicar) {
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
		if (tamanio >= lugar_a_ubicar && tamanio < menor_tamanio_presente) {
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

	int k = 0;

	for (uint32_t i = 1; k < tabla_segmentos->elements_amount; i++) {
		char* id_char = string_itoa(i);
		data_tabla* registro = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

		if (registro == NULL) {
			log_info(extense_logger_memoria, "Registro NULL leido");
		} else {
			k++;
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

	int k = 0;

	for (uint32_t i = 1; k < tabla_segmentos->elements_amount; i++) {
		char* id_char = string_itoa(i);
		data_tabla* registro = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

		if (registro == NULL) {
			log_warning(extense_logger_memoria, "Registro NULL leido");
		} else {
			k++;
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

	int k = 0;

	for (uint32_t i = 1; k < tabla_segmentos->elements_amount; i++) {
		char* id_char = string_itoa(i);
		data_tabla* registro = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

		if (registro == NULL) {
			log_warning(extense_logger_memoria, "Registro NULL leido");
		} else {
			k++;
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

	int k = 0;

	for (uint32_t i = 1; k < tabla_segmentos->elements_amount; i++) {
		char* id_char = string_itoa(i);
		data_tabla* registro = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

		if (registro == NULL) {
			log_warning(extense_logger_memoria, "Registro NULL leido");
		} else {
			k++;
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

	int k = 0;

	for (uint32_t i = 1; k < tabla_segmentos->elements_amount; i++) {
		char* id_char = string_itoa(i);
		data_tabla* registro = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

		if (registro == NULL) {
			log_warning(extense_logger_memoria, "Registro NULL leido");
		} else {
			k++;
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

	int k = 0;

	for (uint32_t i = 1; k < tabla_segmentos->elements_amount; i++) {
		char* id_char = string_itoa(i);
		data_tabla* registro = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

		if (registro == NULL) {
			log_warning(extense_logger_memoria, "Registro NULL leido");
		} else {
			k++;
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

void dump_cache(int signal) {
	char* file_name = string_new();
	char* fecha = temporal_get_string_time();

	string_append(&file_name, "dump_");
	string_append(&file_name, fecha);
	string_append(&file_name, ".txt");

	FILE* archivo_dump = txt_open_for_append(file_name);

	if (archivo_dump == NULL) {
		log_error(extense_logger_memoria, "No se ha podido hacer el dump de la memoria, ha ocurrido un problema al crear el archivo de dump");
		free(file_name);
		free(fecha);
		txt_close_file(archivo_dump);
	} else {
		char* dump;
		void* particion = memoria;
		void* final_memoria = memoria + tamanio_memoria;
		int numero_particion = 1;

		while (particion != final_memoria) {
			particion_libre* particion_para_dump = buscar_particion_libre(particion);
			if (particion_para_dump == NULL) {
				data_tabla* registro_para_dump = buscar_registro(particion);

				if (registro_para_dump == NULL) {
					// la cagamos
					log_error(extense_logger_memoria, "El dump de la memoria se ha detenido abruptamente, no se ha encontrado la particion comenzada en %06p", particion);
					free(dump);
					free(file_name);
					free(fecha);
					txt_close_file(archivo_dump);
				} else {
					particion = registro_para_dump->base + registro_para_dump->tamanio_particion;
					dump = obtener_dump_registro(registro_para_dump, numero_particion);
				}
			} else {
				particion = particion_para_dump->hasta;
				dump = obtener_dump_particion_libre(particion_para_dump, numero_particion);
			}
			numero_particion++;
			txt_write_in_file(archivo_dump, dump);
			free(dump);
		}
	}

	txt_close_file(archivo_dump);
}

data_tabla* buscar_registro(void* particion) {
	int id_a_buscar = 1;
	int cantidad_registros_leidos = 0;
	while (cantidad_registros_leidos < tabla_segmentos->elements_amount) {
		char* id_char = string_itoa(id_a_buscar);
		data_tabla* registro = (data_tabla*) dictionary_get(tabla_segmentos, id_char);

		if (registro != NULL) {
			if (registro->base == particion) {
				return registro;
			}
			cantidad_registros_leidos++;
		}

		id_a_buscar++;

		free(id_char);
	}

	return NULL;
}

particion_libre* buscar_particion_libre(void* particion) {
	for (int i = 0; i < particiones_libres->elements_count; i++) {
		particion_libre* particion_libre = list_get(particiones_libres, i);
		if (particion_libre->desde == particion) {
			return particion_libre;
		}
	}
	return NULL;
}

char* obtener_dump_particion_libre(particion_libre* particion_libre, int numero_particion) {
	return string_from_format("Partición %i: %06p - %06p.      [L]      Size: %i b\n",
			numero_particion,
			particion_libre->desde,
			particion_libre->hasta,
			tamanio_particion_libre(particion_libre));
}

char* obtener_dump_registro(data_tabla* registro, int numero_particion) {
	return string_from_format("Partición %i: %06p - %06p.      [X]      Size: %i b      LRU: %i      Cola: %i      ID: %i\n",
			numero_particion,
			registro->base,
			registro->base + registro->tamanio_particion,
			registro->tamanio_particion,
			registro->id_modificacion,
			registro->tipo,
			registro->id);
}
