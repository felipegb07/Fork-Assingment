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
    int* vector = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        if (fscanf(archivo, "%d", &vector[i]) != 1) {
            break;
        }
    }

    /*Cierre del archivo */
    fclose(archivo);
    return vector;
}

/*Función main */
int main(int argc, char *argv[]) {

    
    if (argc != 5) {
        printf("Parámetros mal escritos...%s\n", argv[0]);
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

    
    int fd[2];
    if (pipe(fd) == -1) {
        perror("Error en pipe");
        return 1;
    }

    /*ID de procesos para hacer la identificación con pid_t (Identificador especial de sys/types.h)*/
    pid_t pid1, pid2;

    /*Fork a primer procesos para la creación de proceso hijo */
    pid1 = fork();
    
    /*Verificación para saber si estamos en el proceso hijo */
    if (pid1 == 0) {
        
        /*Creación de proceso nieto mediante el fork */
        pid_t pid_nieto = fork();
        if (pid_nieto == 0) {
            /*Calculo de la sumaA para nuestro fichero00 */
            int sumaA = 0;
            for (int i = 0; i < n1; i++) sumaA += vector1[i];
            /*Cierre de pipe */
            close(fd[0]);

            /*Enviamos los datos teniendo en cuenta el tamaño de un entero con el pipe en modo 1 siendo escritura del archivo */
            write(fd[1], &sumaA, sizeof(int));

            /*Cerramos la escritura del archivo */
            close(fd[1]);
            exit(0);
        } else{
	    /*Hacemos la sua total de los vectores */
            int sumaTotal = 0;
            for (int i = 0; i < n1; i++) sumaTotal += vector1[i];
            for (int i = 0; i < n2; i++) sumaTotal += vector2[i];
            wait(NULL);
	    /*Hacemos el cierre de la pipe en modo lectura */
            close(fd[0]);
	   /*Hacemos el paso de los datos por la pipe en modo escritura */
            write(fd[1], &sumaTotal, sizeof(int));
	    /*Cerramos los datos */
            close(fd[1]);
            exit(0);
        }
    } else{
	/*Hacemos el fork al proceso2 */
        pid2 = fork();

        if (pid2 == 0) {
            int sumaB = 0;
            for (int i = 0; i < n2; i++) sumaB += vector2[i];
            close(fd[0]);
            write(fd[1], &sumaB, sizeof(int));
            close(fd[1]);
            exit(0);
        } else{
            close(fd[1]);
            int sA, sB, sTotal;

            /*Lectura de los archivos */
            read(fd[0], &sA, sizeof(int));
            read(fd[0], &sTotal, sizeof(int));
            read(fd[0], &sB, sizeof(int));

            /*Valores finales de la suma */
            printf("SumaA: %d\n", sA);
            printf("SumaB: %d\n", sB);
            printf("Suma Total: %d\n", sTotal);

	    /*Hacemos el cierre de la pipe en modo lectura */
            close(fd[0]);
            wait(NULL);
            wait(NULL);
        }
    }
    /*Libermos la memoria */
    free(vector1);
    free(vector2);
    return 0;
}
