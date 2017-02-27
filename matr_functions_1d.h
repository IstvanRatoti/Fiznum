typedef struct mtr
{
	int rows;
	int columns;
	double * values;
}matrix_1d;

//typedef struct mtr matrix;

matrix_1d read_matrix_1d(char * file_name);

void write_matrix_1d(matrix_1d matr, char * file_name);

void set_value(int row, int column, matrix_1d matr, double value);

double get_value(int row, int column, matrix_1d matr);
