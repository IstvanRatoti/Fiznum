#include<stdio.h>
#include<stdlib.h>

#include"matr_functions_1d.h"

void usage(char * prog_name);

matrix_1d mult_matrices(matrix_1d mat1, matrix_1d mat2);

int main(int argc, char *argv[])
{
	matrix_1d mat1, mat2, result;

	if(argc!=4)
		usage(argv[0]);

	mat1 = read_matrix_1d(argv[1]);
	//write_matrix_1d(mat1, "stdout");    // Debug code.
	//printf("\n");
	mat2 = read_matrix_1d(argv[2]);
	//write_matrix_1d(mat2, "stdout");    // Debug code.
	//printf("\n");

	//printf("Calculating result:\n");    // Debug code.

	result = mult_matrices(mat1, mat2);

	if(result.columns == -1)    // "Catches" the error in the multiplication.
		fprintf(stderr, "There was an error during matrix multiplication!\n");
	else
		write_matrix_1d(result, argv[3]);

    free(mat1.values);
	free(mat2.values);      // 14th Amendment.
    free(result.values);

	return 0;
}

/*
*   Simple usage function if we give the program an illegal number of arguments.
*   It also details how, and what arguments can be given to the program.
*/
void usage(char * prog_name)
{
	fprintf(stderr, "Illegal number of arguments!\nUsage: %s <first input file name> <second input file name> <output file name>\n", prog_name);
	fprintf(stderr, "You can also specify the output to be the standard output, by writing \"stdout\" instead of a file name.\n");
	exit(1);
}

/*
*   Matrix multiplier. Pretty basic, but checks for incompatible matrices too.
*/
matrix_1d mult_matrices(matrix_1d mat1, matrix_1d mat2)
{
	matrix_1d result;
	int i, j, k;
	double temp = 0;
	double t1, t2;

	if(mat1.columns != mat2.rows)   // Checks if the two matrices are compatible.
	{
		fprintf(stderr, "The two matrices are not compatible! They cannot be multiplied.\n");
		result.columns = -1;    // Value for error handling.
		return result;
	}

	result.rows = mat1.rows;
	result.columns = mat2.columns;  // Need to initialize these values, because we use them.
	result.values = (double *)calloc(mat1.rows*mat2.columns, sizeof(double));

	//printf("Loop boundaries:\nOuter: %d\tMiddle: %d\tInner: %d\n", mat1.rows, mat2.columns, mat1.columns);    Debug code

	for(i=0;i<mat1.rows;i++)
	{
		//result.values[i] = (double *)calloc(mat2.columns, sizeof(double));

		for(j=0;j<mat2.columns;j++)
		{
			//result.values[i][j] = 0;

			for(k=0;k<mat1.columns;k++)
			{
				//result.values[i][j] += mat1.values[i][k] * mat2.values[k][j];
				t1 = get_value(i, k, mat1);
				t2 = get_value(k, j, mat2);

				//printf("Multiplying %g (Row: %d Column: %d) with\n", t1, i, k);
				//printf("with %g (Row: %d Column: %d)\n", t2, k, j);   Debug code
                //getchar();

				temp += t1*t2;
            }

            //printf("The value of element at row %d column %d: %g", i, j, temp);   Debug code
            //getchar();

            set_value(i, j, result, temp);
            temp = 0;
		}
	}

	//printf("Out of the loop!\n"); Debug code

	return result;
}
