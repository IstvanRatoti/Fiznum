#include "matr_functions_1d.h"

// Struct to be able to return 2 numbers (i.e. a pair, or a position).
typedef struct pr
{
    int num1;
    int num2;
}pair;

/*
*   This function checks a given row in a matrix, and returns the biggest absolute value there.
*/
double get_largest_in_row(matrix_1d matrix, int row);

/*
*   This function gives back the position of the largest absolute value
*   in the submatrix (given by the start argument) of a matrix.
*/
pair get_largest(matrix_1d matrix, int start);

/*
*   Switches 2 rows in a matrix.
*/
int switch_rows(int row1, int row2, matrix_1d matrix);

/*
*   Switches 2 columns in a matrix, and returns the 2 switched columns.
*/
pair switch_columns(int column1, int column2, matrix_1d matrix);

/*
*   Multiplies a row in a matrix by a specified amount, returns "true" if the operation succeeded.
*/
int mult_row(double mult, int row, matrix_1d matrix);

/*
*   Subtracts the second row from the first row in a matrix, returns "true" if the operation succeeded.
*/
int sub_row(int row1, int row2, matrix_1d matrix, double mult);

/*
*   The complete eliminator function. Requires two matrices as arguments and returns the result
*   of the elimination. The first needs to be a square matrix, the second either a vector or an
*   identity matrix. Returns "true" if the operation was successful.
*/
int gauss_eliminator(matrix_1d matrix1, matrix_1d matrix2);
