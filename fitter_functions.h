#ifndef FITTER_FUNCTIONS_H_INCLUDED
#define FITTER_FUNCTIONS_H_INCLUDED

#include "matr_functions_1d.h"
#include "gauss_functions.h"

/*
*   Transposes a matrix. That's all.
*/
matrix_1d transpose_matrix(matrix_1d matrix);

/*
*   This function helps to calculate all the different powers.
*/
matrix_1d set_powers(matrix_1d * result, matrix_1d prev_powers, int variables);

/*
*   Creates a matrix containing the power of each variable for the
*   given coefficient.
*/
matrix_1d create_powers(int variables, int order);

/*
*   Creates the matrix used to calculate the 2 "sides of the linear
*   equation. The order determines the columns of the matrix.
*   (ATM, only "lonely variable"-s and their powers)
*/
matrix_1d create_poly_matrix(matrix_1d variables, int order);

/*
*   Creates the square matrix and the vector needed to perform the Gaussian
*   Elimination. the first 2 arguments are the Gaussian matrices, the next 3
*   are the data provided, and the last one is the order of fit to be used.
*/
int create_linear_matrices(matrix_1d * gauss_matrix, matrix_1d * gauss_vector,
                           matrix_1d variables, matrix_1d values, matrix_1d error,
                           int order);

/*
*   Calls the creation of the linear matrices and the Gaussian eliminator.
*   Return the result of the fit.
*/
matrix_1d fit_polynomial(matrix_1d variables, matrix_1d values,
                        matrix_1d error, int order);

#endif // FITTER_FUNCTIONS_H_INCLUDED
