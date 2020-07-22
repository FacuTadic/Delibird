#ifndef PROTOCOLOGAMECARD_H_
#define PROTOCOLOGAMECARD_H_

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<pthread.h>


uint32_t id_modulo;
t_log* loggerDevProtocolo;


typedef enum
{
	NEW          = 1,
    APPEARED     = 2,
    CATCH        = 3,
    CAUGHT       = 4,
	GET          = 5,
	LOCALIZED	 = 6,
	SUSCRIPTOR 	 = 7,
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



typedef struct {
    char* pokemon;
    uint32_t pos_X, pos_Y, cantidad;
    uint32_t id;
} t_newLlegada;

// Appeared ‘Pikachu’ 1 5
typedef struct
{
	uint32_t idMensaje;
	uint32_t sizePokemon;
	char*	 pokemon;
	uint32_t  coordX;
	uint32_t  coordY;
}t_appeared;


typedef struct {
    char* pokemon;
    uint32_t id;
} t_getLlegada;

// Localized ‘Pikachu’ 3 4 5 1 5 9 3
typedef struct
{
	uint32_t idMensaje;
	uint32_t sizePokemon;
	char*	 pokemon;
    uint32_t lugares;
    t_list* l_coordenadas;
}t_localized;

typedef struct {
    char* pokemon;
    uint32_t pos_X, pos_Y;
    uint32_t id;
} t_catchLlegada;

typedef struct{

	uint32_t idMensaje;
	bool flagCaught;

 }t_caught;

 typedef struct {          // adaptar
 	uint32_t id_cliente;
 	uint32_t id_cola;
 } t_suscripcion;





typedef struct {
	uint32_t id_cliente;
	int socket_cliente;
} info_modulo;

int mandar_suscripcion(int socket_broker, uint32_t id_cola);

t_newLlegada* recibir_new(int socket_cliente, uint32_t* size, t_log* logger);
void enviar_appeared(int socket, t_appeared* appearedAEnviar);

t_catchLlegada* recibir_catch(int socket_cliente, uint32_t* size, t_log* logger);
void enviar_caught(int socket, t_caught* caughtAEnviar);

t_getLlegada* recibir_get(int socket_cliente, uint32_t* size, t_log* logger);
void enviar_localized(int socket, t_localized* localizedAEnviar);

void liberar_conexion(int socket_cliente);

void devolver_id(int socket_cliente, uint32_t id, t_log* logger);

int devolver_ack(int socket_broker);


#endif /* PROTOCOLOGAMECARD_H_ */
