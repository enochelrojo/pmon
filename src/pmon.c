#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>

#define MAX_PROC_NUM 10

char* obtener_fecha_hora();

int main(int argc, char** argv) {
    /* Cantidad de segundos en que se efectua un monitoreo */
    int segundos; 

    /* Puntero al descriptor del archivo de log */
    FILE* archivo;

    /* Variable de control para el ciclo de monitoreo de procesos */
    int k;

    /* Al menos 2 argumentos; en caso contrario error */
    if (argc < 3) {
        printf("Uso: pmon <segundos> <pid1> <pid2> ... <pid10>\n");
        return -1;
    }

    archivo = fopen("pmon.log", "w");    
    segundos = atoi(argv[1]);


    for(k = 2; k < (argc - 2); k++) {

    }

    fprintf(archivo, "%d segundos\n", segundos);
    fprintf(archivo, "%s\n", obtener_fecha_hora());
    fclose(archivo);



    return 0;
}

char* obtener_fecha_hora() {
    time_t fecha_hora;

    time(&fecha_hora);

    return ctime(&fecha_hora);
}