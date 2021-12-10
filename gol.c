/**
 * File: gol.c
 *
 * Implementation of the game of life simulator functions.
 */

#define _XOPEN_SOURCE 600

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>

#include "gol.h"

/**
 *Translates world into torus and 2D becomes 1D array.
 *
 *@param row Row in 2D array
 *@param col Column in 2D array
 *@param num_rows Total number of rows
 *@param num_cols Total number of columns
 *
 * @return The correct index for the ID array including making sure it is a
 * torus.
 */
int translate_to_1D(int row, int col, int num_rows, int num_cols){
	if(col <= -1) { //update column when trying to access index outside edges of world
    	col = num_cols - 1;
  	}
  	else if(col >= num_cols) {
    	col = 0;
  	}
  	if(row <= -1) { //update row when trying to access index outside edges of world
    	row = num_rows - 1;
  	}
  	else if(row >= num_rows) {
 	   row = 0;
  	}
	return (row * num_cols) + col;
}

/**
 *Initializes what the world should look like at iteration 0.
 *
 *@param *config_filename The name of the file being read.
 *@param *num_cols The total number of columns in the world.
 *@param *num_rows The total number of rows in the world.
 *
 *@return The newly created and initialized world or if file is bad format or null return NULL. Also returns the number of cols and rows through use of parameters as return values.
 *
 */
int *initialize_world(char *config_filename, int *num_cols, int *num_rows) {
	FILE *data_file = fopen(config_filename, "r"); //open file using read mode
	if (data_file == NULL){ //return NULL if the file is unable to be read
		printf("file is null");
		return NULL;
	}
	int num_coords, row_val, col_val;
	int count_scan = fscanf(data_file, "%d",num_rows); //read in first three values of file
	count_scan += fscanf(data_file, "%d", num_cols);
	count_scan += fscanf(data_file, "%d", &num_coords);
	
	if (count_scan != 3){ //return NULL if improper file format
		printf("improper file format");
		return NULL;
	}	
	int *arr = calloc(*num_rows * *num_cols, sizeof(int)); //create array for world
	if(arr == NULL){ //exit if cannot allocate memory
		printf("calloc fail in init world");
		endwin();
		exit(1);
	}
	for (int i= 0; i < num_coords; i++){ //for loop to make initital cells alive as stated in file
		fscanf(data_file,"%d %d", &col_val, &row_val);
		int coords = translate_to_1D(row_val, col_val,*num_rows, *num_cols);
		arr[coords] = 1;
	}
	return arr;
}

/**
 *Updates the world for one time step in the simulation.
 *
 *@param *world The world array.
 *@param width Total number of columns in the world.
 *@param height Total number of rows in the world.
 *
 *@return No return, void function.
 *
 */
void update_world(int *world, int width, int height) {
	int *temp_arr = calloc(height * width, sizeof(int));//create a temp array for reading pruposes
	if(temp_arr == NULL){//exit if unable to allocate memory
		printf("calloc fail in update_world");
		endwin();
		exit(1);
	}
	for (int i=0; i < height; i++){	//copy the world into the temp array
		for (int j = 0; j < width; j++){
			int x = translate_to_1D(i, j,height, width);
			temp_arr[x] = world[x];		
		}
	}
	for (int i=0; i < height; i++){ //count living neighbors for all cells in world
		for (int j = 0; j < width; j++){
			int living = 0;
			for (int row = -1; row <= 1; row++){ //loop through the eight surrounding neighbors
				for (int col = -1; col <= 1; col++){
					living += temp_arr[translate_to_1D(i + row, j+ col, height, width)]; //adds 0 when neighbor is dead and 1 when alive
				}
			}

			int coord = translate_to_1D(i, j, height, width);
			if(world[coord] ==1 ){ //make sure that living cell itself is not counted as a neighbor
				living -= 1;
			}
			if(world[coord] == 1 && (living == 0 || living == 1)){ //A live cell with zero or one live neighbors dies from loneliness.
				world[coord] = 0;
			}
			else if(world[coord] == 1 && living >= 4){ //A live cell with four or more live neighbors dies due to overpopulation.
				world[coord] = 0;
			}
			else if(world[coord] == 0 && living == 3){ //A dead cell with exactly three live neighbors becomes alive.
				world[coord] = 1;
			}
		}
	}
	free(temp_arr); //free memory that was allocated
}


/**
 *Prints the world to the screen.
 *
 *@param *world The world array.
 *@param width The total number of columns in the world.
 *@param height The total number of rows in the world.
 *@param turn The turn number.
 *
 *@return No return, void function.
 *
 */
void print_world(int *world, int width, int height, int turn) {
	clear(); // clears the screen
	
	for (int row = 0; row < height; row++) { 
		for (int col = 0; col < width; col++) {//iterate through the array
			int coord = translate_to_1D(row, col, height, width); //get 1D coordinate for 2D array
			if (world[coord] == 1) {
				mvaddch(row, col, '@'); //print @ if cell is alive
			}
			else {
				mvaddch(row, col, '.'); //print . if cell is dead
			}

		}
	}
	char my_str[100]; //allocate space for the string
	sprintf(my_str, "\nTime Step: %d", turn); //turn adds becomes a string
	mvaddstr(height, 0, my_str);	//prints Time Step and number 
	refresh(); // displays the text we've added

}
