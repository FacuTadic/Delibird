#ifndef SRC_BITARRAY_H_
#define SRC_BITARRAY_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<signal.h>
#include<string.h>
#include<unistd.h>
#include <semaphore.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/string.h>
#include <commons/bitarray.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

sem_t bloques_bitmap;


void crear_bitarray();




#endif /* SRC_BITARRAY_H_ */
