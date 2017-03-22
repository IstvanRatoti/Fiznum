#ifndef MATR_FUNC_H
#define MATR_FUNC_H

typedef struct mtr
{
	int rows;
	int columns;
	double * values;
}matrix_1d;

matrix_1d read_matrix_1d(char * file_name);

void write_matrix_1d(matrix_1d matr, char * file_name);

void set_value(int row, int column, matrix_1d matr, double value);

double get_value(int row, int column, matrix_1d matr);

matrix_1d create_identity(int dimensions);

matrix_1d mult_matrices(matrix_1d mat1, matrix_1d mat2);

#endif // MATR_FUNC_H
