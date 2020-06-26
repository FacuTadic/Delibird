#ifndef TEAM_UTILS_H_
#define TEAM_UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include <commons/collections/queue.h>
#include <commons/collections/list.h>

#endif

typedef enum
{
	NEW          = 1,
    APPEARED     = 2,
    CATCH        = 3,
    CAUGHT       = 4,
	GET          = 5,
	LOCALIZED    = 6,
	SUSCRIPCION  = 7
} op_code;

typedef struct
{
	uint32_t size;
	char* nombre;
	uint32_t coordx;
	uint32_t coordY;
} t_pokemon;

typedef struct
{
	uint32_t coordx;
	uint32_t coordy;
} t_coord;

typedef struct
{
	uint32_t sizeTotal;
	uint32_t size;
	char* nombre;
	uint32_t cantidad;
	t_list* coords; //t_coord
} t_localized;

//typedef struct
//{
//	uint32_t sizeTotal;
//	uint32_t size;
//	char* nombre;
//	uint32_t coordx;
//	uint32_t coordY;
//} t_appeared;

int crear_conexion(char* ip, char* puerto);
int iniciar_escucha_game_boy(char* ip, char* puerto);
