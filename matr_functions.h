typedef struct mtr
{
	int rows;
	int columns;
	double ** values;
}matrix;

//typedef struct mtr matrix;

matrix read_matrix(char * file_name);

void write_matrix(matrix matr, char * file_name);
