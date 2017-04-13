#include<math.h>
#include<stdlib.h>
#include<stdio.h>

#include "fitter_functions.h"

/*
*   Transposes a matrix. That's all.
*/
matrix_1d transpose_matrix(matrix_1d matrix)
{
    int i, j;

    matrix_1d result;
    result.values = (double *)calloc(matrix.rows*matrix.columns, sizeof(double));
    result.columns = matrix.rows;
    result.rows = matrix.columns;

    // Transpose the matrix.
    for(i=0;i<matrix.columns;i++)
    {
        for(j=0;j<matrix.rows;j++)
                set_value(i, j, result, get_value(j, i, matrix));
    }

    return result;
}

/*
*   This function helps to calculate all the different powers.
*/
matrix_1d set_powers(matrix_1d * result, matrix_1d prev_powers, int variables)
{
    int i, j, k;
    int * new_row = (int *)calloc(variables, sizeof(int));

    matrix_1d next_powers;
    next_powers.columns = variables;    // Initialize the next_powers matrix.
    next_powers.rows = 0;
    next_powers.values = (double *)malloc(sizeof(double));


    if(0==prev_powers.columns)                     // If the prev_powers matrix is not
        prev_powers = create_identity(variables);  // initialized, create an identity for it.

    for(i=0;i<prev_powers.rows;i++)
    {
        for(j=0;j<variables;j++)
        {
            for(k=0;k<variables;k++)    // Create the new row to be added.
            {
                if(k==j)    // The k-th value needs to be incremented.
                    new_row[k] =(int )get_value(i, k, prev_powers) + 1;
                else        // The others dont.
                    new_row[k] = (int )get_value(i, k, prev_powers);
            }

            // If this row is not in the matrices yet, add it.
            if(row_not_present(next_powers, new_row))
            {
                next_powers.rows++;
                next_powers.values = (double *)realloc(next_powers.values,  // Reallocate for the next_powers matrix.
                                                       next_powers.columns*next_powers.rows*sizeof(double));
                result->rows++;
                result->values = (double *)realloc(result->values,          // Reallocate for the result matrix.
                                                   result->columns*result->rows*sizeof(double));

                for(k=0;k<variables;k++)
                {
                    set_value((next_powers.rows-1), k, next_powers, new_row[k]);    // Set the new elements of next_powers...
                    set_value((result->rows-1), k, *result, new_row[k]);            // and result too.
                }
            }
        }
    }

    free(new_row);

    return next_powers;
}

/*
*   Creates a matrix containing the power of each variable for the
*   given coefficient.
*/
matrix_1d create_powers(int variables, int order)
{
    int i;
    matrix_1d result = create_identity(variables);


    matrix_1d prev_powers;
    prev_powers.columns = 0;    // This matrix is not initialized first.

    for(i=order-1;i>0;i--)
        prev_powers = set_powers(&result, prev_powers, variables);

    return result;
}

/*
*   Creates the matrix used to calculate the 2 "sides of the linear
*   equation. The order determines the columns of the matrix.
*   (All the combinations in this one.)
*/
matrix_1d create_poly_matrix_full(matrix_1d variables, int order)
{
    int i;

    matrix_1d order_matrix = create_powers(variables.rows, order);

    matrix_1d result;
    result.columns = 1 + variables.columns*order_matrix.rows;
    result.rows = variables.rows;
    result.values = (double *)calloc(result.columns*result.rows, sizeof(double));

    //printf("Allocation complete.\nColumns: %d\tRows: %d\n", result.columns, result.rows);
    //getchar();    // Debug code.

    for(i=0;i<result.rows;i++)
    {
        for(j=0;j<result.columns;j++)
        {
            if(0==j)    // The first values are the constants.
            {
                set_value(i, j, result, 1);
                //printf("Constant value set.\n");
                //getchar();      // Debug code.
            }
            else
            {

            }
        }
    }

    return result;
}

/*
*   Creates the matrix used to calculate the 2 "sides of the linear
*   equation. The order determines the columns of the matrix.
*   (ATM, only "lonely variable"-s and their powers)
*/
matrix_1d create_poly_matrix(matrix_1d variables, int order)
{
    int i, j, power, var_number;

    matrix_1d result;
    result.values = (double *)calloc((1 + variables.columns*order)*variables.rows, sizeof(double));
    result.columns = 1 + variables.columns*order;   // Allocate and initialize our matrix.
    result.rows = variables.rows;

    //printf("Allocation complete.\nColumns: %d\tRows: %d\n", result.columns, result.rows);
    //getchar();    // Debug code.

    for(i=0;i<result.rows;i++)
    {
        for(j=0;j<result.columns;j++)
        {
            if(0==j)    // The first values are the constants.
            {
                set_value(i, j, result, 1);
                //printf("Constant value set.\n");
                //getchar();      // Debug code.
            }
            else        // The others are some power of a variable.
            {
                power = (j-1)%order + 1;    // Sets the power of the next value.
                var_number = (j-1)/order;   // Tells which original variable we need.

                // Sets the value of the next value.
                set_value(i, j, result, pow(get_value(i, var_number, variables), power));
                //printf("Power value set.\n");
                //getchar();      // Debug code.
            }
        }
    }

    return result;
}

/*
*   Creates the square matrix and the vector needed to perform the Gaussian
*   Elimination. the first 2 arguments are the Gaussian matrices, the next 3
*   are the data provided, and the last one is the order of fit to be used.
*/
int create_linear_matrices(matrix_1d * gauss_matrix, matrix_1d * gauss_vector, matrix_1d variables, matrix_1d values, matrix_1d error, int order)
{
    int i;
    double divisor = 1;

    if(-1 != error.rows)    // Only need to divide by the error if it exists. (DUH!)
    {
        // Go through all the rows.
        for(i=0;i<variables.rows;i++)
        {
            divisor /= get_value(i, 0, error);  // Set the "divisor" to the inverse of the error.

            mult_row(divisor, i, variables);    // "Divide" the independent variable matrix.
            mult_row(divisor, i, values);       // And the dependent one too.

            divisor = 1;
        }
    }

    matrix_1d poly_matrix = create_poly_matrix(variables, order);   // Create The matrix. (if I knew the name...)
    matrix_1d trans_matrix = transpose_matrix(poly_matrix);         // Make a trans matrix too. (No pun intended)

    //write_matrix_1d(poly_matrix, "stdout");
    //write_matrix_1d(trans_matrix, "stdout");    // Debug code.
    //getchar();

    *gauss_matrix = mult_matrices(trans_matrix, poly_matrix);       // Create the linear matrices.
    *gauss_vector = mult_matrices(trans_matrix, values);

    //write_matrix_1d(*gauss_matrix, "stdout");
    //write_matrix_1d(*gauss_vector, "stdout");   // Debug code.
    //getchar();

    return 0;       // Return. Why did I write this function?
}

/*
*   Calls the creation of the linear matrices and the Gaussian eliminator.
*   Return the result of the fit.
*/
matrix_1d fit_polynomial(matrix_1d variables, matrix_1d values, matrix_1d error, int order)
{
    matrix_1d gauss_matrix, gauss_vector;   // The 2 matrices to be used in the eliminator.

    // Creates the above 2 matrices.
    create_linear_matrices(&gauss_matrix, &gauss_vector, variables, values, error, order);

    // Runs the eliminator.
    gauss_eliminator(gauss_matrix, gauss_vector);

    return gauss_vector;
}
