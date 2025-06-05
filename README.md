# Sudoku en C

Este es un proyecto de Sudoku implementado en lenguaje C, que permite al usuario jugar al clásico rompecabezas de 9x9 con diferentes niveles de dificultad, detección de errores en tiempo real y una interfaz visual en la terminal.

## Características

- Generación de tableros completos y únicos aleatoriamente.
- Eliminación de casillas en base a la dificultad elegida.
- Validación en tiempo real de movimientos del usuario.
- Detección de conflictos por fila, columna o subcuadro.
- Coloreado en la terminal (compatible con ANSI/UTF-8).
- Mecanismo de resolución con técnicas lógicas (`naked singles`, `hidden singles`, `locked candidates`).

## Estructura del proyecto

- `sudoku.c`: Controla el flujo principal del juego, gestiona la interacción con el usuario y la lógica de verificación de movimientos.
- `sudoku.h`: Contiene constantes, estructuras de datos y declaraciones de funciones.
- `generator.c`: Se encarga de generar un tablero completo válido y luego vaciar celdas según la dificultad seleccionada.
- `solver.c`: Contiene algoritmos para contar soluciones y resolver tableros usando técnicas de Sudoku.
- `display.c`: Se encarga de imprimir el tablero en la terminal de forma visualmente amigable y coloreada.

## Compilación

Usá `gcc` para compilar todos los archivos:

```bash
gcc sudoku.c generator.c solver.c display.c -o sudoku
```

## Ejecución

```bash
./sudoku
```

## Niveles de dificultad

Al iniciar el juego, se te pedirá que selecciones un nivel de dificultad del 1 al 5:

1. Fácil
2. Medio
3. Difícil
4. Avanzado
5. Experto

Cuanto más alto el nivel, más celdas vacías y más complejas las técnicas necesarias para resolverlo.

## Controles del juego

- Se te pedirá que ingreses: **fila columna valor**, por ejemplo: `3 4 7`
- Si el movimiento es inválido (por reglas del Sudoku o genera múltiples soluciones), se marcará el error y se mostrará en rojo.
- El juego termina cuando el tablero se completa correctamente.

## Dependencias

Este programa usa solamente la biblioteca estándar de C (`stdio.h`, `stdlib.h`, `stdbool.h`, `time.h`, `unistd.h`), por lo que no requiere instalación de paquetes adicionales.

---

Desarrollado como ejercicio de programación estructurada y algoritmos lógicos en C.
