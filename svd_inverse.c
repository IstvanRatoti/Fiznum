#include<stdio.h>
#include<time.h>

#include "svd_inverse.h"
#include "lapacke.h"

/*
*   Transposes a matrix and returns the transposed matrix. Keeps the original intact.
*/
matrix_1d transpose_matrix(matrix_1d matrix)
{
    int i, j;

    matrix_1d result;
    result.values = (double *)calloc(matrix.rows*matrix.columns, sizeof(double));
    result.columns = matrix.columns;
    result.rows = matrix.rows;

    // Transpose the matrix.
    for(i=0;i<matrix.rows;i++)
    {
        for(j=0;j<matrix.columns;j++)
                set_value(i, j, result, get_value(j, i, matrix));
    }

    return result;
}

/*
*   Calculates the inverse of a diagonal matrix.
*/
matrix_1d invert_diag(matrix_1d matrix)
{
    int i, j, singularity = 0;
    double temp = 0;

    matrix_1d result;
    result.values = (double *)calloc(matrix.columns*matrix.columns, sizeof(double));
    result.columns = matrix.columns;
    result.rows = matrix.columns;

    for(i=0;i<matrix.columns;i++)
    {
        for(j=0;j<matrix.columns;j++)
        {
            if(i==j)    // The singular values are in a column vector,
            {           // need to make a matrix out of them.
                temp = get_value(0, i, matrix);
                if(temp<1e-10) // Code to approximate an inverse.
                {
                    singularity = 1;
                    set_value(i, j, result, 0);
                }
                else
                    set_value(i, j, result, 1/temp);
            }
            else
                set_value(i, j, result, 0);
        }
    }

    if(singularity)
        printf("Singularity detected! Calculating pseudo-inverse.\n");

    return result;
}

/*
*   Calculates the inverse of a matrix using a lapack SVD function.
*/
matrix_1d calculate_inverse(matrix_1d matrix)
{
    clock_t start, finish;
    double runtime;

    matrix_1d result;   // Initialize the result matrix.
    result.values = (double *)calloc(matrix.rows*matrix.columns, sizeof(double));
    result.columns = matrix.columns;
    result.rows = matrix.rows;

    lapack_int m=matrix.rows, n=matrix.columns;     // Variables needed for the algorithm.
    lapack_int lda=matrix.columns, ldu= matrix.rows;
    lapack_int ldvt=matrix.columns, info;

    matrix_1d singular;     // Vector containing the singular values of our matrix.
    singular.values = (double *)calloc(matrix.columns, sizeof(double));
    singular.columns = matrix.columns;
    singular.rows = 1;

    matrix_1d left;         // Left hand singular vectors.
    left.values = (double *)calloc(matrix.rows*matrix.rows, sizeof(double));
    left.rows = matrix.rows;
    left.columns = matrix.columns;

    matrix_1d right;        // Right hand singular vectors.
    right.values = (double *)calloc(matrix.columns*matrix.columns, sizeof(double));
    right.columns = matrix.columns;
    right.rows = matrix.columns;

    double * superb = (double *)calloc(matrix.columns-1, sizeof(double));   // Got the foggiest, what this is...

    start = (long )clock();
    info = LAPACKE_dgesvd(LAPACK_ROW_MAJOR, 'A', 'A', m, n, matrix.values, lda, singular.values,
                           left.values, ldu, right.values, ldvt, superb);
    finish = (long )clock();        // Calculate runtime.
    runtime = ((double )finish-start)/CLOCKS_PER_SEC;
    printf("Runtime: %lf seconds.\n", runtime);

    if(info>0)  // Return value for error checking and stuff? prolly...
    {
        fprintf(stderr, "The algorithm computing SVD failed to converge.\n");
        result.columns = -1;
        return result;
    }

    //write_matrix_1d(left, "stdout");
    //write_matrix_1d(right, "stdout");     // Debug code
    //write_matrix_1d(singular, "stdout");
    //getchar();

    left = transpose_matrix(left);      // Transpose the left hand matrix.
    right = transpose_matrix(right);    // And the right hand.
    singular = invert_diag(singular);   // Also, invert the diagonalized singular values.

    /*write_matrix_1d(left, "stdout");
    write_matrix_1d(right, "stdout");     // Debug code
    write_matrix_1d(singular, "stdout");
    getchar();*/

    singular = mult_matrices(right, singular);  // Multiplying the right hand matrix with the singular values...
    //write_matrix_1d(singular, "stdout");
    //getchar();        // Debug code

    result = mult_matrices(singular, left);     // Then multiplying the result with the left hand matrix.
    //write_matrix_1d(result, "stdout");
    //getchar();        // Debug code

    return result;
}
