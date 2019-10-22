#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <commons/bitarray.h>
#include <commons/string.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void iniciarMmap();
void actualizarBitArray();
void crearBitArray();
void modificarBitArray();
void modificarBitArray2();
void crearArchivoBitmap();

t_bitarray* bitarray;
char *mmapDeBitmap;

int main() {
	//crearArchivoBitmap();
	iniciarMmap();
	bitarray = bitarray_create(mmapDeBitmap, 512);

	//modificarBitArray();
	modificarBitArray2();
	bitarray_destroy(bitarray);
	//si pierdo memoria averiguar mmunmap
}

void crearArchivoBitmap() {
	FILE *f = fopen("bitmap.bin", "w");

	//5192 sale de mystat.st_size (tamanio del archivo de bitmap). tengo que escribir en el archivo para mapear la memoria.
	for (int i = 0; i < 5192; i++) {
		fputc(0, f);
	}

	fclose(f);
}

/*void crearBitArray() {
 FILE* bitmap_f = fopen("bitmap.bin", "w");
 //en vez de 512 seria tamanioenbytesdebloques()
 char* bitmap = malloc(512);
 bitarray = bitarray_create(bitmap, 512);
 fclose(bitmap_f);
 }*/

//Preguntar sobre esto
void iniciarMmap() {
	//Open es igual a fopen pero el FD me lo devuelve como un int (que es lo que necesito para fstat)
	int bitmap = open("bitmap.bin", O_RDWR);
	struct stat mystat;

	//fstat rellena un struct con informacion del FD dado
	if (fstat(bitmap, &mystat) < 0) {
		close(bitmap);
	}

	/*	mmap mapea un archivo en memoria y devuelve la direccion de memoria donde esta ese mapeo
	 *  MAP_SHARED Comparte este área con todos los otros  objetos  que  señalan  a  este  objeto.
                   Almacenar  en  la  región  es equivalente a escribir en el fichero.
	 */
	mmapDeBitmap = mmap(NULL, mystat.st_size, PROT_WRITE | PROT_READ,
			MAP_SHARED, bitmap, 0);
	close(bitmap);

}

void modificarBitArray() {

	int j;
	for (j = 0; j < bitarray_get_max_bit(bitarray); j++) {

		bool bit = bitarray_test_bit(bitarray, j);
		printf("%i", bit);
	}
	printf("\n");
	for (j = 0; j < bitarray_get_max_bit(bitarray) / 2; j++) {

		bool bit = bitarray_test_bit(bitarray, j);
		if (bit == 0) {
			bitarray_set_bit(bitarray, j);
			//encontroUnBloque = 1;
			//bloqueEncontrado = j;
			//break;
		}
	}
	for (j = 0; j < bitarray_get_max_bit(bitarray); j++) {

		bool bit = bitarray_test_bit(bitarray, j);
		printf("%i", bit);
	}
}

void modificarBitArray2() {

	int j;
	for (j = 0; j < bitarray_get_max_bit(bitarray); j++) {

		bool bit = bitarray_test_bit(bitarray, j);
		printf("%i", bit);
	}
	printf("\n");
	for (j = bitarray_get_max_bit(bitarray) - 1;
			j > (bitarray_get_max_bit(bitarray) / 2) - 1; j--) {

		bool bit = bitarray_test_bit(bitarray, j);
		if (bit == 0) {
			bitarray_set_bit(bitarray, j);
			//encontroUnBloque = 1;
			//bloqueEncontrado = j;
			//break;
		}
	}
	for (j = 0; j < bitarray_get_max_bit(bitarray); j++) {

		bool bit = bitarray_test_bit(bitarray, j);
		printf("%i", bit);
	}
}



/*
 void realizarSelect(char* tabla, char* key) {
	string_to_upper(tabla);
	if (existeLaTabla(tabla)) {
		char* pathMetadata = malloc(
				strlen(
						string_from_format("%sTables/",
								structConfiguracionLFS.PUNTO_MONTAJE))
						+ strlen(tabla) + strlen("/Metadata") + 1);
		strcpy(pathMetadata,
				string_from_format("%sTables/",
						structConfiguracionLFS.PUNTO_MONTAJE));
		strcat(pathMetadata, tabla);
		strcat(pathMetadata, "/Metadata");
		t_config *metadata = config_create(pathMetadata);
		int cantidadDeParticiones = config_get_int_value(metadata,
				"PARTITIONS");

		int particionQueContieneLaKey = (atoi(key)) % cantidadDeParticiones;
		printf("La key esta en la particion %i\n", particionQueContieneLaKey);
		char* stringParticion = malloc(4);
		stringParticion = string_itoa(particionQueContieneLaKey);

		char* pathParticionQueContieneKey = malloc(
				strlen(
						string_from_format("%sTables/",
								structConfiguracionLFS.PUNTO_MONTAJE))
						+ strlen(tabla) + strlen("/") + strlen(stringParticion)
						+ strlen(".bin") + 1);
		strcpy(pathParticionQueContieneKey,
				string_from_format("%sTables/",
						structConfiguracionLFS.PUNTO_MONTAJE));
		strcat(pathParticionQueContieneKey, tabla);
		strcat(pathParticionQueContieneKey, "/");
		strcat(pathParticionQueContieneKey, stringParticion);
		strcat(pathParticionQueContieneKey, ".bin");
		t_config *tamanioYBloques = config_create(pathParticionQueContieneKey);
		char** vectorBloques = config_get_array_value(tamanioYBloques, "BLOCK"); //devuelve vector de STRINGS

		int m = 0;
		while (vectorBloques[m] != NULL) {
			m++;
		}

		int timestampActualMayorBloques = -1;
		char* valueDeTimestampActualMayorBloques;

		// POR CADA BLOQUE, TENGO QUE ENTRAR A ESTE BLOQUE
		for (int i = 0; i < m; i++) {
			char* pathBloque = malloc(
					strlen(
							string_from_format("%sBloques/",
									structConfiguracionLFS.PUNTO_MONTAJE))
							+ strlen((vectorBloques[i])) + strlen(".bin") + 1);
			strcpy(pathBloque, "./Bloques/");
			strcat(pathBloque, vectorBloques[i]);
			strcat(pathBloque, ".bin");
			FILE *archivoBloque = fopen(pathBloque, "r");
			if (archivoBloque == NULL) {
				printf("no se pudo abrir archivo de bloques");
				exit(1);
			}

			int cantidadIgualDeKeysEnBloque = 0;
			t_registro* vectorStructs[100];
			obtenerDatosParaKeyDeseada(archivoBloque, (atoi(key)),
					vectorStructs, &cantidadIgualDeKeysEnBloque);

			//cual de estos tiene el timestamp mas grande? guardar timestamp y value
			int temp = 0;
			char* valor;
			for (int k = 1; k < cantidadIgualDeKeysEnBloque; k++) {
				for (int j = 0; j < (cantidadIgualDeKeysEnBloque - k); j++) {
					if (vectorStructs[j]->timestamp
							< vectorStructs[j + 1]->timestamp) {
						temp = vectorStructs[j + 1]->timestamp;
						valor = malloc(strlen(vectorStructs[j + 1]->value));
						strcpy(valor, vectorStructs[j + 1]->value);

						vectorStructs[j + 1]->timestamp =
								vectorStructs[j]->timestamp;
						vectorStructs[j + 1]->value = vectorStructs[j]->value;

						vectorStructs[j]->timestamp = temp;
						vectorStructs[j]->value = valor;
					}
				}
			}
			if (vectorStructs[0]->timestamp > timestampActualMayorBloques) {
				timestampActualMayorBloques = vectorStructs[0]->timestamp;
				valueDeTimestampActualMayorBloques = malloc(
						strlen(vectorStructs[0]->value));
				//strcpy(valueDeTimestampActualMayorBloques, vectorStructs[0]->value);
				valueDeTimestampActualMayorBloques = vectorStructs[0]->value; // ESTO NO DEBERIA SER ASI, Y DEBERIA FUNCIONAR LA LINEA DE ARRIBA
			}
			fclose(archivoBloque);
			free(pathBloque);
			//free(vectorStructs);
		} 	//cierra el for

		// si encontro alguno, me guarda el timestamp mayor en timestampActualMayorBloques
		// y guarda el valor en valueDeTimestampActualMayorBloques
		// si no hay ninguno en vectorBloques (porque por ej, esta en los temporales)
		// entonces timestampActualMayorBloques = -1 y
		// valueDeTimestampActualMayorBloques = NULL

		//-------------------------------------------------

		// AHORA ABRO ARCHIVOS TEMPORALES. EL PROCEDIMIENTO ES MUY PARECIDO AL ANTERIOR

		 char* pathTemporales = malloc(strlen("./Tables/") + strlen(tabla) + 1);
		 strcpy(pathTemporales, "./Tables/");
		 strcat(pathTemporales, tabla);

		 DIR *directorioTemporal = opendir(pathTemporales);
		 struct dirent *directorioTemporalALeer;

		 int timestampActualMayorTemporales = -1;
		 char* valueDeTimestampActualMayorTemporales;

		 while((directorioTemporalALeer = readdir(directorioTemporal)) != NULL) { //PARA CADA .TMP
		 if( directorioTemporalALeer termina en .tmp  ){

		 //obtengo el nombre de ese archivo .tmp . Ejemplo obtengo A1.tmp siendo A1 el nombre (tipo char*)

		 char* pathTemporal = malloc(strlen("./Tables/") + strlen(tabla) + strlen("/") + strlen( nombreArchivoTemporal ) + strlen(".tmp") + 1);
		 strcpy(pathTemporal, "./Tables/");
		 strcat(pathTemporal, tabla);
		 strcat(pathTemporal, "/");
		 strcat(pathTemporal, nombreArchivoTemporal );
		 strcat(pathTemporal, ".tmp");
		 FILE *archivoTemporal = fopen(pathTemporal, "r");
		 if (archivoTemporal == NULL) {
		 printf("no se pudo abrir archivo de temporales");
		 exit(1);
		 }

		 int cantidadIgualDeKeysEnTemporal = 0;
		 t_registro* vectorStructsTemporal[100];
		 obtenerDatosParaKeyDeseada(archivoTemporal, (atoi(key)), vectorStructsTemporal, &cantidadIgualDeKeysEnTemporal);

		 //cual de estos tiene el timestamp mas grande? guardar timestamp y value
		 int tempo = 0;
		 char* valorTemp;
		 for (int k = 1; k < cantidadIgualDeKeysEnTemporal; k++){
		 for (int j = 0; j < (cantidadIgualDeKeysEnTemporal-k); j++){
		 if (vectorStructsTemporal[j]->timestamp < vectorStructsTemporal[j+1]->timestamp){
		 tempo = vectorStructsTemporal[j+1]->timestamp;
		 valorTemp = malloc(strlen(vectorStructsTemporal[j+1]->value));
		 strcpy(valorTemp,vectorStructsTemporal[j+1]->value);

		 vectorStructsTemporal[j+1]->timestamp = vectorStructsTemporal[j]->timestamp;
		 vectorStructsTemporal[j+1]->value = vectorStructsTemporal[j]->value;

		 vectorStructsTemporal[j]->timestamp = tempo;
		 vectorStructsTemporal[j]->value = valorTemp;
		 }
		 }
		 }

		 if(vectorStructsTemporal[0]->timestamp > timestampActualMayorBloques){
		 timestampActualMayorTemporales = vectorStructsTemporal[0]->timestamp;
		 valueDeTimestampActualMayorTemporales = malloc(strlen(vectorStructsTemporal[0]->value));
		 //strcpy(valueDeTimestampActualMayorTemporales, vectorStructsTemporal[0]->value);
		 valueDeTimestampActualMayorTemporales = vectorStructsTemporal[0]->value; // ESTO NO DEBERIA SER ASI, Y DEBERIA FUNCIONAR LA LINEA DE ARRIBA
		 }
		 fclose(archivoTemporal);
		 free(pathTemporal);
		 //free(vectorStructsTemporal)
		 }
		 } //cierra el while

		 // si encontro alguno, me guarda el timestamp mayor en timestampActualMayorTemporales
		 // y guarda el valor en valueDeTimestampActualMayorTemporales
		 // si no hay ninguno en vectorStructsTemporal
		 // entonces timestampActualMayorTemporales = -1 y
		 // valueDeTimestampActualMayorTemporales = NULL

		 closedir(directorioTemporal);

		 // ----------------------------------------------------


		 // aca iria verificar los datos tamb de la memoria de la tabla

		 //-----------------------------------------------------



		free(pathMetadata);
		free(pathParticionQueContieneKey);
		config_destroy(tamanioYBloques);
		config_destroy(metadata);

		// SI NO ENCUENTRA LA TABLA (lo de abajo)
	} else {
		char* mensajeALogear = malloc(
				strlen("Error: no existe una tabla con el nombre ")
						+ strlen(tabla) + 1);
		strcpy(mensajeALogear, "Error: no existe una tabla con el nombre ");
		strcat(mensajeALogear, tabla);
		t_log* g_logger;
		//Si uso LOG_LEVEL_ERROR no lo imprime ni lo escribe
		g_logger = log_create("./erroresSelect.log", "LFS", 1, LOG_LEVEL_INFO);
		log_info(g_logger, mensajeALogear);
		log_destroy(g_logger);
		free(mensajeALogear);
	}
}

*/
