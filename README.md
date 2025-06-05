# Sudoku Game in C

## Overview
This is a terminal-based Sudoku game implemented in C. The game generates a valid Sudoku puzzle based on a user-selected difficulty level, allows players to fill in the board, and provides visual feedback for invalid moves and game progress. The program ensures that puzzles have a unique solution and supports multiple difficulty levels with varying solving techniques.

## Features
- **Dynamic Puzzle Generation**: Generates valid Sudoku puzzles with a unique solution for five difficulty levels: Easy, Medium, Hard, Advanced, and Expert.
- **Difficulty-Based Techniques**:
  - Easy: Solvable with Naked Singles and at least one 3x3 subgrid with a single empty cell.
  - Medium: Solvable with Naked Singles and Hidden Singles.
  - Hard/Advanced/Expert: Requires advanced techniques like Locked Candidates, ensuring puzzles aren’t solvable with simpler methods.
- **Visual Interface Enhancements**:
  - **Color-Coded Subgrids**: Alternating white and gray backgrounds for 3x3 subgrids to enhance readability.
  - **Fixed Cell Highlighting**: Fixed cells (initial and user-locked numbers) are displayed in bold blue.
  - **Error Feedback**: Invalid moves highlight the conflicting row, column, or subgrid in red for one turn.
  - **Fancy Borders**: Uses Unicode characters (e.g., ═, ║, ╬) for a polished grid, with an ASCII fallback for compatibility.
  - **Progress Indicator**: Displays the percentage of filled cells above the board.
- **Error Tracking**: Counts and displays the number of invalid moves made by the player.
- **Input Validation**: Ensures user inputs are valid (e.g., numbers 1-9, no modification of fixed cells).
- **Terminal Compatibility**: Colors and Unicode characters are disabled if the output isn’t a terminal (e.g., redirected to a file).

## Files
- `sudoku.c`: Main game loop, user input handling, and puzzle initialization.
- `sudoku.h`: Header file with global variables, structures, and function declarations.
- `generator.c`: Functions for generating a full board and removing numbers based on difficulty.
- `solver.c`: Functions for solving puzzles and checking solvability with specific techniques.
- `display.c`: Functions for printing the board with visual enhancements.
- `README.md`: This file.

## Requirements
- A C compiler (e.g., `gcc`).
- A terminal that supports ANSI escape codes for colors and Unicode characters for borders (e.g., Linux/macOS terminals, Windows Terminal). For terminals with limited support, set `USE_UNICODE` to `0` in `display.c` for ASCII borders.

## Compilation
To compile the program, navigate to the project directory and run:

```bash
gcc -o sudoku sudoku.c generator.c solver.c display.c
```

Ensure all source files (`sudoku.c`, `
