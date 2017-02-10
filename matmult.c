#include"matr_functions.h"

void usage(char * prog_name);

matrix mult_matrices(matrix mat1, matrix mat2);

int main(int argc, char *argv[])
{
	matrix mat1, mat2, result;
	
	if(argc!=4)
		usage(argv[0]);
	
	mat1 = read_matrix(argv[1]);
	write_matrix(mat1, "stdout");
	printf("\n");
	mat2 = read_matrix(argv[2]);
	write_matrix(mat2, "stdout");
	printf("\n");
	
	result = mult_matrices(mat1, mat2);
	
	if(result.columns == -1)
		fprintf(stderr, "There was an error during matrix multiplication!\n");
	else
		write_matrix(result, argv[3]);
	
	return 0;
}

void usage(char * prog_name)
{
	fprintf(stderr, "Illegal number of arguments!\nUsage: %s <first input file name> <second input file name> <output file name>\n", prog_name);
	fprintf(stderr, "You can also specify the output to be the standard output, by writing stdout instead of a file name.\n");
	exit(1);
}

matrix mult_matrices(matrix mat1, matrix mat2)
{
	matrix result;
	int i, j, k;
	
	if(mat1.columns != mat2.rows)
	{
		fprintf(stderr, "The two matrices are not compatible! They cannot be multiplied.\n");
		result.columns = -1;
		return result;
	}
	
	result.values = (double **)calloc(mat1.rows, sizeof(double));
	
	for(i=0;i<mat1.rows;i++)
	{
		result.values[i] = (double *)calloc(mat2.columns, sizeof(double));
		
		for(j=0;j<mat2.columns;j++)
		{
			result.values[i][j] = 0;
			
			for(k=0;k<mat1.columns;k++)
				result.values[i][j] += mat1.values[i][k] * mat2.values[k][j];
		}
	}
	
	result.rows = mat1.rows;
	result.columns = mat2.columns;
	
	return result;
}
