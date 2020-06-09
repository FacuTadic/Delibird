#include "archivosYDirectorios.h"



char* generadorDeRutaDeCreacionDeDirectorios(char* ruta, char* nombreDelDirectorio){
	char* rutaDeCreacionDirectorio = string_new();
	string_append(&rutaDeCreacionDirectorio, ruta);
	string_append(&rutaDeCreacionDirectorio, "/");
	string_append(&rutaDeCreacionDirectorio, nombreDelDirectorio);

	log_info(loggerDev, "La ruta final es: %s",rutaDeCreacionDirectorio);
	return rutaDeCreacionDirectorio;

}


char* generadorDeRutaDeCreacionDeArchivos(char* rutaDirectorio,char* nombreDelArchivo, char* tipoDeDato){

	char* rutaDeCreacion = string_new();
	string_append(&rutaDeCreacion, rutaDirectorio);
	string_append(&rutaDeCreacion, "/");
	string_append(&rutaDeCreacion, nombreDelArchivo);
	string_append(&rutaDeCreacion, tipoDeDato);

	log_info(loggerDev, "La ruta final es: %s",rutaDeCreacion);
	return rutaDeCreacion;
}

bool noExisteDirectorio(char* ruta){
	return stat(ruta, &st1) == -1;
}

bool existeArchivo(char* ruta){
	return access(ruta, F_OK ) != -1;
}

uint32_t tamanioDeUnArchivo(char* rutaDelArchivo){
	FILE* archivo;
	archivo = fopen(rutaDelArchivo,"r");
	fseek(archivo, 0, SEEK_END);
	uint32_t tamanioArchivo = ftell(archivo);
	fseek(archivo, 0, SEEK_SET);
	fclose(archivo);
	return tamanioArchivo;
}


void crearDirectorio(char* directorio, char* puntoMontaje){
	char* rutaAbsoluta = generadorDeRutaDeCreacionDeDirectorios(puntoMontaje, directorio);

	if (noExisteDirectorio(rutaAbsoluta)) {
	    mkdir(rutaAbsoluta, 0700);
	    log_info(loggerDev, "Se creo el directorio: %s", directorio);
	}
}

void crearArchivoEnDirectorio(char* nombreDelArchivo, char* directorio){
	FILE *archivo;

	char* rutaDeCreacion = generadorDeRutaDeCreacionDeArchivos(directorio, nombreDelArchivo, ".bin");
	archivo = fopen(rutaDeCreacion,"w+");

	if(archivo == NULL){
	log_info(loggerDev, "No se pudo crear el archivo en el dicrectorio");
	exit(1);
	}
	fclose(archivo);
}

char* devolverValueDeKey(char* key, char* rutaArchivo){
	return "aa";
}

void modificarValueDeKey(char* key, char* value){

}










