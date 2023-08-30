# About
The purpose of this project is to create a program capable of not only solving a sudoku puzzle but also generating a sudoku puzzle, by reducing the problem to an exact cover problem and using [Knuth's dancing links algorithm X](https://www.ocf.berkeley.edu/~jchu/publicportal/sudoku/0011047.pdf) (DLX). This is an improvement on my previous method, which used a simple backtracking algorithm. DLX also uses backtracking of course, but this solution is more efficient and capable of finding all possible solutions to a given puzzle.

A key component of a "valid Sudoku" is that it has only one solution. My previous program would generate a Sudoku puzzle that was solvable, but it might have many solutions. Using DLX, I will be able to generate a puzzle that has only one possible solution. 

## Stretch goals
- Adding a grading scale, so that the user can select the difficulty of the puzzle they want generated.
  - Some components that could indicate difficulty include the number of initially blank squares and the number of squares that have forced values or only a few possible options from initial conditions. 
- Add a GUI with python to show puzzles and solutions

