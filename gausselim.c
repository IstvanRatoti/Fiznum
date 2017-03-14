#include<stdio.h>

#include "gauss_test.h"
#include "gauss_functions.h"

int main(int argc, char *argv[])
{
    test_functions();

    matrix_1d matrix1 = read_matrix_1d("elimtest.dat");
    matrix_1d matrix2 = read_matrix_1d("identity.dat");

    write_matrix_1d(matrix1, "stdout");
    write_matrix_1d(matrix2, "stdout");

    if(!gauss_eliminator(matrix1, matrix2))
    {
        fprintf(stderr, "The eliminator encountered an error!\n");
    }

    return 0;
}
