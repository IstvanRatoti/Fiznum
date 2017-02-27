#include "matr_functions_1d.h"

int main(int argc, char *argv[])
{
	matrix_1d mat = read_matrix_1d(argv[1]);

	//mat = read_matrix("data.txt");
	write_matrix_1d(mat, "stdout");

	return 0;
}
