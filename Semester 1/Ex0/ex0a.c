
/* ============================================================================
 * Name:       ex0
 * Written by: Sahar Itzhak, Ron Efraim
 * ID:         308389485, 204661425
 * Login:      saharyz, ronef
 * Program:    Allocate a matrix with data
 * ============================================================================
 *
 * This program opens a file of integers.
 * the program will insert ints from the file to a struct, and check for a row
 * will total highest values, if such was not found, program will print -1 and
 * free the memory allocated.
 *
 * Compile: gc -Wall ex0.c -0 ex0
 * Run: ex0
 * Input: a file of ints.
 * Output: the num of the line with the total highest values, else -1.
 * Run Time: m*n^2 when m is the lines_len array and n is the 2nd array.
 * ============================================================================
 */


//===========================================================================//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//===========================================================================//

struct Data
{
	int _num_of_lines;
	int ** _the_data;
	int * _lines_len;
};

//===========================================================================//

struct Data allocate(FILE *input_file, struct Data new_data);
int max_row(struct Data new_data);
void check_allocate_row(int *row);
void check_allocate_matrix(int **matrix);
void free_memory(struct Data new_data);

//===========================================================================//

int main(int argc, char **argv)
{
	//read file
	FILE* input_file;
	int print_row;

	//if bad file
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <inp file>\n", argv[0]);
		exit(EXIT_FAILURE) ;
	}

	//allocate new struct
	struct Data new_data,
	temp;

	new_data._lines_len = NULL;
	new_data._the_data = NULL;
	new_data._num_of_lines = 0;

	//opens file
	input_file = fopen(argv[1], "r");

	//if file can't be opened
	if (input_file == NULL)
	{
		fputs("Cannot open files\n", stderr);
		exit(EXIT_FAILURE);
	}

	//make mat
	temp = allocate(input_file, new_data);
	//finds max row
	print_row = max_row(temp);

	printf("%d\n", print_row);

	//close file
	fclose(input_file);


	return EXIT_SUCCESS;
}

//===========================================================================//

struct Data allocate(FILE *input_file, struct Data new_data)
{
	int size,
	used_rows = 0,
	value = 0,
	i = 0;

	//read a number
	fscanf(input_file, "%d", &size);

	//check if allocate a row is needed
	while(!feof(input_file))
	{
		//check if new alloc is needed
		if(new_data._num_of_lines == used_rows)
		{
			//make 1d
			new_data._lines_len = (int**) realloc(new_data._lines_len,
					((new_data._num_of_lines == 0) ? 1 :
							new_data._num_of_lines * 2)
							*sizeof(int *));

			//make 2d
			new_data._the_data = (int**) realloc(new_data._the_data,
					((new_data._num_of_lines == 0) ? 1 :
							new_data._num_of_lines * 2)
							*sizeof(int *));

			//check alloc 1d
			check_allocate_row(new_data._lines_len);
			//check alloc 2d
			check_allocate_matrix(new_data._the_data);

			//inc num of lines
			new_data._num_of_lines = (new_data._num_of_lines == 0) ? 1 :
					new_data._num_of_lines * 2;
		}

		//alloc new memory
		new_data._the_data[used_rows] = (int*) malloc
				(size * sizeof(int));
		//check allocation 2d
		check_allocate_matrix(new_data._the_data);

		//insert values
		for(i = 0; i < size; i++)
		{
			fscanf(input_file, "%d", &value);
			new_data._the_data[used_rows][i] = value;
		}
		new_data._lines_len[used_rows] = size;
		used_rows ++;
		fscanf(input_file, "%d", &size);
	}
	new_data._the_data = (int**)realloc(  new_data._the_data, used_rows*sizeof(int*));
	new_data._lines_len = (int*)realloc(new_data._lines_len,used_rows*sizeof(int));
	new_data._num_of_lines = used_rows;

	return new_data;
}

//===========================================================================//

int max_row(struct Data new_data)
{
	int	c_cell = 0,
			c_col = 0,
			c_row = 0,
			cell_value = 0;
	bool exist = true;

	//run on row
	for(c_row = 0; c_row < new_data._num_of_lines; c_row++)
	{
		//run on col
		for(c_col = 0; c_col < new_data._lines_len[c_row]; c_col++)
		{
			//insert values
			cell_value = new_data._the_data[c_row][c_col];

			for(c_cell = 0; c_cell < new_data._num_of_lines; c_cell++)
			{
				if(c_col >= new_data._lines_len[c_cell])
					continue;

				if(c_row != c_cell)
				{
					if(new_data._the_data[c_row][c_col] >=
							new_data._the_data[c_col][c_cell])
						exist = true;
					else
					{
						if(c_cell > 0)
							c_cell = 0;
						exist = false;
						break;
					}
				}
			}
			if(!exist)
				break;
			return c_row;
		}
	}
	return -1;
}

//===========================================================================//

void check_allocate_row(int *row)
{
	//check allocation in a row
	if(row == NULL)
	{
		fputs("Cannot Allocate memory\n", stderr);
		exit(EXIT_FAILURE);
	}
}

//===========================================================================//

void check_allocate_matrix(int **matrix)
{
	//check allocation in matrix
	if(matrix == NULL)
	{
		fputs("Cannot Allocate memory\n", stderr);
		exit(EXIT_FAILURE);
	}
}

//===========================================================================//

void free_memory(struct Data new_data)
{
	int row;
	//free each row in the matrix
	for(row = 0 ; row < new_data._num_of_lines; row++)

		free(new_data._the_data[row]);

	free(new_data._the_data);		// free the matrix itself
	free(new_data._lines_len);		// free allocated array
}

