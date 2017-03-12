#include<math.h>
#include<stdio.h>

#include "gauss_functions.h"

/*
*   This function gives back the position of the largest absolute value
*   in the submatrix (given by the start argument) of a matrix.
*/
pair get_largest(matrix_1d matrix, int start)
{
    int i, j;
    double largest = 0;
    double temp;
    pair pos;       // Position of the largest element.
    pos.num1 = 0;
    pos.num2 = 0;

    // Start position can't be larger than the matrix size.
    if(start >= matrix.rows)                                                                            // Test for wrong starting position.                Done
    {
        //fprintf(stderr, "Start position is larger than the matrix size!\n");  Move up to the eliminator.
        pos.num2 = -1;

        return pos;
    }

    // Usually, the outer loop goes through the rows, but
    // we need to check the starting row for singularity.
    for(i=start;i<matrix.columns;i++)
    {
        for(j=start;j<matrix.rows;j++)
        {   // Remember, rows and columns are switched now!!!
            temp = get_value(j, i, matrix);
            if(fabs(temp)>fabs(largest))                                                                // Test for:
            {                                                                                           //      - larger positive then another positive     Done
                largest = temp;                                                                         //      - smaller negative then another negative    Done
                pos.num1 = j;                                                                           //      - "larger" negative then another positive   Done
                pos.num2 = i;                                                                           //      - larger positive then another negative     Done
            }
        }

        // This code checks for a singular matrix.
        if((i == start)&&(0 == largest))                                                                // Test for singularity!                            Done
        {
            //fprintf(stderr, "The matrix is singular!\n");     Move up to the eliminator.
            pos.num1 = -1;

            return pos;
        }
    }

    // Check for numerical singularity, and write a warning.
    if(fabs(largest) < 1e-10)                                                                           // Test for numerical singularity? How?
        fprintf(stderr, "*** Warning! ***\nPossible numerical singularity!\n");

    return pos;
}

/*
*   Switches 2 rows in a matrix, returns "true" if it succeeded, "false" if it did not.
*/
int switch_rows(int row1, int row2, matrix_1d matrix)
{
    int i;
    double temp;

    // If we have less rows than the specified, return "false".
    if((row1>=matrix.rows)||(row2>=matrix.rows))                                                        // Test for trying to switch with non-existing rows.    Done
        return 0;

    // Go through all the columns, and switch every value.
    for(i=0;i<matrix.columns;i++)                                                                       // Test if it really swapped the elements.              Done
    {
        temp = get_value(row1, i, matrix);
        set_value(row1, i, matrix, get_value(row2, i, matrix));
        set_value(row2, i, matrix, temp);
    }

    return 1;   // This value signifies that the operation was successful.
}

/*
*   Switches 2 columns in a matrix, and returns the 2 switched columns.
*/
pair switch_columns(int column1, int column2, matrix_1d matrix)
{
    int i;
    double temp;

    pair pr;
    pr.num1 = 0;
    pr.num2 = 0;

    // If we have less rows than the specified, set -1 as a signal, and return.
    if((column1>=matrix.columns)||(column2>=matrix.columns))                                                // Test for trying to switch with non-existing columns.     Done
    {
        pr.num1 = -1;
        return pr;
    }

    // Go through all the rows, and switch every value.
    for(i=0;i<matrix.rows;i++)                                                                              // Test if it really swapped the elements.                  Done
    {
        temp = get_value(i, column1, matrix);
        set_value(i, column1, matrix, get_value(i, column2, matrix));
        set_value(i, column2, matrix, temp);
    }

    pr.num1 = column1;
    pr.num2 = column2;
    return pr;
}

/*
*   Multiplies a row in a matrix by a specified amount, returns "true" if the operation succeeded.
*/
int mult_row(double mult, int row, matrix_1d matrix)
{
    int i;
    double temp;

    // Check for invalid row, return "false" if matrix doesn't have that much rows.
    if(row>=matrix.rows)                                                                                    // Test with invalid row.                                   Done
        return 0;

    // Loop through the row, and multiple every element with the specified amount.
    for(i=0;i<matrix.columns;i++)                                                                           // Test the multiplication with:
    {                                                                                                       //                          - positive integer              Done
        temp = mult*get_value(row, i, matrix);                                                              //                          - negative integer              Done
        set_value(row, i, matrix, temp);                                                                    //                          - double                        Done
    }                                                                                                       //                          - value less than 1 (division)  Done

    return 1;
}

/*
*   Subtracts the second row from the first row in a matrix, returns "true" if the operation succeeded.
*/
int sub_row(int row1, int row2, matrix_1d matrix)
{
    int i;
    double temp;

    // Checks if we have given 2 valid rows.
    if((row1>=matrix.rows)||(row2>=matrix.rows))                                                            // Test valid rows.                                             Done
        return 0;

    // The loop subtracts the values in row2 from the values in row1, then sets
    // the row1 values to this new value.
    for(i=0;i<matrix.columns;i++)                                                                           // Test values with rows containing:
    {                                                                                                       //                              - positive integers             Done
        temp = get_value(row1, i, matrix) - get_value(row2, i, matrix);                                     //                              - negative integers (addition)  Done
        set_value(row1, i, matrix, temp);                                                                   //                              - doubles                       Done
    }

    return 1;
}
