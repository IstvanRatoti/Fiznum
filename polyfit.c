#include<stdio.h>
#include<stdlib.h>

#include "fitter_functions.h"
#include "matr_functions_1d.h"

void usage(char * progname);

/*
*   This function reads the given file, and separates the data into 3 different
*   matrices to be used by the fitter algorithm.
*/
int read_data(char * filename, int var_count, matrix_1d * variables, matrix_1d * values, matrix_1d * error);

int main(int argc, char *argv[])
{
    int order, var_count;

    if(4!=argc)
    {
        usage(argv[0]);
    }

    order = atoi(argv[2]);
    var_count = atoi(argv[1]);

    matrix_1d variables, values, error, result;

    if(!read_data(argv[3], var_count, &variables, &values, &error))
    {
        fprintf(stderr, "There was an error!\n");
        return 1;
    }

    //write_matrix_1d(variables, "stdout");
    //write_matrix_1d(values, "stdout");  // Debug code.
    //write_matrix_1d(error, "stdout");
    //getchar();

    result = fit_polynomial(variables, values, error, order);

    printf("Parameters of the fit:\n");
    write_matrix_1d(result, "stdout");

    return 0;
}

void usage(char * progname)
{
    fprintf(stderr, "Invalid number of arguments!\n");
    fprintf(stderr, "Usage: %s <independent_variables> <fit_order> <input_file>\n", progname);
    exit(1);
}

int read_data(char * filename, int var_count, matrix_1d * variables, matrix_1d * values, matrix_1d * error)
{
    int i, j;
    double element;

    matrix_1d raw_data = read_matrix_1d(filename);  // Read the raw data.

    write_matrix_1d(raw_data, "stdout");
    getchar();      // Debug code.

    if(-1==raw_data.rows)   // Return if read was unsuccessful.
        return 0;

    // Check if the raw data matrix is the right size.
    if((raw_data.columns == (var_count+1))||(raw_data.columns == (var_count+2)))
    {
        if(raw_data.columns == (var_count+1))  // write a warning and mark that we have no error values.
        {
            fprintf(stderr, "Warning! No error values were given!\n");
            error->rows = -1;
        }
        else    // Allocate for the error values.
        {
            error->values = (double *)calloc(raw_data.rows, sizeof(double));
            error->rows = raw_data.rows;
            error->columns = 1;
        }

        variables->values = (double *)calloc(var_count*raw_data.rows, sizeof(double));
        variables->rows = raw_data.rows;     // Allocate for our independent variables.
        variables->columns = var_count;

        values->values = (double *)calloc(raw_data.rows, sizeof(double));
        values->rows = raw_data.rows;        // Allocate for the dependent variables.
        values->columns = 1;

        // Go through the raw data matrix.
        for(i=0;i<raw_data.rows;i++)
        {
            for(j=0;j<raw_data.columns;j++)
            {
                element = get_value(i, j, raw_data);

                if(j == (var_count))      // Set the dependent variables.
                    set_value(i, 0, *values, element);
                else if(j == (var_count+1)) // Set the errors. (if we have those)
                    set_value(i, 0, *error, element);
                else                        // Set the independent variables.
                    set_value(i, j, *variables, element);
            }
        }
    }
    else    // Return if..read the freakin' error message under!
    {
        fprintf(stderr, "Matrix didn't match the expected format!\n");
        return 0;
    }

    return 1;
}
