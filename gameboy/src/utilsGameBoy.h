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

typedef enum
{
	NEW          = 1,
    APPEARED     = 2,
    GET          = 3,
    LOCALIZED    = 4,
    CATCH        = 5,
    CAUGHT       = 6,
	MENSAJE 	 = 7 //TEST
}op_code;


typedef enum
{
	BROKER       = 0,
	TEAM	     = 1,
	GAMECARD     = 2,
	SUSCRIPTOR   = 3,
	SERVER		 = 4 //TEST
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



/*
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
*/

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

/*
typedef struct
{
    t_place place;         // 12 + nombre
    uint32_t cantidad;      // 4
}t_new;               // 16 + nombre
*/


// Localized ‘Pikachu’ 3 4 5 1 5 9 3

typedef struct
{
    t_buffer pokemon;       // 16 + nombre
    uint32_t lugares;       // 4
    t_coord* coord;          // 8*n
}t_localized;

typedef struct
{
    t_buffer pokemon;
}t_get;

// Appeared ‘Pikachu’ 1 5

typedef struct
{
    t_buffer pokemon;
    t_coord    coord;
}t_appeared;

// Catch ‘Pikachu’ 1 5

typedef struct
{
    t_buffer pokemon;
    t_coord    coord;
}t_catch;

typedef struct
{
    t_buffer pokemon;
    t_coord    coord;
}t_place;

//Caught 0

//typedef caught uint32_t



int crear_conexion(char* ip, char* puerto);
void enviar_mensaje(char* mensaje, int socket_cliente, t_log* logger);
char* recibir_mensaje(int socket_cliente, t_log* logger);
void eliminar_paquete(t_paquete* paquete);
void liberar_conexion(int socket_cliente);

#endif /* UTILS_H_ */
