#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <errno.h>

#define MAX_PROC_NUM 10


char* obtener_fecha_hora();
void manejador_senal(int);
void verificar_procesos(FILE*, pid_t[], int);


/* Esta varibable define si pmon se sigue ejecutando o no */
int flag = 1;

int main(int argc, char** argv) {
    /* Cantidad de segundos en que se efectua un monitoreo */
    int segundos; 

    /* Puntero al descriptor del archivo de log */
    FILE* log;
    log = fopen("pmon.log", "w");

    /* Vector de PIDs con los procesos que tengo que monitorear */
    pid_t procesos[MAX_PROC_NUM];

    /* Contador del numero de procesos que tengo que monitorear */
    int num_procesos = argc - 2;

    /* Variable de control para el ciclo de monitoreo de procesos */
    int k;

    /* Al menos 3 argumentos: nom del programa, segundos, pid1; 
        en caso contrario error y finaliza la ejecución */
    if (argc < 3) {
        printf("Uso: pmon <segundos> <pid1> <pid2> ... <pid10>\n");
        return -1;
    }


    /* Leo la cantidad de segundos entre cada monitoreo */
    segundos = atoi(argv[1]);

    /* Leo los PIDs que tengo que monitorizar */
    for(k = 0; k < num_procesos; k++) {
        procesos[k] = (pid_t) atoi(argv[k+2]);
    }

    /* Si me llega SIGTERM (un kill por defecto) se llama a manejador_senal */
    signal(SIGTERM, manejador_senal);

    /* Mientras flag sea 1, ejecuta el ciclo y verifica el estado de los procesos
    que llegaron por parametro, luego espera la cantidad de segundos especificada */
    while (flag) {
        verificar_procesos(log, procesos, num_procesos);
        sleep(segundos);
    }

    
    
    fclose(log);



    return 0;
}

char* obtener_fecha_hora() {
    time_t fecha_hora;

    time(&fecha_hora);

    return ctime(&fecha_hora);
}

void manejador_senal(int sig) {    
    FILE* log = fopen("pmon.log", "a");

    fprintf(log, "%s", obtener_fecha_hora());
    fprintf(log, "%s\n","Fin de la monitorizacion");
    flag = 0;

    fclose(log);
}

void verificar_procesos(FILE* log, pid_t procesos[], int num_procesos) {
    int k;

    fprintf(log, "%s", obtener_fecha_hora());

    for(k = 0; k < num_procesos; k++) {
        if (procesos[k] > 0) {
            if (kill(procesos[k], 0) == ESRCH) {
                fprintf(log, "El proceso %d terminó\n", procesos[k]);
                
                /* Invalido el proceso para no verificarlo más */
                procesos[k] = 0;
            } else {
                fprintf(log, "El proceso %d aún activo\n", procesos[k]);
            }
        }
    }
    
    fflush(log);
}