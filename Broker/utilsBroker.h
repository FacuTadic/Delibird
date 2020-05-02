#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<pthread.h>

//#ifndef CONEXIONES_H_
#define CONEXIONES_H_

//#define IP "127.0.0.1"
//#define PUERTO "4444"


typedef enum
{
	NEW          = 1,
    APPEARED     = 2,
    GET          = 3,
    LOCALIZED    = 4,
    CATCH        = 5,
    CAUGHT       = 6,
	MENSAJE 	 = 7
}op_code;


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



typedef struct
{
   uint32_t coordenadaX;   // 4
   uint32_t coordenadaY;   // 4
   t_coord* sgte;          // 8*n
}t_coord;

// New ‘Pikachu’ 5 10 2

typedef struct
{
    t_buffer pokemon;       // 4 + nombre
    t_coord  coord;         // 8
    uint32_t cantidad;      // 4
}t_new;                   // 16 + nombre

typedef struct
{
   t_place place;         // 12 + nombre
   uint32_t cantidad;      // 4
}t_new;                    // 16 + nombre



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


/*
 * Caught 0
typedef caught{
	uint32_t algo;
} t_caught;
*/



pthread_t thread;

void* recibir_buffer(int*, int);

int iniciar_servidor(void);
void esperar_cliente(int);
void* recibir_mensaje(int socket_cliente, int* size);
int recibir_operacion(int);
void process_request(int cod_op, int cliente_fd);
void serve_client(int *socket);
void* serializar_paquete(t_paquete* paquete, int bytes);
void devolver_mensaje(void* payload, int size, int socket_cliente);


