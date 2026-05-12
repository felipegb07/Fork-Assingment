# Descripción del proyecto

Este proyecto fue desarrollado para la materia de Sistemas Operativos de la Pontificia Universidad Javeriana con el objetivo de implementar conceptos fundamentales relacionados con:

- Creación de procesos mediante `fork()`
- Comunicación entre procesos usando `pipe()`
- Sincronización de procesos con `wait()`
- Manejo de memoria dinámica
- Lectura de archivos en lenguaje C

El programa recibe dos archivos de texto con números enteros, crea múltiples procesos (padre, hijos y nieto) y realiza diferentes cálculos de sumas que posteriormente son enviados al proceso padre mediante pipes.

---

# Integrantes

- Juan Felipe González Barón
- María Alejandra Rodríguez Betancourt
- Marlon Johan García Restrepo

---

# Institución

Pontificia Universidad Javeriana

---

# Materia

Sistemas Operativos

---

# Objetivo principal

Implementar un sistema de procesamiento concurrente utilizando procesos y comunicación entre procesos en C.

El sistema:

1. Lee dos archivos de texto.
2. Almacena los datos en memoria dinámica.
3. Crea múltiples procesos usando `fork()`.
4. Calcula:
   - La suma del primer archivo.
   - La suma del segundo archivo.
   - La suma total de ambos archivos.
5. Envía los resultados mediante `pipe()`.
6. Organiza los datos usando estructuras para evitar problemas de concurrencia.

---

# Tecnologías utilizadas

- Lenguaje C
- GCC
- Linux / Unix
- Pipes (`pipe()`)
- Procesos (`fork()`)
- Sincronización (`wait()`)

---

# Estructura del proyecto

```txt
.
├── LICENSE
├── README.md
└── tallerFork
    ├── archivo00.txt
    ├── archivo01.txt
    ├── taller_procesos02.c
    └── taller_procesos.c
