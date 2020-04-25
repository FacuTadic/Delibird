typedef enum
{
	NEW          = 1,
    APPEARED     = 2,
    GET          = 3,
    LOCALIZED    = 4,
    CATCH        = 5,
    CAUGHT       = 6 
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
    uint32_t coordenadaX,   // 4
             coordenadaY,   // 4
    t_coord* sgte;          // 8*n    
}t_coord;

// New ‘Pikachu’ 5 10 2

typedef struct
{
    t_buffer pokemon;       // 4 + nombre
    t_coord  coord;         // 8
             cantidad;      // 4
}t_new                      // 16 + nombre

typedef struct
{
    t_place place;         // 12 + nombre
            cantidad;      // 4
}t_new                     // 16 + nombre



// Localized ‘Pikachu’ 3 4 5 1 5 9 3

typedef struct
{
    t_buffer pokemon;       // 16 + nombre
    uint32_t lugares;       // 4
    t_coord* coord;          // 8*n
}t_localized

typedef struct
{
    t_buffer pokemon;
}t_get;

// Appeared ‘Pikachu’ 1 5

typedef struct
{
    t_buffer pokemon;
    t_coord    coord;
}t_appeared

// Catch ‘Pikachu’ 1 5

typedef struct
{
    t_buffer pokemon;
    t_coord    coord;
}t_catch

typedef struct
{
    t_buffer pokemon;
    t_coord    coord;    
}t_place

//Caught 0

typedef caught uint32_t
