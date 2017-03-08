#include "matr_functions_1d.h"

// Struct to be able to return 2 numbers (i.e. a pair, or a position).
typedef struct pr
{
    int num1;
    int num2;
}pair;

/*
*   This function gives back the position of the largest absolute value
*   in the submatrix (given by the start argument) of a matrix.
*/
pair get_largest(matrix_1d matrix, int start);

/*
*   Switches 2 rows in a matrix.
*/
void switch_rows(int row1, int row2, matrix_1d matrix);

/*
*   Switches 2 columns in a matrix, and returns the 2 switched columns.
*/
pair switch_columns(int column1, int column2, matrix_1d matrix);

/*
*   Multiplies a row in a matrix by a specified amount.
*/
void mult_row(double mult, int row, matrix_1d matrix);

/*
*   Substracts one row from another in a matrix.
*/
void sub_row(int row1, int row2, matrix_1d matrix);
