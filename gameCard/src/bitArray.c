#include"bitArray.h"

void setear_bitarray(int nr_blocks) {
	uint32_t i = 0;
	char c;

	FILE *fp;
	fp = fopen(bitMap, "r+b");
	if (fp == NULL) {
		log_error(loggerDev, "Hubo un error abriendo el bitmap.");
		return;
	}

	fseek(fp, 0L, SEEK_END);
	uint32_t file_size = ftell(fp);

	if (file_size == 0) {	//Bitmap vacio
		fseek(fp, 0L, SEEK_SET);
		for (i = 0; i < nr_blocks; i++) {
			bitarray_clean_bit(bitarray, i);
			fprintf(fp, "%d", bitarray_test_bit(bitarray, i));
		}
		crear_bloques_FS(nr_blocks);
	} else {
		printf("Bitmap ya seteado\n");
		fseek(fp, 0L, SEEK_SET);
		while (!feof(fp)) {
			c = fgetc(fp);
			if (!feof(fp)) {
				if (atoi(&c) == 1) {
					bitarray_set_bit(bitarray, i);
				} else {
					bitarray_clean_bit(bitarray, i);
				}
				i++;
			}
		}
	}
	fclose(fp);
}


void crear_bitarray() {
	char *data;
	data = malloc(cantidadDeBloques);
	memset(data, '0', cantidadDeBloques);

	bitarray = bitarray_create_with_mode(data, cantidadDeBloques, LSB_FIRST);
	setear_bitarray(cantidadDeBloques);
}


int elegir_bloque_libre() {
	int free_block, i, flag_free_block = 0;

	sem_wait(&bloques_bitmap);
	for (i = 0; i < cantidadDeBloques; i++) {
		if (flag_free_block == 0) {
			if (bitarray_test_bit(bitarray, i) == 0) {
				flag_free_block = 1;
				free_block = i;
				bitarray_set_bit(bitarray, i);
				guardar_bitarray(i);
				sem_post(&bloques_bitmap);
				return free_block;
			}
		}
	}
	sem_post(&bloques_bitmap);
	return -1;
}




char* buscarPosicionDisponibleEnElBitMap(){

}
