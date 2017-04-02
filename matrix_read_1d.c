#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "matr_functions_1d.h"

void set_value(int row, int column, matrix_1d matr, double value)
{
    matr.values[row*matr.columns + column] = value;
}

double get_value(int row, int column, matrix_1d matr)
{
    return matr.values[row*matr.columns + column];
}

/* Basic Write matrix function. Can write to stdout or a specified file. No return value. */
void write_matrix_1d(matrix_1d matr, char * file_name)
{
	FILE * file;
	int i, j;
	double value = 0;

	if(matr.rows == -1)		// Checking for reading error.
	{
		fprintf(stderr, "Matrix is empty.\nReason: reading error.\n");
		return;
	}
	else if(matr.columns == -1)		// Checking for matrix multiplication error.
	{
		fprintf(stderr, "Matrix is empty.\nReason: multiplication error.\n");
		return;
	}

	if(strcmp(file_name, "stdout"))		// Check where we want to write.
		file = fopen(file_name, "w");
	else
	{
		file = stdout;
		printf("Details of the matrix:\nRows: %d\nColumns: %d\nThe matrix:\n", matr.rows, matr.columns);
	}

	if(file == NULL)
	{
		perror("Error opening file.\n");
		return;
	}

	for(i=0;i<matr.rows;i++)
	{
		for(j=0;j<matr.columns;j++)
		{
		    value = get_value(i, j, matr);
			fprintf(file, "%5.20f\t", value);

			/*if((j+1)==matr.columns)
			{
				if((i+1)==matr.rows)
					goto END;			// Not pretty, but we use it to not write a \n before EOF.
				fprintf(file, "\n");
			}
			else
				fprintf(file, "\t");*/
		}
		fprintf(file, "\n");
	}

//END:
	if(file != stdout)		// Dont think we want to close stdout, do we?
		fclose(file);
}

matrix_1d read_matrix_1d(char * file_name)
{
	matrix_1d matr;
	FILE * file;
	int i=0, j=0, c=0;		// Using an int for the characters so we can catch EOF.
	int par=0;				// Number for checking if the parsing was successful.
	double num;
	char ch;
	char str[100] = "";

	file = fopen(file_name, "r");

	if(file == NULL)
	{
		fprintf(stderr, "Error opening file.\n");
		matr.rows = -1;					// This value is useful to determine later if the reading was successful.
		return matr;
	}

	matr.columns = 0;

	do
	{
		c = fgetc(file);

		if(j == 0)		// Call the row allocation only if there is a new line.
		{
			if(i == 0)	// The first one have to be a calloc for it to be an array.
				matr.values = (double *)calloc(1, sizeof(double));
			else if((c==EOF)&&(strlen(str)==0))
			{
				i--;
				break;
			}
		}

		if((c==' ')||(c=='\t'))			// Parse the number when encountering a whitespace.
		{
			par = sscanf(str, "%lf", &num);

			if((par != EOF)&&(par != -1))		// Store ONLY valid numbers.
			{
				++j;
                matr.values = (double *)realloc(matr.values, (i*matr.columns + j)*sizeof(double));
				set_value(i, j-1, matr, num);
			}

			str[0] = '\0';				// "Reset" the string after a parsing.
		}
		else if((c=='\n')||(c==EOF))	// Also parse on newline, or EOF.
		{
			par = sscanf(str, "%lf", &num);

			if((par != EOF)&&(par != -1))		// Again, only valid numbers.
			{
				j++;
                matr.values = (double *)realloc(matr.values, (i*matr.columns + j)*sizeof(double));

				if(c!=EOF)		// Allocate for the next row ONLY if its not the EOF.
				{
					i++;
					set_value(i-1, j-1, matr, num);
                    matr.columns = j;		// Set the columns of the matrix. Possible error detection /w if(j!=matr.columns)
					j = 0;
				}
				else
                    set_value(i, j-1, matr, num);	// If EOF reached, just store the value.
			}
			else	// A new line could mean a new row.
			{
				if((j!=0)&&(c!=EOF))	// But only if there was at least one element in the last row.
					i++;				// Also, if EOF is reached, there will be no more rows.
				j=0;	// A new line ALWAYS starts with the first element.
			}

			str[0] = '\0';
		}
		else
        {
			ch = (char )c;
			if(ch == ',')   // Little code to handle colons.
                ch = '.';
			strncat(str, &ch, 1);		// If no "special" characters were found, its probably a "part" of a number.
        }
	} while(c != EOF);

	matr.rows = i+1;		// Set the rows.

	fclose(file);

	return matr;
}

matrix_1d create_identity(int dimensions)
{
    int i=0, j=0;

    matrix_1d result;
    result.columns = dimensions;
    result.rows = dimensions;
    result.values = (double *)calloc(dimensions*dimensions, sizeof(double));

    if(-1==dimensions)
    {
        fprintf(stderr, "Cannot create identity, because file was not properly read!\n");

        return result;
    }

    for(i=0;i<dimensions;i++)
    {
        for(j=0;j<dimensions;j++)
        {
            if(i==j)
                set_value(i, j, result, 1);
            else
                set_value(i, j, result, 0);
        }
    }

    return result;
}

matrix_1d mult_matrices(matrix_1d mat1, matrix_1d mat2)
{
	matrix_1d result;
	int i, j, k;
	double temp = 0;
	double t1, t2;

	if(mat1.columns != mat2.rows)   // Checks if the two matrices are compatible.
	{
		fprintf(stderr, "The two matrices are not compatible! They cannot be multiplied.\n");
		result.columns = -1;    // Value for error handling.
		return result;
	}

	result.rows = mat1.rows;
	result.columns = mat2.columns;  // Need to initialize these values, because we use them.
	result.values = (double *)calloc(mat1.rows*mat2.columns, sizeof(double));

	//printf("Loop boundaries:\nOuter: %d\tMiddle: %d\tInner: %d\n", mat1.rows, mat2.columns, mat1.columns);    Debug code

	for(i=0;i<mat1.rows;i++)
	{
		for(j=0;j<mat2.columns;j++)
		{
			for(k=0;k<mat1.columns;k++)
			{
				t1 = get_value(i, k, mat1);
				t2 = get_value(k, j, mat2);

				//printf("Multiplying %g (Row: %d Column: %d) with\n", t1, i, k);
				//printf("with %g (Row: %d Column: %d)\n", t2, k, j);   Debug code
                //getchar();

				temp += t1*t2;
            }

            //printf("The value of element at row %d column %d: %g", i, j, temp);   Debug code
            //getchar();

            set_value(i, j, result, temp);
            temp = 0;
		}
	}

	//printf("Out of the loop!\n"); Debug code

	return result;
}
