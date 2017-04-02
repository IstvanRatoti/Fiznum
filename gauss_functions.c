#include<math.h>
#include<stdio.h>
#include<stdlib.h>

#include "gauss_functions.h"

/*
*   This function checks a given row in a matrix, and returns the biggest absolute value there.
*/
double get_largest_in_row(matrix_1d matrix, int row)
{
    int i;
    double temp, largest = 0;

    // Check for invalid row.
    if(row>=matrix.rows)                                                                                // Test with invalid row.       Done
        return -1;

    // Loops through the elements of the row and searches for the largest element.
    for(i=0;i<matrix.columns;i++)                                                                       // Test with:
    {                                                                                                   //          - positive integer  Done
        temp = get_value(row, i, matrix);                                                               //          - negative double   Done

        if(fabs(temp)>fabs(largest))
            largest = temp;
    }

    return largest;
}

/*
*   This function gives back the position of the largest absolute value
*   in the sub-matrix (given by the start argument) of a matrix.
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
        if((i == start)&&(fabs(largest) < 1.0e-50))                                                    // Test for singularity!                            Done
        {
            //fprintf(stderr, "The matrix is singular!\n");     Move up to the eliminator.
            pos.num1 = -1;

            return pos;
        }
    }

    // Check for numerical singularity, and write a warning.
    if((fabs(largest) < 1e-10)||(fabs(largest) > 1e10))                                                                           // Test for numerical singularity? How?
        fprintf(stderr, "Warning! Possible numerical singularity!\n");

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
*   Subtracts the second row multiplied by a double from the first row in a matrix, returns "true" if the operation succeeded.
*/
int sub_row(int row1, int row2, matrix_1d matrix, double mult)
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
        temp = get_value(row1, i, matrix) - mult*get_value(row2, i, matrix);                                //                              - negative integers (addition)  Done
        set_value(row1, i, matrix, temp);                                                                   //                              - doubles                       Done
    }                                                                                                       //                              - integer multiplier            Done
                                                                                                            //                              - negative double multiplier    Done
    return 1;
}

/*
*   The complete eliminator function. Requires two matrices as arguments. The result will be in
*   the second matrix. The first needs to be a square matrix, the second either a vector or an
*   identity matrix. Returns "true" if the operation was successful.
*/
int gauss_eliminator(matrix_1d matrix1, matrix_1d matrix2)
{
    int i, j, switch_count = 0;
    double temp;
    pair tmp_pair;
    pair * switches;

    // Check if we had any problem reading the matrix.
    if((-1==matrix1.rows)||(-1==matrix2.rows))                                                              // Test with an unread matrix.
    {
        fprintf(stderr, "Unread matrix error!\n");
        matrix2.rows = -1;
        matrix2.columns = -1;

        return 0;
    }

    // Check if we have a square matrix.
    if(matrix1.rows != matrix2.rows)                                                                     // Test with a non-square matrix.
    {
        fprintf(stderr, "Incompatible matrices!\n");
        matrix2.columns = -1;

        return 0;
    }

    /*printf("\n\n\n");
    printf("***\tMatrices before eliminator\t***\n");
    printf("***\tFirst Matrix\t***\n");
    write_matrix_1d(matrix1, "stdout"); // Debug code.
    printf("***\tSecond Matrix\t***\n");
    write_matrix_1d(matrix2, "stdout");
    printf("\n\n\n");
    getchar();*/

    // Now we zero out the lower triangle.
    for(i=0;i<matrix1.rows;i++)
    {
        /*printf("\n\n\n");
        printf("***\tTriangle before zeroing #%d\t***\n", i+1);
        printf("***\tFirst Matrix\t***\n");
        write_matrix_1d(matrix1, "stdout"); // Debug code.
        printf("***\tSecond Matrix\t***\n");
        write_matrix_1d(matrix2, "stdout");
        printf("\n\n\n");
        getchar();*/

        // First we want to "normalize" the matrix/sub-matrix.
        for(j=i;j<matrix1.rows;j++)
        {
            temp = get_largest_in_row(matrix1, j);

            //printf("Largest absolute value in normalization: %e", temp);    // Debug code.
            //getchar();

            if(fabs(temp) < 1e-50)     // Check for singularity in normalization.
            {
                fprintf(stderr, "The matrix is singular!\n");
                return 0;
            }

            // Check for numerical singularity, and write a warning.
            if((fabs(temp) < 1e-10)||(fabs(temp) > 1e10))                                                                           // Test for numerical singularity? How?
                fprintf(stderr, "Warning! Possible numerical singularity!\n");

            temp = 1.0 / temp;

            if(!mult_row(temp, j, matrix1))     // Try to divide in the first matrix.
            {
                fprintf(stderr, "There was an error in row multiplication!\n");
                matrix2.columns = -1;
                return 0;
            }
            if(!mult_row(temp, j, matrix2))     // And in the second matrix.
            {
                fprintf(stderr, "There was an error in row multiplication!\n");
                matrix2.columns = -1;
                return 0;
            }
        }

        /*printf("\n\n\n");
        printf("***\tMatrices after normalization\t***\n");
        printf("***\tFirst Matrix\t***\n");
        write_matrix_1d(matrix1, "stdout"); // Debug code.
        printf("***\tSecond Matrix\t***\n");
        write_matrix_1d(matrix2, "stdout");
        printf("\n\n\n");
        getchar();*/

        // We need to find the largest element in the matrix.
        tmp_pair = get_largest(matrix1, i);

        //printf("Largest absolute value: %e", get_value(tmp_pair.num1, tmp_pair.num2, matrix1)); // Debug code.
        //printf("At loop: %d\n", i);
        //printf("Largest at row: %d column: %d\n", tmp_pair.num1, tmp_pair.num2);
        //getchar();

        if(-1==tmp_pair.num1)       // Check for singularity.
        {
            fprintf(stderr, "The matrix is singular!\n");
            return 0;
        }

        if(tmp_pair.num1!=i)    // If the largest element is in another row
        {                       // switch the current row with that one.
            switch_rows(tmp_pair.num1, i, matrix1);
            switch_rows(tmp_pair.num1, i, matrix2);   // The other matrix too.

            //printf("Switching row %d with row %d.\n", tmp_pair.num1, i);    // Debug code.
            //getchar();
        }

        if(tmp_pair.num2!=i)  // If the largest element is in another column
        {               // switch the current column with that one.
            //printf("Switching column %d with column %d.\n", tmp_pair.num2, i);  // Debug code.
            //getchar();

            tmp_pair = switch_columns(tmp_pair.num2, i, matrix1);
            /*if(1!=matrix2.columns) // If its an identity, switch those columns too.
                tmp_pair = switch_columns(tmp_pair.num2, i, matrix1);*/
                // Note: Don't think we need to bother about identities.

            // Register the column switches in switches and switch_count.
            if(0==switch_count)     // Store the first switch.
            {
                switches = (pair *)calloc(1, sizeof(pair));
                switches[switch_count++] = tmp_pair;    // Increment switch_count and store the pair.
            }
            else                    // Store the rest.
            {
                switches = (pair *)realloc(switches, (switch_count+1)*sizeof(pair));
                switches[switch_count++] = tmp_pair;    // Increment switch_count and store the pair.
            }

            //printf("The new pair: %d %d", switches[switch_count-1].num1, switches[switch_count-1].num2);  Debug code.
        }

        /*printf("\n\n\n");
        printf("***\tTriangle after switching #%d\t***\n", i+1);
        printf("***\tFirst Matrix\t***\n");
        write_matrix_1d(matrix1, "stdout"); // Debug code.
        printf("***\tSecond Matrix\t***\n");
        write_matrix_1d(matrix2, "stdout");
        printf("\n\n\n");
        getchar();*/

        // Zero out the elements under our current one.
        for(j=i+1;j<matrix1.rows;j++)
        {
            temp = get_value(j, i, matrix1);    // Set the multiplier.

            sub_row(j, i, matrix1, temp);       // Zero out matrix1 row.
            sub_row(j, i, matrix2, temp);       // Zero out matrix2 row.

        }

        /*printf("\n\n\n");
        printf("***\tTriangle after zeroing #%d\t***\n", i+1);
        printf("***\tFirst Matrix\t***\n");
        write_matrix_1d(matrix1, "stdout"); // Debug code.
        printf("***\tSecond Matrix\t***\n");
        write_matrix_1d(matrix2, "stdout");
        printf("\n\n\n");
        getchar();*/
    }

    /*printf("\n\n\n");
    printf("***\tTriangle after lower triangle\t***\n");
    printf("***\tFirst Matrix\t***\n");
    write_matrix_1d(matrix1, "stdout"); // Debug code.
    printf("***\tSecond Matrix\t***\n");
    write_matrix_1d(matrix2, "stdout");
    printf("\n\n\n");
    getchar();*/

    // Zero out the upper triangle.
    for(i=matrix1.rows-1;i>0;i--)
    {
        for(j=i-1;j>=0;j--)
        {
            temp = get_value(j, i, matrix1);    // Set the multiplier.

            sub_row(j, i, matrix1, temp);       // Zero out matrix1 row.
            sub_row(j, i, matrix2, temp);       // Zero out matrix2 row.
        }
    }

    /*printf("\n\n\n");
    printf("***\tMatrices before switching\t\t***\n");
    printf("***\tFirst Matrix\t***\n");
    write_matrix_1d(matrix1, "stdout"); // Debug code.
    printf("***\tSecond Matrix\t***\n");
    write_matrix_1d(matrix2, "stdout");
    printf("\n\n\n");
    getchar();*/

    //printf("Switched %d columns.\n", switch_count); // Debug code.

    // Switch for the column switches.
    for(i=switch_count-1;i>=0;i--)     // Start with the last switch, finish with the first.
    {
        //printf("Switching row %d with row %d.\n", switches[i].num1, switches[i].num2);  // Debug code.
        switch_rows(switches[i].num1, switches[i].num2, matrix2);
    }

    /*printf("\n\n\n");
    printf("***\tMatrices after eliminator\t\t***\n");
    printf("***\tFirst Matrix\t***\n");
    write_matrix_1d(matrix1, "stdout"); // Debug code.
    printf("***\tSecond Matrix\t***\n");
    write_matrix_1d(matrix2, "stdout");
    printf("\n\n\n");
    getchar();*/

    return 1;
}

