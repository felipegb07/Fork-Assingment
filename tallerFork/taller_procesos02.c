/*
        Autor\es: Juan Felipe González Barón,
                  María Alejandra Rodriguez Betancourt,
                  Marlon Johan García Restrepo.
        Institución: Pontificia universidad Javeriana.
        Materia: Sistemas Operativos.
        Instrucción: Implementar los conceptos de procesos  y comunicación entre procesos.
*/

/*Librerías implementadas */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/*Estructura para identificación de mensajes enviados mediante pipe */
typedef struct {
    int tipo;
    int valor;
} Mensaje;

/*Funciones */
/*Función de lectura de archivos */
int* lecturaFichero(char* nombre_archivo, int n) {

    /*Abrimos el archivo y hacemos el paso de los parámetros en modo "read" o lectura */
    FILE* archivo = fopen(nombre_archivo, "r");

    /*Comprobación para saber si funciona el archivo de manera correcta */
    if (archivo == NULL) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    /*Creamos un vector de manera dinámica con el tamaño de enteros */
    int* vector = malloc(n * sizeof(int));

    /*Lectura de datos del archivo */
    for (int i = 0; i < n; i++) {
        if (fscanf(archivo, "%d", &vector[i]) != 1) {
            break;
        }
    }

    /*Cierre del archivo */
    fclose(archivo);

    /*Retornamos el vector */
    return vector;
}

/*Función main */
int main(int argc, char *argv[]) {

    /*Verificación de parámetros enviados desde bash */
    if (argc != 5) {
        printf("Parámetros mal escritos... %s\n", argv[0]);
        return 1;
    }

    /*Creación y conversión de parámetros para posterior utilización pasados desde el bash */
    int n1 = atoi(argv[1]);
    char* f1 = argv[2];

    int n2 = atoi(argv[3]);
    char* f2 = argv[4];

    /*Creación de vectores para lectura de datos */
    int* vector1 = lecturaFichero(f1, n1);
    int* vector2 = lecturaFichero(f2, n2);

    /*Creación de pipe para comunicación entre procesos */
    int fd[2];

    /*Verificación de creación correcta del pipe */
    if (pipe(fd) == -1) {
        perror("Error en pipe");
        return 1;
    }

    /*ID de procesos para hacer la identificación con pid_t */
    pid_t pid1, pid2;

    /*Fork a primer proceso para creación de hijo */
    pid1 = fork();

    /*Verificación para saber si estamos en el proceso hijo */
    if (pid1 == 0) {

        /*Creación de proceso nieto */
        pid_t pid_nieto = fork();

        /*Verificación del proceso nieto */
        if (pid_nieto == 0) {

            /*Variable para suma del primer archivo */
            int sumaA = 0;

            /*Recorrido del vector1 */
            for (int i = 0; i < n1; i++) {
                sumaA += vector1[i];
            }

            /*Cierre de lectura del pipe */
            close(fd[0]);

            /*Creación de mensaje */
            Mensaje mensaje;

            /*Tipo 1 correspondiente a sumaA */
            mensaje.tipo = 1;
            mensaje.valor = sumaA;

            /*Envío de mensaje al pipe */
            write(fd[1], &mensaje, sizeof(Mensaje));

            /*Cierre de escritura */
            close(fd[1]);

            /*Liberación de memoria */
            free(vector1);
            free(vector2);

            exit(0);

        } else {

            /*Variable para suma total */
            int sumaTotal = 0;

            /*Suma de vector1 */
            for (int i = 0; i < n1; i++) {
                sumaTotal += vector1[i];
            }

            /*Suma de vector2 */
            for (int i = 0; i < n2; i++) {
                sumaTotal += vector2[i];
            }

            /*Esperamos al nieto */
            wait(NULL);

            /*Cierre de lectura */
            close(fd[0]);

            /*Creación de mensaje */
            Mensaje mensaje;

            /*Tipo 3 correspondiente a suma total */
            mensaje.tipo = 3;
            mensaje.valor = sumaTotal;

            /*Escritura en pipe */
            write(fd[1], &mensaje, sizeof(Mensaje));

            /*Cierre de escritura */
            close(fd[1]);

            /*Liberación de memoria */
            free(vector1);
            free(vector2);

            exit(0);
        }

    } else {

        /*Creación del segundo hijo */
        pid2 = fork();

        /*Verificación del segundo hijo */
        if (pid2 == 0) {

            /*Variable para suma del segundo archivo */
            int sumaB = 0;

            /*Recorrido del vector2 */
            for (int i = 0; i < n2; i++) {
                sumaB += vector2[i];
            }

            /*Cierre de lectura */
            close(fd[0]);

            /*Creación de mensaje */
            Mensaje mensaje;

            /*Tipo 2 correspondiente a sumaB */
            mensaje.tipo = 2;
            mensaje.valor = sumaB;

            /*Escritura del mensaje */
            write(fd[1], &mensaje, sizeof(Mensaje));

            /*Cierre de escritura */
            close(fd[1]);

            /*Liberación de memoria */
            free(vector1);
            free(vector2);

            exit(0);

        } else {

            /*Cierre de escritura en el padre */
            close(fd[1]);

            /*Variables para almacenamiento final */
            int sA = 0;
            int sB = 0;
            int sTotal = 0;

            /*Variable de estructura para recepción */
            Mensaje mensaje;

            /*Lectura de los tres mensajes */
            for (int i = 0; i < 3; i++) {

                read(fd[0], &mensaje, sizeof(Mensaje));

                /*Verificación del tipo de mensaje */
                if (mensaje.tipo == 1) {
                    sA = mensaje.valor;

                } else if (mensaje.tipo == 2) {
                    sB = mensaje.valor;

                } else if (mensaje.tipo == 3) {
                    sTotal = mensaje.valor;
                }
            }

            /*Impresión de resultados */
            printf("SumaA (Nieto): %d\n", sA);
            printf("SumaB (Segundo hijo): %d\n", sB);
            printf("Suma Total (Primer hijo): %d\n", sTotal);

            /*Cierre de lectura */
            close(fd[0]);

            /*Esperamos a los hijos */
            wait(NULL);
            wait(NULL);
        }
    }

    /*Liberación final de memoria */
    free(vector1);
    free(vector2);

    return 0;
}
