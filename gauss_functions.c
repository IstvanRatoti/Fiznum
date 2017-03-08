#include<math.h>

#include "gauss_functions.h"
//#include "matr_functions_1d.h"

/*
*   This function gives back the position of the largest absolute value
*   in the submatrix (given by the start argument) of a matrix.
*/
pair get_largest(matrix_1d matrix, int start)
{
    int i, j;
    double largest = 0;
    double temp;
    pair pos;
    pos.num1 = 0;
    pos.num2 = 0;

    for(i=start;i<matrix.rows;i++)
    {
        for(j=start;j<matrix.columns;j++)
        {
            temp = get_value(i, j, matrix);
            if(fabs(temp)>fabs(largest))
            {
                largest = temp;
                pos.num1 = i;
                pos.num2 = j;
            }
        }
    }

    return pos;
}

/*
*   Switches 2 rows in a matrix.
*/
void switch_rows(int row1, int row2, matrix_1d matrix)
{

}

/*
*   Switches 2 columns in a matrix, and returns the 2 switched columns.
*/
pair switch_columns(int column1, int column2, matrix_1d matrix)
{
    pair pr;
    pr.num1 = 0;
    pr.num2 = 0;

    return pr;
}

/*
*   Multiplies a row in a matrix by a specified amount.
*/
void mult_row(double mult, int row, matrix_1d matrix)
{

}

/*
*   Substracts one row from another in a matrix.
*/
void sub_row(int row1, int row2, matrix_1d matrix)
{

}
