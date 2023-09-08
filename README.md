# About
The purpose of this project is to create a program capable of not only solving a sudoku puzzle but also generating a sudoku puzzle, by reducing the problem to an exact cover problem and using [Knuth's dancing links algorithm X](https://www.ocf.berkeley.edu/~jchu/publicportal/sudoku/0011047.pdf) (DLX). In the past I have done something similar, using a simple recursive backtracking technique to solve a sudoku puzzle. I would like to build on that experience to produce a more efficient algorithm, that is capable of obtaining all possible solutions to a given puzzle. 

A key component of a valid Sudoku is that it has only one solution. My previous program would generate a Sudoku puzzle that was solvable, but it might have many solutions. Using DLX, I will be able to generate a puzzle that has only one possible solution. 

## Exact Cover Problem
    
An [exact cover problem](https://www.geeksforgeeks.org/introduction-to-exact-cover-problem-and-algorithm-x/) is defined thusly. Given a collection S of subsets of set X, an exact cover is the subset S* of S such that each element of X is contained is exactly one subset of S*. It should satisfy following two conditions. 
  1. The Intersection of any two subsets in S* should be empty. That is, each element of X should be contained in at most one subset of S*
  2. Union of all subsets in S* is X. That means union should contain all the elements in set X. So we can say that S* covers X.

What does that mean exactly? Well, the best way to think of an exact cover problem is as a matrix of ones and zeroes, as shown below. 

            A B C D E F G

    Row 0   0 1 0 1 0 0 1
    Row 1   1 0 1 0 1 0 1
    Row 2   0 1 0 1 0 1 0
    Row 3   1 0 0 1 0 1 0 
    Row 4   1 0 1 0 1 1 0

The solution to an exact cover problem is a set of rows, which, between them, have a single one in every column. In the exact cover matrix shown above, there are several solutions; {Row 0, Row 4} and {Row 1, Row 2} are both solutions. 

In other writings on the subject, the columns are often described as constraints. For me, it makes more sense to think of them as a requirement. Each column in the table represents a requirement that must be fulfilled to obtain a complete solution. In the solution of an exact cover problem, a requirement can only be fulfilled by one choice. Each row is a choice that fulfills some of the requirements. 

A sudoku puzzle requires every cell to be filled, and each row, column, and box must have only one of each value (1 through 9). There are nine different ways to fulfill every requirement.
To represent a sudoku puzzle we implement a matrix where each row represents a cell filled with a specific value. The final solution will consists of 81 rows, which collectively fulfill all of the requirements of a sudoku puzzle. 

## Algorithm X
Here are the basic steps of the algorithm X.

1. If there are no columns left, store this solution and return. When there are no columns left that means all requirements have been fulfilled. 
2. Choose a column. 
3. Cycle through each row in the column. Assume this row is part of the solution. Since it is part of the solution, cover up this column. Additionally, cover up any row that fulfills the same requirements as this row. Cover up any columns that are fulfilled by these rows. 
4. Continue, recursively, until either you get to a column that does not have any elements in the rows beneath it, or there are no more columns left. 
- - If the column does not have any elements in the rows beneath it, this solution is not possible. Backtrack from there. 
  - If there are no more columns left uncovered, that means that all the requirements have been fulfilled. Store this current, valid solution, and backtrack. (The algorithm will continue to cycle through other possible solutions until all are found). 


## Stretch goals
- Adding a grading scale, so that the user can select the difficulty of the puzzle they want generated.
  - Some components that could indicate difficulty include the number of initially blank squares and the number of squares that have forced values or only a few possible options from initial conditions. 
- Add a GUI with python to show puzzles and solutions

### References 
[https://www.geeksforgeeks.org/introduction-to-exact-cover-problem-and-algorithm-x/](https://www.geeksforgeeks.org/introduction-to-exact-cover-problem-and-algorithm-x/)


