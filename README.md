# gol
For this project, I implemented a program that plays Conway’s Game of Life.
Conway’s Game of Life is an example of discrete event simulation, where a world of entities live, die, or are born based on their surrounding neighbors. 
Each time step simulates another round of living or dying.
My world is represented by a 2D array of values (0 or 1). 
If a grid cell’s value is 1, it represents a live object, if it is 0 a dead object. 
At each discrete time step, every cell in the 2D grid gets a new value based on the current value of its eight neighbors:

A live cell with zero or one live neighbors dies from loneliness.
A live cell with four or more live neighbors dies due to overpopulation.
A dead cell with exactly three live neighbors becomes alive.

My 2D world is a torus; every cell in the grid has exactly eight neighbors. 
In the torus world, cells on the edge of the grid have neighbors that wrap around to the opposite edge. 
