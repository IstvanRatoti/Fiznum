#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct mtr
{
	int rows;
	int columns;
	double ** values;
}matrix;

/* Basic Write matrix function. Can write to stdout or a specified file. No return value. */
void write_matrix(matrix matr, char * file_name)
{	
	FILE * file;
	int i, j;
	
	if(matr.rows == -1)		// Checking for reading error.
	{
		fprintf(stderr, "Matrix is empty.\nReason: reading error.");
		return;
	}
	else if(matr.columns == -1)		// Checking for matrix multiplication error.
	{
		fprintf(stderr, "Matrix is empty.\nReason: multiplication error.");
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
		perror("Error opening file.");
		return;
	}
	
	for(i=0;i<matr.rows;i++)
	{
		for(j=0;j<matr.columns;j++)
		{
			fprintf(file, "%5.2f", matr.values[i][j]);
			
			if((j+1)==matr.columns)
			{
				if((i+1)==matr.rows)
					goto END;			// Not pretty, but we use it to not write a \n before EOF.
				fprintf(file, "\n");
			}
			else
				fprintf(file, "\t");
		}
	}
	
END:
	if(file != stdout)		// Dont think we want to close stdout, do we?
		fclose(file);
}

matrix read_matrix(char * file_name)
{
	matrix matr;
	FILE * file;
	int i=0, j=0, c=0;		// Using an int for the characters so we can catch EOF.
	int par=0;				// Number for checking if the parsing was successful.
	double num;
	char str[100] = "";
	
	file = fopen(file_name, "r");
	
	if(file == NULL)
	{
		perror("Error opening file.");
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
				matr.values = (double **)calloc(1, sizeof(double));
			else if((c==EOF)&&(strlen(str)==0))
			{
				i--;
				break;
			}
			else
				matr.values = (double **)realloc(matr.values, (i+1)*sizeof(double));
		}
		
		if((c==' ')||(c=='\t'))			// Parse the number when encountering a whitespace.
		{
			par = sscanf(str, "%lf", &num);
			
			if((par != EOF)&&(par != -1))		// Store ONLY valid numbers.
			{
				++j;
				matr.values[i] = (double *)realloc(matr.values[i], j*sizeof(double));	// Heap allocation for the new number.
				matr.values[i][j-1] = num;
				if(matr.columns < j)
						matr.columns = j;		// Set the columns of the matrix. Possible error detection /w if(j!=matr.columns)
			}
			
			str[0] = '\0';				// "Reset" the string after a parsing.
		}
		else if((c=='\n')||(c==EOF))	// Also parse on newline, or EOF.
		{
			par = sscanf(str, "%lf", &num);
			
			if((par != EOF)&&(par != -1))		// Again, only valid numbers.
			{
				j++;
				
				if(j==1)													// Realloc is a bastard, so we need to calloc a single column matrix
					matr.values[i] = (double *)calloc(1, sizeof(double));	// (i.e. vector), so we can access the allocated memory as an nx1 array.
				else
					matr.values[i] = (double *)realloc(matr.values[i], j*sizeof(double));

				if(c!=EOF)		// Allocate for the next row ONLY if its not the EOF.
				{
					i++;
					matr.values[i-1][j-1] = num;
					if(matr.columns == 0)
						matr.columns = j;		// Set the columns of the matrix. Possible error detection /w if(j!=matr.columns)
					j = 0;
				}
				else
					matr.values[i][j-1] = num;	// If EOF reached, just store the value.
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
			snprintf(str, sizeof(str)+1, "%s%c", str, c);		// If no "special" characters were found, its probably a "part" of a number.
	} while(c != EOF);
	
	matr.rows = i+1;		// Set the rows.
	
	fclose(file);
	
	return matr;
}
