# Sudoku en C

Este es un proyecto de Sudoku implementado en lenguaje C, que permite al usuario jugar al clásico rompecabezas de 9x9 con diferentes niveles de dificultad, detección de errores en tiempo real y una interfaz visual en la terminal.

## Características

- Generación de tableros completos y únicos aleatoriamente con simetría rotacional de 180 grados.
- Eliminación inteligente de casillas en base a la dificultad elegida.
- Validación en tiempo real de movimientos del usuario.
- Detección de conflictos por fila, columna o subcasilla.
- Coloreado en la terminal (compatible con ANSI/UTF-8).
- Mecanismo de resolución con varias técnicas lógicas (`naked singles`, `hidden singles`, `locked candidates`, `naked pairs/triples`, `x-wing, swordfish`, `forcing chains, XY-wing`).

## Estructura del proyecto

- `sudoku.c`: Controla el flujo principal del juego, gestiona la interacción con el usuario y la lógica de selección de idioma.
- `sudoku.h`: Contiene constantes, estructuras de datos y declaraciones de funciones para el juego de Sudoku.
- `generator.c`: Genera un tablero completo de Sudoku y elimina celdas según el nivel de dificultad elegido.
- `solver.c`: Verifica si un tablero es resoluble utilizando técnicas específicas para cada nivel de dificultad.
- `display.c`: Imprime el tablero en la terminal con mejoras visuales como colores, bordes y resaltado de errores.
- `solutions.c`: Cuenta el número de soluciones para garantizar que cada puzzle tenga una solución única.
- `candidates.c`: Administra listas de candidatos para las celdas y aplica técnicas básicas como Naked Singles y Hidden Singles.
- `advanced_techniques.c`: Implementa técnicas avanzadas de resolución como Locked Candidates, X-Wing y Swordfish.
- `utils.c`: Proporciona funciones de utilidad, como verificar subcuadrículas con una celda vacía.

## Compilación

Usá `gcc` para compilar todos los archivos:

```bash
gcc -g -fstack-protect -o sudoku sudoku.c generator.c solver.c display.c solutions.c candidates.c advanced_techniques.c utils.c
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

Desarrollado como ejercicio de práctica para la materia "Algorítmos y estructuras de datos" en lenguaje C.
