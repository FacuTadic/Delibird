#ifndef UTILS_H
#define UTILS_H_

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>
#include<commons/log.h>
#include<commons/config.h>
#include<commons/string.h>


t_log* loggerDev;
t_config* config;
t_log* loggerGameBoy;


char* ip;
char* puerto;

char* ip_game_boy;
char* puerto_game_boy;

uint32_t idGameBoy;


typedef enum
{
	NEW          = 1,
    APPEARED     = 2,
    CATCH        = 3,
    CAUGHT       = 4,
	GET          = 5,
	LOCALIZED	 = 6,
	SUSCRIPTOR 	 = 7
}op_code;


typedef enum
{
	BROKER       = 0,
	TEAM	     = 1,
	GAMECARD     = 2,
}op_codeModulo;


//******************************************************* PRUEBA **************************************************
typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;
//**********************************************************************************************************************************************************


struct t_coord
{
	uint32_t coordenadaX;   // 4
    uint32_t coordenadaY;   // 4
    struct t_coord* sgte;          // 8*n
};

typedef struct t_coord t_coord;


// New ‘Pikachu’ 5 10 2

typedef struct
{
    uint32_t sizePokemon;
	char*	 pokemon;       // 4 + nombre
    uint32_t  coordX;		// 4
    uint32_t  coordY;		// 4
    uint32_t cantidad;      // 4
}t_new;                     // 16 + nombre

// Localized ‘Pikachu’ 3 4 5 1 5 9 3

typedef struct
{
	uint32_t sizePokemon;
	char*	 pokemon;
    uint32_t lugares;       // 4
    t_coord* coord;          // 8*n
}t_localized;

typedef struct
{
	uint32_t sizePokemon;
	char*	 pokemon;;
}t_get;

// Appeared ‘Pikachu’ 1 5

typedef struct
{
	uint32_t sizePokemon;
	char*	 pokemon;;
	uint32_t  coordX;		// 4
	uint32_t  coordY;
}t_appeared;

// Catch ‘Pikachu’ 1 5

typedef struct
{
	uint32_t sizePokemon;
	char*	 pokemon;
    uint32_t  coordX;		// 4
    uint32_t  coordY;		// 4
}t_catch;

//Caught 0

typedef struct{

 uint32_t flagCaught;

 }t_caught;



int crear_conexion(char* ip, char* puerto);
void enviar_mensaje(char*argv[], int socket_cliente);
void recibir_mensaje(int socket_cliente);
void recibirMensajesDeSuscripcion(int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
void liberar_conexion(int socket_cliente);

#endif /* UTILS_H_ */
