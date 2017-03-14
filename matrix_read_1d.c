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
			fprintf(file, "%5.8f\t", value);

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
		perror("Error opening file.\n");
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
