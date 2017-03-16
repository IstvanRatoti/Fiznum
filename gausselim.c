#include<stdio.h>
#include<string.h>

#include "gauss_test.h"
#include "gauss_functions.h"

void usage(char * prog_name);

int run_func_tests(void);

/*
*   The program can receive 1 or 2 arguments.
*   If it receives 1 argument, it will Either invert a matrix, or run the tests
*   with the "TEST" keyword (you will need the test files to run the tests).
*   If it receives 2 arguments, it will solve a system of linear equations. The
*   first matrix must be the coefficients of the variables, the second matrix
*   must be the results of the equation.
*/
int main(int argc, char *argv[])
{
    if(2==argc) // If one argument is given.
    {
        if(!strcmp("TEST", argv[1])) // Run tests.
        {
            return run_func_tests();
        }
        else    // Compute inverse.
        {
            matrix_1d matrix1 = read_matrix_1d(argv[1]);
            matrix_1d matrix2 = create_identity(matrix1.rows);

            printf("***\t\tThe matrix to be inverted:\t\t***\n");
            write_matrix_1d(matrix1, "stdout");     // Debug code.

            if(!gauss_eliminator(matrix1, matrix2))
            {
                fprintf(stderr, "The eliminator encountered an error!\n");

                return 1;
            }
            else
            {
                printf("***\t\tThe inverse of the matrix:\t\t***\n");
                write_matrix_1d(matrix2, "stdout");
            }
        }
    }
    else if(3==argc)    // Compute solution of a system of linear equations.
    {
        matrix_1d matrix1 = read_matrix_1d(argv[1]);
        matrix_1d matrix2 = read_matrix_1d(argv[2]);

        printf("***\t\tThe input:\t\t***\n");
        write_matrix_1d(matrix1, "stdout");     // Debug code.
        write_matrix_1d(matrix2, "stdout");

        if(!gauss_eliminator(matrix1, matrix2))
        {
            fprintf(stderr, "The eliminator encountered an error!\n");

            return 1;
        }
        else
        {
            printf("***\t\tThe result::\t\t***\n");
            write_matrix_1d(matrix2, "stdout");
        }

    }
    else    // Wrong number of arguments.
    {
        usage(argv[0]);

        return 1;
    }

    return 0;
}

// Basic usage function, to let the user know how the program works.
void usage(char * progname)
{
    fprintf(stderr, "Illegal number of arguments!\nUsage:\n");
    fprintf(stderr, "%s <matrix file name> [OPTIONAL] <vector file name>\n", progname);
    fprintf(stderr, "Calling the program with one argument inverts a matrix.\n");
    fprintf(stderr, "The second argument can be used to supply a vector to solve a system of linear equations.\n");
}

// Run tests on the functions. Can be expanded on demand.
int run_func_tests(void)
{
    int success = 0;

    success &= test_functions();

    return success;
}
